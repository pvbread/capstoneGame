//#include <iostream>
//#include "GameEngine.h"
#include "Game.h"

//Phoenix* gameEngine;
EscapeFromCapstone* game;

int main(int argc, char* argv[])
{
    const char* title = "Escape from the Capstone Underground";
    game = new EscapeFromCapstone(SDL_INIT_VIDEO, title, 20, 20, 640, 480);
    game->runGameLoop();

    delete game;
    
    return 0;
}