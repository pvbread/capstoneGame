#include "Game.h"

YourGame::YourGame(Uint32 flags, 
                       const char* title, 
                       int x, int y, 
                       int w, int h) : 
    Archimedes(flags, title, x, y, w, h){}

void YourGame::runGameLoop()
{
    ////////////// INITIALIZE VARIABLES HERE ///////////////////////////
    std::string str1 = "avid is not reading this";
    std::string str2 = "ading this";

    int fontSize = 50;
    SDL_Rect textBox = {10, 10, 100, 100};
    SDL_Rect textBox2 = {10, 110, 0, 0};
    SDL_Color textColor = Color::white;
    SDL_Color backgroundColor = Color::navy;
    TTF_Font* font = TTF_OpenFont("../../assets/fonts/Lato-Regular.ttf", fontSize);
    TTF_SizeText(font, str1.c_str(), &textBox.w, &textBox.h);
    TTF_SizeText(font, str2.c_str(), &textBox2.w, &textBox2.h);
    bool change = false;
    


    TextBox betterText = TextBox(str1, 200, 20, 20, 800, 100, Font::lato, Color::cyan, Color::white);
    TextBox betterText2 = TextBox(str2, 200, 20, 120, 800, 100, Font::lato, Color::cyan, Color::navy);
    ////////////// END SECTION OF INITIALIZING VARIABLES ///////////////
    
    SDL_Event event;
    while (!getQuit())
    {
        //////////// INPUT EVENT HANDLING //////////////
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                setToQuit();
                break;
            }
            if (event.type == SDL_KEYDOWN)
            {
                change = !change;
                if (change)
                {
                    textColor = Color::gray;
                }
                else
                {
                    textColor = Color::white;
                }
            }
        }
        /////////////// END INPUT EVENT HANDLING //////////////

        //you can change the background color here
        SDL_Color backgroundColor = Color::navy;
        SDL_SetRenderDrawColor(getRenderer(), backgroundColor.r, backgroundColor.g, backgroundColor.b, 255);
        SDL_RenderClear(getRenderer());

        /////////////// RENDER EVENTS HERE ///////////////////

        SDL_SetRenderDrawColor(getRenderer(), backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
        SDL_RenderFillRect(getRenderer(), &textBox);
        SDL_RenderFillRect(getRenderer(), &textBox2);
        std::stringstream textStream;
        textStream << str1;
        SDL_Surface* surface = TTF_RenderUTF8_Blended(font, textStream.str().c_str(), textColor); 
        SDL_Texture* texture = SDL_CreateTextureFromSurface(getRenderer(), surface); 
        SDL_RenderCopy(getRenderer(), texture, nullptr, &textBox); 

        
        std::stringstream textStream2;
        textStream2 << str2;
        surface = TTF_RenderUTF8_Blended(font, textStream2.str().c_str(), Color::white); 
        texture = SDL_CreateTextureFromSurface(getRenderer(), surface); 
        SDL_RenderCopy(getRenderer(), texture, nullptr, &textBox2); 

        //textBox2.y += 1;
        
        
        //textBox.y += 1;
        
        /*
        betterText.render(getRenderer());
        betterText2.render(getRenderer());
        */
        
        //////////////// END RENDER EVENTS ///////////////////
        
        //Update screen (generally goes last)
        SDL_RenderPresent(getRenderer());
    }
}
