#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "math.h"

#include "entities.h"

#define WIDTH 1280
#define HEIGHT 720

#define MS_PER_UPDATE 16

#define vel 10

// GLOBALS
SDL_Window *win;
SDL_Renderer *rend;

SDL_Texture *load_bmp_to_texture(const char path[]);

void render_dynamic(sprite_t *entity);
void render_static(sprite_t *entity);

int loop;
int game_state;

enum engine_states {
	MENU,
	GAME,
	DEBUG
};

//void update_input(sprite_t player, const Uint8 *keys);
void fixed_update_loop();
void render_menu();
void render_game();
void render_debug();

int main(int argc, char *argv[])
{
	//initialise SDL components
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		printf("error SDL failed to initialise: %s\n", SDL_GetError());
		return 1;
	}

	if (TTF_Init() < 0) {
		printf("error TTF failed to initialise: %s\n", TTF_GetError());
		return 1;
	}

	//create an SDL window with define parameters
	win = NULL;
	win = SDL_CreateWindow("Paws, pre alpha",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			WIDTH, HEIGHT,
			SDL_WINDOW_SHOWN);
	if (!win) {
		printf("error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	//create a renderer which sets up drivers and rendering options
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	rend = NULL;
	rend = SDL_CreateRenderer(win, -1, render_flags);
	if (!rend) {
		printf("error creating renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	loop = 1;

	//Game stuff
	vecf_t camera;
	camera.x = 100;
	camera.y = 100;
	camera.z = 100;

	sprite_t player;
	player.texture = load_bmp_to_texture("assets/player_texture.bmp");
	player.current_frame = 0;
	player.frames_per_row = 8;
	player.tile_w = 20;
	player.tile_h = 20;
	player.w = 100;
	player.h = 100;

	player.x = 200;
	player.y = 200;

	//Static Entity test
	sprite_t door;
	door.texture = load_bmp_to_texture("assets/doore.bmp");
	door.tile_w = 200;
	door.tile_h = 200;
	door.w = 120;
	door.h = 120;

	//test font
	TTF_Font *test_font;
	test_font = TTF_OpenFont("/usr/share/fonts/truetype/mononoki/mononoki-Regular.ttf", 100);
	SDL_Color fg, bg;
	bg.a = 0;
	fg.r = 0xAA; fg.g = 0xBB; fg.b = 0xFF; fg.a = 0xFF;
	const char *text_content = "This is a text example";

	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	SDL_Event event;

	double pre_loop_t = SDL_GetTicks();
	double lag_t = 0.00;

	//double start_anim = SDL_GetTicks();

	while (loop)
	{
		//fix timestep
		double cur_t = SDL_GetTicks();
		double dt = cur_t - pre_loop_t;
		pre_loop_t = cur_t;
		lag_t += dt;

		//update keymap and handle window event queue
		while (SDL_PollEvent(&event) > 0)
		{
			loop = (event.type != SDL_QUIT);
			//update_input(player, keys);
		}

		while (lag_t >= MS_PER_UPDATE)
		{
			fixed_update_loop();

			// end fixed update
			lag_t -= MS_PER_UPDATE;
		}

		SDL_RenderClear(rend);

		switch (game_state) {
			case MENU:

			case GAME:
				//render entities to game
				render_dynamic(&player);
				render_static(&door);

				//text test
				SDL_Surface *text = TTF_RenderUTF8_Solid(test_font, text_content, fg);
				SDL_Texture *text_texture = SDL_CreateTextureFromSurface(rend, text);
				SDL_Rect dst = {100, 100, 800, 150};
				SDL_RenderCopy(rend, text_texture, NULL, &dst);

			case DEBUG:
				break;
		};

		SDL_RenderPresent(rend);
	}

	//clear entity textures from memory
	SDL_DestroyTexture(player.texture);
	SDL_DestroyTexture(door.texture);
	TTF_CloseFont(test_font);

	//stop SDL subsystems
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);

	SDL_Quit();

	return 0;
}

/* Game engine functions 
void upate_input (sprite_t player, const Uint8 *keys)
{
	printf("hello does this work?\n");
}
*/

void fixed_update_loop ()
{
}

void render_menu ()
{
}

void render_scene ()
{
}

void render_debug ()
{
}

/* return the loaded SDL_Texture from a bitmap image */
SDL_Texture *load_bmp_to_texture(const char path[])
{
	SDL_Surface *raw = NULL;
	if ((raw = SDL_LoadBMP(path)) == NULL) {
		printf("unable to load image: %s, SDL error: %s\n", path, SDL_GetError());
		raw = SDL_LoadBMP("assets/missing.bmp");
	}

	SDL_SetColorKey(raw, SDL_TRUE, SDL_MapRGB(raw->format, 0, 0xFF, 0));

	SDL_Texture *texture = NULL;
	texture = SDL_CreateTextureFromSurface(rend, raw);
	if (texture == NULL) {
		printf("unable to create texture from: %s, SDL error: %s\n", path, SDL_GetError());
		return NULL;
	}

	SDL_FreeSurface(raw);
	return texture;
}

/* draw an animated entity */
void render_dynamic(sprite_t *entity)
{
	/* find the correct src from a frame index and image dimensions */
	SDL_Rect source = {
		(entity->current_frame % entity->frames_per_row) * entity->tile_w,
		(entity->current_frame / entity->frames_per_row) * entity->tile_h,
		entity->tile_w,
		entity->tile_h
	};

	SDL_Rect target = {
		(int)entity->x, (int)entity->y,
		entity->w, entity->h
	};
	
	SDL_RenderCopyEx(
		rend, entity->texture, &source, &target, 0.00, NULL, SDL_FLIP_NONE);
}

/* draw a static entity */
void render_static(sprite_t *entity)
{
	SDL_Rect src = {
		0,0,
		entity->tile_w,
		entity->tile_h
	};

	SDL_Rect dst = {
		(int)entity->x, (int)entity->y,
		entity->w, entity->h
	};
	
	SDL_RenderCopyEx(
		rend, entity->texture, &src, &dst, 0.00, NULL, SDL_FLIP_NONE);
}
