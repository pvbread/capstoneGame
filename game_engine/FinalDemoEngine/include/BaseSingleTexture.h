#pragma once

#include "pch.h"

#include "TextureWrapper.h"

/// @brief Abstract container for a single texture
class BaseSingleTexture
{
public:
    BaseSingleTexture(int mainVelocity, 
                      int velocityX, 
                      int velocityY, 
                      SDL_Rect collisionBox);

    /// @brief processes user input
    /// @param event 
    virtual void onInput(SDL_Event& event){};

    /// @brief updates the textures position
    /// @param xBoundary 
    /// @param yBoundary 
    virtual void move(int xBoundary, int yBoundary) = 0;

    /// @brief if there is a camera, will center it on the texture
    /// @param camera 
    virtual void centerScreen(SDL_Rect& camera);

    /// @brief renders the texture
    /// @param renderer 
    /// @param camera 
    /// @param debugControllerTexture 
    virtual void render(SDL_Renderer* renderer, 
                        const SDL_Rect& camera, 
                        TextureWrapper& debugControllerTexture) = 0;

    /// @brief returns the collisionBox
    /// @return 
    SDL_Rect getCollisionBox() const;
private:    
    int mainVelocity;
    int velocityX;
    int velocityY;
    SDL_Rect collisionBox;
};