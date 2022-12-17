#pragma once

#include "pch.h"
#include "TileType.h"
#include "Tile.h"
#include "TextBox.h"
#include "Color.h"
#include "Font.h"
#include "Timer.h"

/// @brief Where the engine gets implemented
class Archimedes
{
public:
    Archimedes(Uint32 flags, const char* title, int x, int y, int w, int h);
    ~Archimedes();
    virtual void runGameLoop();

    /// @brief Quits the game
    void stopGameLoop();

    /// @brief loads tiles into tileMap. 
    /// Additionally we have it loading coordinates and event maps.
    /// @param tileMap 
    /// @param coordinateToTileTypeMap 
    /// @param coordinateToEventTypeMap 
    /// @param TILE_COUNT 
    /// @param TYPE_COUNT 
    /// @param TILE_LENGTH 
    /// @return 
    bool loadTiles(std::vector<Tile*>& tileMap, 
                   const std::vector<int>& levelInfo,
                   std::map<std::pair<int, int>, TileType>& coordinateToTileTypeMap,
                   std::map<std::pair<int, int>, std::string>& coordinateToEventTypeMap,  
                   int TILE_COUNT, 
                   int TYPE_COUNT, 
                   int TILE_LENGTH
    );

    /// @brief Loads the image assets
    /// @param renderer 
    /// @param textureFilePaths 
    /// @return bool of success or failure
    bool loadImageAssets(SDL_Renderer* renderer, 
                         std::unordered_map<TextureWrapper*, std::string> textureFilePaths
    );

    /// @brief Given the sheet specs, will clip the sheet into SDL rectangles.
    /// @param ROWS 
    /// @param COLS 
    /// @param BLOCK_LENGTH 
    /// @param BLOCK_HEIGHT 
    /// @param TYPE_COUNT 
    /// @param sheetClipped 
    /// @return 
    bool clipSheet(int ROWS,
                   int COLS, 
                   int BLOCK_LENGTH,
                   int BLOCK_HEIGHT,
                   int TYPE_COUNT,
                   std::vector<SDL_Rect>& sheetClipped
    );

    /// @brief the public method to access the window object pointer
    /// @return pointer to the engine's SDL_Window instance
    SDL_Window* getWindow() const;

    /// @brief the public method to access the renderer object pointer
    /// @return pointer to the engine's SDL_Renderer instance
    SDL_Renderer* getRenderer() const;

    /// @brief Takes in a .map file and parses it into a vector
    /// in the format that we need it. It's an intermediate step to account for
    /// variable size maps.
    /// @param pathName 
    /// @return 
    std::vector<int> convertMapToVector(std::string pathName);

    bool getQuit() const;
    int getWidth() const;
    int getHeight() const;
    void setToQuit();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    bool quit = false;
    int width;
    int height;
};