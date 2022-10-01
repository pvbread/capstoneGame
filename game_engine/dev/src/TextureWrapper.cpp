#include "TextureWrapper.h"
#include <string>
#include <iostream>

TextureWrapper::TextureWrapper() : 
                texture{nullptr}, width{0}, height{0} {}

TextureWrapper::~TextureWrapper()
{
    freeMemory();
}

void TextureWrapper::freeMemory()
{
    if (texture == nullptr)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
        width = 0;
        height = 0;
    }
}

bool TextureWrapper::loadImage(SDL_Renderer* renderer, std::string filePath)
{
    // guard against texture already being allocated
    if (texture != nullptr)
        freeMemory();

    SDL_Texture* textureToLoad = nullptr;

    SDL_Surface* surface = IMG_Load(filePath.c_str());

    if (surface == nullptr)
    {
        SDL_Log("Error loading the image at path");
        return false;
    }
    
    int didLoad = SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 255, 255));

    if (didLoad < 0)
    {
        SDL_Log("Error setting color");
        return false;
    }

    textureToLoad = SDL_CreateTextureFromSurface(renderer, surface);
    if (textureToLoad == nullptr)
    {
        SDL_Log("Error creating texture from surface");
        return false;
    }

    width = surface->w;
    height = surface->h;

    SDL_FreeSurface(surface);
    texture = textureToLoad;
    if (texture == nullptr)
        return false;
    return true;
}

bool TextureWrapper::setColor(Uint8 r, Uint8 g, Uint8 b)
{
    int didSet = SDL_SetTextureColorMod(texture, r, g, b);
    if (didSet < 0)
    {
        SDL_Log("Error setting color");
        return false;
    }
    return true;
}

bool TextureWrapper::setAlpha(Uint8 alpha)
{
    int didSet = SDL_SetTextureAlphaMod(texture, alpha);
    if (didSet < 0)
    {
        SDL_Log("Error setting alpha");
        return false;
    }
    return true; 
}

bool TextureWrapper::render(SDL_Renderer* renderer,
                            int x, int y, 
                            const SDL_Rect* clip, 
                            double angle, 
                            SDL_Point* center,
                            SDL_RendererFlip flip)
{
    SDL_Rect rectangle = {x, y, width, height};
    if (clip != nullptr)
    {
        rectangle.w = clip->w;
        rectangle.h = clip->h;
    }
    // Copies a portion of the texture to current rendering
    // with rotation/flipping option

    //right now the "texture" param is invalid, so is it nullptr?

    int didCopy = SDL_RenderCopyEx(renderer, texture, clip, &rectangle, angle, center, flip);

    if (didCopy < 0)
    {
        SDL_Log("RenderCopyEx failed %s\n", SDL_GetError());
        return false;
    }

    return true;
}

int TextureWrapper::getWidth()
{
    return width;
}

int TextureWrapper::getHeight()
{
    return height;
}