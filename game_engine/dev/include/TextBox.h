#pragma once

#include "pch.h"

class TextBox
{
public:
    TextBox(std::string text,
            int fontSize,
            int x, int y,
            int w, int h, 
            std::string inputFont = "Raleway-Medium",
            SDL_Color textColor = {255, 0, 0, 255}, 
            SDL_Color backgroundColor = {0, 0, 0, 0}
    );
    void render(SDL_Renderer* renderer);
private:
    std::string text;
    TTF_Font *font;
    int fontSize;
    SDL_Rect textBox;
    SDL_Color textColor;
    SDL_Color backgroundColor;
};