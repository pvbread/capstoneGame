#pragma once

#include "TextureWrapper.h"
#include "loadTiles.h"

//TODO factor this to take an unordered map of Wrappers to paths

bool loadImageAssets(SDL_Renderer* renderer, std::vector<TextureWrapper>& textureWrappers, std::vector<Tile> tiles)
{
    //TODO Fix this hard coding
    if (!textureWrappers[0].loadImage(renderer, "tilesDraft.png"))
    {
        SDL_Log("Failed to load tiles");
        return false;
    }

    if (!textureWrappers[0].loadImage(renderer, "dot.bmp"))
    {
        SDL_Log("Failed to load debug controller");
        return false;
    }

    //level width 1280, level height 960
    //divided by 80 is 16x12 = 192
    const int TILE_COUNT = 192;

    if (!loadTiles(tiles, TILE_COUNT))
    {
        SDL_Log("Failed to load tile set");
        return false;
    }

    return true;
}