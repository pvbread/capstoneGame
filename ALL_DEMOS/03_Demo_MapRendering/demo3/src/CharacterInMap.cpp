#include "BaseSingleTexture.h"
#include "CharacterInMap.h"
#include "TextureWrapper.h"
//#include "TileType.h"
//#include "isColliding.h"


CharacterInMap::CharacterInMap(int mainVelocity, 
                               int velocityX, 
                               int velocityY, 
                               SDL_Rect collisionBox) :
    BaseSingleTexture(mainVelocity, velocityX, velocityY, collisionBox)                   
{
    this->mainVelocity = mainVelocity;
    this->velocityX = velocityX;
    this->velocityY = velocityY;
    this->collisionBox.x = collisionBox.x;
    this->collisionBox.y = collisionBox.y;
    this->collisionBox.w = collisionBox.w;
    this->collisionBox.h = collisionBox.h;
}

bool CharacterInMap::isMoveValid(std::string direction, 
                 std::map<std::pair<int, int>, TileType>& coordinateToTileTypeMap)
{

    //we have a collision box, which gives us x, y coords in the hitbox
    //as well as we can ask if the hitbox is colliding (prob not necessary)
    //we're offset by 30, 30 so the actual position of the player is
    //x-startingOffset, y-startingOffset
    //x-30/collisionBoxWidth, y-30/collisionBoxWidth pos
    //tileSet[y][x]

    std::pair<int,int> coordinates = std::make_pair(collisionBox.x-30, collisionBox.y-30);
    TileType currentTileType = coordinateToTileTypeMap[coordinates];

    //can't go down
    if ( direction == "down" &&
            (
                currentTileType == BLACK ||
                currentTileType == TUP ||
                currentTileType == UPRIGHT ||
                currentTileType == LEFTRIGHT ||
                currentTileType == LEFTUP
            )
       )
    {
        return false;
    }

    //can't go up
    if ( direction == "up" &&
            (
                currentTileType == BLACK ||
                currentTileType == DOWNRIGHT ||
                currentTileType == TDOWN ||
                currentTileType == LEFTRIGHT ||
                currentTileType == LEFTDOWN
            )
       )
    {
        return false;
    }

    //can't go left
    if ( direction == "left" &&
            (
                currentTileType == BLACK ||
                currentTileType == TRIGHT ||
                currentTileType == UPRIGHT ||
                currentTileType == UPDOWN ||
                currentTileType == DOWNRIGHT
            )
       )
    {
        return false;
    }
    //can't go right 
    if ( direction == "right" &&
            (
                currentTileType == BLACK ||
                currentTileType == TLEFT ||
                currentTileType == LEFTDOWN ||
                currentTileType == UPDOWN ||
                currentTileType == LEFTUP
            )
       )
    {
        return false;
    }

    return true;
}

void CharacterInMap::updateEvent(std::string& nextMapEvent,
                                 bool& boxOpen,
                                 std::pair<int, int> coordinates, 
                                 std::map<std::pair<int, int>, std::string>& coordinateToEventTypeMap
                                )
{
    if (coordinateToEventTypeMap[coordinates] != "BLANKEVENT")
    { 
        nextMapEvent = coordinateToEventTypeMap[coordinates];
        boxOpen = true;  
        coordinateToEventTypeMap[coordinates] = "BLANKEVENT"; 
    }
    else if (coordinateToEventTypeMap[coordinates] == "BLANKEVENT")
    {
        nextMapEvent = "BLANKEVENT";
        boxOpen = false;
    }
}

void CharacterInMap::onInput(SDL_Event& event)
{
    
    //std::pair<int,int> coordinates = std::make_pair(collisionBox.x-30, collisionBox.y-30);
                //TileType currentTileType = coordinateToTileTypeMap[coordinates];
                          
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_a:
            {
                collisionBox.x -= mainVelocity; 
                break;
            }
            case SDLK_w:
            {
                collisionBox.y -= mainVelocity;  
                break;
            }
            case SDLK_d:
            {
                collisionBox.x += mainVelocity;
                break;
            }
            case SDLK_s:
            {
                collisionBox.y += mainVelocity;
                break;
            }
        }
    }
}

void CharacterInMap::move(int xBoundary, 
                          int yBoundary)
{  
    if (collisionBox.x < 30)
        collisionBox.x += mainVelocity;
    else if (collisionBox.x >= xBoundary)
        collisionBox.x -= mainVelocity; 
    else if (collisionBox.y < 30) 
        collisionBox.y += mainVelocity;
    else if (collisionBox.y >= yBoundary)
        collisionBox.y -= mainVelocity;
}

void CharacterInMap::render(SDL_Renderer* renderer, const SDL_Rect& camera, TextureWrapper& characterTexture)
{
    //the debug object is drawn at it's distance from the camera's idea of 0
    characterTexture.render(renderer, collisionBox.x-camera.x, collisionBox.y-camera.y);
}


void CharacterInMap::centerScreen(SDL_Rect& camera, int xBoundary, int yBoundary)
{
    // TODO fix this hardcoding to take in any size
    camera.x = collisionBox.x - (camera.w/2);
    camera.y = collisionBox.y - (camera.h/2);

    if (camera.x < 0)
        camera.x = 0;
    if (camera.y < 0)
        camera.y = 0;
    if (camera.x > xBoundary - camera.w)
        camera.x = xBoundary - camera.w;
    if (camera.y > yBoundary - camera.h)
        camera.y = yBoundary - camera.h;
}