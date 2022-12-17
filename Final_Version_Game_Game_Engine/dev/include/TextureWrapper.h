#pragma once

/*
#include <SDL.h>
#include <SDL_image.h>
#include <string>
*/

#include "pch.h"

/// @brief Middle-level object that is used for a lot of the map rendering.
/// Occasionally we use it in the game directly when we want to play with 
/// transparency levels.
class TextureWrapper
{
public:
    TextureWrapper();
    
    ~TextureWrapper();

    bool loadImage(SDL_Renderer* renderer, std::string filePath);
    
    /// @brief in case the user wants to free memory explicitly
    /// will give that functionality and prevent double deletion
    /// plus avoid direct calls to destructor.
    /// The game was never intricate enough to need this and we dropped this
    /// functionality on later classes. But we might want to extend it in the future.
    void freeMemory();
    
    bool setColor(Uint8 r, Uint8 g, Uint8 b);

    /// @brief Sets the transparency level
    /// @param alpha 
    /// @return 
    bool setAlpha(Uint8 alpha);
    
    /// @brief Highly customizable rendering function
    /// @param renderer 
    /// @param x 
    /// @param y 
    /// @param clip allows to just pick a portion of the texture
    /// @param angle allows to rotate the angle
    /// @param center allows to rotate upon a central focal point
    /// @param flip allows for up-down-left-right flips
    /// @return 
    bool render(SDL_Renderer* renderer,
                int x, int y, 
                const SDL_Rect* clip = nullptr,
                double angle = 0.0f, 
                SDL_Point* center = nullptr,
                SDL_RendererFlip flip = SDL_FLIP_NONE);

    SDL_Texture* getTexture() const;
    
private:
    SDL_Texture* texture;
    int width;
    int height;
};