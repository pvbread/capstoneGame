#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

int main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    bool quit = false;

    SDL_Init(SDL_INIT_VIDEO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    Mix_Music *music = Mix_LoadMUS("test1.wav");
    Mix_PlayMusic(music, -1);

    SDL_Rect rect = { 320, 240, 100, 100 };

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
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
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

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}