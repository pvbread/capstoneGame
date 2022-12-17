#include <SDL2/SDL.h>

//draws a white canvas with a blue square that moves around the screen

int main(int argv, char** args)
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event event;
    bool quit = false;
    
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Rect rect = { 320, 240, 100, 100 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    rect.y -= 10;
                    break;
                case SDLK_DOWN:
                    rect.y += 10;
                    break;
                case SDLK_LEFT:
                    rect.x -= 10;
                    break;
                case SDLK_RIGHT:
                    rect.x += 10;
                    break;
                }
                //redraw the rectangle
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
                SDL_RenderPresent(renderer);
            }

            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
    
}