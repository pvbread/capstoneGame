#pragma once

/*
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
*/

#include "TextureWrapper.h"
#include "TileType.h"

class Tile
{
public:
    //make this an enum type
    Tile(int x, int y, int w, int h, TileType tileType);
    bool render(SDL_Renderer* renderer, 
                TextureWrapper& tileTexture, 
                const SDL_Rect& camera, 
                const std::vector<SDL_Rect>& tileClips);
    TileType getType();
    SDL_Rect getCollisionBox();
private:
    // Tile attributes
    SDL_Rect collisionBox;
    TileType tileType;
};