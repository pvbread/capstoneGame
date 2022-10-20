#include "CharacterInMap.h"
#include "TextureWrapper.h"

CharacterInMap::CharacterInMap() : 
                                            mainVelocity{0},
                                            velocityX{0},
                                            velocityY{0}
{
    //TODO don't hard code this
    collisionBox.x = 0;
    collisionBox.y = 0;
    collisionBox.w = 80;
    collisionBox.h = 80;
}

void CharacterInMap::onInput(SDL_Event& event, int xBoundary, int yBoundary)
{
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_KP_A:
            {
                if (collisionBox.x - collisionBox.w >= 0)
                    collisionBox.x -= collisionBox.w;
                break;
            }
            case SDLK_KP_W:
            {
                if (collisionBox.y - collisionBox.h >= 0)
                    collisionBox.y -= collisionBox.h;
                break;
            }
            case SDLK_KP_D:
            {
                if (collisionBox.x + collisionBox.w < xBoundary)
                    collisionBox.x += collisionBox.w;
                break;
            }
            case SDLK_KP_S:
            {
                if (collisionBox.y + collisionBox.h < yBoundary)
                    collisionBox.y += collisionBox.h;
                break;
            }
        }
    }
}

void CharacterInMap::render(SDL_Renderer* renderer, const SDL_Rect& camera, TextureWrapper& characterTexture)
{
    //the debug object is drawn at it's distance from the camera's idea of 0
    characterTexture.render(renderer, collisionBox.x-camera.x, collisionBox.y-camera.y);
}