
//sprite construct for game entities
typedef struct sprite {
	int current_frame;
	int frames_per_row;
	float x, y, vx, vy, ax, ay;
	int w, h;
	int dir;
	int state;
	int tile_w, tile_h, img_w, img_h;
	SDL_Texture *texture;
} sprite_t;

//vector
typedef struct vecf {
	float x, y, z;
} vecf_t;

//color
typedef struct color {
	Uint8 r, g, b;
} color_t;

//different animation states
enum anim_type {
	WALKING,
	RUNNING,
	IDLE
};
	
//direction of the current animation
enum anim_direction {
	FORWARD,
	RIGHT,
	BACK,
	LEFT
};
