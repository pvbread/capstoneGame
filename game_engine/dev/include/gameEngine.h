#pragma once

/*
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
*/
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
    bool loadTiles(std::vector<Tile*>& tileSet, 
                   std::vector<SDL_Rect>& tilesClipped, 
                   int TILE_COUNT, 
                   int TYPE_COUNT, 
                   int TILE_LENGTH);
    bool loadImageAssets(SDL_Renderer* renderer,  
                         std::vector<Tile*>& tileSet, 
                         std::vector<SDL_Rect>& tilesClipped,
                         std::unordered_map<TextureWrapper*, std::string> textureFilePaths);
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