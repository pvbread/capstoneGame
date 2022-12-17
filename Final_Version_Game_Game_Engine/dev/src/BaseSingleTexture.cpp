#include "BaseSingleTexture.h"

BaseSingleTexture::BaseSingleTexture(int mainVelocity, 
                               int velocityX, 
                               int velocityY, 
                               SDL_Rect collisionBox)                       
{
    this->mainVelocity = mainVelocity;
    this->velocityX = velocityX;
    this->velocityY = velocityY;
    this->collisionBox.x = collisionBox.x;
    this->collisionBox.y = collisionBox.y;
    this->collisionBox.w = collisionBox.w;
    this->collisionBox.h = collisionBox.h;
}


void BaseSingleTexture::centerScreen(SDL_Rect& camera)
{
    // finds where the center of the texture is and positions the
    // top left portion of the camera accordingly
    camera.x = collisionBox.x - (camera.w/2);
    camera.y = collisionBox.y - (camera.h/2);

    //bounds checking, hardcoded for our dimensions
    //this is actually fixed in the texturess that inherit this classs
    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > 1280 - camera.w)
        camera.x = 1280 - camera.w;
    if (camera.y > 960 - camera.h)
        camera.y = 960 - camera.h;
}

SDL_Rect BaseSingleTexture::getCollisionBox() const
{
    return collisionBox;
}