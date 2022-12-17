#include "GameEngine.h"

#include "TextureWrapper.h"
#include "Tile.h"
#include "CharacterInMap.h"
#include "MapDebugController.h"
#include "Screen.h"
#include "TileType.h"
#include "BaseMenu.h"

#include "pch.h"


 
Archimedes::Archimedes(Uint32 flags, const char* title, int x, int y, int w, int h)
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

Archimedes::~Archimedes()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/// @brief The loop that our game takes over to work withs
void Archimedes::runGameLoop()
{}

void Archimedes::stopGameLoop()
{
    quit = true;
}


bool Archimedes::loadTiles(std::vector<Tile*>& tileMap, 
                        const std::vector<int>& levelInfo,
                        std::map<std::pair<int, int>, TileType>& coordinateToTileTypeMap,
                        std::map<std::pair<int, int>, std::string>& coordinateToEventTypeMap,
                        int TILE_COUNT, 
                        int TYPE_COUNT, 
                        int TILE_LENGTH)
{
    //we feed this function a pre-parsed vector of all the level info
    //the vector contains ints that map both the coordinate dimensions
    //and tile types
    //this is where we also populate events and coordinate types

    int x = 0;
    int y = 0;

    //tyle type
    int tileType;
    const int LEVEL_WIDTH = levelInfo[1]*80;

    //start at index 2 (because first 2 are the dimensions)
    for (int i = 0; i < TILE_COUNT; i++)
    {
        //need to offset the 2 dimensions
        tileType = levelInfo[i+2];

        //need to cast tileType here otherwise
        //it can't read it in right
        if (tileType >= 0 && tileType < TYPE_COUNT)
        {
            tileMap[i] = new Tile(x, y, TILE_LENGTH, TILE_LENGTH, (TileType)tileType);
            std::pair<int, int> coordinates = std::make_pair(x,y);
            coordinateToTileTypeMap[coordinates] = (TileType)tileType;
            if ((TileType)tileType != BLACK)
            coordinateToEventTypeMap[coordinates] = "";
        }
        
        x += TILE_LENGTH;

        if (x >= LEVEL_WIDTH)
        {
            x = 0;
            y += TILE_LENGTH;
        }
       
    }

    return true;

}

bool Archimedes::loadImageAssets(SDL_Renderer* renderer,  
                              std::unordered_map<TextureWrapper*, std::string> textureFilePaths)
{
    // we get around the issue of not being able to map objects
    // by mapping pointers to objects instead here
    // this way we have a factored function that can take a ton of textures
    // and load them efficiently
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

bool Archimedes::clipSheet(int ROWS,
                        int COLS, 
                        int BLOCK_LENGTH,
                        int BLOCK_HEIGHT,
                        int TYPE_COUNT,
                        std::vector<SDL_Rect>& sheetClipped
                       )
{
    // the clipping here basically takes a tile/sprite sheet
    // and maps the coordinates to SDL Rectangles
    // note the variable dimension size of the algorithm
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

SDL_Window* Archimedes::getWindow() const
{
    return window;
}

SDL_Renderer* Archimedes::getRenderer() const
{
    return renderer;
}


std::vector<int> Archimedes::convertMapToVector(std::string pathName)
{
    //preprocess our specific raw map data to vector form

    std::ifstream level(pathName);
    std::vector<int> levelData;
    int rows;
    int cols;
    int element;

    level >> rows;
    level >> cols;

    levelData.push_back(rows);
    levelData.push_back(cols);  

    for (int i = 0; i < rows * cols; i++)
    {
        level >> element;
        levelData.push_back(element);
    }
    level.close();

    return levelData;
}

bool Archimedes::getQuit() const

{
    return quit;
}
    
int Archimedes::getWidth() const
{
    return width;
}
    
int Archimedes::getHeight() const
{
    return height;
}

void Archimedes::setToQuit()
{
    quit = !quit;
}