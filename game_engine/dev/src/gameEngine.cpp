#include "GameEngine.h"

#include "TextureWrapper.h"
#include "Tile.h"
#include "CharacterInMap.h"
#include "MapDebugController.h"
//#include "loadMedia.h"
#include "Screen.h"

#include "pch.h"
/*
#include <iostream>
#include <sstream>
#include <unordered_map>
*/

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

    if (TTF_Init() < 0)
        SDL_Log("TTF Init error!");

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        SDL_Log("Load Mixer Error: %s", Mix_GetError());  
}

Phoenix::~Phoenix()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Phoenix::runGameLoop()
{}

void Phoenix::stopGameLoop()
{
    quit = true;
}

bool Phoenix::loadTiles(std::vector<Tile*>& tileSet, 
                        std::vector<SDL_Rect>& tilesClipped, 
                        int TILE_COUNT, 
                        int TYPE_COUNT, 
                        int TILE_LENGTH)
{
    int x = 0;
    int y = 0;

    std::ifstream level("testLevel.map");

    if (level.fail())
    {
        SDL_Log("Failure loading level");
        return false;
    }

    //tyle type
    int tileType;

    for (int i = 0; i < TILE_COUNT; i++)
    {
        level >> tileType;

        if (level.fail())
        {
            SDL_Log("EOF error with level read at %d", i);
            return false;
        }

        //need to cast tileType here otherwise
        //it can't read it in right
        if (tileType >= 0 && tileType < TYPE_COUNT)
        {
            tileSet[i] = new Tile(x, y, TILE_LENGTH, TILE_LENGTH, (TileType)tileType);
        }
        //TODO DON't HARD CODE THIS
        //MAP_WIDTH
        x += TILE_LENGTH;

        if (x >= 1280)
        {
            x = 0;
            y += TILE_LENGTH;
        }
       
    }

    
    //close file
    level.close();

    //clip
    //math is for 4 x 3 tile map 
    //we have 240 x 160 if tile size is 80 (factor this away later)
    //so iterate on the smaller y dimension and set the boundaries of the 
    //rectangles accordingly
    tileType = 0;
    for (y = 0; y < 3; y++)
    {
        for (x = 0; x < 4; x++)
        { 
            tilesClipped[tileType].x = x*TILE_LENGTH;
            tilesClipped[tileType].y = y*TILE_LENGTH;
            tilesClipped[tileType].w = TILE_LENGTH;
            tilesClipped[tileType].h = TILE_LENGTH;
            tileType++;
        }
    }

    return true;
}

bool Phoenix::loadImageAssets(SDL_Renderer* renderer,  
                              std::vector<Tile*>& tileSet, 
                              std::vector<SDL_Rect>& tilesClipped,
                              std::unordered_map<TextureWrapper*, std::string> textureFilePaths)
{
    for (auto [texturePtr, textureFilePath]: textureFilePaths)
    {
        bool didTextureLoad = texturePtr->loadImage(renderer, textureFilePath);
        if (!didTextureLoad)
        {
            SDL_Log("Failed to load tiles");
            return false;
        }
    }

    //level width 1280, level height 960
    //divided by 80 is 16x12 = 192
    const int TILE_COUNT = 192;
    const int TILE_LENGTH = 80;
    const int TYPE_COUNT = 12;

    if (!loadTiles(tileSet, tilesClipped, TILE_COUNT, TYPE_COUNT, TILE_LENGTH))
    {
        SDL_Log("Failed to load tile set");
        return false;
    }

    return true;
}

SDL_Window* Phoenix::getWindow() const
{
    return window;
}

SDL_Renderer* Phoenix::getRenderer() const
{
    return renderer;
}

bool Phoenix::getQuit() const
{
    return quit;
}
    
int Phoenix::getWidth() const
{
    return width;
}
    
int Phoenix::getHeight() const
{
    return height;
}

void Phoenix::setToQuit()
{
    quit = !quit;
}