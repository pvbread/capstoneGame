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

    Timer* timer = Timer::instance();
    MapDebugController debugCont = MapDebugController();


    //maybe enum this
    std::unordered_map<int, int> teamItemPool {
        {NORMAL_HIT, 0},
        {RARE_HIT, 0},
        {EPIC_HIT, 0},
        {NORMAL_DODGE, 0},
        {RARE_DODGE, 0},
        {EPIC_DODGE, 0},
        {NORMAL_SPEED, 0},
        {RARE_SPEED, 0},
        {EPIC_SPEED, 0}
    };    

    ///////// END ITEM INIT /////////

    // Timer Init
    //Timer* timer = Timer::instance();
    //bool timerStarted = false;
    //float countTime;

    // temporary place for this
    Screen screen = INTRO;
    //temporary 
    TTF_Font *font = TTF_OpenFont("./Raleway-Medium.ttf", 100);
    
    
    //temp
    SDL_Rect cursor = { 45, 160, 50, 50 };
    //incredibly temp


    
 

    /////////// START INIT STATES ///////

    // When new game is selected, set to false
    // when team dies, set back to true (or if you quite from in-game menu)
    bool STATE_gameOver = true;
    bool STATE_newGameSelected = false;
    bool STATE_battle = false;
    bool STATE_enemiesSet = false;
    bool STATE_isPlayerTurn = false; // also functions as inverse (isComputerTurn)
    bool STATE_combatMenuItemSelected = false;
    bool STATE_combatMenuTargetSelected = false;
    bool STATE_roundsSet = false;
    bool STATE_roundOver = false;
    bool STATE_mapEventboxOpen = false;
    bool STATE_timerStarted = false;
    bool STATE_debug = false;
    bool STATE_itemNotificationShowing = false;
    bool STATE_healNotificationShowing = false;
    bool STATE_preTransition = false;
    bool STATE_postTransition = true;
    bool STATE_statMenu = false;
    bool STATE_mapScreenOpenForTransition = false;
    bool STATE_timerAnimationStarted = false;
    bool STATE_didAnimationHappen = false;
    int STATE_lastCurrTarget = 0;
    float STATE_timerCount;
    float STATE_timerAnimationCount;
    int STATE_amountHealed;
    int STATE_characterDirection = LEFT;
    std::string STATE_introSelectedOption = "NONE";
    std::string STATE_helpMenuSelectedOption = "NONE";
    std::string STATE_combatSelectedOption = "NONE";
    std::string STATE_itemFound = "NONE";

    /////////// END INIT STATES /////// 

    //////////// MUSIC INIT /////////////////
    Mix_Music *SelectOST = Mix_LoadMUS("./bgmusic1.wav");
    Mix_Chunk *SelectMusic = Mix_LoadWAV("./MenuSelect.wav");
    //Mix_PlayMusic(SelectOST, -1); 

    //////////// START.TEXTURE LOADING /////////////
    TextureWrapper tileTexture;
    TextureWrapper characterInMapTextureDown;
    TextureWrapper characterInMapTextureLeft;
    TextureWrapper characterInMapTextureRight;
    TextureWrapper characterInMapTextureUp;
    TextureWrapper characterTestTexture;
    TextureWrapper combatScreenTexture;
    TextureWrapper blackScreenTransition;
    TextureWrapper flutistTexture;
    TextureWrapper bassistTexture;
    TextureWrapper drummerTexture;
    TextureWrapper linebackerTexture;
    TextureWrapper currPlayerTexture;
    TextureWrapper targetTexture;
    TextureWrapper getHitEffect;

    //add sprite sheet here
    std::unordered_map<TextureWrapper*, std::string> textureFilePaths = {
        {&tileTexture, "../../assets/image/newspritedraft.png"},
        {&characterInMapTextureDown, "../../assets/image/chars/flutist-sprite-down.png"},
        {&characterInMapTextureLeft, "../../assets/image/chars/flutist-sprite-left.png"},
        {&characterInMapTextureRight, "../../assets/image/chars/flutist-sprite-right.png"},
        {&characterInMapTextureUp, "../../assets/image/chars/flutist-sprite-up.png"},
        {&characterTestTexture, "../../assets/image/char.png"},
        {&combatScreenTexture, "../../assets/image/combat_screen.png"},
        {&blackScreenTransition, "../../assets/image/blackScreen.png"},
        {&flutistTexture, "../../assets/image/chars/flutist.png"},
        {&bassistTexture, "../../assets/image/chars/bassist.png"},
        {&drummerTexture, "../../assets/image/chars/drummer.png"},
        {&linebackerTexture, "../../assets/image/chars/linebacker.png"},
        {&currPlayerTexture, "../../assets/image/treble.png"},
        {&targetTexture, "../../assets/image/sixteenth.png"},  
        {&getHitEffect, "../../assets/image/explosion-notes.png"}
    }; 
    
    //so there's going to be a couple of these per char
    //maybe a map would do well here?

    //TODO in the destructor, clean this up
    //maybe better to have a dedicated function to map the coordinate tiles?
    bool didTexturesLoad = loadImageAssets(getRenderer(), 
                                           textureFilePaths
    );
    if (!didTexturesLoad)
    {
        SDL_Log("error loading image assets");
        setToQuit();
    }

    //////////// END TEXTURE LOADING /////////////
    
  


    

    //////////// START TILE LOADING /////////////
    
    // load random map
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> intDist(0,4);
    int mapRandom = intDist(gen);
    std::vector<std::string> tests = {"testLevel.map", "testLevel2.map", "testLevel3.map", "testLevel4.map", "testLevel5.map"};
    std::string testLevel = tests[mapRandom]; 

    std::vector<int> levelInfo = convertMapToVector("../../assets/maps/" + testLevel);
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

    ////////////START SCREEN TRANSITION INIT ///////////

    int alphaValueScreenTransition = 255;

    ////////////END START SCREEN TRANSITION INIT ///////////

    //////////// START MAP EVENT GENERATION ////////

    std::vector<std::string> eventList(6, "BLANKEVENT");
    std::vector<std::string> eventsToAdd {
        "BATTLE",
        "BATTLE",
        "HEAL",
        "JOKE",
        "ITEM"
    };
    for (auto event: eventsToAdd)
        eventList.push_back(event);
    
    std::uniform_int_distribution<> dist(0,10); 

    for (auto& [coordinate, event]: coordinateToEventTypeMap)
    {
        if (coordinateToTileTypeMap[coordinate] == ENDDOWN ||
            coordinateToTileTypeMap[coordinate] == ENDUP ||
            coordinateToTileTypeMap[coordinate] == ENDLEFT ||
            coordinateToTileTypeMap[coordinate] == ENDRIGHT)
        {
            event = "BOSS";
        }
        else
            event = eventList[dist(gen)];  
    }

    std::string nextMapEvent = "";
    SDL_Rect mapEventBox = {200, 200, 400, 100};

    //////////// END MAP EVENT GENERATION //////// 

    /////////// START  CAMERA AND MAP CHARACTER INIT ///////
    //set this to 0 whenever we want a clear debug controller

    SDL_Rect characterControllerHitbox = {30, 30, 80, 80};
    CharacterInMap characterController(80, 0, 0, characterControllerHitbox);

    const int SCREEN_WIDTH = 960;
    const int SCREEN_HEIGHT = 720;
    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    /////////// END CAMERA AND MAP CHARACTER INIT ///////

    

    

  

    //BaseItem test("some item", "normal", 5);

    //double degrees = 0;
    //SDL_RendererFlip flipType = SDL_FLIP_NONE;
    int STATUS_show = 1;
    float STATE_angle = 0.0;
    SDL_Event event;
    SDL_Rect r1 = {0, 0, 80, 80};
    SDL_Rect r2 = {80, 0, 80, 80};
    SDL_Rect r3 = {160, 0, 80, 80};
    SDL_Rect r4 = {240, 0, 80, 80};
    SDL_Rect r5 = {240, 80, 80, 80};
    SDL_Point point = {200, 200};
    while (!getQuit())
    {
        /// POST-PROCESSOR ///

        timer->update();

        //////SCREEN TRANSITIONS////////
        
        
        //////END SCREEN TRANSITIONS////////

        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                setToQuit();
                break;
            }
            if (STATUS_show == 6)
                debugCont.onInput(event, MAP_WIDTH, MAP_HEIGHT);
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_1:
                    {
                        STATUS_show = 1;
                        break;
                    }
                    case SDLK_2:
                    {
                        STATUS_show = 2;
                        break;
                    }
                    case SDLK_3:
                    {
                        STATUS_show = 3;
                        break;
                    }
                    case SDLK_4:
                    {
                        STATUS_show = 4;
                        break;
                    }
                    case SDLK_5:
                    {
                        STATUS_show = 5;
                        break;
                    }
                    case SDLK_6:
                    {
                        STATUS_show = 6;
                        break;
                    }
                    case SDLK_7:
                    {
                        STATUS_show = 7;
                        break;
                    }
                    case SDLK_8:
                    {
                        STATUS_show = 8;
                        break;
                    }
                    case SDLK_9:
                    {
                        STATUS_show = 9;
                        break;
                    }
                    case SDLK_0:
                    {
                        STATUS_show = 10;
                        break;
                    }
                    case SDLK_q:
                    {
                        STATUS_show = 7;
                        STATE_angle += 15;
                        break;
                    }
                }
            }
            
            
        }
        //Clear screen
        SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
        SDL_RenderClear(getRenderer());

        
        
        if (STATUS_show == 1)
            tileTexture.render(getRenderer(), 0, 0);
        if (STATUS_show == 2)
        {
            tileTexture.render(getRenderer(), 0, 0, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
            tileTexture.render(getRenderer(), 350, 350, &r1, 0.0, nullptr, SDL_FLIP_NONE);
        }
        if (STATUS_show == 3)
        {
            tileTexture.render(getRenderer(), 0, 0, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
            tileTexture.render(getRenderer(), 350, 350, &r2, 0.0, nullptr, SDL_FLIP_NONE);
        }
        if (STATUS_show == 4)
        {
            tileTexture.render(getRenderer(), 0, 0, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
            tileTexture.render(getRenderer(), 350, 350, &r3, 0.0, nullptr, SDL_FLIP_NONE);
        }
        if (STATUS_show == 5)
        {
            tileTexture.render(getRenderer(), 0, 0, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
            tileTexture.render(getRenderer(), 350, 350, &r4, 0.0, nullptr, SDL_FLIP_NONE);
        }
        if (STATUS_show == 6)
        {
            debugCont.move(camera);
            for(int i = 0; i < tileMap.size(); i++)
            {
                tileMap[i]->render(getRenderer(), tileTexture, camera, tilesClipped);
            } 
        }
        
                
               
        //Update screen
        SDL_RenderPresent(getRenderer());
       
 
    }
}
