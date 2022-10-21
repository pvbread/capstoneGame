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
    // TODO fix this hardcoding to take in any size
    camera.x = collisionBox.x - (640/2);
    camera.y = collisionBox.y - (480/2);

    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > 1280 - camera.w)
        camera.x = 1280 - camera.w;
    if (camera.y > 960 - camera.h)
        camera.y = 960 - camera.h;
}