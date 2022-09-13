//to compile on mac: g++ testingMenuText.cpp -I/Library/Frameworks/SDL2.framework -F/Library/Frameworks -framework SDL2 -I/Library/Frameworks/SDL2_ttf.framework/Headers -framework SDL2_ttf -I/Library/Frameworks/SDL2_mixer.framework/Headers -framework SDL2_mixer

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>

//demo menu with 3 text options and sound effects every time you select an option

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

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("./Raleway-Medium.ttf", 18);

    SDL_Color color = { 255, 0, 0, 255 };
    // create 3 surfaces, one for each option
    SDL_Surface *surface1 = TTF_RenderText_Solid(font, "Option 1", color);
    SDL_Surface *surface2 = TTF_RenderText_Solid(font, "Option 2", color);
    SDL_Surface *surface3 = TTF_RenderText_Solid(font, "Option 3", color);

    // create 3 textures, one for each option
    SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer, surface3);

    // free the surfaces
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);

    // create 3 rects, one for each option
    SDL_Rect rect1 = { 100, 140, 400, 100 };
    SDL_Rect rect2 = { 100, 240, 400, 100 };
    SDL_Rect rect3 = { 100, 340, 400, 100 };

    // create a rect for the cursor
    SDL_Rect cursor = { 45, 160, 50, 50 };

    // create a texture for the cursor
    SDL_Surface *surfaceCursor = TTF_RenderText_Solid(font, ">", color);

    SDL_Texture *textureCursor = SDL_CreateTextureFromSurface(renderer, surfaceCursor);

    SDL_FreeSurface(surfaceCursor);

    // create a sound effect for cursor movement
    Mix_Chunk *cursorMove = Mix_LoadWAV("./menuMove.wav");

    // create a sound effect for selection
    Mix_Chunk *selection = Mix_LoadWAV("./menuSelect.wav");

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture1, nullptr, &rect1);
    SDL_RenderCopy(renderer, texture2, nullptr, &rect2);
    SDL_RenderCopy(renderer, texture3, nullptr, &rect3);
    SDL_RenderCopy(renderer, textureCursor, nullptr, &cursor);
    SDL_RenderPresent(renderer);

    // get user input while the user hasn't quit

    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
            quit = true;
            break;
        }

        // if the user presses the up arrow key
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP)
        {
            // play the cursor move sound effect
            Mix_PlayChannel(-1, cursorMove, 0);

            // move the cursor up
            cursor.y -= 100;

            // if the cursor is out of bounds, move it back down
            if (cursor.y < 160)
            {
                cursor.y = 360;
            }
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN)
        {
            // play the cursor move sound effect
            Mix_PlayChannel(-1, cursorMove, 0);

            // move the cursor down
            cursor.y += 100;

            // if the cursor is out of bounds, move it back up
            if (cursor.y > 360)
            {
                cursor.y = 160;
            }
        }
        // if the user presses the return key

        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
        {
            // play the selection sound effect
            Mix_PlayChannel(-1, selection, 0);
        }

        // render the menu
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture1, nullptr, &rect1);
        SDL_RenderCopy(renderer, texture2, nullptr, &rect2);
        SDL_RenderCopy(renderer, texture3, nullptr, &rect3);
        SDL_RenderCopy(renderer, textureCursor, nullptr, &cursor);
        SDL_RenderPresent(renderer);
        
    }

    // free the textures
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
    SDL_DestroyTexture(textureCursor);

    // free the font
    TTF_CloseFont(font);


    // free the sound effects
    Mix_FreeChunk(cursorMove);
    Mix_FreeChunk(selection);

    // close the mixer
    Mix_CloseAudio();

    // close the font system
    TTF_Quit();

    // destroy the renderer
    SDL_DestroyRenderer(renderer);

    // destroy the window
    SDL_DestroyWindow(window);

    // quit SDL
    SDL_Quit();
}