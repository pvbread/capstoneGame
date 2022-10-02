#include <fstream>

bool loadTiles(std::vector<Tile*>& tileSet, 
               std::vector<SDL_Rect>& tilesClipped, 
               int TILE_COUNT, 
               int TYPE_COUNT, 
               int TILE_LENGTH)
{
    int x = 0;
    int y = 0;

    std::ifstream level("testLevel.map");

    if (level.fail())
    {
        SDL_Log("Failure loading level");
        return false;
    }

    //tyle type
    int type;

    for (int i = 0; i < TILE_COUNT; i++)
    {
        level >> type;

        if (level.fail())
        {
            SDL_Log("EOF error with level read at %d", i);
            return false;
        }

        //define total tile types
        if (type >= 0 && type < TYPE_COUNT)
        {
            tileSet[i] = new Tile(x, y, TILE_LENGTH, TILE_LENGTH, type);
        }
        //TODO DON't HARD CODE THIS
        //MAP_WIDTH
        x += TILE_LENGTH;

        if (x >= 1280)
        {
            x = 0;
            y += TILE_LENGTH;
        }
       
    }

    //clip
    //math is for 4 x 3 tile map
    //we have 240 x 160 if tile size is 80 (factor this away later)
    //so iterate on the smaller y dimension and set the boundaries of the 
    //rectangles accordingly
    type = 0;
    for (y = 0; y <= 160; y+=TILE_LENGTH)
    {
        
        for (x = 0; x <= 240; x+=TILE_LENGTH)
        {
            
            tilesClipped[type].x = x;
            tilesClipped[type].y = y;
            tilesClipped[type].w = TILE_LENGTH;
            tilesClipped[type].h = TILE_LENGTH;
            type++;
        
        }
    }
    level.close();

    return true;
}