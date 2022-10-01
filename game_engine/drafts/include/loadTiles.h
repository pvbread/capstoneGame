#include <fstream>

bool loadTiles(std::vector<Tile> tiles, std::vector<SDL_Rect>& tilesClipped, int TILE_COUNT, int TYPE_COUNT)
{
    int x = 0;
    int y = 0;

    std::ifstream level("testLevel.map");

    if (level.fail())
    {
        SDL_Log("Failure loading level");
        return false;
    }

    for (int i = 0; i < TILE_COUNT; i++)
    {
        //tile type
        int type;

        level >> type;

        if (level.fail())
        {
            SDL_Log("EOF error with level read at %d", i);
            return false;
        }

        //define total tile types
        if (type >= 0 && type < TYPE_COUNT)
        {
            SDL_Log("Tyle type error at %d", i);
            return false;
        }

        if (x >= MAP_WIDTH)
        {
            x = 0;
            y += TILE_HEIGHT;
        }
        else
        {
            x += TILE_WIDTH;
        }
    }

    //clip
    //math is for 4 x 3 tile map
    //we have 240 x 160 if tile size is 80 (factor this away later)
    //so iterate on the smaller y dimension and set the boundaries of the 
    //rectangles accordingly
    for (y = 0; y <= 160; y+=TILE_WIDTH)
    {
        for (x = 0; y <= 240; x+=TILE_HEIGHT)
    }

    return true;
}