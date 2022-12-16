#pragma once

/// @brief checks if two hitboxes are colliding.
/// @param rect1 
/// @param rect2 
/// @return 
bool isColliding(const SDL_Rect* rect1, const SDL_Rect* rect2)
{
    int leftRect1, leftRect2, 
        topRect1, topRect2, 
        rightRect1, rightRect2, 
        bottomRect1, bottomRect2;
    
    leftRect1 = rect1->x;
    leftRect2 = rect2->x;
    topRect1 = rect1->y;
    topRect2 = rect2->y;
    rightRect1 = rect1->x + rect1->w;
    rightRect2 = rect2->x + rect2->w;
    bottomRect1 = rect1->y + rect1->h;
    bottomRect2 = rect2->y + rect2->h;

    if (leftRect1 >= rightRect2 ||
        leftRect2 >= rightRect1 ||
        topRect1 >= bottomRect2 ||
        topRect2 >= bottomRect1)
    {
        return false;
    }
    
    return true;
}