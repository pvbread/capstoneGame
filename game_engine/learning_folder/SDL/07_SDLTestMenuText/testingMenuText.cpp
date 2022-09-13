//to compile on mac: g++ testingMenuText.cpp -I/Library/Frameworks/SDL2.framework -F/Library/Frameworks -framework SDL2 -I/Library/Frameworks/SDL2_ttf.framework/Headers -framework SDL2_ttf

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

int main()
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event event;
    bool quit = false;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("./Raleway-Medium.ttf", 24);

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

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture1, NULL, &rect1);
    SDL_RenderCopy(renderer, texture2, NULL, &rect2);
    SDL_RenderCopy(renderer, texture3, NULL, &rect3);
    SDL_RenderCopy(renderer, textureCursor, NULL, &cursor);
    SDL_RenderPresent(renderer);

    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                cursor.y -= 100;
                //if cursor is off the top of the screen, move it to the bottom
                if (cursor.y < 160)
                {
                    cursor.y = 360;
                }
                break;
            case SDLK_DOWN:
                cursor.y += 100;
                //if cursor is off the bottom of the screen, move it to the top
                if (cursor.y > 360)
                {
                    cursor.y = 160;
                }
                break;
            }
            break;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture1, nullptr, &rect1);
        SDL_RenderCopy(renderer, texture2, nullptr, &rect2);
        SDL_RenderCopy(renderer, texture3, nullptr, &rect3);
        SDL_RenderCopy(renderer, textureCursor, nullptr, &cursor);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
    SDL_DestroyTexture(textureCursor);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}