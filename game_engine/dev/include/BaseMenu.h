#pragma once

#include "pch.h"

/// @brief Creates a menu. Just specify a font size, some dimensions,
/// a list of options, font path and color, and a renderer.
/// Additionally it keeps track of which option was selected.
class BaseMenu
{
public:
    BaseMenu(int x, int y, int w, int h, 
         int fontSize, 
         const std::vector<std::string>& optionNames,
         std::string fontPath,
         SDL_Color fontColor,
         SDL_Renderer* renderer);
    /// @brief Processes input (goes in the event reading loop).
    /// @param event 
    /// @param SelectMusic Customize which sounds the menu will make
    /// @param optionSelected Updates to which option was selected
    void onInput(SDL_Event& event, Mix_Chunk* SelectMusic, std::string& optionSelected);

    /// @brief Renders the menu, goes in the render part of the game loop.
    /// @param renderer
    /// E.g. myMenu.render(getRenderer())
    void render(SDL_Renderer* renderer);

    /// @brief Helper function to get the selected index
    /// @return the index of the option that the user selected
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