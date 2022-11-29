#pragma once

#include "pch.h"

#include "TextureWrapper.h"

class ColorRect
{
public:
    ColorRect();
    void setRect(SDL_Rect rect);
    void setColor(SDL_Color color);
    void render(SDL_Renderer* renderer);
        
private:
    SDL_Rect rect;
    SDL_Color color;
};