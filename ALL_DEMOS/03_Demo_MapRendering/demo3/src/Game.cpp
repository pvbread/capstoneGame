#include "Game.h"
#include "Game/Utility/isTeamAlive.h"
#include "Game/Utility/setRoundTurns.h"

DashDaCapo::DashDaCapo(Uint32 flags, 
                       const char* title, 
                       int x, int y, 
                       int w, int h) : 
    Archimedes(flags, title, x, y, w, h){}

void DashDaCapo::runGameLoop()
{
    MapDebugController debugCont = MapDebugController();

    bool STATE_debug = false;

    //////////// START.TEXTURE LOADING /////////////
    TextureWrapper tileTexture;
    TextureWrapper tileTexture2;
    TextureWrapper characterInMapTexture;  
    //add sprite sheet here
    std::unordered_map<TextureWrapper*, std::string> textureFilePaths = {
        {&tileTexture, "../../assets/image/newspritedraft1.png"},
        {&tileTexture2, "../../assets/image/alt2.png"},
        {&characterInMapTexture, "../../assets/image/maxwell.png"},
    }; 
    
    bool didTexturesLoad = loadImageAssets(getRenderer(), 
                                           textureFilePaths
    );
    if (!didTexturesLoad)
    {
        SDL_Log("error loading image assets");
        setToQuit();
    }

    //////////// END TEXTURE LOADING /////////////
    
   


    //////////// START RANDOM MAP GEN /////////////
    
    std::string commandCall = "python ../mapBuilder/drunkardWalkTestMinusLibs.py ";
    
    system("python ../mapBuilder/drunkardWalkTestMinusLibs.py 1");
    
    
    
   

    //////////// END RANDOM MAP GEN /////////////

    //////////// START TILE LOADING /////////////
    
    

    std::vector<int> levelInfo = convertMapToVector("../../assets/maps/testLevelIntegration.map");
    const int MAP_COLS = levelInfo[1];
    const int MAP_ROWS = levelInfo[0]; 
    const int TILE_COUNT = MAP_COLS * MAP_ROWS;
    const int MAP_WIDTH = levelInfo[1] * 80;
    const int MAP_HEIGHT = levelInfo[0] * 80; 
    const int TILE_LENGTH = 80;
    const int TILE_TYPE_COUNT = 16;

    std::vector<Tile*> tileMap(TILE_COUNT);
    std::vector<SDL_Rect> tilesClipped(TILE_TYPE_COUNT);// this is the total tiles
    std::map<std::pair<int, int>, TileType> coordinateToTileTypeMap;
    std::map<std::pair<int, int>, std::string> coordinateToEventTypeMap;

    bool didTilesLoad = loadTiles(tileMap, 
                                  levelInfo,
                                  coordinateToTileTypeMap, 
                                  coordinateToEventTypeMap, 
                                  TILE_COUNT, 
                                  TILE_TYPE_COUNT, 
                                  TILE_LENGTH
    );
    if (!didTilesLoad)
    {
        SDL_Log("Error loading tiles");
        setToQuit();
    }

    const int TILE_SHEET_ROWS = 4;
    const int TILE_SHEET_COLS = 4;
    //clip tiles
    bool didClip = clipSheet(TILE_SHEET_ROWS, TILE_SHEET_COLS, TILE_LENGTH, TILE_LENGTH, TILE_TYPE_COUNT, tilesClipped);
    if (!didClip)
    {
        setToQuit();
    }
   
    //////////// END TILE LOADING /////////////

    

    /////////// START  CAMERA AND MAP CHARACTER INIT ///////
    //set this to 0 whenever we want a clear debug controller

    SDL_Rect characterControllerHitbox = {30, 30, 80, 80};
    CharacterInMap characterController(80, 0, 0, characterControllerHitbox);

    const int SCREEN_WIDTH = 960;
    const int SCREEN_HEIGHT = 720;
    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    /////////// END CAMERA AND MAP CHARACTER INIT ///////
    bool STATE_first = true;

    SDL_Event event;
    while (!getQuit())
    {

        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                setToQuit();
               
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_9)
                    STATE_debug = !STATE_debug;
                if (event.key.keysym.sym == SDLK_1)
                    STATE_first = !STATE_first;
            }
            if (STATE_debug)
                debugCont.onInput(event, MAP_WIDTH, MAP_HEIGHT);
            else 
                characterController.onInput(event);
     
        }
        ////////////// BEGIN RENDER SECTION /////////////////////

        //Clear screen
        SDL_SetRenderDrawColor(getRenderer(), 255, 255, 255, 255);
        SDL_RenderClear(getRenderer());
        
        if (STATE_debug)
        {
            debugCont.move(camera); 
        }
        else
        {
            characterController.move(MAP_WIDTH, MAP_HEIGHT);
            characterController.centerScreen(camera, MAP_WIDTH, MAP_HEIGHT);
        }
        if (STATE_first)
        for(int i = 0; i < tileMap.size(); i++)
        {
            tileMap[i]->render(getRenderer(), tileTexture, camera, tilesClipped);
        }
        if (!STATE_first)
        {
            for(int i = 0; i < tileMap.size(); i++)
        {
            tileMap[i]->render(getRenderer(), tileTexture2, camera, tilesClipped);
        } 
        }
          
        //Update screen
        SDL_RenderPresent(getRenderer());

        ////////////// END RENDER SECTION ///////////////////// 
 
    }
}
