#pragma once

#include "pch.h"

#include "TextureWrapper.h"
#include "loadTiles.h"

bool loadImageAssets(SDL_Renderer* renderer,  
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