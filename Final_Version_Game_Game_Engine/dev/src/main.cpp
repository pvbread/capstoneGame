//#include <iostream>
//#include "GameEngine.h"
#include "Game.h"

DashDaCapo* game;

int main(int argc, char* argv[])
{
    //This is where we instantiate and run the game.
    const char* title = "Dash Da Capo!";
    game = new DashDaCapo(SDL_INIT_VIDEO, title, 20, 20, 960, 720);
    game->runGameLoop();

    delete game;
    
    return 0;
}