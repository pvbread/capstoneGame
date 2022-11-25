#pragma once


#include "pch.h"

#include "BaseSingleTexture.h"
#include "TextureWrapper.h"
#include "TileType.h"

/// @brief Game-specific class for the character representation in map
class CharacterInMap : public BaseSingleTexture
{
public:
    CharacterInMap(int mainVelocity, 
                   int velocityX, 
                   int velocityY, 
                   SDL_Rect collisionBox
    ); 

    /// @brief helper function to check if the user move was valid
    /// @param direction 
    /// @param coordinateToTileTypeMap 
    /// @return 
    bool isMoveValid(std::string direction, 
                     std::map<std::pair<int, int>, TileType>& coordinateToTileTypeMap
    );

    /// @brief Helper function that updates the next map event 
    /// @param nextMapEvent string holding the state of the nextMapEvent
    /// @param boxOpen checks if event box is already open
    /// @param coordinates map coordinates
    /// @param coordinateToEventTypeMap coordinate-to-event map
    void updateEvent(std::string& nextMapEvent,
                     bool& boxOpen,
                     std::pair<int, int> coordinates, 
                     std::map<std::pair<int, int>, std::string>& coordinateToEventTypeMap
    );

    /// @brief defines the movement for the character in the map
    /// @param event 
    /// @param nextMapEvent 
    /// @param boxOpen 
    /// @param coordinateToTileTypeMap 
    /// @param coordinateToEventTypeMap 
    void onInput(SDL_Event& event,
                 std::string& nextMapEvent,
                 bool& boxOpen,
                 std::map<std::pair<int, int>, TileType>& coordinateToTileTypeMap,
                 std::map<std::pair<int, int>, std::string>& coordinateToEventTypeMap
    );

    /// @brief Updates the character state in the map
    /// @param xBoundary 
    /// @param yBoundary 
    void move(int xBoundary, 
              int yBoundary);

    void centerScreen(SDL_Rect& camera, int xBoundary, int yBoundary);

    void render(SDL_Renderer* renderer, 
                const SDL_Rect& camera, 
                TextureWrapper& characterTexture);
private:    
    int mainVelocity;
    int velocityX;
    int velocityY;
    SDL_Rect collisionBox;
};