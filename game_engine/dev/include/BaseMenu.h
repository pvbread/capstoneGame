#pragma once

#include "pch.h"

class BaseMenu
{
public:
    BaseMenu(int x, int y, int w, int h, 
         int fontSize, 
         int numTextures,
         const std::vector<const char*>& optionNames,
         const char* fontPath,
         SDL_Color fontColor,
         SDL_Renderer* renderer);
    void onInput(SDL_Event& event, Mix_Chunk* SelectMusic);
    void render(SDL_Renderer* renderer);

private:
    int startX;
    int startY;
    int optionWidth;
    int optionHeight;
    std::vector<SDL_Texture*> menuTextures;
    std::vector<SDL_Rect> optionRectangles;
    SDL_Rect cursorRectangle;
    SDL_Texture* menuCursorTexture;
    TTF_Font *menuFont;
};