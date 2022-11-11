#pragma once

#include "pch.h"

class TextBox
{
public:
    //TODO add overloaded constructors that allow for typing of typical colors
    TextBox(std::string text,
            int fontSize,
            int x, int y,
            int w, int h, 
            std::string inputFont = "Raleway-Medium",
            SDL_Color textColor = {255, 0, 0, 255}, 
            SDL_Color backgroundColor = {0, 0, 0, 0}
    );
    void render(SDL_Renderer* renderer);
    void changeText(std::string text);
    void changeDimensions(int w, int h);
    void changePosition(int x, int y);
    void changeTextColor(SDL_Color color);
    void changeBackgroundColor(SDL_Color color);
private:
    std::string text;
    TTF_Font *font;
    int fontSize;
    SDL_Rect textBox;
    SDL_Color textColor;
    SDL_Color backgroundColor;
};