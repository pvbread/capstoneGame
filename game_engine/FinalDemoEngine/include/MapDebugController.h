#pragma once

/*
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
*/
#include "pch.h"

#include "TextureWrapper.h"
#include "BaseSingleTexture.h"


class MapDebugController : public BaseSingleTexture
{
public:
    MapDebugController(int mainVelocity, 
                       int velocityX, 
                       int velocityY, 
                       SDL_Rect collisionBox); 
    void onInput(SDL_Event& event);
    void move(int xBoundary, int yBoundary);
    void centerScreen(SDL_Rect& camera, int xBoundary, int yBoundary);
    void render(SDL_Renderer* renderer, 
                const SDL_Rect& camera, 
                TextureWrapper& debugControllerTexture);
private:    
    int mainVelocity;
    int velocityX;
    int velocityY;
    SDL_Rect collisionBox;
};