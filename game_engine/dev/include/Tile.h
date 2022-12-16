#pragma once

/*
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
*/

#include "TextureWrapper.h"
#include "TileType.h"

/// @brief Your friendly next-door-tile. Does everything
/// a tile can. It's the backbone class for our map.
class Tile
{
public:

    Tile(int x, int y, int w, int h, TileType tileType);

    /// @brief 
    /// @param renderer 
    /// @param tileTexture takes a particular texture from tileSheet
    /// @param camera needs the camera position to know whether to render or not
    /// @param tileClips vector of the coordinates to render in
    /// @return 
    bool render(SDL_Renderer* renderer, 
                TextureWrapper& tileTexture, 
                const SDL_Rect& camera, 
                const std::vector<SDL_Rect>& tileClips);

    TileType getType();

    SDL_Rect getCollisionBox();
private:
    SDL_Rect collisionBox;
    TileType tileType;
};