CC = g++

INCLUDE_PATHS = -I/usr/include/x86_64-linux-gnu/SDL2 -Wall -02
LIBRARY_PATHS = -L/usr/lib/x86_64-linux-gnu

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2main -lSDL2 -lSDL2_ttf

OBJS = objs/main.o objs/ui.o
OBJ_NAME = rev

$(OBJ_NAME) : $(OBJS)
	$(CC) $(OBJS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)

objs/main.o : main.c
	$(CC) -c main.c -o objs/main.o

objs/ui.o : ui.c ui.h
	$(CC) -c ui.c -o objs/ui.o

clean :
	rm objs/*.o
	rm $(OBJ_NAME)
