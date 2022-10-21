#include "BaseSingleTexture.h"
#include "CharacterInMap.h"
#include "TextureWrapper.h"


CharacterInMap::CharacterInMap(int mainVelocity, 
                               int velocityX, 
                               int velocityY, 
                               SDL_Rect collisionBox) :
    BaseSingleTexture(mainVelocity, velocityX, velocityY, collisionBox)                   
{
    this->mainVelocity = mainVelocity;
    this->velocityX = velocityX;
    this->velocityY = velocityY;
    this->collisionBox.x = collisionBox.x;
    this->collisionBox.y = collisionBox.y;
    this->collisionBox.w = collisionBox.w;
    this->collisionBox.h = collisionBox.h;
}


void CharacterInMap::onInput(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_a:
            {
                collisionBox.x -= mainVelocity;
                break;
            }
            case SDLK_w:
            {
                collisionBox.y -= mainVelocity;
                break;
            }
            case SDLK_d:
            {
                collisionBox.x += mainVelocity;
                break;
            }
            case SDLK_s:
            {
                collisionBox.y += mainVelocity;
                break;
            }
        }
    }
}

void CharacterInMap::move(int xBoundary, int yBoundary)
{
    // TODO update to not be hard coded
    if(collisionBox.x <= 0 || collisionBox.x >= 1280)
    {
        collisionBox.x -= velocityX;
    }

    collisionBox.y += velocityY;
    if(collisionBox.y <= 0 || collisionBox.y >= 960)
    {
        collisionBox.y -= velocityY;
    }
}

void CharacterInMap::render(SDL_Renderer* renderer, const SDL_Rect& camera, TextureWrapper& characterTexture)
{
    //the debug object is drawn at it's distance from the camera's idea of 0
    characterTexture.render(renderer, collisionBox.x-camera.x, collisionBox.y-camera.y);
}


void CharacterInMap::centerScreen(SDL_Rect& camera)
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