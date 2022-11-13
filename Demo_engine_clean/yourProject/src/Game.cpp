#include "Game.h"

YourGame::YourGame(Uint32 flags, 
                       const char* title, 
                       int x, int y, 
                       int w, int h) : 
    Archimedes(flags, title, x, y, w, h){}

void YourGame::runGameLoop()
{
    ////////////// INITIALIZE VARIABLES HERE ///////////////////////////
    
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
        }
        /////////////// END INPUT EVENT HANDLING //////////////

        //you can change the background color here
        SDL_Color backgroundColor = Color::navy;
        SDL_SetRenderDrawColor(getRenderer(), backgroundColor.r, backgroundColor.g, backgroundColor.b, 255);
        SDL_RenderClear(getRenderer());

        /////////////// RENDER EVENTS HERE ///////////////////
        
        
        //////////////// END RENDER EVENTS ///////////////////
        
        //Update screen (generally goes last)
        SDL_RenderPresent(getRenderer());
    }
}
