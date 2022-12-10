#include "MapDebugController.h"

MapDebugController::MapDebugController()
{
    newCameraPos = {0, 0, 960, 720};
}

void MapDebugController::onInput(SDL_Event& event, int xBoundary, int yBoundary)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_LEFT:
            {
                newCameraPos.x -= 50;
                if (newCameraPos.x < 0)
                    newCameraPos.x = 0;
                break;
            }
            case SDLK_UP:
            {
                newCameraPos.y -= 50;
                if (newCameraPos.y < 0)
                    newCameraPos.y = 0;
                break;
            }
            case SDLK_RIGHT:
            {
                newCameraPos.x += 50;
                if (newCameraPos.x > (xBoundary-newCameraPos.w))
                    newCameraPos.x = xBoundary-newCameraPos.w;
                break;
            }
            case SDLK_DOWN:
            {
                newCameraPos.y += 50;
                if (newCameraPos.y > (yBoundary-newCameraPos.h))
                    newCameraPos.y = yBoundary-newCameraPos.h;
                break;
            }
        }
    }
}

void MapDebugController::move(SDL_Rect& camera)
{
    camera = newCameraPos;
}