#include "Tile.h"
#include "TextureWrapper.h"
#include "isColliding.h"
//#include <vector>

Tile::Tile(int x, int y, int w, int h, TileType tileType)
{
    collisionBox.x = x;
    collisionBox.y = y;
    collisionBox.w = w;
    collisionBox.h = h;
    this->tileType = tileType;
}

bool Tile::render(SDL_Renderer *renderer, 
                  TextureWrapper& tileTexture, 
                  const SDL_Rect& camera, 
                  const std::vector<SDL_Rect>& tileClips)
{
    bool success = true;
    // check if box is in camera
    if (isColliding(&camera, &collisionBox))
    {
        success =  tileTexture.render(renderer,
                                      collisionBox.x - camera.x, 
                                      collisionBox.y - camera.y,
                                      &tileClips[tileType]);
    }
    if (!success)
    {
        SDL_Log("error occured rendering the tile");
    }
    return success;
}

int Tile::getType()
{
    return tileType;
}

SDL_Rect Tile::getCollisionBox()
{
    return collisionBox;
}