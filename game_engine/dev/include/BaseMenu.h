#pragma once

#include "pch.h"

class BaseMenu
{
public:
    BaseMenu(int fontSize, 
             int x, int y, int w, int h, 
             const std::vector<std::string>& optionNames,
             std::string fontPath,
             SDL_Color fontColor,
             SDL_Color fontHighlightColor,
             SDL_Renderer* renderer
    );
    //might need to do a combatMenu with extra onInput behavior
    void onInput(SDL_Event& event, Mix_Chunk* SelectMusic, std::string& optionSelected);
    void render(SDL_Renderer* renderer);  
    void updateHighlight(int prevIdx, int currIdx);

private:
    int startX;
    int startY;
    int optionWidth;
    int optionHeight;
    int initialCursorHeight;
    int cursorIndex;
    std::vector<SDL_Texture*> menuTextures;
    std::vector<SDL_Rect> optionRectangles;
    std::vector<SDL_Texture*> cursorTextures;
    std::vector<SDL_Rect> cursorRectangles;
    std::vector<std::string> optionNames;
    SDL_Color fontColor;
    SDL_Color highlightColor;
    SDL_Rect cursorRectangle;
    SDL_Texture* menuCursorTexture;
    TTF_Font* menuFont;
    SDL_Renderer* renderer;
};