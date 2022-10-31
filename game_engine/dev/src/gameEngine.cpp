#include "GameEngine.h"

#include "TextureWrapper.h"
#include "Tile.h"
#include "CharacterInMap.h"
#include "MapDebugController.h"
#include "Screen.h"
#include "TileType.h"
#include "BaseMenu.h"

#include "pch.h"


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

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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

bool Phoenix::loadTiles(std::vector<Tile*>& tileMap, 
                        std::map<std::pair<int, int>, TileType>& coordinateToTileTypeMap,
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
            SDL_Log("Error with level read at %d", i);
            return false;
        }

        //need to cast tileType here otherwise
        //it can't read it in right
        if (tileType >= 0 && tileType < TYPE_COUNT)
        {
            tileMap[i] = new Tile(x, y, TILE_LENGTH, TILE_LENGTH, (TileType)tileType);
            std::pair<int, int> coordinates = std::make_pair(x,y);
            coordinateToTileTypeMap[coordinates] = (TileType)tileType; 
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
    level.close();

    return true;

}

bool Phoenix::loadImageAssets(SDL_Renderer* renderer,  
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

    return true;
}

bool Phoenix::clipSheet(int ROWS,
                        int COLS, 
                        int BLOCK_LENGTH,
                        int BLOCK_HEIGHT,
                        int TYPE_COUNT,
                        std::vector<SDL_Rect>& sheetClipped
                       )
{
    int blockType = 0;
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        { 
            if (blockType == TYPE_COUNT)
            {
                SDL_Log("Error clipping");
                return false;
            }
            //c and r might have to get flipped when not square
            sheetClipped[blockType].x = c*BLOCK_LENGTH;
            sheetClipped[blockType].y = r*BLOCK_HEIGHT;
            sheetClipped[blockType].w = BLOCK_LENGTH;
            sheetClipped[blockType].h = BLOCK_HEIGHT;
            blockType++;
        }
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