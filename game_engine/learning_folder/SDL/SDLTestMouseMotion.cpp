#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
#include <assert.h>

int main(int argc, char* argv[]) {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event event;
    bool quit = false;
    int result = 1;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("./Raleway-Medium.ttf", 24);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0);
        std::cout << "Load Mixer Error: " << Mix_GetError() << std::endl;//error check for mixer
    
    Mix_Music *SelectOST = Mix_LoadMUS("./test1.wav");
    Mix_Chunk *SelectMusic = Mix_LoadWAV("./MenuSelect.wav");
    Mix_PlayMusic(SelectOST, -1); //Plays our OST music
  
    SDL_Color color = { 255, 0, 0, 255 };
    // create 3 surfaces, one for each option
    SDL_Surface *surface1 = TTF_RenderText_Solid(font, "Option 1", color);
    SDL_Surface *surface2 = TTF_RenderText_Solid(font, "Option 2", color);
    SDL_Surface *surface3 = TTF_RenderText_Solid(font, "Option 3", color);
    SDL_Surface *surface4 = TTF_RenderText_Solid(font, "Quit", color);
    // create 3 textures, one for each option
    SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
    SDL_Texture *texture4 = SDL_CreateTextureFromSurface(renderer, surface4);
    // free the surfaces
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);
    SDL_FreeSurface(surface4);
    // create 3 rects, one for each option
    SDL_Rect rect1 = { 100, 40, 400, 100 };// {start (x,y) then length}
    SDL_Rect rect2 = { 100, 140, 400, 100 };
    SDL_Rect rect3 = { 100, 240, 400, 100 };
    SDL_Rect rect4 = { 100, 340, 200, 100 };
    // create a rect for the cursor
    SDL_Rect cursor = { 45, 60, 50, 50 };
    // create a texture for the cursor
    SDL_Surface *surfaceCursor = TTF_RenderText_Solid(font, ">", color);
    SDL_Texture *textureCursor = SDL_CreateTextureFromSurface(renderer, surfaceCursor);
    SDL_FreeSurface(surfaceCursor);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture1, NULL, &rect1);
    SDL_RenderCopy(renderer, texture2, NULL, &rect2);
    SDL_RenderCopy(renderer, texture3, NULL, &rect3);
    SDL_RenderCopy(renderer, texture4, NULL, &rect4);
    SDL_RenderCopy(renderer, textureCursor, NULL, &cursor);
    SDL_RenderPresent(renderer);
    int played;//-------------------test
    //while program is running, wait for user input

    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;
              case SDL_MOUSEMOTION: {
                SDL_MouseMotionEvent *m = (SDL_MouseMotionEvent*)&event;
                //assert(m->state == 0);
                int x, y;
                SDL_GetMouseState(&x, &y);
                //assert(x == m->x);//throwing error when moving to fast (may need to be a void function to do mouse)
                //assert(y == m->y);//our problem child is here
                printf("motion: %d,%d  %d,%d\n", m->x, m->y, m->xrel, m->yrel);
                result += 2 * (m->x + m->y + m->xrel + m->yrel);
                break;
            }

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
                //move cursor up
                cursor.y -= 100;
                played = Mix_PlayChannel(-1, SelectMusic, 0);//Plays select sound
                if (played == -1){
                    std::cout << "error ";//error check to see if it returns -1
                }
                //if cursor is off the top of the screen, move it to the bottom
                if (cursor.y < 60)
                {
                    cursor.y = 360;
                }
                break;
            case SDLK_DOWN:
                //move cursor down
                cursor.y += 100;
                Mix_PlayChannel(-1, SelectMusic, 0);//Plays select sound
                //if cursor is off the bottom of the screen, move it to the top
                if (cursor.y > 360)
                {
                    cursor.y = 60;
                }
                break;
                
            case SDLK_RETURN:
            //checks for enter press
                if(cursor.y == 360)
                {
                    goto labelquit;
                    std::cout << "goto quit error: " << std::endl;
                }
                break;
            }
        }


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);
        SDL_RenderCopy(renderer, texture3, NULL, &rect3);
        SDL_RenderCopy(renderer, texture4, NULL, &rect4);
        SDL_RenderCopy(renderer, textureCursor, NULL, &cursor);
        SDL_RenderPresent(renderer);
    }

    labelquit:

    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
    SDL_DestroyTexture(texture4);
    SDL_DestroyTexture(textureCursor);
    TTF_CloseFont(font);
    TTF_Quit();
    
    Mix_FreeChunk(SelectMusic);
    Mix_FreeMusic(SelectOST);
    Mix_CloseAudio();
    SelectMusic = nullptr;  
    SelectOST = nullptr;
    Mix_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
    
}
