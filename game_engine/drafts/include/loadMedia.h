#include "TextureWrapper.h"

bool loadImageAssets(SDL_Renderer* renderer, TextureWrapper& textureWrapper)
{
    //TODO Fix this hard coding
    if (!textureWrapper.loadImage(renderer, "myTile.png"))
    {
        SDL_Log("Failed to load assets");
        return false;
    }
    return true;
}