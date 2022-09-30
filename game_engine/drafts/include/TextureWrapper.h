#pragma once

#include <SDL.h>
#include <SDL_image.h>

class TextureWrapper
{
public:
    TextureWrapper();
    ~TextureWrapper();
    bool loadImage(std::string filePath);
    /// @brief in case the user wants to free memory explicitly
    /// will give that functionality and prevent double deletion
    /// plus avoid direct calls to destructor
    void freeMemory();
    bool setColor(Uint8 r, Uint8 g, Uint8 b);
    bool setAlpha(Uint8 alpha);
    //bool setBlendMode
    //
    bool render(int x, int y, 
                SDL_Rect* clip = nullptr,
                double angle = 0.0f, 
                SDL_Point* center = nullptr,
                SDL_RendererFlip flip = SDL_FLIP_NONE);
    
    int getWidth();
    int getHeight();

private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    int width;
    int height;
};