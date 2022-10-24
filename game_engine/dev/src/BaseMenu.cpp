#include "BaseMenu.h"

BaseMenu::BaseMenu(int x, int y, int w, int h, 
           int fontSize, 
           int numTextures,
           const std::vector<const char*>& optionNames,
           const char* fontPath, 
           SDL_Color fontColor,
           SDL_Renderer* renderer)
{
    startX = x;
    startY = y;
    optionWidth = w;
    optionHeight = h;
    menuTextures = std::vector<SDL_Texture*>(numTextures);
    TTF_Font *menuFont = TTF_OpenFont(fontPath, fontSize);
    //TODO add error checking

    //Rectangles
    optionRectangles = std::vector<SDL_Rect>(numTextures);
    SDL_Surface *surface;

    //set the textures
    for (int i = 0; i < numTextures; i++)
    {
        optionRectangles[i] = { startX,
                                startY+(i*optionHeight),
                                optionWidth,
                                optionHeight
                              };
        surface = TTF_RenderText_Solid(menuFont, optionNames[i], fontColor);
        menuTextures[i] = SDL_CreateTextureFromSurface(renderer, surface); 
    }

    //cursor initialization
    cursorRectangle = { x-55, y+20, 50, 50 };
    surface = TTF_RenderText_Solid(menuFont, ">", fontColor);
    SDL_Texture* menuCursorTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    optionRectangles.push_back(cursorRectangle);
    menuTextures.push_back(menuCursorTexture);
    
}

void BaseMenu::onInput(SDL_Event& event, Mix_Chunk* SelectMusic)
{
    int played;
    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_UP:
            {
                //hack, see constructor and render method
                //for notes on why i chose to do it
                optionRectangles.back().y -= 100;
                //cursorRectangle.y -= 100;
                played = Mix_PlayChannel(-1, SelectMusic, 0);
                if (played == -1){
                    SDL_Log("audio error");
                }
                //if cursor is off the top of the screen, move it to the bottom
                if (optionRectangles.back().y < 160)
                {
                    optionRectangles.back().y = 360;
                }
                break;
            }
            case SDLK_DOWN:
            {
                optionRectangles.back().y += 100;
                played = Mix_PlayChannel(-1, SelectMusic, 0);
                if (played == -1){
                    SDL_Log("audio error");
                }
                //if cursor is off the bottom of the screen, move it to the top
                if (optionRectangles.back().y > 360)
                {
                    optionRectangles.back().y = 160;
                }
            }
            break;
        }
    }
}

void BaseMenu::render(SDL_Renderer* renderer)
{
    //TODO one idea is to hack the cursor onto the back of the stack
    //for some reason the lone call triggers a segfault
    for (int i = 0; i < menuTextures.size(); i++)
    {
        SDL_RenderCopy(renderer, menuTextures[i], nullptr, &optionRectangles[i]);
    }
    //SDL_RenderCopy(renderer, menuCursorTexture, nullptr, &cursor);
}