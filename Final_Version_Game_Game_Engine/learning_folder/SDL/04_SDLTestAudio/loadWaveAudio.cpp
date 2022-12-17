#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

//plays a wave file

int main()
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event event;
    bool quit = false;

    SDL_Init(SDL_INIT_VIDEO);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    Mix_Music *music = Mix_LoadMUS("test1.wav");
    Mix_PlayMusic(music, -1);

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

    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
    
}