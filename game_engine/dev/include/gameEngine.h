#pragma once

#include "pch.h"
#include "TileType.h"
#include "Tile.h"

class Phoenix
{
public:
    Phoenix(Uint32 flags, const char* title, int x, int y, int w, int h);
    ~Phoenix();
    virtual void runGameLoop();
    void stopGameLoop();
    bool loadTiles(std::vector<Tile*>& tileMap, 
                   std::vector<SDL_Rect>& tilesClipped,
                   std::map<std::pair<int, int>, TileType>& coordinateToTileTypeMap, 
                   int TILE_COUNT, 
                   int TYPE_COUNT, 
                   int TILE_LENGTH);
    bool loadImageAssets(SDL_Renderer* renderer,  
                         std::vector<Tile*>& tileMap, 
                         std::vector<SDL_Rect>& tilesClipped,
                         std::unordered_map<TextureWrapper*, std::string> textureFilePaths,
                         std::map<std::pair<int, int>, TileType>& coordinateToTileTypeMap);
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