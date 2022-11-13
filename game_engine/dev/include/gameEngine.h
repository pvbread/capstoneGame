#pragma once

#include "pch.h"
#include "TileType.h"
#include "Tile.h"
#include "TextBox.h"
#include "Color.h"
#include "Font.h"
#include "Timer.h"

class Phoenix
{
public:
    Phoenix(Uint32 flags, const char* title, int x, int y, int w, int h);
    ~Phoenix();
    virtual void runGameLoop();
    void stopGameLoop();
    bool loadTiles(std::vector<Tile*>& tileMap, 
                   std::map<std::pair<int, int>, TileType>& coordinateToTileTypeMap,
                   std::map<std::pair<int, int>, std::string>& coordinateToEventTypeMap,  
                   int TILE_COUNT, 
                   int TYPE_COUNT, 
                   int TILE_LENGTH
    );
    bool loadImageAssets(SDL_Renderer* renderer, 
                         std::unordered_map<TextureWrapper*, std::string> textureFilePaths
    );
    bool clipSheet(int ROWS,
                   int COLS, 
                   int BLOCK_LENGTH,
                   int BLOCK_HEIGHT,
                   int TYPE_COUNT,
                   std::vector<SDL_Rect>& sheetClipped
    );
    SDL_Window* getWindow() const;
    SDL_Renderer* getRenderer() const;
    bool getQuit() const;
    int getWidth() const;
    int getHeight() const;
    void setToQuit();
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool quit = false;
    int width;
    int height;
};