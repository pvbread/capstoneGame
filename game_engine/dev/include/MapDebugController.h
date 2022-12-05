#pragma once

#include "pch.h"

class MapDebugController
{
public:
    MapDebugController(); 
    void onInput(SDL_Event& event, int xBoundary, int yBoundary);
    void move(SDL_Rect& camera);
    
private:    
    SDL_Rect newCameraPos;
};