#include "gameEngine.h"
#include "TextureWrapper.h"
#include "Tile.h"
#include "MapDebugController.h"
#include "loadMedia.h"


#include <iostream>

Phoenix::Phoenix(Uint32 flags, const char* title, int x, int y, int w, int h)
{
    this->height = h;
    this->width = w;

    if (SDL_Init(flags) < 0)
    {
        SDL_Log("Init error!");
    }

    if(SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) == SDL_FALSE)
    {
        SDL_Log("Linear texture filtering wasn't enabled");
    }

    window = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_Log("Error creating window");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr)
    {
        SDL_Log("Renderer wasn't created properly");
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_Log("Error initializing SDL_image");
    }
}

Phoenix::~Phoenix()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Phoenix::runGameLoop()
{
    // temporary place for this
    TextureWrapper tileTexture;
    TextureWrapper debugControllerTexture;
    std::vector<TextureWrapper> textureWrappers{tileTexture, debugControllerTexture};
    std::vector<Tile> tiles;

    if (!loadImageAssets(renderer, textureWrappers, tiles))
    {
        SDL_Log("error loading image assets");
        quit = true;
    }

    //creating random test tile to check if the code runs
    Tile tile(10,10,10,10,1);
    MapDebugController mdc;

    SDL_Rect camera = {0,0, 640, 480};

    double degrees = 0;
    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    SDL_Event event;
    while (!quit)
    {
        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
            {
                quit = true;
                break;
            }
            //only for testing flips
            case SDL_KEYDOWN:
            {
                switch( event.key.keysym.sym )
                {
                    case SDLK_a:
                    degrees -= 90;
                    break;
                    
                    case SDLK_d:
                    degrees += 90;
                    break;

                    case SDLK_q:
                    flipType = SDL_FLIP_HORIZONTAL;
                    break;

                    case SDLK_w:
                    flipType = SDL_FLIP_NONE;
                    break;

                    case SDLK_e:
                    flipType = SDL_FLIP_VERTICAL;
                    break;
                }
            }
        }

        //Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        //Render arrow
        //w, h are screen width and screen height
        // different flip types
        // SDL_FLIP_VERTICAL
        // SDL_FLIP_HORIZONTAL
        // SDL_FLIP_NONE
        tileTexture.render(renderer, 0, 0, nullptr, degrees, nullptr, flipType);
        tileTexture.render(renderer, 80, 0, nullptr, degrees, nullptr, SDL_FLIP_HORIZONTAL);

        //Update screen
        SDL_RenderPresent(renderer);
        
    }
    
}

void Phoenix::stopGameLoop()
{
    quit = true;
}