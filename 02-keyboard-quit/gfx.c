#include <SDL.h>
#include <stdio.h>

// Building on 01, this version shows how to scan for an event
// and implement an ESC key handler that will terminate the application
// if the key is pressed three times in a row.
//
// Compiled on my Mac with:
//  gcc `sdl2-config --libs --cflags` -o gfx gfx.c

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
        800, 200,
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

    do {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                // Cmd+Q or Window close icon, immediate exit !
                stop = SDL_TRUE;
                break;
            case SDL_KEYUP:
                // Key released, was it ESC ?
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quitCount++;
                    if (quitCount >= quitTrigger) {
                        stop = SDL_TRUE;
                    }
                    // one line guru code:
                    //     stop = (++quitCount >= quitTrigger);
                    break;
                default:
                    quitCount = 0;
                    break;
                }
            default:
                break;
            }
        }
    } while (!stop);

    return 0;
}
