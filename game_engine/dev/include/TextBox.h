#pragma once

#include "pch.h"

/// @brief container for text, can select from given fonts/colors
/// give the path to your own string font path / SDL colors.
class TextBox
{
public:
    TextBox() = default;
    TextBox(std::string text,
            int fontSize,
            int x, int y,
            int w, int h, 
            std::string inputFont = "../../assets/fonts/Roboto-Medium.ttf",
            SDL_Color textColor = {255, 0, 0, 255}, 
            SDL_Color backgroundColor = {0, 0, 0, 0},
            bool isClear = false
    );
    /// @brief Renders the given textbox
    /// @param renderer, when invoking, you can usually obtain this with getRenderer()
    /// e.g. myTextBox.render(getRenderer()) 
    void render(SDL_Renderer* renderer);

    /// @brief Changes the contents of the text
    /// @param text 
    void changeText(std::string text);

    void changeDimensions(int w, int h);

    /// @brief Changes the position of the text
    /// @param x 
    /// @param y 
    void changePosition(int x, int y);

    /// @brief Changes the color of the text
    /// @param color 
    void changeTextColor(SDL_Color color);

    /// @brief Changes the background color of the text
    /// @param color 
    void changeBackgroundColor(SDL_Color color);

    int getBottomY() const;
private:
    std::string text;
    TTF_Font *font;
    int fontSize;
    SDL_Rect textBox;
    SDL_Color textColor;
    SDL_Color backgroundColor;
    bool isClear;
};