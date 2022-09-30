#pragma once
#include <SDL.h>
#include <SDL_image.h>

class Tile
{
public:
    Tile(int x, int y, int w, int h, int type);
    bool render(SDL_Rect* camera);
    int getType();
    SDL_Rect getCollisionBox();
private:
    // Tile attributes
    SDL_Rect box;
    int type;
}