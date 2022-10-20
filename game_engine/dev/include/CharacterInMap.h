#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "BaseSingleTexture.h"
#include "TextureWrapper.h"
#include <vector>

class CharacterInMap : public BaseSingleTexture
{
public:
    CharacterInMap(); 
    void onInput(SDL_Event& event);
    void centerScreen(SDL_Rect& camera);
    void render(SDL_Renderer* renderer, 
                const SDL_Rect& camera, 
                TextureWrapper& debugControllerTexture);
private:    
    int mainVelocity;
    int velocityX;
    int velocityY;
    SDL_Rect collisionBox;
};