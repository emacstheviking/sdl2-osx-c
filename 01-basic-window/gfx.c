#include <SDL.h>
#include <stdio.h>

// This code contains the error checking I left out of the article
// for reasons of brevity and clarity.
//
// Compiled on my Mac with:
//  gcc `sdl2-config --libs --cflags` -o gfx gfx.c
//

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

    do {
        SDL_PollEvent(&e);
    } while (e.type != SDL_QUIT);

    return 0;
}
