#include "Tile.h"

Tile::Tile(int x, int y, int w, int h, int type)
{
    collisionBox.x = x;
    collisionBox.y = y;
    collisionBox.w = w;
    collisionBox.h = h;
    this->type = type;
}

bool Tile::render(SDL_Rect& camera)
{
    bool success == true;
    // check if box is in camera
    if (isColliding(camera, collisionBox))
    {
        // success == //gtiletexture.render //of type LTexture
        //returns success
    }
    if (!success)
    {
        SDL_Log("error occured rendering the tile")
    }
    return success;
}

int Tile::getType()
{
    return type;
}

SDL_Rect Tile::getCollisionBox()
{
    return collisionBox;
}