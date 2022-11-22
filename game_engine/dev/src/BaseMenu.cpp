#include "BaseMenu.h"
#include "padMenuStrings.h"

BaseMenu::BaseMenu(int fontSize,
                   int x, int y, int w, int h,  
                   const std::vector<std::string>& optionNames,
                   std::string fontPath, 
                   SDL_Color fontColor,
                   SDL_Color fontHighlightColor,
                   SDL_Renderer* renderer)
{
    int numTextures = optionNames.size(); 
    startX = x;
    startY = y;
    optionWidth = w;
    optionHeight = h;
    this->fontColor = fontColor;
    this->highlightColor = fontHighlightColor;
    this->renderer = renderer;
    menuTextures = std::vector<SDL_Texture*>(numTextures);
    cursorTextures = std::vector<SDL_Texture*>(numTextures);
    menuFont = TTF_OpenFont(fontPath.c_str(), fontSize);
   
    optionRectangles = std::vector<SDL_Rect>(numTextures);
    cursorRectangles = std::vector<SDL_Rect>(numTextures);;
    this->optionNames = optionNames;
    SDL_Surface *surface;

    std::string CURSOR = ">";
    cursorIndex = 0;
    initialCursorHeight = startY+(optionHeight/2);

    for (int i = 0; i < numTextures; i++)
    {
        optionRectangles[i] = { startX,
                                startY+(i*optionHeight),
                                optionWidth,
                                optionHeight
                              };
        TTF_SizeText(menuFont, optionNames[i].c_str(), &optionRectangles[i].w, &optionRectangles[i].h);
        cursorRectangles[i] = {
            x-(optionHeight/2),
            startY+(i*optionHeight),
            optionHeight/2,
            optionHeight
        };
        if (i == 0)
            surface = TTF_RenderUTF8_Blended(menuFont, optionNames[i].c_str(), highlightColor);
        else
            surface = TTF_RenderUTF8_Blended(menuFont, optionNames[i].c_str(), fontColor);
        menuTextures[i] = SDL_CreateTextureFromSurface(renderer, surface); 

        TTF_SizeText(menuFont, CURSOR.c_str(), &cursorRectangle.w, &cursorRectangle.h);
        surface = TTF_RenderUTF8_Blended(menuFont, CURSOR.c_str(), fontColor);
        cursorTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
    }
    SDL_FreeSurface(surface);
   
    
}

void BaseMenu::onInput(SDL_Event& event, Mix_Chunk* SelectMusic, std::string& optionSelected)
{
    int played;
    int prevIdx, currIdx;
    if (event.type == SDL_KEYDOWN)
    {
        //alias to make code more readable
        //SDL_Rect & cursor = optionRectangles.back(); 
        switch (event.key.keysym.sym)
        {
            case SDLK_UP:
            {
                prevIdx = cursorIndex;
                cursorIndex--;
                if (cursorIndex < 0)
                    cursorIndex = cursorRectangles.size()-1;
                played = Mix_PlayChannel(-1, SelectMusic, 0);
                currIdx = cursorIndex;
                updateHighlight(prevIdx, currIdx); 

                if (played == -1){
                    SDL_Log("audio error");
                }
                break;
            }
            case SDLK_DOWN:
            {
                prevIdx = cursorIndex;
                cursorIndex = (cursorIndex + 1) % cursorRectangles.size();
                played = Mix_PlayChannel(-1, SelectMusic, 0);
                currIdx = cursorIndex;
                updateHighlight(prevIdx, currIdx); 

                if (played == -1){
                    SDL_Log("audio error");
                }
                break;
            }
            case SDLK_RETURN:
            {
                optionSelected = optionNames[cursorIndex];
                //deselect(selectedIndex);
                break;
            }
        }
    }
}

void BaseMenu::render(SDL_Renderer* renderer)
{
    for (int i = 0; i < menuTextures.size(); i++)
    {
        SDL_RenderCopy(renderer, menuTextures[i], nullptr, &optionRectangles[i]);
        if (i == cursorIndex)
            SDL_RenderCopy(renderer, cursorTextures[i], nullptr, &cursorRectangles[i]);
    }
}

void BaseMenu::updateHighlight(int prevIdx, int currIdx)
{
    SDL_DestroyTexture(menuTextures[currIdx]);
    TTF_SizeText(menuFont, this->optionNames[currIdx].c_str(), &optionRectangles[currIdx].w, &optionRectangles[currIdx].h);
    SDL_Surface* surface = TTF_RenderUTF8_Blended(menuFont, optionNames[currIdx].c_str(), highlightColor);
    menuTextures[currIdx] = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroyTexture(menuTextures[prevIdx]);
    TTF_SizeText(menuFont, optionNames[prevIdx].c_str(), &optionRectangles[prevIdx].w, &optionRectangles[prevIdx].h);
    surface = TTF_RenderUTF8_Blended(menuFont, optionNames[prevIdx].c_str(), fontColor);
    menuTextures[prevIdx] = SDL_CreateTextureFromSurface(renderer, surface); 
    
    SDL_FreeSurface(surface); 
}