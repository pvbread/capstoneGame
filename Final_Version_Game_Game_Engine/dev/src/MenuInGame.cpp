#include "MenuInGame.h"
#include <iostream>
#include <string>

// We never ended up finishing implementing this idea because
// we took a different approach. I might pick it up again later
// which is why I leave it here.

Menu::Menu(int x, int y, int w, int h)
{
    menuBox.x = x;
    menuBox.y = y;
    menuBox.w = w;
    menuBox.h = h;
}

bool Menu::render(SDL_Renderer* renderer, 
            TextureWrapper& menuTexture)
{
    bool success = true;
    success = menuTexture.render(renderer, menuBox.x, menuBox.y);
    if (!success){
        SDL_Log("RenderCopy failed %s\n", SDL_GetError());
        return false;
    }
    // checks if box is in
    
    return success;
}

SDL_Rect Menu::GetMenuBox()
{
    return menuBox;
}



void Menu::message()
{
    SDL_Log("hi");
}
