#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "TextureWrapper.h"

/// @brief Unfinished class that was supposed to be
/// a pop-up menu screen.
class Menu{
public:
    Menu(int x, int y, int w, int h);
    
    bool render(SDL_Renderer* renderer,
                TextureWrapper& menuTexture);

    /// @brief 
    /// @return the hitbox of the menu
    SDL_Rect GetMenuBox();

    /// @brief returns the message to be displayed
    void message();
private:
    SDL_Rect menuBox;
};
