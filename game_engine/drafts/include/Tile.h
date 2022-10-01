#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "TextureWrapper.h"

class Tile
{
public:
    Tile(int x, int y, int w, int h, int type);
    bool render(SDL_Renderer* renderer, TextureWrapper tileTexture, const SDL_Rect& camera, const std::vector<SDL_Rect>& tileClips);
    int getType();
    SDL_Rect getCollisionBox();
private:
    // Tile attributes
    SDL_Rect collisionBox;
    int type;
};