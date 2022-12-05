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

    ////////// START CHARACTER INIT ////////
    BasePlayer conductor = BasePlayer("conductor", 30, 3, 3, 0, 3, 3, 3);
    BasePlayer drum = BasePlayer("drummer", 50, 2, 1, 0, 3, 3, 3);
    BasePlayer flute = BasePlayer("flutist", 20, 6, 1, 0, 3, 3, 3);
    Bass bass = Bass("bassist", 60, 1, 3, 0, 3, 3, 3);
    flute.setNewParticipantsIndex(0);
    conductor.setNewParticipantsIndex(1);
    bass.setNewParticipantsIndex(2);
    drum.setNewParticipantsIndex(3);
    std::vector<BaseCharacter> playerTeam{flute, conductor, bass, drum};
    std::vector<BaseCharacter> enemies;
    std::vector<BaseCharacter> combatParticipants;
    // set player index
    

    ///////// END CHARACTER INIT //////


    ///////// START ITEM INIT ////////

    BaseItem normalHit = BaseItem("Normal Intonation Enhancement", "hit", 1);
    BaseItem rareHit = BaseItem("Rare Intonation Enhancement", "hit", 3);
    BaseItem epicHit = BaseItem("Epic Intonation Enhancement", "hit", 5);
    BaseItem normalDodge = BaseItem("Normal Faking Enhancement", "dodge", 1);
    BaseItem rareDodge = BaseItem("Rare Faking Enhancement", "dodge", 2);
    BaseItem epicDodge = BaseItem("Epic Faking Enhancement", "dodge", 3);
    BaseItem normalSpeed = BaseItem("Normal Metronome", "speed", 1);
    BaseItem rareSpeed = BaseItem("Rare Metronome", "speed", 2);
    BaseItem epicSpeed = BaseItem("Epic Metronome", "speed", 3);

    // access in the following way itemList[NORMAL_HIT]
    // since we already have an ItemEnum for the item indexes
    std::vector<BaseItem> itemList {
        normalHit,
        rareHit,
        epicHit,
        normalDodge,
        rareDodge,
        epicDodge,
        normalSpeed,
        rareSpeed,
        epicSpeed
    };

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

    //////////// START MENUS INIT ///////////////

    const std::vector<std::string> introOptions = {
        "New Game",
        "Load Game",
        "Credits"
    };

    //const char* railwayFontPath = "./Raleway-Medium.ttf";
    SDL_Color introMenuColor = { 255, 0, 0, 255 };
    BaseMenu introMenu = BaseMenu(100, 100, 140, 400, 100,  
                                  introOptions, 
                                  Font::raleway, 
                                  Color::red,
                                  Color::maroon,
                                  getRenderer()
    );

    const std::vector<std::string> combatOptionsStrings = {
        "Attack",
        "Buff",
        "Debuff",
        "Move"
    };

    //SDL_Color combatMenuColor = { 0, 0, 255, 255 };
    /*
    BaseMenu combatMenu = BaseMenu(50, 25, 520, 200, 50, 
                                   combatOptionsStrings, 
                                   Font::raleway, 
                                   Color::red,
                                   Color::maroon, 
                                   getRenderer()
    );*/
    BaseMenu combatMenu = BaseMenu(50, 750, 490, 100, 50, 
                                   combatOptionsStrings, 
                                   Font::openSans, 
                                   Color::white, 
                                   Color::navy,
                                   getRenderer()
    );

    //////////// END MENUS INIT ///////////////
 

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
    float STATE_timerCount;
    int STATE_amountHealed;
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
    TextureWrapper characterInMapTexture;
    TextureWrapper debugControllerTexture;
    TextureWrapper characterTestTexture;
    TextureWrapper combatScreenTexture;
    //add sprite sheet here
    std::unordered_map<TextureWrapper*, std::string> textureFilePaths = {
        {&tileTexture, "../../assets/image/newspritedraft.png"},
        {&characterInMapTexture, "../../assets/image/dot.bmp"},
        {&debugControllerTexture, "../../assets/image/dot.bmp"},
        {&characterTestTexture, "../../assets/image/char.png"},
        {&combatScreenTexture, "../../assets/image/combat_screen.png"} 
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
    
    /////////// COMBAT SCREEN THINGS //////////
    std::string prefixHP = "HP: ";

    

    TextBox combatStatFlute = TextBox("flute", 25, 50, 630, 150, 30);
    TextBox combatStatFluteHP = TextBox(prefixHP + std::to_string(flute.getHp()), 25, 50, 660, 150, 30);
    
    TextBox combatStatConductor = TextBox("conductor", 25, 200, 630, 150, 30);
    TextBox combatStatConductorHP = TextBox(prefixHP + std::to_string(conductor.getHp()), 25, 200, 660, 150, 30);

    TextBox combatStatBass = TextBox("bass", 25, 350, 630, 150, 30);
    TextBox combatStatBassHP = TextBox(prefixHP + std::to_string(bass.getHp()), 25, 350, 660, 150, 30);
    
    TextBox combatStatDrum = TextBox("drum", 25, 500, 630, 150, 30);
    TextBox combatStatDrumHP = TextBox(prefixHP + std::to_string(drum.getHp()), 25, 500, 660, 150, 30);

    std::vector<TextBox> combatStatusRow {
        combatStatFlute, combatStatConductor,
        combatStatBass, combatStatDrum,
        combatStatFluteHP, combatStatConductorHP,
        combatStatBassHP, combatStatDrumHP, 
    };


    ///////////// END COMBAT SCREEN THINGS /////////


    //////////// START RANDOM MAP GEN /////////////
    /*
    std::string commandCall = "python ./mapBuilder/drunkardWalkTestMinusLibs.py ";
    std::vector<std::string> commandCalls;
    for (int i = 2; i < 6; i++)
    {
        commandCalls.push_back(commandCall + std::to_string(i));
    }
    system("python ./mapBuilder/drunkardWalkTestMinusLibs.py 1");
    */
    /*
    for (auto call: commandCalls)
    {
        system(call.c_str()); 
    }
    */
   

    //////////// END RANDOM MAP GEN /////////////

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
    const int TEST_CHAR_SHEET_ROWS = 1;
    const int TEST_CHAR_SHEET_COLS = 5;
    const int ANIMATION_FRAME_COUNT = 5;
    int currFrameNum = 0;
    int currRectNum = 0;
    
    std::vector<SDL_Rect> spriteClipped(ANIMATION_FRAME_COUNT);
    std::unordered_map<TextureWrapper*, int> textureFrameCount = {
        {&characterTestTexture, ANIMATION_FRAME_COUNT}
    };
    std::unordered_map<TextureWrapper*, std::vector<SDL_Rect>> texturePtrToSpriteMap = {
        {&characterTestTexture, spriteClipped}
    };

    didClip = clipSheet(TEST_CHAR_SHEET_ROWS, TEST_CHAR_SHEET_COLS, 64, 64, ANIMATION_FRAME_COUNT, spriteClipped);
    if (!didClip)
    {
        setToQuit();
    }

    //////////// END TILE LOADING /////////////

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
    debugControllerTexture.setAlpha(0);

    SDL_Rect debugHitbox = {0, 0, 1, 1};
    SDL_Rect characterControllerHitbox = {30, 30, 80, 80};
    MapDebugController debugController(10, 0, 0, debugHitbox);
    CharacterInMap characterController(80, 0, 0, characterControllerHitbox);

    const int SCREEN_WIDTH = 960;
    const int SCREEN_HEIGHT = 720;
    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    /////////// END CAMERA AND MAP CHARACTER INIT ///////



    std::vector<SDL_Rect> charBoxes(8);
    for (int i = 0; i < charBoxes.size(); i++)
    {
        SDL_Rect temp = {(i*100), 300, 64, 64};
        charBoxes[i] = temp; 
    }

    std::vector<SDL_Rect> hpBoxes(8);
    for (int i = 0; i < hpBoxes.size(); i++)
    {
        SDL_Rect temp = {(50+(i*100)), 30, 64, 64};
        hpBoxes[i] = temp; 
    }

    ////////// START BATTLE ORDER INIT ///////////////

    std::vector<SDL_Rect> orderBoxes(8);
    for (int i = 0; i < orderBoxes.size(); i++)
    {
        SDL_Rect temp = {750, 30+(i*50), 200, 30};
        orderBoxes[i] = temp; 
    }

    SDL_Rect orderTitleBox = {750, 280, 200, 30};
    std::string orderTitle = "ORDER        ";
    int currOrderNum = 0;
    std::vector<std::string> roundOrder;

    ////////// END BATTLE ORDER INIT ///////////////

    ////////// START BATTLE NOTIFICATION //////////

    TextBox battleNotification = TextBox("", 30, 0, 564, 500, 60);

    /////////  END BATTLE NOTIFICATION ///////////


    std::vector<std::string> tempCharNames {
        "flute        ",
        "conductor    ",
        "drums        ",
        "bass         ",
        "coneheadAlpha",
        "coneheadBeta ",
        "coneheadTheta",
        "Carl         "
    };
    //TODO account for dead chars in order
    

    TTF_Font *orderFont = TTF_OpenFont("./Raleway-Medium.ttf", 50);

    int currTarget = 0;
    std::vector<std::vector<int>> validMoves;
    bool actionChosen = false;


    ///////////////////////////////////
    ///////// BEGIN SANDBOX ///////////
    ///////////////////////////////////

    //char b = '\u0444';
    
    //Menu///////////////////////////////////

    const std::vector<std::string> sandboxOptionsStrings = {
        "Attack",
        "Buff",
        "Debuff",
        "Move"
    };

    BaseMenu sandboxMenu = BaseMenu(50, 750, 490, 100, 50, 
                                   sandboxOptionsStrings, 
                                   Font::openSans, 
                                   Color::white, 
                                   Color::navy,
                                   getRenderer()
    );

    //Order///////////////////////////////////

    int initialOrderHeight = 50;
    std::vector<TextBox> sandboxQue;
    std::vector<TextBox> sandboxQueIcons;

    for (int i = 0; i < 8; i++)
    {
        TextBox orderRect = TextBox("-->", 50, 750, (initialOrderHeight + (i*50)),
            80, 50, Font::roboto, Color::blue, Color::white);
        sandboxQue.push_back(orderRect);
        //Icons
        TextBox orderIcons = TextBox("X", 50, 830, (initialOrderHeight + (i*50)),
            50, 50, Font::roboto, Color::blue, Color::red);
        sandboxQueIcons.push_back(orderIcons);
    }
    
    //Status///////////////////////////////////
    //std::vector<TextBox> StatusRow;
    TextBox statBass = TextBox("bass", 25, 50, 630, 150, 30);
    TextBox statBassHP = TextBox("Hp:", 25, 50, 660, 150, 30);

    TextBox statDrum = TextBox("drum", 25, 200, 630, 150, 30);
    TextBox statDrumHP = TextBox("Hp:", 25, 200, 660, 150, 30);

    TextBox statFlute = TextBox("flute", 25, 350, 630, 150, 30);
    TextBox statFluteHP = TextBox("Hp:", 25, 350, 660, 150, 30);
    
    TextBox statConductor = TextBox("conductor", 25, 500, 630, 150, 30);
    TextBox statConductorHP = TextBox("Hp:", 25, 500, 660, 150, 30);

    std::vector<TextBox> statusRow{statBass, statDrum, statFlute, statConductor};


    ///////// END SANDBOX ///////////
    //////// CHARACTER STATS ////////
    int startBoxWidth = 100;
    int slashBoxWidth = 150;
    std::vector<TextBox> characterStatsHP;
    std::vector<TextBox> characterStatsSpeed;
    std::vector<TextBox> characterStatsHit;
    std::vector<TextBox> characterStatsArmor;
    std::vector<TextBox> characterStatsDodge;
    std::vector<TextBox> characterStatSlash;
    for (int i = 0; i < 4; i++)
    {
        TextBox statsHP = TextBox("HP  ", 100, 200, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan);
        characterStatsHP.push_back(statsHP);
        
        TextBox statsSpeed = TextBox("Speed  ", 100, 325, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan);
        characterStatsSpeed.push_back(statsSpeed);

        TextBox statsHit = TextBox("Hit  ", 100, 450, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan);
        characterStatsHit.push_back(statsHit);

        TextBox statsArmor = TextBox("Armor  ", 100, 575, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan);
        characterStatsArmor.push_back(statsArmor);
        
        TextBox statsDodge = TextBox("Dodge ", 100, 700, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan);
        characterStatsDodge.push_back(statsDodge);

        TextBox justASlash = TextBox("/", 100, 240, (slashBoxWidth + (i*150)),
         10, 50, Font::roboto, Color::blue, Color::cyan);
        characterStatSlash.push_back(justASlash);
        
    }   
    
    ////// END CHARACTER STATS //////

    //BaseItem test("some item", "normal", 5);

    //double degrees = 0;
    //SDL_RendererFlip flipType = SDL_FLIP_NONE;

    SDL_Event event;
    while (!getQuit())
    {
        timer->update();
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                setToQuit();
                break;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_1:
                    {
                        screen = INTRO;
                        break;
                    }
                    case SDLK_2:
                    {
                        screen = MAP;
                        break;
                    }
                    case SDLK_3:
                    {
                        //screen = COMBAT;//will segfault hahahhahahahah
                        break;
                    }
                    case SDLK_4:
                    {
                        screen = SANDBOX;
                        break;
                    }
                    case SDLK_5:
                    {
                        screen = STATUS_MENU;
                        break;
                    }
                    case SDLK_6:
                    {
                        screen = WIN;
                        break;
                    }
                    case SDLK_7:
                    {
                        screen = DEFEAT;
                        break;
                    }
                }
            }
            switch (screen)
            { 
                case MAP:
                {
                    if (event.type == SDL_KEYDOWN)
                    {
                        if (event.key.keysym.sym == SDLK_9)
                            STATE_debug = !STATE_debug;
                    }
                    if (STATE_debug)
                        debugController.onInput(event);
                    else if (!STATE_mapEventboxOpen)
                        characterController.onInput(event, nextMapEvent, STATE_mapEventboxOpen, coordinateToTileTypeMap, coordinateToEventTypeMap);
                    break;
                }
            }
            
        }
        //Clear screen
        SDL_SetRenderDrawColor(getRenderer(), 255, 255, 255, 255);
        SDL_RenderClear(getRenderer());

        switch (screen)
        {

            case MAP:
            {
                if (STATE_debug)
                {
                    debugController.move(MAP_WIDTH, MAP_HEIGHT);
                    debugController.centerScreen(camera, MAP_WIDTH, MAP_HEIGHT);
                }
                else
                {
                    characterController.move(MAP_WIDTH, MAP_HEIGHT);
                    characterController.centerScreen(camera, MAP_WIDTH, MAP_HEIGHT);
                }
                for(int i = 0; i < tileMap.size(); i++)
                {
                    tileMap[i]->render(getRenderer(), tileTexture, camera, tilesClipped);
                }
                if (STATE_debug)
                    debugController.render(getRenderer(), camera, debugControllerTexture);
                else
                    characterController.render(getRenderer(), camera, characterInMapTexture);
                
                break;
            } 
            
            
        }

        
        //Update screen
        SDL_RenderPresent(getRenderer());
       
 
    }
}
