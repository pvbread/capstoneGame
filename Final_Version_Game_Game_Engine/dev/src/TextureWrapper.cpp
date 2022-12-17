#include "TextureWrapper.h"

//#include <string>
//#include <iostream>

TextureWrapper::TextureWrapper() : 
                texture{nullptr}, width{0}, height{0} {}

TextureWrapper::~TextureWrapper()
{
    freeMemory();
}

void TextureWrapper::freeMemory()
{
    //this is a pattern we don't end up doing on our other objects just
    //out of lack of time
    if (texture != nullptr)
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
    // changes the alpha setting,
    // to be clear, this still needs the SLD blend context to be activated
    // to be of any use
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

    //clip is only if we want to render a portion of this texture
    //like in the case of the tilemap
    if (clip != nullptr)
    {
        rectangle.w = clip->w;
        rectangle.h = clip->h;
    }
    // Copies a portion of the texture to current rendering
    // with rotation/flipping option which we may eventually want to use
    // the src is the clip and dest is the &rect at the position we specified
    // we never ended up using the extra functionality
    // but we didn't know how far we'd go

    int didCopy = SDL_RenderCopyEx(renderer, texture, clip, &rectangle, angle, center, flip);

    if (didCopy < 0)
    {
        SDL_Log("RenderCopyEx failed %s\n", SDL_GetError());
        return false;
    }

    return true;
}

SDL_Texture* TextureWrapper::getTexture() const
{
    return texture;
}

