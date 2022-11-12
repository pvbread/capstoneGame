#include "Game.h"

YourGame::YourGame(Uint32 flags, 
                       const char* title, 
                       int x, int y, 
                       int w, int h) : 
    Archimedes(flags, title, x, y, w, h){}

void YourGame::runGameLoop()
{
    ////////////// INITIALIZE VARIABLES HERE ///////////////////////////
    Timer* timer = Timer::instance();
    std::vector<std::string> options{"op1", "op2", "op3","op4"};
    BaseMenu menu = BaseMenu(100, 20, 400, 100, 600, options, Font::lato, Color::gray, getRenderer());
    std::string whatHappened;
    TextBox myText = TextBox("start", 200, 100, 600, 400, 100, Font::montserrat, Color::black, Color::white);
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
            menu.onInput(event, whatHappened);     
        }
        /////////////// END INPUT EVENT HANDLING //////////////

        //you can change the background color here
        SDL_Color backgroundColor = Color::navy;
        SDL_SetRenderDrawColor(getRenderer(), backgroundColor.r, backgroundColor.g, backgroundColor.b, 255);
        SDL_RenderClear(getRenderer());

        /////////////// RENDER EVENTS HERE ///////////////////
        
        //menu.render(getRenderer());
        //if (whatHappened != "")
        //    myText.changeText(whatHappened);
        myText.render(getRenderer());
        
        //////////////// END RENDER EVENTS ///////////////////
        
        //Update screen (generally goes last)
        SDL_RenderPresent(getRenderer());
    }
}
