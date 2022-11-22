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
    menuFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    //TODO add error checking

    //std::vector<std::string> optionsStringsPadded = padMenuStrings(optionNames);

    //Rectangles
    optionRectangles = std::vector<SDL_Rect>(numTextures);
    this->optionNames = optionNames;
    SDL_Surface *surface;

    //set the textures
    for (int i = 0; i < numTextures; i++)
    {
        optionRectangles[i] = { startX,
                                startY+(i*optionHeight),
                                optionWidth,
                                optionHeight
                              };
        TTF_SizeText(menuFont, optionNames[i].c_str(), &optionRectangles[i].w, &optionRectangles[i].h);
        if (i == 0)
            surface = TTF_RenderUTF8_Blended(menuFont, optionNames[i].c_str(), highlightColor);
        else
            surface = TTF_RenderUTF8_Blended(menuFont, optionNames[i].c_str(), fontColor);
        menuTextures[i] = SDL_CreateTextureFromSurface(renderer, surface); 
    }

    //cursor initialization
    cursorRectangle = { x-(optionHeight/2), y+(optionHeight/5), optionHeight/2, optionHeight/2 };
    std::string CURSOR = ">";
    initialCursorHeight = y+(optionHeight/5);
    surface = TTF_RenderUTF8_Blended(menuFont, CURSOR.c_str(), fontColor);
    SDL_Texture* menuCursorTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    optionRectangles.push_back(cursorRectangle);
    menuTextures.push_back(menuCursorTexture);
    
}

void BaseMenu::onInput(SDL_Event& event, Mix_Chunk* SelectMusic, std::string& optionSelected)
{
    int played;
    int prevIdx, currIdx;
    if (event.type == SDL_KEYDOWN)
    {
        //alias to make code more readable
        SDL_Rect & cursor = optionRectangles.back(); 
        switch (event.key.keysym.sym)
        {
            case SDLK_UP:
            {
                prevIdx = getOptionSelectedIndex();
                //hack, see constructor and render method
                //for notes on why i chose to do it
                cursor.y -= optionHeight;
                //cursorRectangle.y -= 100;
                played = Mix_PlayChannel(-1, SelectMusic, 0);
                if (played == -1){
                    SDL_Log("audio error");
                }
                //if cursor is off the top of the screen, move it to the bottom
                //checks against the first option
                if (cursor.y < optionRectangles[0].y)
                {
                    cursor.y += (optionRectangles.size()-1) * optionHeight;
                }
                currIdx = getOptionSelectedIndex();
                updateHighlight(prevIdx, currIdx); 
                break;
            }
            case SDLK_DOWN:
            {
                prevIdx = getOptionSelectedIndex(); 
                optionRectangles.back().y += optionHeight;
                played = Mix_PlayChannel(-1, SelectMusic, 0);
                if (played == -1){
                    SDL_Log("audio error");
                }
                //if cursor is off the bottom of the screen, move it to the top
                //checks next to last (because of cursor hack)
                if (cursor.y > optionRectangles.end()[-2].y + cursor.h)
                {
                    cursor.y -= (optionRectangles.size()-1) * optionHeight;
                    
                }
                currIdx = getOptionSelectedIndex();
                updateHighlight(prevIdx, currIdx); 
                break;
            }
            case SDLK_RETURN:
            {
                int selectedIndex = getOptionSelectedIndex();
                optionSelected = optionNames[selectedIndex];
                //deselect(selectedIndex);
                break;
            }
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

int BaseMenu::getOptionSelectedIndex()
{
    // The logic is this:
    // we have an initial cursor height, say 70
    // the extra options can only be below it,
    // and the cursor only moves by OPTIONHEIGHT distance at a time
    // if OPTIONHEIGHT is 100, we'd have 70, 170, 270 etc
    // to retrieve the index of the given option
    // we take the difference of the current height and the initial height
    // and divide by option height
    SDL_Rect & cursor = optionRectangles.back(); 
    int difference = cursor.y - initialCursorHeight;
    int selectedIndex = difference / optionHeight;
    return selectedIndex;
    
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