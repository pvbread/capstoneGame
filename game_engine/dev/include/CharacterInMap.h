#pragma once

/*
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
*/

#include "pch.h"

#include "BaseSingleTexture.h"
#include "TextureWrapper.h"
#include "TileType.h"

class CharacterInMap : public BaseSingleTexture
{
public:
    CharacterInMap(int mainVelocity, 
                   int velocityX, 
                   int velocityY, 
                   SDL_Rect collisionBox); 
    void onInput(SDL_Event& event);
    void move(int xBoundary, 
              int yBoundary, 
              std::map<std::pair<int, int>, TileType>& coordinateToTileTypeMap);
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