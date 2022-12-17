#include "gameEngine.h"

#include <iostream>

Phoenix::Phoenix(Uint32 flags, const char* title, int x, int y, int w, int h)
{
    this->height = h;
    this->width = w;

    if (SDL_Init(flags) < 0)
    {
        SDL_Log(SDL_GetError());
    }
    window = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
}

Phoenix::~Phoenix()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Phoenix::runGameLoop()
{
    SDL_Event event;
    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect rect = { 320, 240, 100, 100 };

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);
    }
    
}

void Phoenix::stopGameLoop()
{
    quit = true;
}