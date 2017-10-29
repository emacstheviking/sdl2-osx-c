#include <SDL.h>
#include <stdio.h>

// Building on 02, this version shows how to use keyboard events to
// control the position and size of a simple shape on the screen. This
// should give you some insight into how to control *anything* by using
// the keyboard.
//
// Compiled on my Mac with:
//  gcc `sdl2-config --libs --cflags` -o gfx gfx.c

typedef struct _st_shape {

    int xpos;
    int ypos;
    int dx;
    int dy;
    int width;
    int height;

} SHAPE;


void centre_shape(SDL_Window* w, SHAPE* s)
{
    if (w && s) {

	int widthScreen, heightScreen;
	SDL_GetWindowSize(w, &widthScreen, &heightScreen);

	int newShapeX = ( widthScreen / 2 ) - ( s->width / 2 ); 
	int newShapeY = ( heightScreen / 2 ) - ( s->height / 2 ); 

	s->xpos = newShapeX;
	s->ypos = newShapeY;
    }
}


void draw_shape(SDL_Renderer* r, SHAPE* s)
{
    if (r && s) {
	// top horizontal line of the box
	SDL_RenderDrawLine(r, s->xpos, s->ypos, s->xpos+s->width-1, s->ypos);

	// bottom horizontal line of the box
	SDL_RenderDrawLine(r, s->xpos, s->ypos+s->height-1, s->xpos+s->width-1, s->ypos+s->height-1);

	// left vertical
	SDL_RenderDrawLine(r, s->xpos, s->ypos, s->xpos, s->ypos+s->height-1);

	// right vertical
	SDL_RenderDrawLine(r, s->xpos+s->width-1, s->ypos, s->xpos+s->width-1, s->ypos+s->height-1);

	// performance note: you *can* create an array of contiguous lines and 
	// use SDL_DrawLines() but I am keeping it simple here!
    }
}


int main(int argc, char* argv[])
{
    printf("Your SDL version: %s\n", SDL_GetRevision());

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("failed to SDL_Init: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* w = SDL_CreateWindow(
        "SDL is easy!",
        0, 0,
        800, 800,
        SDL_WINDOW_RESIZABLE);

    if (NULL == w) {
        printf("failed to SDL_CreateWindow: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* r = SDL_CreateRenderer(
        w,
        -1,
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    if (NULL == r) {
        printf("failed to SDL_CreateRenderer: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Event e;
    SDL_bool stop = SDL_FALSE;
    int quitTrigger = 3;
    int quitCount = 0;
    //
    // Initialise our shape. 100 pixels square, pixel rate on move is
    // 1 pixel per update.
    //
    SHAPE shape = {0};
    shape.width = shape.height = 100;
    shape.dx = shape.dy = 1;
    centre_shape(w, &shape);


    do {
	SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
	SDL_RenderClear(r);
	SDL_SetRenderDrawColor(r, 255, 0, 0, 255);

	draw_shape(r, &shape);

	SDL_RenderPresent(r);


        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                stop = SDL_TRUE;
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
		    stop = (++quitCount >= quitTrigger);
                    break;
                default:
                    quitCount = 0;
                    break;
                }

		case SDL_KEYDOWN:
		    // Here is where we decide what to do to the shape
		    // (if anything) according to which key down event
		    // just happened.
		    switch(e.key.keysym.sym) {
			case SDLK_UP:
			    shape.ypos -= shape.dy;
			    break;
			case SDLK_DOWN:
			    shape.ypos += shape.dy;
			    break;
			case SDLK_LEFT:
			    shape.xpos -= shape.dx;
			    break;
			case SDLK_RIGHT:
			    shape.xpos += shape.dx;
			    break;
		    }
		    break;
            default:
                break;
            }
        }
    } while (!stop);

    return 0;
}
