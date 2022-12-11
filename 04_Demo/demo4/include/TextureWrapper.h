#pragma once

/*
#include <SDL.h>
#include <SDL_image.h>
#include <string>
*/

#include "pch.h"

class TextureWrapper
{
public:
    TextureWrapper();
    ~TextureWrapper();
    bool loadImage(SDL_Renderer* renderer, std::string filePath);
    /// @brief in case the user wants to free memory explicitly
    /// will give that functionality and prevent double deletion
    /// plus avoid direct calls to destructor
    void freeMemory();
    bool setColor(Uint8 r, Uint8 g, Uint8 b);
    bool setAlpha(Uint8 alpha);
    //bool setBlendMode
    //
    bool render(SDL_Renderer* renderer,
                int x, int y, 
                const SDL_Rect* clip = nullptr,
                double angle = 0.0f, 
                SDL_Point* center = nullptr,
                SDL_RendererFlip flip = SDL_FLIP_NONE);
    
private:
    SDL_Texture* texture;
    int width;
    int height;
};