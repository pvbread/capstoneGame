#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "TextureWrapper.h"
#include <vector>

//need to make these abstract methods
class BaseSingleTexture
{
public:
    BaseSingleTexture(){}; 
    virtual void onInput(SDL_Event& event) = 0;
    virtual void move(int xBoundary, int yBoundary) = 0;
    virtual void centerScreen(SDL_Rect& camera);
    virtual void render(SDL_Renderer* renderer, 
                const SDL_Rect& camera, 
                TextureWrapper& debugControllerTexture) = 0;
private:    
    int mainVelocity;
    int velocityX;
    int velocityY;
    SDL_Rect collisionBox;
};