#ifndef GAMEENGINE_H
#define GAMEENGINE_H
#endif

#include <iostream>
#include <SDL.h>

class Phoenix
{
public:
    Phoenix(Uint32 flags, const char* title, int x, int y, int w, int h);
    ~Phoenix();
    void runGameLoop();
    void stopGameLoop();
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool quit = false;
    int width;
    int height;
};