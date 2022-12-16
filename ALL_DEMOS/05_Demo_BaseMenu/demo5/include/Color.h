#pragma once

#include "pch.h"

/// @brief Provides static access to common colors
class Color
{
public:
    static constexpr SDL_Color black = {0, 0, 0, 0};
    static constexpr SDL_Color white = {255, 255, 255, 0};
    static constexpr SDL_Color red = {255, 0, 0, 0};
    static constexpr SDL_Color lime = {0, 255, 0, 0};
    static constexpr SDL_Color blue = {0, 0, 255, 0};
    static constexpr SDL_Color yellow = {255, 255, 0, 0};
    static constexpr SDL_Color cyan = {0, 255, 255, 0};
    static constexpr SDL_Color magenta = {255, 0, 255, 0};
    static constexpr SDL_Color silver = {192, 192, 192, 0};
    static constexpr SDL_Color gray = {128, 128, 128, 0};
    static constexpr SDL_Color maroon = {128, 0, 0, 0};
    static constexpr SDL_Color olive = {128, 128, 0, 0};
    static constexpr SDL_Color green = {0, 128, 0, 0};
    static constexpr SDL_Color purple = {128, 0, 128, 0};
    static constexpr SDL_Color teal = {0, 128, 128, 0};
    static constexpr SDL_Color navy = {0, 0, 128, 0};
    static constexpr SDL_Color lightNavy = {0, 0, 200, 0};
    static constexpr SDL_Color darkGreen = {0, 90, 0, 0};
};