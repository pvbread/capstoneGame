#pragma once

#include "pch.h"

/// @brief A class to allow the developer to scroll through the map
/// It simply moves the camera around (is a camera controller)
class MapDebugController
{
public:
    MapDebugController(); 
    void onInput(SDL_Event& event, int xBoundary, int yBoundary);
    void move(SDL_Rect& camera);
    
private:    
    SDL_Rect newCameraPos;
};