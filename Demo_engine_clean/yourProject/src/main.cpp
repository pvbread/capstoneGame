#include "Game.h"

YourGame* game;

int main(int argc, char* argv[])
{
    const char* title = "Your Game";
    game = new YourGame(SDL_INIT_VIDEO, title, 20, 20, 960, 720);
    game->runGameLoop();

    delete game;
    
    return 0;
}