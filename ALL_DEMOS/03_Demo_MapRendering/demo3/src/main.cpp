//#include <iostream>
//#include "GameEngine.h"
#include "Game.h"

//Phoenix* gameEngine;
DashDaCapo* game;

int main(int argc, char* argv[])
{
    const char* title = "Archimedes Engine";
    game = new DashDaCapo(SDL_INIT_VIDEO, title, 20, 20, 960, 720);
    game->runGameLoop();

    delete game;
    
    return 0;
}