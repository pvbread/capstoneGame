#pragma once

#include "pch.h"

class BaseMenu
{
public:
    BaseMenu(int x, int y, int w, int h, 
         int fontSize, 
         const std::vector<std::string>& optionNames,
         std::string fontPath,
         SDL_Color fontColor,
         SDL_Renderer* renderer);
    //might need to do a combatMenu with extra onInput behavior
    void onInput(SDL_Event& event, Mix_Chunk* SelectMusic, std::string& optionSelected);
    void render(SDL_Renderer* renderer);
    int getOptionSelectedIndex();

private:
    int startX;
    int startY;
    int optionWidth;
    int optionHeight;
    int initialCursorHeight;
    std::vector<SDL_Texture*> menuTextures;
    std::vector<SDL_Rect> optionRectangles;
    std::vector<std::string> optionNames;
    SDL_Rect cursorRectangle;
    SDL_Texture* menuCursorTexture;
    TTF_Font *menuFont;
};