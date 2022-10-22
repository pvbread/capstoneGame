#include <iostream>
#include "GameEngine.h"

Phoenix* gameEngine;

int main(int argc, char* argv[])
{
    const char* title = "engine test";
    gameEngine = new Phoenix(SDL_INIT_VIDEO, title, 20, 20, 640, 480);
    gameEngine->runGameLoop();

    delete gameEngine;
    
    return 0;
}