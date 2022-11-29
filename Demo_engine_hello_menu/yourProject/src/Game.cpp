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
    TextBox myText = TextBox("start: very silly story, it was a about a band of musicians", 600, 100, 600, 700, 100, Font::openSans, Color::white, Color::navy);
    TextBox myText2 = TextBox("they were bored to death by the mindless                  ", 600, 100, 700, 700, 100, Font::openSans, Color::white, Color::navy);
    TextBox myText3 = TextBox("gigs they had to always play                              ", 600, 100, 800, 700, 100, Font::openSans, Color::white, Color::navy);
    TextBox myText4 = TextBox("plus, the deli by the rehearsal hall suckeddddd           ", 600, 100, 900, 700, 100, Font::openSans, Color::white, Color::navy);

    TextBox testTimer = TextBox("Show for 3 seconds", 600, 100, 200, 700, 100, Font::openSans, Color::white, Color::navy);
    bool timerStarted = false;
    float countTime;
    ////////////// END SECTION OF INITIALIZING VARIABLES ///////////////

    ////surface///
    SDL_Surface* testSurface = SDL_CreateRGBSurface(0, 100, 100, 32, 0xFF, 0xA5, 0x00, 0xFF);
    SDL_SetSurfaceBlendMode(testSurface, SDL_BLENDMODE_BLEND);
    
    SDL_Texture* testTexture = SDL_CreateTextureFromSurface(getRenderer(), testSurface);
    SDL_Rect testRect = {20, 20, 200, 100};
    
    SDL_Event event;
    while (!getQuit())
    {
        timer->update();
        //////////// INPUT EVENT HANDLING //////////////
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                setToQuit();
                break;
            }
            menu.onInput(event, whatHappened);
            if (!timerStarted && event.type == SDL_KEYDOWN)
            {
                timerStarted = true;
                countTime = timer->deltaTimer() + 3;
            }   
        }
        /////////////// END INPUT EVENT HANDLING //////////////

        //you can change the background color here
        SDL_Color backgroundColor = Color::white;
        SDL_SetRenderDrawColor(getRenderer(), backgroundColor.r, backgroundColor.g, backgroundColor.b, 255);
        SDL_RenderClear(getRenderer());

        /////////////// RENDER EVENTS HERE ///////////////////
        
        SDL_RenderCopy(getRenderer(), testTexture, nullptr, &testRect);

        //////////////// END RENDER EVENTS ///////////////////
        
        //Update screen (generally goes last)
        SDL_RenderPresent(getRenderer());
    }
}
