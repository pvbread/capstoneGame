#pragma once

#include "pch.h"
#include "Color.h"

/// @brief Creates a menu. Just specify a font size, some dimensions,
/// a list of options, font path and color, and a renderer.
/// Additionally it keeps track of which option was selected.
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
    
    /// @brief 
    /// @param event: reads the input event
    /// @param SelectMusic: the sound to be made when scrolling 
    /// @param optionSelected: the variable that retains the option chosen by the end
    void onInput(SDL_Event& event, Mix_Chunk* SelectMusic, std::string& optionSelected);
    
    /// @brief renders the menu
    /// @param renderer 
    void render(SDL_Renderer* renderer);

    /// @brief updates the highlight color of the option selected
    /// @param prevIdx index of the previously chosen option
    /// @param currIdx index of the currently chosent option
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