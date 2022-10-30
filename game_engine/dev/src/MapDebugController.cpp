#include "BaseSingleTexture.h"
#include "MapDebugController.h"
#include "TextureWrapper.h"


MapDebugController::MapDebugController(int mainVelocity, 
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


void MapDebugController::onInput(SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT:
            {
                velocityX -= mainVelocity;
                break;
            }
            case SDLK_UP:
            {
                velocityY -= mainVelocity;
                break;
            }
            case SDLK_RIGHT:
            {
                velocityX += mainVelocity;
                break;
            }
            case SDLK_DOWN:
            {
                velocityY += mainVelocity;
                break;
            }
        }
    }
    if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT:
            {
                velocityX += mainVelocity;
                break;
            }
            case SDLK_UP:
            {
                velocityY += mainVelocity;
                break;
            }
            case SDLK_RIGHT:
            {
                velocityX -= mainVelocity;
                break;
            }
            case SDLK_DOWN:
            {
                velocityY -= mainVelocity;
                break;
            }
        }
    }
}

void MapDebugController::move(int xBoundary, int yBoundary)
{
    // TODO update to not be hard coded
    collisionBox.x += velocityX;
    if(collisionBox.x < 0 || collisionBox.x > 1280)
    {
        collisionBox.x -= velocityX;
    }

    collisionBox.y += velocityY;
    if(collisionBox.y < 0 || collisionBox.y > 960)
    {
        collisionBox.y -= velocityY;
    }
}

void MapDebugController::render(SDL_Renderer* renderer, 
                                const SDL_Rect& camera, 
                                TextureWrapper& debugControllerTexture)
{
    //the debug object is drawn at it's distance from the camera's idea of 0
    debugControllerTexture.render(renderer, collisionBox.x-camera.x, collisionBox.y-camera.y);
}


void MapDebugController::centerScreen(SDL_Rect& camera)
{
    camera.x = collisionBox.x - (camera.w/2);
    camera.y = collisionBox.y - (camera.w/2);

    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > 1280 - camera.w)
        camera.x = 1280 - camera.w;
    if (camera.y > 960 - camera.h)
        camera.y = 960 - camera.h;
    
}
