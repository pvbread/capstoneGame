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
    std::vector<TextureWrapper*> textureWrappers{&tileTexture, &debugControllerTexture};
    
    const int TILE_TYPE_COUNT = 12;
    const int TILE_COUNT = 192;
    std::vector<Tile*> tileSet(TILE_COUNT);
    std::vector<SDL_Rect> tilesClipped(TILE_COUNT);// this is the total tiles

    if (!loadImageAssets(renderer, textureWrappers, tileSet, tilesClipped))
    {
        SDL_Log("error loading image assets");
        quit = true;
    }

    MapDebugController debugController;

    SDL_Rect camera = {0,0, 640, 480};

    double degrees = 0;
    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    SDL_Event event;
    while (!quit)
    {
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            debugController.onInput(event);
        }
        
        //here have to poll event maybe in a loop?
        

        debugController.move(1280, 960);

        debugController.centerScreen(camera);

        //Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for(int i = 0; i < tileSet.size(); i++)
        {
            tileSet[i]->render(renderer, tileTexture, camera, tilesClipped);
        }

        //Render arrow
        //w, h are screen width and screen height
        // different flip types
        // SDL_FLIP_VERTICAL
        // SDL_FLIP_HORIZONTAL
        // SDL_FLIP_NONE
        //tileTexture.render(renderer, 0, 0, nullptr, degrees, nullptr, flipType);
        //tileTexture.render(renderer, 80, 0, nullptr, degrees, nullptr, SDL_FLIP_HORIZONTAL);

        debugController.render(renderer, camera, debugControllerTexture);
        //Update screen
        SDL_RenderPresent(renderer);
        
    }
    
}

void Phoenix::stopGameLoop()
{
    quit = true;
}