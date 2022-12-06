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
    TextureWrapper flutistTexture;
    TextureWrapper bassistTexture;
    TextureWrapper linebackerTexture;
    TextureWrapper currPlayerTexture;
    TextureWrapper targetTexture;
    //add sprite sheet here
    std::unordered_map<TextureWrapper*, std::string> textureFilePaths = {
        {&tileTexture, "../../assets/image/newspritedraft.png"},
        {&characterInMapTexture, "../../assets/image/dot.bmp"},
        {&debugControllerTexture, "../../assets/image/dot.bmp"},
        {&characterTestTexture, "../../assets/image/char.png"},
        {&combatScreenTexture, "../../assets/image/combat_screen.png"},
        {&flutistTexture, "../../assets/image/chars/flutist.png"},
        {&bassistTexture, "../../assets/image/chars/bassist.png"},
        {&linebackerTexture, "../../assets/image/chars/linebacker.png"},
        {&currPlayerTexture, "../../assets/image/treble.png"},
        {&targetTexture, "../../assets/image/sixteenth.png"}  
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
    /*
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
    }*/

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
        SDL_Rect temp = {(i*90), 300, 64, 64};
        charBoxes[i] = temp; 
    }

    std::vector<SDL_Rect> hpBoxes(8);
    for (int i = 0; i < hpBoxes.size(); i++)
    {
        SDL_Rect temp = {(50+(i*100)), 30, 64, 64};
        hpBoxes[i] = temp; 
    }

    ////////// START BATTLE ORDER INIT ///////////////

    std::vector<TextBox> orderBoxes(8);
    for (int i = 0; i < orderBoxes.size(); i++)
    {
        TextBox temp = TextBox("", 25, 750, 30+(i*50), 200, 30, Font::openSans, Color::black, Color::gray);
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
        "flutist",
        "conductor",
        "drummer",
        "bassist",
        "coneheadAlpha",
        "coneheadBeta ",
        "coneheadTheta",
        "Carl"
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
                case INTRO:
                { 
                    introMenu.onInput(event, SelectMusic, STATE_introSelectedOption);
                    if (STATE_introSelectedOption != "NONE")
                    {
                        if (STATE_introSelectedOption == "New Game")
                        {
                            STATE_newGameSelected = true;
                            STATE_gameOver = false;
                            screen = MAP;
                        }
                    }
                    break;
                }
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
                    
                    
                    if (nextMapEvent == "BATTLE")
                    {
                        //init enemy characters
                        BaseCharacter e1 = BaseCharacter("coneheadAlpha", 10, 2, 1, 0, 3, 3, 3, true);
                        BaseCharacter e2 = BaseCharacter("coneheadBeta ", 10, 6, 1, 0, 3, 3, 3, true);
                        BaseCharacter e3 = BaseCharacter("coneheadKappa", 10, 2, 1, 0, 3, 3, 3, true);
                        BaseCharacter e4 = BaseCharacter("Carl         ", 20, 0, 1, 0, 3, 3, 3, true);
                        //normally this will just get enemies from a randomly selected "PACK"
                        e1.setNewParticipantsIndex(4);
                        e2.setNewParticipantsIndex(5);
                        e3.setNewParticipantsIndex(6);
                        e4.setNewParticipantsIndex(7);
                        std::vector<BaseCharacter> enemies{e1, e2, e3, e4};
                        
                        //this might not be necessary
                        combatParticipants = playerTeam;
                        combatParticipants.insert(std::end(combatParticipants), std::begin(enemies), std::end(enemies));

                        //change enemiesSet state
                        STATE_enemiesSet = true;
                        //change battle state
                        STATE_battle = true;
                        //setRoundOrder
                        roundOrder = setRoundTurns(combatParticipants);
                        STATE_roundsSet = true;
                        screen = COMBAT;
                        nextMapEvent = "BLANKEVENT";
                        STATE_mapEventboxOpen = false;
                        // update setRoundTurns display
                        for(int i = 0; i < roundOrder.size(); i++)
                        {
                            orderBoxes[i].changeText(roundOrder[i]);
                        }
                    }
                    else if (nextMapEvent == "ITEM" && !STATE_itemNotificationShowing)
                    {
                        //std::random_device rd;
                        //std::mt19937 gen(rd());
                        std::uniform_int_distribution<> distForRarity(1,100);
                        std::uniform_int_distribution<> distForItem(0,2);
                        int itemRoll = distForItem(gen);
                        int rarityRoll = distForRarity(gen);
                        //if it's 0 it's a NORMAL_HIT
                        if (itemRoll == 1)
                        {
                            itemRoll = NORMAL_DODGE;
                        }
                        else if (itemRoll == 2)
                        {
                            itemRoll = NORMAL_SPEED;
                        }
                        //this just bumps up the item quality
                        if (rarityRoll > 70 && rarityRoll < 95)
                        {
                            itemRoll += 1;
                        }
                        else if (rarityRoll >= 95)
                        {
                            itemRoll += 2;
                        }
                        //add the item to the item pool
                        teamItemPool[itemRoll]++;
                        STATE_itemFound = itemList[itemRoll].getName();
                        STATE_itemNotificationShowing = true;
                        
                    }
                    else if (nextMapEvent == "HEAL" && !STATE_healNotificationShowing)
                    {
                        std::uniform_int_distribution<> distForHeal(1,3);
                        STATE_amountHealed = distForHeal(gen);
                        for (auto& player: playerTeam)
                        {
                            if (player.getHp() != player.getMaxHp())
                                player.setHp(player.getHp() + STATE_amountHealed);
                        }
                        STATE_healNotificationShowing = true;
                    }
                    
                    
                    if (event.type == SDL_KEYDOWN)
                    {
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_RETURN:
                            {
                                STATE_mapEventboxOpen = false;
                                STATE_itemNotificationShowing = false;
                                STATE_healNotificationShowing = false;
                                nextMapEvent = "BLANKEVENT";
                                break;
                            }
                            case SDLK_e:
                            {
                                screen = STATUS_MENU;
                                break;
                            }
                        }
                    } 
                    break;
                }
                case COMBAT:
                {   
                    
                    if (STATE_combatSelectedOption != "NONE")
                    {
                        if (event.type == SDL_KEYDOWN)
                        {
                            switch (event.key.keysym.sym)
                            {
                                case SDLK_LEFT:
                                {
                                    currTarget--;
                                    if (currTarget < 0)
                                        currTarget = 0;
                                    break;
                                }
                                case SDLK_RIGHT:
                                {
                                    currTarget++;
                                    if (currTarget == validMoves.size())
                                        currTarget = validMoves.size()-1;
                                    break;
                                }
                                case SDLK_RETURN:
                                {
                                    STATE_combatMenuTargetSelected = true;

                                    break;
                                }
                            }
                        }     
                    }

                    // create new round and set round turns
                    if(!STATE_roundsSet && STATE_roundOver)
                    {
                        // get new round turn order
                        roundOrder = setRoundTurns(combatParticipants);
                        
                        STATE_roundsSet = true;
                        STATE_roundOver = false;
                    }
     
                    if (STATE_combatSelectedOption == "NONE" && !STATE_combatMenuTargetSelected)
                        combatMenu.onInput(event, SelectMusic, STATE_combatSelectedOption);
                    

                    // state for when a round ends
                    if ((currOrderNum + 1) == roundOrder.size() && STATE_combatSelectedOption!= "None" && STATE_combatMenuTargetSelected)
                    {
                        STATE_roundOver = true;
                        STATE_roundsSet = false;
                    }
                    if (STATE_combatMenuTargetSelected)
                    {   
                        //do thing;
                        if (STATE_combatSelectedOption == "Attack")
                        {
                            //WAS the round order properly set??
                            std::vector<int> attackDamage;
                            STATE_combatSelectedOption = "NONE";

                            //std::vector<int> attackDamage;
                            for (int i = 0; i < combatParticipants.size();i++)
                            {
                                if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                                {
                                    int charIndex = combatParticipants[i].getParticipantsIndex();
                                    validMoves = combatParticipants[i].getValidMoves(ATTACK,charIndex,combatParticipants);
                                    std::string currPlayerName = combatParticipants[i].getName();
                                    currPlayerName.erase(std::remove_if(currPlayerName.begin(),currPlayerName.end(), ::isspace),currPlayerName.end());
                                    std::string targetNotification;
                                    targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                    combatParticipants = combatParticipants[i].doAction(ATTACK,attackDamage,validMoves[currTarget],combatParticipants);
                                    std::string attackNotification;
                                    for (int j = 0; j < validMoves[currTarget].size(); j++)
                                    {
                                        if (j == 0)
                                        {
                                            attackNotification += currPlayerName;
                                            attackNotification += " attacks: ";
                                            attackNotification += std::to_string(attackDamage[j]);
                                            attackNotification += " dmg dealt to ";
                                            attackNotification += targetNotification;
                                            continue;
                                        }
                                        attackNotification += " *** ";
                                        attackNotification += std::to_string(attackDamage[j]);
                                        attackNotification += " dmg dealt to ";
                                        attackNotification += combatParticipants[validMoves[currTarget][j]].getName();

                                    }
                                    battleNotification.changeText(attackNotification);
                                    STATE_timerStarted = true;
                                    STATE_timerCount = timer->deltaTime() + 3;
                                    break;
                                }
                            }  
                        }

                        if (STATE_combatSelectedOption == "Buff")
                        {
                            //WAS the round order properly set??
                            
                            STATE_combatSelectedOption = "NONE";
                            std::vector<int> healAmount;

                            //look at roundOrder
                            for (int i = 0; i < combatParticipants.size();i++)
                            {
                                if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                                {
                                    int charIndex = combatParticipants[i].getParticipantsIndex();
                                    validMoves = combatParticipants[i].getValidMoves(BUFF,charIndex,combatParticipants);
                                    std::string currPlayerName = combatParticipants[i].getName();
                                    currPlayerName.erase(std::remove_if(currPlayerName.begin(),currPlayerName.end(), ::isspace),currPlayerName.end());
                                    std::string targetNotification;
                                    targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                    combatParticipants = combatParticipants[i].doAction(BUFF,healAmount,validMoves[currTarget],combatParticipants);
                                    std::string healNotification;
                                    for (int j = 0; j < validMoves[currTarget].size(); j++)
                                    {
                                        if (j == 0)
                                        {
                                            healNotification += currPlayerName;
                                            healNotification += " heals: ";
                                            healNotification += std::to_string(healAmount[j]);
                                            healNotification += " healed for ";
                                            healNotification += targetNotification;
                                            continue;
                                        }
                                    }
                                    battleNotification.changeText(healNotification);
                                    STATE_timerStarted = true;
                                    STATE_timerCount = timer->deltaTime() + 3;
                                    break;
                                }
                            }
                        }

                        if (STATE_combatSelectedOption == "Debuff")
                        {
                            //WAS the round order properly set??
                            STATE_combatSelectedOption = "NONE";

                            //look at roundOrder
                            std::vector<int> newSpeed;
                            for (int i = 0; i < combatParticipants.size();i++)
                            {
                                if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                                {
                                    int charIndex = combatParticipants[i].getParticipantsIndex();
                                    validMoves = combatParticipants[i].getValidMoves(DEBUFF,charIndex,combatParticipants);
                                    std::string currPlayerName = combatParticipants[i].getName();
                                    currPlayerName.erase(std::remove_if(currPlayerName.begin(),currPlayerName.end(), ::isspace),currPlayerName.end());
                                    std::string targetNotification;
                                    targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                    combatParticipants = combatParticipants[i].doAction(DEBUFF,newSpeed,validMoves[currTarget],combatParticipants);
                                    std::string debuffNotification;
                                    for (int j = 0; j < validMoves[currTarget].size(); j++)
                                    {
                                        if (j == 0)
                                        {
                                            debuffNotification += currPlayerName;
                                            debuffNotification += " debuffs: ";
                                            debuffNotification += std::to_string(newSpeed[j]);
                                            debuffNotification += " is the new speed for ";
                                            debuffNotification += targetNotification;
                                            continue;
                                        }
                                    }
                                    battleNotification.changeText(debuffNotification);
                                    STATE_timerStarted = true;
                                    STATE_timerCount = timer->deltaTime() + 3;
                                    break;
                                }
                            }
                        }

                        if (STATE_combatSelectedOption == "Move")
                        {
                            //WAS the round order properly set??
                            STATE_combatSelectedOption = "NONE";
                            //look at roundOrder
                            std::vector<int> nothing;
                            for (int i = 0; i < combatParticipants.size();i++)
                            {
                                if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                                {
                                    int charIndex = combatParticipants[i].getParticipantsIndex();
                                    validMoves = combatParticipants[i].getValidMoves(MOVE,charIndex,combatParticipants);
                                    std::string currPlayerName = combatParticipants[i].getName();
                                    currPlayerName.erase(std::remove_if(currPlayerName.begin(),currPlayerName.end(), ::isspace),currPlayerName.end());
                                    std::string targetNotification;
                                    std::string moveNotification;

                                    if (validMoves[currTarget][0]!=charIndex) 
                                    {
                                        
                                        targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                        combatParticipants = combatParticipants[i].doAction(MOVE,nothing,validMoves[currTarget],combatParticipants);

                                        moveNotification += currPlayerName;
                                        moveNotification += " switch places with ";
                                        moveNotification += targetNotification;
                                    }
                                    else 
                                    {
                                        moveNotification = " Your teammates are dead. You just wasted a turn.";
                                    }
                            
                                    battleNotification.changeText(moveNotification);
                                    STATE_timerStarted = true;
                                    STATE_timerCount = timer->deltaTime() + 3;
                                    break;       
                                }
                            }
                            
                            
                        }
                        bool isNextTurnAlive = false;
                        do 
                        {
                            currOrderNum = (currOrderNum + 1) % roundOrder.size();
                            for (int i = 0; i < combatParticipants.size(); i++)
                            {
                                if (combatParticipants[i].getName() == roundOrder[currOrderNum])
                                {
                                    if (combatParticipants[i].isAlive())
                                    {
                                        isNextTurnAlive = true;
                                    }
                                }
                            }
                        }
                        while (!isNextTurnAlive);


                        //check end state for battle (edit this when victory/defeat screens get merged to main)
                        bool isPlayerTeamAlive = isTeamAlive(combatParticipants, false);
                        bool isEnemyTeamAlive = isTeamAlive(combatParticipants, true);

                        if (isPlayerTeamAlive == false || isEnemyTeamAlive == false)
                        {
                            //saves player team's stats
                            playerTeam = {combatParticipants[0],combatParticipants[1],combatParticipants[2],combatParticipants[3]};
                            STATE_combatMenuTargetSelected = false;
                            currTarget = 0;
                            currOrderNum = 0;
                            screen = MAP;
                            break;
                        }

                        STATE_combatMenuTargetSelected = false;
                        currTarget = 0;
                    }
                }
                case SANDBOX:
                {
                    sandboxMenu.onInput(event, SelectMusic, STATE_introSelectedOption);
                    break;
                }
                case STATUS_MENU:
                {
                    if (event.type == SDL_KEYDOWN)
                    {
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_e:
                            {
                                screen = MAP;
                                break;
                            }
                        }
                    } 

                    break;
                }
                case WIN:
                {
                    break;
                }
                case DEFEAT:
                {
                    break;
                }
            }
            
        }
        //Clear screen
        SDL_SetRenderDrawColor(getRenderer(), 255, 255, 255, 255);
        SDL_RenderClear(getRenderer());

        switch (screen)
        {
            case INTRO:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());
                introMenu.render(getRenderer()); 
                break;
            }
            case MAP:
            {
                //write macro for this eventually
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
                if (STATE_mapEventboxOpen)
                {   
                    if (nextMapEvent == "ITEM")
                    {
                        std::string textNotification = STATE_itemFound + " was found!";
                        TextBox itemNotification = TextBox(textNotification, 30, 20, 20, 300, 100, Font::openSans, Color::darkGreen, Color::black);
                        itemNotification.render(getRenderer());
                        //STATE_itemFound = "NONE"; gotta do this after? no
                    }
                    else if (nextMapEvent == "JOKE")
                    {
                        TextBox jokeNotification = TextBox("some joke", 30, 20, 20, 300, 100, Font::openSans, Color::darkGreen, Color::black);
                        jokeNotification.render(getRenderer());
                    }
                    else if (nextMapEvent == "HEAL")
                    {
                        
                        std::string healText = std::to_string(STATE_amountHealed) + "hp healed for all team members";
                        TextBox healNotification = TextBox(healText, 30, 20, 20, 300, 100, Font::openSans, Color::darkGreen, Color::black);
                        healNotification.render(getRenderer());
                    }
                }
                break;
            } 
            case COMBAT:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());

                combatScreenTexture.render(getRenderer(), 0, 0);

                
                //Status Pane
                SDL_Rect statusPane = {0, 600, 720, 120};
                SDL_Color colStatus = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), colStatus.r, colStatus.g, colStatus.b, 0);
                SDL_RenderFillRect(getRenderer(), &statusPane);

                //Order Pane
                SDL_Rect orderPane = {720, 0, 240, 480};
                SDL_Color colOrder = Color::gray;
                SDL_SetRenderDrawColor(getRenderer(), colOrder.r, colOrder.g, colOrder.b, 0);
                SDL_RenderFillRect(getRenderer(), &orderPane);

                //Menu Pane
                SDL_Rect menuPane = {720, 480, 720, 240};
                SDL_Color colMenu = Color::maroon;
                SDL_SetRenderDrawColor(getRenderer(), colMenu.r, colMenu.g, colMenu.b, 0);
                SDL_RenderFillRect(getRenderer(), &menuPane);
                
                combatMenu.render(getRenderer());
                
                
                //SDL_Rect* currFrameRect = &spriteClipped[currFrameNum];

                //make this into a loop
                //only render if char is alive
                flutistTexture.render(getRenderer(), 0, 400);
                flutistTexture.render(getRenderer(), 90, 400);
                bassistTexture.render(getRenderer(), 180, 400);
                bassistTexture.render(getRenderer(), 270, 400);
                linebackerTexture.render(getRenderer(), 360, 400);
                linebackerTexture.render(getRenderer(), 450, 400);
                linebackerTexture.render(getRenderer(), 540, 400);
                linebackerTexture.render(getRenderer(), 630, 400);

                if (STATE_timerStarted && timer->deltaTime() < STATE_timerCount)

                {
                    battleNotification.render(getRenderer());
                }
                if (timer->deltaTime() > STATE_timerCount)
                {
                    STATE_timerStarted = false; 
                }

                
                //targetBoxes
                if (STATE_combatSelectedOption == "Attack")
                {
                    // rerender bug if I don't update validMoves
                    for (int i = 0; i < combatParticipants.size();i++)
                    {
                        if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                        {
                            validMoves = combatParticipants[i].getValidMoves(ATTACK, combatParticipants[i].getParticipantsIndex(),combatParticipants);
                            for (auto target: validMoves[currTarget])
                            {
                                //SDL_RenderFillRect(getRenderer(), &charBoxes[target]);
                                targetTexture.render(getRenderer(), charBoxes[target].x, charBoxes[target].y);
                            }
                        }
                    }
                    
                }
                if (STATE_combatSelectedOption == "Buff")
                {
                    for (int i = 0; i < combatParticipants.size();i++)
                    {
                        if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                        {
                            validMoves = combatParticipants[i].getValidMoves(BUFF, combatParticipants[i].getParticipantsIndex(),combatParticipants);
                            for (auto target: validMoves[currTarget])
                            {
                                //SDL_RenderFillRect(getRenderer(), &charBoxes[target]);
                                targetTexture.render(getRenderer(), charBoxes[target].x, charBoxes[target].y);
                            }
                        }
                    }
                    
                    
                }

                if (STATE_combatSelectedOption == "Debuff")
                {
                    SDL_SetRenderDrawColor(getRenderer(), 150, 0, 0, 255);
                    // rerender bug if I don't update validMoves
                    for (int i = 0; i < combatParticipants.size();i++)
                    {
                        if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                        {
                            validMoves = combatParticipants[i].getValidMoves(DEBUFF, combatParticipants[i].getParticipantsIndex(),combatParticipants);
                            for (auto target: validMoves[currTarget])
                            {
                                //SDL_RenderFillRect(getRenderer(), &charBoxes[target]);
                                targetTexture.render(getRenderer(), charBoxes[target].x, charBoxes[target].y);
                            }
                        }
                    }
                }

                if (STATE_combatSelectedOption == "Move")
                {
                    SDL_SetRenderDrawColor(getRenderer(), 0, 150, 0, 255);
                    // rerender bug if I don't update validMoves
                    for (int i = 0; i < combatParticipants.size();i++)
                    {
                        if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                        {
                            validMoves = combatParticipants[i].getValidMoves(MOVE, combatParticipants[i].getParticipantsIndex(),combatParticipants);
                            for (auto target: validMoves[currTarget])
                            {
                                //SDL_RenderFillRect(getRenderer(), &charBoxes[target]);
                                targetTexture.render(getRenderer(), charBoxes[target].x, charBoxes[target].y);
                            }
                        }
                    }
                }
                
                SDL_SetRenderDrawColor(getRenderer(), 0, 170, 0, 255);
                //TODO FIX THIS BEFORE IT MELTS DAVID'S COMPUTER
                //TODO Something wrong with the rendering of currPlayer
                int currPlayer;
                for (int i = 0; i < combatParticipants.size(); i++)
                {
                    if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                    {
                        currPlayer = combatParticipants[i].getParticipantsIndex();
                    }
                }
                //int currPlayer = roundOrder[currOrderNum]->getParticipantsIndex();
                //SDL_RenderFillRect(getRenderer(), &charBoxes[currPlayer]);
                currPlayerTexture.render(getRenderer(), charBoxes[currPlayer].x, charBoxes[currPlayer].y);

                // update order display
                for (int i = 0; i < combatParticipants.size(); i++)
                {
                    for (int j = 0; j < roundOrder.size(); j++)
                    {
                        if (combatParticipants[i].getName() == roundOrder[j])
                        {
                            // if character in round order is alive, write its name in the order box
                            if (combatParticipants[i].isAlive())
                                orderBoxes[j].changeText(roundOrder[j]);
                            // if character in round order is dead, overwrite its name with an empty string
                            else 
                                orderBoxes[j].changeText("");
                        }
                    }
                }
                // if round order size less than order boxes size, replace dead character names with empty string
                for (int i = roundOrder.size(); i < orderBoxes.size(); i++)
                {
                    orderBoxes[i].changeText("");
                }
                
                
                for (int i = currOrderNum; i < orderBoxes.size(); i++)
                {
                    orderBoxes[i].render(getRenderer());
                    //charNameStream << tempCharNames[i];
                }
                
                //Update Position and text renderings
               
            
                for (int i = 0; i < 4; i++)
                {
                    combatStatusRow[i].changeText(combatParticipants[i].getName());
                    combatStatusRow[i+4].changeText(prefixHP + std::to_string(combatParticipants[i].getHp()));
                }
               
                for (auto el: combatStatusRow)
                {
                    el.render(getRenderer());
                } 

                break;
            }
            case SANDBOX:           
            {
                //Combat Pane
                SDL_Rect combatPane = {0, 0, 720, 600};
                SDL_Color colCombat = Color::navy;
                SDL_SetRenderDrawColor(getRenderer(), colCombat.r, colCombat.g, colCombat.b, 0);
                SDL_RenderFillRect(getRenderer(), &combatPane);

                //Status Pane
                SDL_Rect statusPane = {0, 600, 720, 120};
                SDL_Color colStatus = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), colStatus.r, colStatus.g, colStatus.b, 0);
                SDL_RenderFillRect(getRenderer(), &statusPane);

                //Order Pane
                SDL_Rect orderPane = {720, 0, 240, 480};
                SDL_Color colOrder = Color::gray;
                SDL_SetRenderDrawColor(getRenderer(), colOrder.r, colOrder.g, colOrder.b, 0);
                SDL_RenderFillRect(getRenderer(), &orderPane);

                //Menu Pane
                SDL_Rect menuPane = {720, 480, 720, 240};
                SDL_Color colMenu = Color::maroon;
                SDL_SetRenderDrawColor(getRenderer(), colMenu.r, colMenu.g, colMenu.b, 0);
                SDL_RenderFillRect(getRenderer(), &menuPane);
                
                //Recent Attack Pane
                SDL_Rect recentAttackPane = {0, 540, 500, 60};
                SDL_Color colRecentAttack = Color::gray;
                SDL_SetRenderDrawColor(getRenderer(), colRecentAttack.r, colRecentAttack.g, colRecentAttack.b, 0);
                SDL_RenderFillRect(getRenderer(), &recentAttackPane);
                sandboxMenu.render(getRenderer());
                
                
                
                // list
                for (int i = 0; i < 8; i++)
                {
                    sandboxQue[i].render(getRenderer());
                    sandboxQueIcons[i].render(getRenderer());
                    
                }
                

                //stat
                /*
                for (int i = 0; i < 4; i++)
                {
                    statusRow[i].render(getRenderer());
                    
                }
                */
                statBass.render(getRenderer());
                statFlute.render(getRenderer());
                statDrum.render(getRenderer());
                statConductor.render(getRenderer());

                statBassHP.render(getRenderer());
                statFluteHP.render(getRenderer());
                statDrumHP.render(getRenderer());
                statConductorHP.render(getRenderer());
                combatScreenTexture.render(getRenderer(), 0, 0);
                break;
            }
            case STATUS_MENU:
            {
                std::string statMenuDisplayStr;

                //////Background Color////////
                SDL_Rect backgroundPane1 = {0, 0, 960, 730};
                SDL_Color backgroundMenu1 = Color::navy;
                SDL_SetRenderDrawColor(getRenderer(), backgroundMenu1.r, backgroundMenu1.g, backgroundMenu1.b, 0);
                SDL_RenderFillRect(getRenderer(), &backgroundPane1);
                SDL_Rect backgroundPane2 = {10, 10, 940, 700};
                SDL_Color backgroundMenu2 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), backgroundMenu2.r, backgroundMenu2.g, backgroundMenu2.b, 0);
                SDL_RenderFillRect(getRenderer(), &backgroundPane2);
                //////End Background Color////////

                ///////Background under the text////////
                
                SDL_Rect StatMenuBackground1 = {100, 100, 750, 100};
                SDL_Color StatMenuColor1 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor1.r, StatMenuColor1.g, StatMenuColor1.b, 0);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground1);

                SDL_Rect StatMenuBackground2 = {100, 250, 750, 100};
                SDL_Color StatMenuColor2 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor2.r, StatMenuColor2.g, StatMenuColor2.b, 0);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground2);

                SDL_Rect StatMenuBackground3 = {100, 400, 750, 100};
                SDL_Color StatMenuColor3 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor3.r, StatMenuColor3.g, StatMenuColor3.b, 0);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground3);

                SDL_Rect StatMenuBackground4 = {100, 550, 750, 100};
                SDL_Color StatMenuColor4 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor4.r, StatMenuColor4.g, StatMenuColor4.b, 0);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground4);

                //SDL_Rect StatMenuBackground5 = {800, 60, 150, 630};
                //SDL_Color StatMenuColor5 = Color::gray;
                //SDL_SetRenderDrawColor(getRenderer(), StatMenuColor5.r, StatMenuColor5.g, StatMenuColor5.b, 0);
                //SDL_RenderFillRect(getRenderer(), &StatMenuBackground5);

                ///////End Background under the text////////

                ///////Lines that split the screen//////////
                
                SDL_Rect splitLine1 = {195, 100, 5, 550};
                SDL_Color splitLineColor1 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor1.r, splitLineColor1.g, splitLineColor1.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine1);
                
                SDL_Rect splitLine2 = {300, 100, 5, 550};
                SDL_Color splitLineColor2 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor2.r, splitLineColor2.g, splitLineColor2.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine2);

                SDL_Rect splitLine3 = {430, 100, 5, 550};
                SDL_Color splitLineColor3 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor3.r, splitLineColor3.g, splitLineColor3.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine3);

                SDL_Rect splitLine4 = {550, 100, 5, 550};
                SDL_Color splitLineColor4 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor4.r, splitLineColor4.g, splitLineColor4.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine4);

                SDL_Rect splitLine5 = {680, 100, 5, 550};
                SDL_Color splitLineColor5 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor5.r, splitLineColor5.g, splitLineColor5.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine5);
                
                ///////End Lines that split the screen//////////

                ///////Stat Names////////
                for (int i = 0; i < 4; i++)
                {
                    characterStatsHP[i].render(getRenderer());
                    characterStatsSpeed[i].render(getRenderer());
                    characterStatsHit[i].render(getRenderer());
                    characterStatsArmor[i].render(getRenderer());
                    characterStatsDodge[i].render(getRenderer());
                    characterStatSlash[i].render(getRenderer());

                }

                ///////End Stat Names////////

                ///////Base Stats/////////
                //name
                TextBox baseName = TextBox("Bass  ", 40, 100, 115, 100, 70, Font::roboto, Color::blue, Color::cyan);
                baseName.render(getRenderer());
                //HP
                statMenuDisplayStr = std::to_string(playerTeam[2].getHp());
                TextBox bassHPName = TextBox(statMenuDisplayStr, 40, 200, 150, 40, 50, Font::roboto, Color::blue, Color::cyan);
                bassHPName.changeText(statMenuDisplayStr);
                bassHPName.render(getRenderer());
                //Max HP
                statMenuDisplayStr = std::to_string(playerTeam[2].getMaxHp());
                TextBox bassMaxHPName = TextBox(statMenuDisplayStr, 40, 250, 150, 40, 50, Font::roboto, Color::blue, Color::cyan);
                bassMaxHPName.render(getRenderer());
                //Speed
                statMenuDisplayStr = std::to_string(playerTeam[2].getSpeed());
                TextBox bassSpeedName = TextBox(statMenuDisplayStr, 40, 355, 150, 30, 50, Font::roboto, Color::blue, Color::cyan);
                bassSpeedName.render(getRenderer());
                //Hit
                statMenuDisplayStr = std::to_string(playerTeam[2].getHit());
                TextBox bassHitName = TextBox(statMenuDisplayStr, 40, 470, 150, 30, 50, Font::roboto, Color::blue, Color::cyan);
                bassHitName.render(getRenderer());
                //Armor
                statMenuDisplayStr = std::to_string(playerTeam[2].getArmor());
                TextBox bassArmorName = TextBox(statMenuDisplayStr, 40, 600, 150, 30, 50, Font::roboto, Color::blue, Color::cyan);
                bassArmorName.render(getRenderer());
                //Dodge
                statMenuDisplayStr = std::to_string(playerTeam[2].getDodgeModifier());
                TextBox bassDodgeName = TextBox(statMenuDisplayStr, 40, 730, 150, 30, 50, Font::roboto, Color::blue, Color::cyan);
                bassDodgeName.render(getRenderer());
                ///////End Base Stats/////////

                ///////Drum Stats/////////
                //name
                TextBox drumName = TextBox("Drum  ", 40, 100, 265, 100, 70, Font::roboto, Color::blue, Color::cyan);
                drumName.render(getRenderer());
                //HP
                statMenuDisplayStr = std::to_string(playerTeam[3].getHp());
                TextBox drumHPName = TextBox(statMenuDisplayStr, 40, 200, 300, 40, 50, Font::roboto, Color::blue, Color::cyan);
                drumHPName.changeText(statMenuDisplayStr);
                drumHPName.render(getRenderer());
                //Max HP
                statMenuDisplayStr = std::to_string(playerTeam[3].getMaxHp());
                TextBox drumMaxHPName = TextBox(statMenuDisplayStr, 40, 250, 300, 40, 50, Font::roboto, Color::blue, Color::cyan);
                drumMaxHPName.render(getRenderer());
                //Speed
                statMenuDisplayStr = std::to_string(playerTeam[3].getSpeed());
                TextBox drumSpeedName = TextBox(statMenuDisplayStr, 40, 355, 300, 30, 50, Font::roboto, Color::blue, Color::cyan);
                drumSpeedName.render(getRenderer());
                //Hit
                statMenuDisplayStr = std::to_string(playerTeam[3].getHit());
                TextBox drumHitName = TextBox(statMenuDisplayStr, 40, 470, 300, 30, 50, Font::roboto, Color::blue, Color::cyan);
                drumHitName.render(getRenderer());
                //Armor
                statMenuDisplayStr = std::to_string(playerTeam[3].getArmor());
                TextBox drumArmorName = TextBox(statMenuDisplayStr, 40, 600, 300, 30, 50, Font::roboto, Color::blue, Color::cyan);
                drumArmorName.render(getRenderer());
                //Dodge
                statMenuDisplayStr = std::to_string(playerTeam[3].getDodgeModifier());
                TextBox drumDodgeName = TextBox(statMenuDisplayStr, 40, 730, 300, 30, 50, Font::roboto, Color::blue, Color::cyan);
                drumDodgeName.render(getRenderer());
                ///////End Drum Stats/////////

                ///////Flute Stats/////////
                //name
                TextBox fluteName = TextBox("Flute  ", 40, 100, 415, 100, 70, Font::roboto, Color::blue, Color::cyan);
                fluteName.render(getRenderer());
                //HP
                statMenuDisplayStr = std::to_string(playerTeam[0].getHp());
                TextBox fluteHPName = TextBox(statMenuDisplayStr, 40, 200, 450, 40, 50, Font::roboto, Color::blue, Color::cyan);
                fluteHPName.changeText(statMenuDisplayStr);
                fluteHPName.render(getRenderer());
                //Max HP
                statMenuDisplayStr = std::to_string(playerTeam[0].getMaxHp());
                TextBox fluteMaxHPName = TextBox(statMenuDisplayStr, 40, 250, 450, 40, 50, Font::roboto, Color::blue, Color::cyan);
                fluteMaxHPName.render(getRenderer());
                //Speed
                statMenuDisplayStr = std::to_string(playerTeam[0].getSpeed());
                TextBox fluteSpeedName = TextBox(statMenuDisplayStr, 40, 355, 450, 30, 50, Font::roboto, Color::blue, Color::cyan);
                fluteSpeedName.render(getRenderer());
                //Hit
                statMenuDisplayStr = std::to_string(playerTeam[0].getHit());
                TextBox fluteHitName = TextBox(statMenuDisplayStr, 40, 470, 450, 30, 50, Font::roboto, Color::blue, Color::cyan);
                fluteHitName.render(getRenderer());
                //Armor
                statMenuDisplayStr = std::to_string(playerTeam[0].getArmor());
                TextBox fluteArmorName = TextBox(statMenuDisplayStr, 40, 600, 450, 30, 50, Font::roboto, Color::blue, Color::cyan);
                fluteArmorName.render(getRenderer());
                //Dodge
                statMenuDisplayStr = std::to_string(playerTeam[0].getDodgeModifier());
                TextBox fluteDodgeName = TextBox(statMenuDisplayStr, 40, 730, 450, 30, 50, Font::roboto, Color::blue, Color::cyan);
                fluteDodgeName.render(getRenderer());
                ///////End Flute Stats/////////

                ///////Conductor Stats/////////
                //name
                TextBox conductorName = TextBox("Conductor  ", 40, 100, 570, 100, 60, Font::roboto, Color::blue, Color::cyan);
                conductorName.render(getRenderer());
                //HP
                statMenuDisplayStr = std::to_string(playerTeam[1].getHp());
                TextBox conductorHPName = TextBox(statMenuDisplayStr, 40, 200, 600, 40, 50, Font::roboto, Color::blue, Color::cyan);
                conductorHPName.changeText(statMenuDisplayStr);
                conductorHPName.render(getRenderer());
                //Max HP
                statMenuDisplayStr = std::to_string(playerTeam[1].getMaxHp());
                TextBox conductorMaxHPName = TextBox(statMenuDisplayStr, 40, 250, 600, 40, 50, Font::roboto, Color::blue, Color::cyan);
                conductorMaxHPName.render(getRenderer());
                //Speed
                statMenuDisplayStr = std::to_string(playerTeam[1].getSpeed());
                TextBox conductorSpeedName = TextBox(statMenuDisplayStr, 40, 355, 600, 30, 50, Font::roboto, Color::blue, Color::cyan);
                conductorSpeedName.render(getRenderer());
                //Hit
                statMenuDisplayStr = std::to_string(playerTeam[1].getHit());
                TextBox conductorHitName = TextBox(statMenuDisplayStr, 40, 470, 600, 30, 50, Font::roboto, Color::blue, Color::cyan);
                conductorHitName.render(getRenderer());
                //Armor
                statMenuDisplayStr = std::to_string(playerTeam[1].getArmor());
                TextBox conductorArmorName = TextBox(statMenuDisplayStr, 40, 600, 600, 30, 50, Font::roboto, Color::blue, Color::cyan);
                conductorArmorName.render(getRenderer());
                //Dodge
                statMenuDisplayStr = std::to_string(playerTeam[1].getDodgeModifier());
                TextBox conductorDodgeName = TextBox(statMenuDisplayStr, 40, 730, 600, 30, 50, Font::roboto, Color::blue, Color::cyan);
                conductorDodgeName.render(getRenderer());
                ///////End Conductor Stats/////////

                //SDL_FreeSurface(surface);
                //SDL_DestroyTexture(texture);
                break;
            }
            case WIN:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 150, 0, 255);
                SDL_RenderClear(getRenderer());

                TextBox baseName = TextBox("Congratulations  ", 100, 200, 100, 500, 200, Font::roboto, Color::black, Color::darkGreen);
                baseName.render(getRenderer());

                //timerStarted = true;
                //countTime = timer->deltaTimer() + 3;


                break;
            }
            case DEFEAT:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());
                break;
            }
            
        }

        
        //Update screen
        SDL_RenderPresent(getRenderer());
       
 
    }
}
