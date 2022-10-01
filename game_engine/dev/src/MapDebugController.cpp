#include "MapDebugController.h"
#include "TextureWrapper.h"

MapDebugController::MapDebugController() : 
                                            mainVelocity{10},
                                            velocityX{0},
                                            velocityY{0}
{
    collisionBox.x = 0;
    collisionBox.y = 0;
    collisionBox.w = 1;
    collisionBox.h = 1;
}

void MapDebugController::onInput(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
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
    if (event.type == SDL_KEYUP)
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
    //640 480
    collisionBox.x += velocityX;
    if(collisionBox.x < 0 || collisionBox.x > 640)
    {
        collisionBox.x -= velocityX;
    }

    collisionBox.y += velocityY;
    if(collisionBox.y < 0 || collisionBox.y < 480)
    {
        collisionBox.y -= velocityY;
    }
}

void MapDebugController::centerScreen(SDL_Rect& camera)
{
    camera.x = (collisionBox.x/2) - 640/2;
    camera.y = (collisionBox.y/2) - 480/2;

    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > 640 - camera.w)
        camera.x = 640 - camera.w;
    if (camera.y > 480 - camera.h)
        camera.y = 480 - camera.h;
    
}

void MapDebugController::render(SDL_Renderer* renderer, const SDL_Rect& camera, TextureWrapper debugControllerTexture)
{
    debugControllerTexture.render(renderer, collisionBox.x-camera.x, collisionBox.y-camera.y);
}