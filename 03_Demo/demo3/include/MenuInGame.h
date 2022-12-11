#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "TextureWrapper.h"

class Menu{
public:
    Menu(int x, int y, int w, int h);
    bool render(SDL_Renderer* renderer,
                TextureWrapper& menuTexture);
    SDL_Rect GetMenuBox();
    void message();
private:
    SDL_Rect menuBox;
};
