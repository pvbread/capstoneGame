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

    ////////// START CHARACTER INIT ////////

    BaseCharacter conductor = BaseCharacter("Conductor", 30, 3, 3, 0, 0, 3, 3,false);
    BaseCharacter drum = BaseCharacter("Drummer", 50, 2, 1, 0, 0, 3, 3,false);
    BaseCharacter flute = BaseCharacter("Flutist", 20, 6, 1, 0, 0, 3, 3,false);
    BaseCharacter bass = BaseCharacter("Bassist", 60, 1, 3, 0, 0, 3, 3,false);
    // Set participant index for combat vector
    flute.setNewParticipantsIndex(0);
    conductor.setNewParticipantsIndex(1);
    bass.setNewParticipantsIndex(2);
    drum.setNewParticipantsIndex(3);
    // Set no items for every character
    BaseItem empty = BaseItem("", "", 0); 
    conductor.setItem(empty);
    flute.setItem(empty);
    bass.setItem(empty);
    drum.setItem(empty);
    // Declaration of vectors for players, enemies, and combat participants for the combat state 
    std::vector<BaseCharacter> playerTeam{flute, conductor, bass, drum};
    std::vector<BaseCharacter> enemies;
    std::vector<BaseCharacter> combatParticipants;
    

    ///////// END CHARACTER INIT //////


    ////////ONE LINER JOKES LIST////////

    int jokeNumber = 1;
    std::unordered_map<int, std::string > jokeList = 
        {{1, "I went to buy some camo pants but couldnt find any."},
        {2, "I failed math so many times at school, I cant even count."},
        {3, "When life gives you melons, you might be dyslexic."},
        {4, "I used to think I was indecisive. But now Im not so sure."},
        {5, "Russian dolls are so full of themselves."},
        {6, "Russian dolls are so full of themselves."}};

    ////////ONE LINER JOKES LIST////////

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
    BaseItem normalArmor = BaseItem("Normal Groove", "armor", 1);
    BaseItem rareArmor = BaseItem("Rare Groove", "armor", 2);
    BaseItem epicArmor = BaseItem("Epic Groove", "armor", 3);

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
        epicSpeed,
        normalArmor,
        rareArmor,
        epicArmor
    };

    ///////// END ITEM INIT /////////

    Screen screen = INTRO; 

    //////////// START MENUS INIT ///////////////

    const std::vector<std::string> introOptions = {
        "New Game",
        "Credits"
    };

    BaseMenu introMenu = BaseMenu(90, 250, 515, 400, 100,  
                                  introOptions, 
                                  Font::raleway, 
                                  Color::navy,
                                  Color::cyan,
                                  getRenderer()
    );

    const std::vector<std::string> combatOptionsStrings = {
        "Attack",
        "Buff",
        "Debuff",
        "Move"
    };

    BaseMenu combatMenu = BaseMenu(50, 760, 490, 100, 50, 
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
    bool STATE_enemyTimerStarted = false;
    bool STATE_debug = false;
    bool STATE_itemNotificationShowing = false;
    bool STATE_healNotificationShowing = false;
    bool STATE_jokeNotificationShowing = false;
    bool STATE_preTransition = false;
    bool STATE_postTransition = true;
    bool STATE_statMenu = false;
    bool STATE_mapScreenOpenForTransition = false;
    bool STATE_timerAnimationStarted = false;
    bool STATE_didAnimationHappen = false;
    bool STATE_updateHP = false;
    bool STATE_youWin = false;
    bool STATE_youLoose = false;
    bool STATE_bossFightBegin = false;
    bool STATE_didGetRandNumForJoke = true;
    bool STATE_isWorseItem = true;
    bool STATE_isBossDead = false;
    int STATE_lastCurrTarget = 0;
    float STATE_timerCount;
    float STATE_timerAnimationCount;
    float STATE_enemyTimerCount;
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
    Mix_Chunk *MapNotificationSound = Mix_LoadWAV("./MapNotification.wav");
    Mix_PlayMusic(SelectOST, -1); 

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
    TextureWrapper conductorTexture;
    TextureWrapper linebackerTexture;
    TextureWrapper enemyBellTexture;
    TextureWrapper enemyBatTexture;
    TextureWrapper pizzaheadTexture;
    TextureWrapper carlTexture;
    TextureWrapper bossTexture;
    TextureWrapper currPlayerTexture;
    TextureWrapper targetTexture;
    TextureWrapper getHitEffect;
    TextureWrapper introScreen;
    TextureWrapper statusBg;
    TextureWrapper orderBg;
    TextureWrapper basemenuBg;
    TextureWrapper combatStatBg;
    TextureWrapper maxwell;


    //add sprite sheet here
    std::unordered_map<TextureWrapper*, std::string> textureFilePaths = {
        {&tileTexture, "../../assets/image/newspritedraft3.png"},
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
        {&conductorTexture, "../../assets/image/chars/conductor.png"},
        {&enemyBellTexture, "../../assets/image/chars/enemysh-bell.png"},
        {&enemyBatTexture, "../../assets/image/chars/enemysh-bat.png"},
        {&pizzaheadTexture, "../../assets/image/chars/pizzahead.png"},
        {&carlTexture, "../../assets/image/chars/Carl.png"},
        {&bossTexture, "../../assets/image/chars/Carl-Boss-vestless.png"},
        {&linebackerTexture, "../../assets/image/chars/linebacker.png"},
        {&currPlayerTexture, "../../assets/image/treble.png"},
        {&targetTexture, "../../assets/image/sixteenth.png"},  
        {&getHitEffect, "../../assets/image/explosion-notes.png"},
        {&introScreen, "../../assets/image/title.png"},
        {&statusBg, "../../assets/image/road.png"},
        {&orderBg, "../../assets/image/orderBg.png"},
        {&basemenuBg, "../../assets/image/basemenuBg.png"}, 
        {&combatStatBg, "../../assets/image/statBg.png"},
        {&maxwell, "../../assets/image/maxwell2.png"}   

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
    
    /////////// COMBAT SCREEN THINGS //////////
    std::string prefixHP = "HP: ";

    

    TextBox combatStatFlute = TextBox("flute", 25, 50, 630, 150, 30, Font::inter, Color::black, Color::cyan, true);
    TextBox combatStatFluteHP = TextBox(prefixHP + std::to_string(flute.getHp()), 25, 50, 660, 150, 30, Font::inter, Color::black, Color::cyan, true);
    
    TextBox combatStatConductor = TextBox("conductor", 25, 200, 630, 150, 30, Font::inter, Color::black, Color::cyan, true);
    TextBox combatStatConductorHP = TextBox(prefixHP + std::to_string(conductor.getHp()), 25, 200, 660, 150, 30, Font::inter, Color::black, Color::cyan, true);

    TextBox combatStatBass = TextBox("bass", 25, 350, 630, 150, 30, Font::inter, Color::black, Color::cyan, true);
    TextBox combatStatBassHP = TextBox(prefixHP + std::to_string(bass.getHp()), 25, 350, 660, 150, 30, Font::inter, Color::black, Color::cyan, true);
    
    TextBox combatStatDrum = TextBox("drum", 25, 500, 630, 150, 30, Font::inter, Color::black, Color::cyan, true);
    TextBox combatStatDrumHP = TextBox(prefixHP + std::to_string(drum.getHp()), 25, 500, 660, 150, 30, Font::inter, Color::black, Color::cyan, true);

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
    
    // Load random map from MapBuilder
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
        "ITEM",
        "HEAL",
        "JOKE",
        "ITEM"
    };
    for (auto event: eventsToAdd)
        eventList.push_back(event);
    
    std::uniform_int_distribution<> dist(0,10); 

    // Set random events throughout the map 
    // First set the end tiles to the boss event
    // Then populate the rest of the tiles with the other events
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

    // stores screen positioning for the characters
    std::vector<int> charRendering = {0, 90, 180, 270, 360, 450, 540, 630};

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
        TextBox temp = TextBox("", 25, 750, 30+(i*50), 200, 30, Font::raleway, Color::black, Color::gray, true);
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

    
    TTF_Font *orderFont = TTF_OpenFont("./Raleway-Medium.ttf", 50);

    int currTarget = 0;
    std::vector<std::vector<int>> validMoves;
    bool actionChosen = false;

    ///Battle damage/death 
    int alphaDamageON = 255;
    int alphaDamageOFF = 0;
    int alphaDeathON = 255;
    int alphaDeathOFF = 0;
    int whichTargetXValueForDamageAnimation = 0;
    std::vector<int> damageTakenPosition = {0, 90, 180, 270, 360, 450, 540, 630};

    ///////////////////////////////////
    ///////// BEGIN SANDBOX ///////////
    ///////////////////////////////////

    
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
    TextBox conductorStats = TextBox("Conductor", 30, 20, 620, 100, 100, Font::sono, Color::white, Color::black);
    TextBox testToHitStats = TextBox("x+ to hit", 30, 666, 620, 100, 100, Font::sono, Color::white, Color::black);
    TextBox conductorStatsHP = TextBox("30/30", 30, 220, 620, 100, 100, Font::sono, Color::white, Color::black);
    TextBox conductorStatsSpeed = TextBox(std::to_string(conductor.getSpeed()), 30, 355, 620, 100, 100, Font::sono, Color::white, Color::black);
    TextBox conductorStatsHit = TextBox(std::to_string(conductor.getHit()), 30, 439, 620, 100, 100, Font::sono, Color::white, Color::black);
    TextBox conductorStatsArmor = TextBox(std::to_string(conductor.getArmor()), 30, 523, 620, 100, 100, Font::sono, Color::white, Color::black);
    TextBox conductorStatsDodge = TextBox(std::to_string(conductor.getDodgeModifier()), 30, 607, 620, 100, 100, Font::sono, Color::white, Color::black);  

    TextBox drummerStats = TextBox("Drummer", 30, 20, 420, 100, 100, Font::sono, Color::white, Color::black);
    TextBox testToHitStats2 = TextBox("x+ to hit", 30, 666, 420, 100, 100, Font::sono, Color::white, Color::black);
    TextBox drummerStatsHP = TextBox("30/30", 30, 220, 420, 100, 100, Font::sono, Color::white, Color::black);
    TextBox drummerStatsSpeed = TextBox(std::to_string(conductor.getSpeed()), 30, 355, 420, 100, 100, Font::sono, Color::white, Color::black);
    TextBox drummerStatsHit = TextBox(std::to_string(conductor.getHit()), 30, 439, 420, 100, 100, Font::sono, Color::white, Color::black);
    TextBox drummerStatsArmor = TextBox(std::to_string(conductor.getArmor()), 30, 523, 420, 100, 100, Font::sono, Color::white, Color::black);
    TextBox drummerStatsDodge = TextBox(std::to_string(conductor.getDodgeModifier()), 30, 607, 420, 100, 100, Font::sono, Color::white, Color::black);       

    std::vector<TextBox> conductorStatsRow{
        testToHitStats,
        conductorStatsHP,
        conductorStatsSpeed,
        conductorStatsHit,
        conductorStatsArmor,
        conductorStatsDodge 
    };

    std::vector<TextBox> drummerStatsRow{
        testToHitStats2,
        drummerStatsHP,
        drummerStatsSpeed,
        drummerStatsHit,
        drummerStatsArmor,
        drummerStatsDodge 
    };


    ///////// END SANDBOX ///////////
    
    //////// CHARACTER STATS ////////

    std::string tempItemModAddStat = "+Dodge";

    ///////Base Stats/////////
    TextBox baseName = TextBox("   Bassist", 40, 50, 95, 50, 30, Font::roboto, Color::navy, Color::cyan, true);
    TextBox bassHPName = TextBox(std::to_string(bass.getHp()) + "-", 30, 130, 145, 40, 50, Font::roboto, Color::navy, Color::cyan, true);
    TextBox bassMaxHPName = TextBox(std::to_string(bass.getMaxHp()), 30, 185, 145, 40, 50, Font::roboto, Color::navy, Color::cyan, true);
    TextBox bassSpeedName = TextBox(std::to_string(bass.getSpeed()), 30, 315, 148, 30, 50, Font::roboto, Color::navy, Color::cyan, true);
    TextBox bassHitName = TextBox(std::to_string(bass.getHit()), 30, 435, 148, 30, 50, Font::roboto, Color::navy, Color::cyan, true);
    TextBox bassArmorName = TextBox(std::to_string(bass.getArmor()), 30, 550, 148, 30, 50, Font::roboto, Color::navy, Color::cyan, true);
    TextBox bassDodgeName = TextBox(std::to_string(bass.getDodgeModifier()), 30, 670, 148, 30, 50, Font::roboto, Color::navy, Color::cyan, true);
    TextBox bassItemModName = TextBox(bass.getItem().getMessage(), 30, 750, 148, 30, 50, Font::roboto, Color::navy, Color::cyan, true);
    
    std::vector<TextBox> statMenuBassRow { baseName , bassHPName, bassMaxHPName, bassSpeedName, bassHitName, bassArmorName, bassDodgeName, bassItemModName };
    ///////End Base Stats/////////
    ///////Drum Stats/////////
    TextBox drumName = TextBox(" Drummer", 40, 50, 245, 50, 30, Font::roboto, Color::blue, Color::cyan, true);
    TextBox drumHPName = TextBox(std::to_string(drum.getHp()) + "-", 30, 130, 295, 40, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox drumMaxHPName = TextBox(std::to_string(drum.getMaxHp()), 30, 185, 295, 40, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox drumSpeedName = TextBox(std::to_string(drum.getSpeed()), 30, 315, 298, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox drumHitName = TextBox(std::to_string(drum.getHit()), 30, 435, 298, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox drumArmorName = TextBox(std::to_string(drum.getArmor()), 30, 550, 298, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox drumDodgeName = TextBox(std::to_string(drum.getDodgeModifier()), 30, 670, 298, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox drumItemModName = TextBox(drum.getItem().getMessage(), 30, 750, 298, 30, 50, Font::roboto, Color::blue, Color::cyan, true);

    std::vector<TextBox> statMenuDrumRow { drumName , drumHPName, drumMaxHPName, drumSpeedName, drumHitName, drumArmorName, drumDodgeName, drumItemModName };
    ///////End Drum Stats/////////
    ///////Flute Stats/////////
    TextBox fluteName = TextBox("    Flutist", 40, 50, 395, 50, 30, Font::roboto, Color::blue, Color::cyan, true);
    TextBox fluteHPName = TextBox(std::to_string(flute.getHp()) + "-", 30, 130, 445, 40, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox fluteMaxHPName = TextBox(std::to_string(flute.getMaxHp()), 30, 185, 445, 40, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox fluteSpeedName = TextBox(std::to_string(flute.getSpeed()), 30, 315, 448, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox fluteHitName = TextBox(std::to_string(flute.getHit()), 30, 435, 448, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox fluteArmorName = TextBox(std::to_string(flute.getArmor()), 30, 550, 448, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox fluteDodgeName = TextBox(std::to_string(flute.getDodgeModifier()), 30, 670, 448, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox fluteItemModName = TextBox(flute.getItem().getMessage(), 30, 750, 448, 30, 50, Font::roboto, Color::blue, Color::cyan, true);

    std::vector<TextBox> statMenuFluteRow { fluteName , fluteHPName, fluteMaxHPName, fluteSpeedName, fluteHitName, fluteArmorName, fluteDodgeName, fluteItemModName };
    ///////End Flute Stats/////////
    ///////Conductor Stats/////////
    TextBox conductorName = TextBox("Conductor", 40, 50, 545, 50, 30, Font::roboto, Color::blue, Color::cyan, true);
    TextBox conductorHPName = TextBox(std::to_string(conductor.getHp()) + "-", 30, 130, 595, 40, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox conductorMaxHPName = TextBox(std::to_string(conductor.getMaxHp()), 30, 185, 595, 40, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox conductorSpeedName = TextBox(std::to_string(conductor.getSpeed()), 30, 315, 598, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox conductorHitName = TextBox(std::to_string(conductor.getHit()), 30, 435, 598, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox conductorArmorName = TextBox(std::to_string(conductor.getArmor()), 30, 550, 598, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox conductorDodgeName = TextBox(std::to_string(conductor.getDodgeModifier()), 30, 670, 598, 30, 50, Font::roboto, Color::blue, Color::cyan, true);
    TextBox conductorItemModName = TextBox(conductor.getItem().getMessage(), 30, 750, 598, 30, 50, Font::roboto, Color::blue, Color::cyan, true);

    std::vector<TextBox> statMenuConductorRow { 
        conductorName , 
        conductorHPName, 
        conductorMaxHPName, 
        conductorSpeedName, 
        conductorHitName, 
        conductorArmorName, 
        conductorDodgeName, 
        conductorItemModName 
    };
    ///////End Conductor Stats/////////
    ///////Display Stat Names/////////
    int startBoxWidth = 95;
    int slashBoxWidth = 150;
    std::vector<TextBox> characterStatsHP;
    std::vector<TextBox> characterStatsSpeed;
    std::vector<TextBox> characterStatsHit;
    std::vector<TextBox> characterStatsArmor;
    std::vector<TextBox> characterStatsDodge;
    std::vector<TextBox> characterStatsItemMod;
    std::vector<TextBox> characterStatSlash;

    for (int i = 0; i < 4; i++)
    {
        TextBox statsHP = TextBox(" HP", 30, 50, (startBoxWidth + (i*150) + 50),
            50, 20, Font::roboto, Color::blue, Color::cyan, true);
        characterStatsHP.push_back(statsHP);
        
        TextBox statsSpeed = TextBox("Speed", 35, 276, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan, true);
        characterStatsSpeed.push_back(statsSpeed);

        TextBox statsHit = TextBox("   Hit   ", 35, 393, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan, true);
        characterStatsHit.push_back(statsHit);

        TextBox statsArmor = TextBox("Armor", 35, 510, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan, true);
        characterStatsArmor.push_back(statsArmor);
        
        TextBox statsDodge = TextBox("Dodge", 35, 627, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan, true);
        characterStatsDodge.push_back(statsDodge);

        TextBox statsItemMod = TextBox("Item Mod", 35, 748, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan, true);
        characterStatsItemMod.push_back(statsItemMod);
        
        
    }   
    ///////End Display Stat Names/////////
    
    ////// END CHARACTER STATS //////

    ///////WIN AND LOSS SCREEN////////

    //win
    TextBox congradsWinText = TextBox("Congratulations  ", 100, 100, 100, 500, 200, Font::roboto, Color::black, Color::darkGreen);
    TextBox moveOnWin = TextBox("Press ''Enter'' to continue", 65, 100, 550, 500, 200, Font::roboto, Color::black, Color::darkGreen);
    
    //Loss
    TextBox congradsYouDiedText = TextBox("  You Died", 100, 200, 100, 500, 200, Font::roboto, Color::maroon, Color::black);
    TextBox moveOnLoss = TextBox("Press ''Escape'' to return to intro", 65, 20, 550, 500, 200, Font::roboto, Color::maroon, Color::black);

    ///////END WIN AND LOSS SCREEN//////

    /////// BEGIN MAP NOTIFS /////////////

    TextBox itemNotification = TextBox("", 30, 20, 20, 300, 100, Font::openSans, Color::white, Color::black);
    TextBox extra = TextBox("but you have something just as good...", 30, 20, 60, 300, 100, Font::openSans, Color::white, Color::black);
    TextBox extra2 = TextBox("so you leave it :(", 30, 20, 100, 300, 100, Font::openSans, Color::white, Color::black);
    TextBox jokeNotification = TextBox(jokeList[jokeNumber], 30, 20, 20, 300, 100, Font::openSans, Color::white, Color::black);
    TextBox healNotification = TextBox("", 30, 20, 20, 300, 100, Font::openSans, Color::white, Color::black);
    ///// .  END MAP NOTIFS //////////////

    ///////// START CREDITS ///////////////
    
    TextBox credit1 = TextBox("David Lin", 50, 710, 20, 300, 100, Font::openSans, Color::black, Color::black, true);
    TextBox credit2 = TextBox("Alexander Michelman", 50, 435, 120, 300, 100, Font::openSans, Color::black, Color::black, true);
    TextBox credit3 = TextBox("Raymond Naula", 50, 535, 210, 300, 100, Font::openSans, Color::black, Color::black, true);
    TextBox credit4 = TextBox("Pedro Vizzarro Vallejos", 50, 375, 320, 300, 100, Font::openSans, Color::black, Color::black, true);
    TextBox credit5 = TextBox("Jack Wu", 50, 750, 420, 300, 100, Font::openSans, Color::black, Color::black, true); 
    ///////// END CREDITS /////////////////

    /////// GAME LOOP ////////////
    // Explanation:
    // First phase: 
    // Timer updates, transition updates, AI updates
    // Second Phase:
    // Input handling and state changing
    // Third Phase:
    // Rendering

    SDL_Event event;
    while (!getQuit())
    {
        /// POST-PROCESSOR ///

        timer->update();

        //////SCREEN TRANSITIONS////////
        if (STATE_introSelectedOption == "New Game")
        {
            // Whenever select new game, reposition character to the beginning of map
            // The value is 31 because the size of the character in the map is 30 x 30 pixels
            characterController.move(31,31);

            // Transition to Map state when New Game is selected
            STATE_preTransition = true;
            if(alphaValueScreenTransition >= 255)
            {
                STATE_preTransition = false;
                screen = MAP;
                STATE_postTransition = true;
                STATE_introSelectedOption = "NONE";
            }
            
        }
        // Transition to combat state when the character in the map lands on a tile with an combat event
        if (nextMapEvent == "BATTLE")
        {
            STATE_preTransition = true;
            if(alphaValueScreenTransition >= 255)
            {

                STATE_preTransition = false;
                screen = COMBAT;
                STATE_postTransition = true;
                STATE_mapEventboxOpen = false;
                nextMapEvent = "BLANKEVENT";
            }
        }
        // Transition for entering the character stats screen
        if (STATE_statMenu == true)
        {
            STATE_preTransition = true;
            if(alphaValueScreenTransition >= 255)
            {
                STATE_preTransition = false;
                screen = STATUS_MENU;
                STATE_postTransition = true;
                STATE_statMenu = false;
            }
        }
        // Transition to go back to the map screen
        if (STATE_mapScreenOpenForTransition == true)
        {
            STATE_preTransition = true;
            if(alphaValueScreenTransition >= 255)
            {
                STATE_preTransition = false;
                screen = MAP;
                STATE_postTransition = true;
                STATE_mapScreenOpenForTransition = false;
            }
        }
        //////END SCREEN TRANSITIONS////////
      
        //////ENEMY AI ACTIONS////////

        // Check timer for the enemy input
        if (screen == COMBAT && STATE_enemyTimerStarted && timer->deltaTime()>STATE_enemyTimerCount)
            STATE_enemyTimerStarted = false;
        // Enemy AI input during the combat
        if (screen == COMBAT && !STATE_enemyTimerStarted)
        {
            // Find character in combat participants from the current player in roundOrder
            for (int i = 4; i < combatParticipants.size(); i++)
            {
                if ( combatParticipants[i].isEnemy() && combatParticipants[i].getName() == roundOrder[currOrderNum])
                {
                    // Call function to return a logical decision of the action the enemy will take along with desired target
                    std::pair<ActionType, std::vector<std::vector<int>>> decision = combatParticipants[i].getActionAndTargets(combatParticipants, "logic");
                    // If more than one valid target, select a random target from the vector
                    std::uniform_int_distribution<> distForTarget(0, decision.second.size()-1);
                    int targetChoice = distForTarget(gen);
                    // Set current target to the enemy AI's target choice
                    currTarget = targetChoice;
                    validMoves = decision.second;
                    // Attack decision
                    if (decision.first == ATTACK)
                    {
                        // Used to display in the textbox the amount of damage
                        std::vector<int> attackDamage;
                        
                        // Find character in combat participants from the current player in roundOrder
                        for (int i = 0; i < combatParticipants.size(); i++)
                        {
                            if (combatParticipants[i].getName() == roundOrder[currOrderNum])
                            {
                                // Save the index of the character's position
                                int charIndex = combatParticipants[i].getParticipantsIndex();
                                // Get valid moves from the current character's position
                                validMoves = combatParticipants[i].getValidMoves(ATTACK, charIndex, combatParticipants);
                                // Get names of current player and target
                                std::string currPlayerName = combatParticipants[i].getName();
                                currPlayerName.erase(std::remove_if(currPlayerName.begin(), currPlayerName.end(), ::isspace), currPlayerName.end());
                                std::string targetNotification;
                                targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                // Commit attack action
                                combatParticipants = combatParticipants[i].doAction(ATTACK, attackDamage, validMoves[currTarget], combatParticipants);
                                // Display textbox notification
                                std::string attackNotification;
                                attackNotification += currPlayerName;
                                attackNotification += " attacks: ";
                                attackNotification += std::to_string(attackDamage[0]);
                                attackNotification += " dmg dealt to ";
                                attackNotification += targetNotification;
                                
                                // Set timer for damage animation popup sprite
                                int currPlayerIdx = charIndex;
                                STATE_lastCurrTarget = currTarget;
                                STATE_timerAnimationStarted = true;
                                STATE_timerAnimationCount = timer->deltaTime() + 1;
                                whichTargetXValueForDamageAnimation = damageTakenPosition[validMoves[currTarget][0]];
                                // Set timer for textbox notification
                                battleNotification.changeText(attackNotification);
                                STATE_timerStarted = true;
                                STATE_timerCount = timer->deltaTime() + 3;
                                break;
                            }
                        }
                    }
                    // Healing buff action
                    if (decision.first == BUFF)
                    {
                        // Used to display the healing amount
                        std::vector<int> healAmount;

                        // Find character in combat participants from the current player in roundOrder
                        for (int i = 0; i < combatParticipants.size(); i++)
                        {
                            if (combatParticipants[i].getName() == roundOrder[currOrderNum])
                            {
                                // Save the index of the character's position
                                int charIndex = combatParticipants[i].getParticipantsIndex();
                                // Get valid moves from the current character's position
                                validMoves = combatParticipants[i].getValidMoves(BUFF, charIndex, combatParticipants);
                                // Get names of current player and target
                                std::string currPlayerName = combatParticipants[i].getName();
                                currPlayerName.erase(std::remove_if(currPlayerName.begin(), currPlayerName.end(), ::isspace), currPlayerName.end());
                                std::string targetNotification;
                                targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                // Commit buff action
                                combatParticipants = combatParticipants[i].doAction(BUFF, healAmount, validMoves[currTarget], combatParticipants);
                                // Display textbox notification
                                std::string healNotification;
                                healNotification += currPlayerName;
                                healNotification += " heals: ";
                                healNotification += std::to_string(healAmount[0]);
                                healNotification += " healed for ";
                                healNotification += targetNotification;
                                battleNotification.changeText(healNotification);
                                // Set timer for textbox notification
                                STATE_timerStarted = true;
                                STATE_timerCount = timer->deltaTime() + 3;
                                break;
                            }
                        }
                    } 
                    // Debuff action
                    if (decision.first == DEBUFF)
                    {
                        // Used to store the new speed for the target
                        std::vector<int> newSpeed;

                        // Find character in combat participants from the current player in roundOrder
                        for (int i = 0; i < combatParticipants.size(); i++)
                        {
                            if (combatParticipants[i].getName() == roundOrder[currOrderNum])
                            {
                                // Save the index of the character's position
                                int charIndex = combatParticipants[i].getParticipantsIndex();
                                // Get valid moves from the current character's position
                                validMoves = combatParticipants[i].getValidMoves(DEBUFF,charIndex,combatParticipants);
                                std::string currPlayerName = combatParticipants[i].getName();
                                // Get names of current player and target
                                currPlayerName.erase(std::remove_if(currPlayerName.begin(), currPlayerName.end(), ::isspace), currPlayerName.end());
                                std::string targetNotification;
                                targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                // Commit debuff action
                                combatParticipants = combatParticipants[i].doAction(DEBUFF, newSpeed, validMoves[currTarget], combatParticipants);
                                // Display textbox notification
                                std::string debuffNotification;
                                debuffNotification += currPlayerName;
                                debuffNotification += " debuffs: ";
                                debuffNotification += std::to_string(newSpeed[0]);
                                debuffNotification += " is the new speed for ";
                                debuffNotification += targetNotification;
                                battleNotification.changeText(debuffNotification);
                                // Set timer for textbox notification
                                STATE_timerStarted = true;
                                STATE_timerCount = timer->deltaTime() + 3;
                                break;
                            }
                        }
                    } 
                    // Move positions action
                    if (decision.first == MOVE)
                    {
                        // Placeholder of empty vector to pass into the doAction function since action is simply moving its position
                        std::vector<int> nothing;
                        // Find character in combat participants from the current player in roundOrder
                        for (int i = 0; i < combatParticipants.size();i++)
                        {
                            if (combatParticipants[i].getName() == roundOrder[currOrderNum])
                            {
                                // Save the index of the character's position
                                int charIndex = combatParticipants[i].getParticipantsIndex();
                                // Get valid moves from the current character's position
                                validMoves = combatParticipants[i].getValidMoves(MOVE, charIndex, combatParticipants);
                                // Get name of current player
                                std::string currPlayerName = combatParticipants[i].getName();
                                currPlayerName.erase(std::remove_if(currPlayerName.begin(),currPlayerName.end(), ::isspace), currPlayerName.end());
                                std::string targetNotification;
                                std::string moveNotification;
                                // Commit move action as long as the target is not itself
                                if (validMoves[currTarget][0] != charIndex) 
                                {
                                    // Get name of target
                                    targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                    // Commit move action
                                    combatParticipants = combatParticipants[i].doAction(MOVE, nothing, validMoves[currTarget], combatParticipants);
                                    // Set textbox notification
                                    moveNotification += currPlayerName;
                                    moveNotification += " switch places with ";
                                    moveNotification += targetNotification;
                                }
                                // Case Scenario for when move action is selected but the rest of its teammates are dead
                                // Does not move its position
                                else 
                                {
                                    moveNotification = " Your teammates are dead. You just wasted a turn.";
                                }
                                // Set timer for textbox notification
                                battleNotification.changeText(moveNotification);
                                STATE_timerStarted = true;
                                STATE_timerCount = timer->deltaTime() + 3;
                                break;       
                            }
                        }
                    }
                    // Go to the next character in the round order that is alive
                    bool isNextTurnAlive = false;
                    do 
                    {
                        // Make sure at the end of the round order, the next current order number resets to zero
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

                    // Check end state for battle if the players are dead or the enemies are dead
                    bool isPlayerTeamAlive = isTeamAlive(combatParticipants, false);
                    bool isEnemyTeamAlive = isTeamAlive(combatParticipants, true);

                    // If all enemies are dead, then transition to victory screen
                    if (isEnemyTeamAlive == false) 
                    {
                        // Saves player team's stats and positions when exiting combat screen
                        playerTeam = {combatParticipants[0], combatParticipants[1], combatParticipants[2], combatParticipants[3]};
                        for (int i = 0; i < 4; i++)
                        {  
                                if (combatParticipants[i].getName() == flute.getName())
                                    flute = combatParticipants[i];
                                if (combatParticipants[i].getName() == drum.getName())
                                    drum = combatParticipants[i];
                                if (combatParticipants[i].getName() == bass.getName())
                                    bass = combatParticipants[i];
                                if (combatParticipants[i].getName() == conductor.getName())
                                    conductor = combatParticipants[i];                                
                        }
                        // Update the player team's stats textboxes for the stat screen
                        conductorHPName.changeText(std::to_string(conductor.getHp()) + "-");
                        statMenuConductorRow[1] = conductorHPName;
                        bassHPName.changeText(std::to_string(bass.getHp()) + "-");
                        statMenuBassRow[1] = bassHPName;
                        drumHPName.changeText(std::to_string(drum.getHp()) + "-");
                        statMenuDrumRow[1] = drumHPName;
                        fluteHPName.changeText(std::to_string(flute.getHp()) + "-");
                        statMenuFluteRow[1] = fluteHPName;
                        // Reset combat states to its initial values when combat ends
                        STATE_combatMenuTargetSelected = false;
                        STATE_battle = false;
                        STATE_enemiesSet = false;
                        STATE_roundsSet = false;
                        STATE_timerStarted = false;
                        STATE_timerAnimationStarted = false;
                        currTarget = 0;
                        currOrderNum = 0;
                        // Transition to victory screen
                        STATE_youWin = true;
                        screen = WIN;
                        break;
                       
                    }
                    // If all player characters are dead, transition to defeat screen
                    else if (isPlayerTeamAlive == false)
                    {
                        // Reset hp stat for player characters
                        flute.setHp(flute.getMaxHp());
                        conductor.setHp(conductor.getMaxHp());
                        bass.setHp(bass.getMaxHp());
                        drum.setHp(drum.getMaxHp());
                        // Reposition player characters in its original positions
                        playerTeam = {flute, conductor, bass, drum};
                        // Update player team's stat textboxes
                        conductorHPName.changeText(std::to_string(conductor.getHp()) + "-");
                        statMenuConductorRow[1] = conductorHPName;
                        bassHPName.changeText(std::to_string(bass.getHp()) + "-");
                        statMenuBassRow[1] = bassHPName;
                        drumHPName.changeText(std::to_string(drum.getHp()) + "-");
                        statMenuDrumRow[1] = drumHPName;
                        fluteHPName.changeText(std::to_string(flute.getHp()) + "-");
                        statMenuFluteRow[1] = fluteHPName;
                        // Reset combat states to its initial vlaues when combat ends
                        STATE_combatMenuTargetSelected = false;
                        STATE_newGameSelected = false;
                        STATE_enemiesSet = false;
                        STATE_battle = false;
                        STATE_roundsSet = false;
                        STATE_timerStarted = false;
                        STATE_timerAnimationStarted = false;
                        currTarget = 0;
                        currOrderNum = 0;
                        // Transition to defeat screen
                        STATE_youLoose = true;
                        screen = DEFEAT;
                        break;
                    }
                    // Once enemy makes a decision reset state for target selected
                    STATE_combatMenuTargetSelected = false;                    
                    currTarget = 0;
                    // Set timer for enemy AI actions between turns
                    STATE_enemyTimerStarted = true;
                    STATE_enemyTimerCount = timer->deltaTime() + 3;
                }
            }
        }
        //////END ENEMY AI ACTIONS////////

        //////USER INPUT EVENT/////////// 
        while(SDL_PollEvent(&event))
        {
            // Close the window and quit out of the game 
            if (event.type == SDL_QUIT)
            {
                setToQuit();
                break;
            }
            // Debug shortcut buttons
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
            // Screen transition states
            switch (screen)
            {
                // Main menu screen
                case INTRO:
                { 
                    introMenu.onInput(event, SelectMusic, STATE_introSelectedOption);
                    //Screen options for intro 
                    if (STATE_introSelectedOption != "NONE")
                    {
                        if (STATE_introSelectedOption == "New Game")
                        {
                            STATE_newGameSelected = true;
                            STATE_gameOver = false; 
                            STATE_mapScreenOpenForTransition = true;
                        }
                        if (STATE_introSelectedOption == "Credits")
                        {
                            screen = CREDITS;
                            STATE_introSelectedOption = "NONE";
                        }
                    }
                    
                    break;
                }
                // Map screen
                case MAP:
                {
                    // Debug camera for the map screen if number 9 key pressed
                    if (event.type == SDL_KEYDOWN)
                    {
                        if (event.key.keysym.sym == SDLK_9)
                            STATE_debug = !STATE_debug;
                    }
                    if (STATE_debug)
                        debugCont.onInput(event, MAP_WIDTH, MAP_HEIGHT);
                    // Move character throughout the map with directional keys
                    else if (!STATE_mapEventboxOpen)
                        characterController.onInput(event, nextMapEvent, STATE_mapEventboxOpen, STATE_characterDirection, coordinateToTileTypeMap, coordinateToEventTypeMap);
                    // If character in map landed on a battle tile
                    if (nextMapEvent == "BATTLE")
                    {
                        // Initialize enemy characters
                        BaseCharacter e1 = BaseCharacter("Conehead A", 10, 2, 1, 0, 3, 3, 3, true);
                        BaseCharacter e2 = BaseCharacter("Conehead B ", 10, 6, 1, 0, 3, 3, 3, true);
                        BaseCharacter e3 = BaseCharacter("Pizza Head", 10, 2, 1, 0, 3, 3, 3, true);
                        BaseCharacter e4 = BaseCharacter("Mini Carl", 20, 0, 1, 0, 3, 3, 3, true);
                        // Set enemy AI index in combat participants vector
                        e1.setNewParticipantsIndex(4);
                        e2.setNewParticipantsIndex(5);
                        e3.setNewParticipantsIndex(6);
                        e4.setNewParticipantsIndex(7);
                        // Set up enemy vector
                        std::vector<BaseCharacter> enemiesTemp{e1, e2, e3, e4};
                        enemies = enemiesTemp;
                        // Set beginning of combat participants vector with player characters                        
                        combatParticipants = playerTeam;
                        // Then push back enemies at the end of the vector 
                        combatParticipants.insert(std::end(combatParticipants), std::begin(enemies), std::end(enemies));

                        // Change enemiesSet state
                        STATE_enemiesSet = true;
                        // Change battle state
                        STATE_battle = true;
                        // Set round order for all the characters in the beginning of the combat event
                        roundOrder = setRoundTurns(combatParticipants);
                        STATE_roundsSet = true;
                        STATE_mapScreenOpenForTransition = false;
                        // Timer set in case the first character's turn is the enemy AI
                        STATE_enemyTimerStarted = true;
                        STATE_enemyTimerCount = timer->deltaTime() + 3;
                    }
                    // If chracter in map landed on a boss tile
                    else if (nextMapEvent == "BOSS")
                    {
                        // Initiazlize boss and 3 dead characters as placeholders because functions deal with a vectors with a size of 8 
                        BaseCharacter boss = BaseCharacter("Carl", 100, 0, 8, 5, 0, 0, 0, true);
                        BaseCharacter e2 = BaseCharacter("", 0, 6, 1, 0, 3, 3, 3, true);
                        BaseCharacter e3 = BaseCharacter("", 0, 2, 1, 0, 3, 3, 3, true);
                        BaseCharacter e4 = BaseCharacter("", 0, 0, 1, 0, 3, 3, 3, true);
                        // Make sure the index of the boss is in the front of the vector
                        boss.setNewParticipantsIndex(4);
                        e2.setNewParticipantsIndex(5);
                        e3.setNewParticipantsIndex(6);
                        e4.setNewParticipantsIndex(7);
                        // Set placeholders as dead so only the boss will be on the screen
                        e2.changeLifeStatus(false);
                        e3.changeLifeStatus(false);
                        e4.changeLifeStatus(false);
                        // Set up enemy vector
                        std::vector<BaseCharacter> enemiesTemp{boss, e2, e3, e4};
                        enemies = enemiesTemp;
                        // Set beginning of combat participants vector with player characters                        
                        combatParticipants = playerTeam;
                        // Then push back enemies at the end of the vector 
                        combatParticipants.insert(std::end(combatParticipants), std::begin(enemies), std::end(enemies));

                        // Change enemiesSet state
                        STATE_enemiesSet = true;
                        // Change battle state
                        STATE_battle = true;
                        // Change boss fights state 
                        STATE_bossFightBegin = true;
                        // Set round order for all the characters in the beginning of the combat event
                        roundOrder = setRoundTurns(combatParticipants);
                        STATE_roundsSet = true;
                        screen = COMBAT;
                        nextMapEvent = "BLANKEVENT";
                        STATE_mapEventboxOpen = false;
                        // Timer set in case the first character's turn is the enemy AI
                        STATE_enemyTimerStarted = true;
                        STATE_enemyTimerCount = timer->deltaTime() + 3;
                        // Update setRoundTurns display
                        for(int i = 0; i < roundOrder.size(); i++)
                        {
                            orderBoxes[i].changeText(roundOrder[i]);
                        }
                    }
                    // If character in map landed on an item tile
                    else if (nextMapEvent == "ITEM" && !STATE_itemNotificationShowing)
                    {
                        Mix_PlayChannel(-1, MapNotificationSound, 0);
                        std::uniform_int_distribution<> distForRarity(1,100);
                        std::uniform_int_distribution<> distForItem(0,3);
                        int itemRoll = distForItem(gen);
                        int rarityRoll = distForRarity(gen);
                        // If it's 0 it's a NORMAL_HIT
                        if (itemRoll == 1)
                        {
                            itemRoll = NORMAL_DODGE;
                        }
                        else if (itemRoll == 2)
                        {
                            itemRoll = NORMAL_SPEED;
                        }
                        else if (itemRoll == 3)
                        {
                            itemRoll = NORMAL_ARMOR;
                        }
                        // This just bumps up the item quality
                        if (rarityRoll > 70 && rarityRoll < 95)
                        {
                            itemRoll += 1;
                        }
                        else if (rarityRoll >= 95)
                        {
                            itemRoll += 2;
                        }
                        
                        STATE_isWorseItem = true;

                        // Check to equip item equipItem
                        
                        if (itemRoll / 3 == 0)
                        {
                            if (itemRoll % 3 + 1 > flute.getItemModifier())
                            {
                                // If item is better, equip
                                int oldMod = flute.getItemModifier();
                                STATE_isWorseItem = false;
                                flute.setItem(itemList[itemRoll]);
                                flute.setItemModifier(itemList[itemRoll].getMod());
                                // Upgrade is the difference between the new and the old
                                int newMod = itemList[itemRoll].getMod() - oldMod;
                                flute.setHit(flute.getHit() + newMod);
                                for (auto player: playerTeam)
                                {
                                    if (player.getName() == "Flute")
                                    {
                                        player.setItem(itemList[itemRoll]);
                                        player.setItemModifier(itemList[itemRoll].getMod());
                                        player.setHit(player.getHit() + newMod);
                                        break;
                                    }
                                }
                                // Update for stat menu
                                fluteItemModName.changeText(flute.getItem().getMessage());
                                statMenuFluteRow[7] = fluteItemModName;
                                fluteHitName.changeText(std::to_string(flute.getHit()));
                                fluteHitName.changeTextColor(Color::lightBlue);
                                statMenuFluteRow[4] = fluteHitName;
                            }
                        }
                        else if (itemRoll / 3 == 1)
                        {
                            if (itemRoll % 3 + 1 > conductor.getItemModifier())
                            {
                                // If item is better, equip
                                int oldMod = conductor.getItemModifier();
                                STATE_isWorseItem = false;
                                conductor.setItem(itemList[itemRoll]);
                                conductor.setItemModifier(itemList[itemRoll].getMod());
                                // Upgrade is the difference between the new and the old
                                int newMod = itemList[itemRoll].getMod() - oldMod;
                                conductor.setDodgeModifier(conductor.getDodgeModifier() + newMod);
                                for (auto player: playerTeam)
                                {
                                    if (player.getName() == "Conductor")
                                    {
                                        player.setItem(itemList[itemRoll]);
                                        player.setItemModifier(itemList[itemRoll].getMod());
                                        player.setDodgeModifier(player.getDodgeModifier() + newMod);
                                        break;
                                    }
                                }
                                // Update for stat menu
                                conductorItemModName.changeText(conductor.getItem().getMessage());
                                statMenuConductorRow[7] = conductorItemModName;
                                conductorDodgeName.changeText(std::to_string(conductor.getDodgeModifier()));
                                conductorDodgeName.changeTextColor(Color::lightBlue);
                                statMenuConductorRow[6] = conductorDodgeName;

                            }
                        }
                        else if (itemRoll / 3 == 2)
                        {
                            if (itemRoll % 3 + 1 > drum.getItemModifier())
                            {
                                // If item is better, equip
                                int oldMod = drum.getItemModifier();
                                STATE_isWorseItem = false;
                                drum.setItem(itemList[itemRoll]);
                                drum.setItemModifier(itemList[itemRoll].getMod());
                                // Upgrade is the difference between the new and the old
                                int newMod = itemList[itemRoll].getMod() - oldMod;
                                drum.setSpeed(drum.getSpeed() + newMod);
                                for (auto player: playerTeam)
                                {
                                    if (player.getName() == "Drummer")
                                    {
                                        player.setItem(itemList[itemRoll]);
                                        player.setItemModifier(itemList[itemRoll].getMod());
                                        player.setSpeed(player.getSpeed() + newMod);
                                        break;
                                    }
                                }
                                // Update for stat menu
                                drumItemModName.changeText(drum.getItem().getMessage());
                                statMenuDrumRow[7] = drumItemModName;
                                drumSpeedName.changeText(std::to_string(drum.getSpeed()));
                                drumSpeedName.changeTextColor(Color::lightBlue);
                                statMenuDrumRow[3] = drumSpeedName;
                            }
                        }
                        if (itemRoll / 3 == 3)
                        {
                            if (itemRoll % 3 + 1 > bass.getItemModifier())
                            {
                                // If item is better, equip
                                int oldMod = bass.getItemModifier();
                                STATE_isWorseItem = false;
                                bass.setItem(itemList[itemRoll]);
                                bass.setItemModifier(itemList[itemRoll].getMod());
                                // Upgrade is the difference between the new and the old
                                int newMod = itemList[itemRoll].getMod() - oldMod;
                                bass.setArmor(bass.getArmor() + newMod);
                                for (auto player: playerTeam)
                                {
                                    if (player.getName() == "Bass")
                                    {
                                        player.setItem(itemList[itemRoll]);
                                        player.setItemModifier(itemList[itemRoll].getMod());
                                        player.setArmor(player.getArmor() + newMod);
                                        break;
                                    }
                                }
                                // Update for stat menu
                                bassItemModName.changeText(bass.getItem().getMessage());
                                statMenuBassRow[7] = bassItemModName;
                                bassArmorName.changeText(std::to_string(bass.getArmor()));
                                bassArmorName.changeTextColor(Color::lightBlue);
                                statMenuBassRow[5] = bassArmorName;
                            }
                        }
                        // Set textbox notification for item found
                        STATE_itemFound = itemList[itemRoll].getName();
                        STATE_itemNotificationShowing = true;
                        
                    }
                    // If character in map landed on healing tile, heal the entire team
                    else if (nextMapEvent == "HEAL" && !STATE_healNotificationShowing)
                    {
                        Mix_PlayChannel(-1, MapNotificationSound, 0);
                        std::uniform_int_distribution<> distForHeal(1,3);
                        STATE_amountHealed = distForHeal(gen);
                        for (auto& player: playerTeam)
                        {
                            if (player.getHp() != player.getMaxHp())
                                player.setHp(player.getHp() + STATE_amountHealed);
                        }
                        STATE_healNotificationShowing = true;
                    }
                    // Display a joke if character in map landed on joke tile
                    else if (nextMapEvent == "JOKE" && !STATE_jokeNotificationShowing)
                    {
                        Mix_PlayChannel(-1, MapNotificationSound, 0);
                        STATE_jokeNotificationShowing = true;
                    }
                
                    //New events add here

                    if (event.type == SDL_KEYDOWN)
                    {
                        switch (event.key.keysym.sym)
                        {
                            // After event notification pops up on screen, press return key to continue
                            case SDLK_RETURN:
                            {
                                STATE_mapEventboxOpen = false;
                                STATE_itemNotificationShowing = false;
                                STATE_healNotificationShowing = false;
                                STATE_didGetRandNumForJoke = true;
                                nextMapEvent = "BLANKEVENT";
                                break;
                            }
                            // Press the 'E' key to go to stat menu screen
                            case SDLK_e:
                            {
                                STATE_updateHP = true;
                                STATE_statMenu = true;
                                STATE_mapScreenOpenForTransition = false;
                                break;
                            }
                        }
                    } 
                    break;
                }
                // Combat state
                case COMBAT:
                {   
                    // State for selecting a target
                    if (STATE_combatSelectedOption != "NONE" && !STATE_combatMenuTargetSelected)
                    {  
                        for (int i = 0; i < combatParticipants.size(); i++)
                        { 
                            if (!combatParticipants[i].isEnemy() && combatParticipants[i].getName()==roundOrder[currOrderNum])
                            {
                                // Event for selecting target with left and right directional arrow keys
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
                        }
                    }
                
                    // Create new round and set round turns
                    if(!STATE_roundsSet && STATE_roundOver)
                    {
                        // Get new round turn order
                        roundOrder = setRoundTurns(combatParticipants);
                        
                        STATE_roundsSet = true;
                        STATE_roundOver = false;
                    }
                    // Select combat menu action from the player
                    if (STATE_combatSelectedOption == "NONE" && !STATE_combatMenuTargetSelected)
                    {
                        for (int i = 0; i < combatParticipants.size(); i++)
                        { 
                            if (!combatParticipants[i].isEnemy() && combatParticipants[i].getName()==roundOrder[currOrderNum])
                            {
                                combatMenu.onInput(event, SelectMusic, STATE_combatSelectedOption);
                            }
                        }
                    }

                    // State for when a round ends
                    if ((currOrderNum + 1) == roundOrder.size() && STATE_combatSelectedOption!= "None" && STATE_combatMenuTargetSelected)
                    {
                        STATE_roundOver = true;
                        STATE_roundsSet = false;
                    }
                    // State once target is selected
                    if (STATE_combatMenuTargetSelected)
                    {   
                        // Attack state
                        if (STATE_combatSelectedOption == "Attack")
                        {
                            // Used to display number for attack damage
                            std::vector<int> attackDamage;
                            STATE_combatSelectedOption = "NONE";
                            // Find player character in combat participants vector
                            for (int i = 0; i < combatParticipants.size(); i++)
                            {
                                if (combatParticipants[i].getName() == roundOrder[currOrderNum])
                                {
                                    // Set character index of current player
                                    int charIndex = combatParticipants[i].getParticipantsIndex();
                                    // Get valid targets for attack action
                                    validMoves = combatParticipants[i].getValidMoves(ATTACK, charIndex, combatParticipants);
                                    // Get name of current player and target
                                    std::string currPlayerName = combatParticipants[i].getName();
                                    currPlayerName.erase(std::remove_if(currPlayerName.begin(), currPlayerName.end(), ::isspace), currPlayerName.end());
                                    std::string targetNotification;
                                    targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                    // Commit attack action
                                    combatParticipants = combatParticipants[i].doAction(ATTACK, attackDamage, validMoves[currTarget], combatParticipants);
                                    // Set up textbox notification
                                    std::string attackNotification;
                                    attackNotification += currPlayerName;
                                    attackNotification += " attacks: ";
                                    attackNotification += std::to_string(attackDamage[0]);
                                    attackNotification += " dmg dealt to ";
                                    attackNotification += targetNotification;
                                    // Set up attack damage popup animation
                                    int currPlayerIdx = charIndex;
                                    STATE_lastCurrTarget = currTarget;
                                    STATE_timerAnimationStarted = true;
                                    STATE_timerAnimationCount = timer->deltaTime() + 1;
                                    whichTargetXValueForDamageAnimation = damageTakenPosition[validMoves[currTarget][0]];
                                    battleNotification.changeText(attackNotification);
                                    // Set up timer for textbox notification
                                    STATE_timerStarted = true;
                                    STATE_timerCount = timer->deltaTime() + 3;
                                    break;
                                }
                            }  
                        }
                        // Buff state
                        if (STATE_combatSelectedOption == "Buff")
                        {
                            
                            STATE_combatSelectedOption = "NONE";
                            // Used to display healing amount for textbox
                            std::vector<int> healAmount;

                            // Find player character in combat participants vector
                            for (int i = 0; i < combatParticipants.size(); i++)
                            {
                                if (combatParticipants[i].getName() == roundOrder[currOrderNum])
                                {
                                    // Set character index of current player
                                    int charIndex = combatParticipants[i].getParticipantsIndex();
                                    // Get valid targets for buff action
                                    validMoves = combatParticipants[i].getValidMoves(BUFF, charIndex, combatParticipants);
                                    // Get name of current player and target
                                    std::string currPlayerName = combatParticipants[i].getName();
                                    currPlayerName.erase(std::remove_if(currPlayerName.begin(),currPlayerName.end(), ::isspace),currPlayerName.end());
                                    std::string targetNotification;
                                    targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                    // Commit buff action
                                    combatParticipants = combatParticipants[i].doAction(BUFF, healAmount, validMoves[currTarget], combatParticipants);
                                    // Set up textbox notification
                                    std::string healNotification;
                                    healNotification += currPlayerName;
                                    healNotification += " heals: ";
                                    healNotification += std::to_string(healAmount[0]);
                                    healNotification += " healed for ";
                                    healNotification += targetNotification;
                                    battleNotification.changeText(healNotification);
                                    // Set timer for textbox notification
                                    STATE_timerStarted = true;
                                    STATE_timerCount = timer->deltaTime() + 3;
                                    break;
                                }
                            }
                        }
                        // Debuff state
                        if (STATE_combatSelectedOption == "Debuff")
                        {
                            STATE_combatSelectedOption = "NONE";
                            // Used to display the new speed of target
                            std::vector<int> newSpeed;
                            // Find player character in combat participants vector
                            for (int i = 0; i < combatParticipants.size(); i++)
                            {
                                if (combatParticipants[i].getName() == roundOrder[currOrderNum])
                                {
                                    // Set character index of current player
                                    int charIndex = combatParticipants[i].getParticipantsIndex();
                                    // Get valid targets for debuff action
                                    validMoves = combatParticipants[i].getValidMoves(DEBUFF, charIndex, combatParticipants);
                                    // Get name of current player and target
                                    std::string currPlayerName = combatParticipants[i].getName();
                                    currPlayerName.erase(std::remove_if(currPlayerName.begin(), currPlayerName.end(), ::isspace), currPlayerName.end());
                                    std::string targetNotification;
                                    targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                    // Commit debuff action
                                    combatParticipants = combatParticipants[i].doAction(DEBUFF, newSpeed, validMoves[currTarget], combatParticipants);
                                    // Set up textbox notification
                                    std::string debuffNotification;
                                    debuffNotification += currPlayerName;
                                    debuffNotification += " debuffs: ";
                                    debuffNotification += std::to_string(newSpeed[0]);
                                    debuffNotification += " is the new speed for ";
                                    debuffNotification += targetNotification;
                                    battleNotification.changeText(debuffNotification);
                                    // Set up timer for textbox notification
                                    STATE_timerStarted = true;
                                    STATE_timerCount = timer->deltaTime() + 3;
                                    break;
                                }
                            }
                        }
                        // Move positions state
                        if (STATE_combatSelectedOption == "Move")
                        {
                            STATE_combatSelectedOption = "NONE";
                            // Placeholder of empty vector to pass into doAction function
                            std::vector<int> nothing;
                            // Find player character in combat participants vector
                            for (int i = 0; i < combatParticipants.size(); i++)
                            {
                                if (combatParticipants[i].getName() == roundOrder[currOrderNum])
                                {
                                    // Set character index of current player
                                    int charIndex = combatParticipants[i].getParticipantsIndex();
                                    // Get valid targets for move action
                                    validMoves = combatParticipants[i].getValidMoves(MOVE, charIndex, combatParticipants);
                                    // Get name of current player
                                    std::string currPlayerName = combatParticipants[i].getName();
                                    currPlayerName.erase(std::remove_if(currPlayerName.begin(), currPlayerName.end(), ::isspace), currPlayerName.end());
                                    std::string targetNotification;
                                    std::string moveNotification;
                                    // If move is selected and as long as target is not its own position, move positions with target
                                    if (validMoves[currTarget][0] != charIndex) 
                                    {
                                        // Get name of target   
                                        targetNotification = combatParticipants[validMoves[currTarget][0]].getName();
                                        // Commit move action
                                        combatParticipants = combatParticipants[i].doAction(MOVE, nothing, validMoves[currTarget], combatParticipants);
                                        // Set up textbox notification
                                        moveNotification += currPlayerName;
                                        moveNotification += " switch places with ";
                                        moveNotification += targetNotification;
                                    }
                                    // If move is selected, but all teammates are dead, do not change positions
                                    else 
                                    {
                                        moveNotification = " Your teammates are dead. You just wasted a turn.";
                                    }
                                    // Set timer for textbox notification
                                    battleNotification.changeText(moveNotification);
                                    STATE_timerStarted = true;
                                    STATE_timerCount = timer->deltaTime() + 3;
                                    break;       
                                }
                            } 
                        }
                        // Go to the next alive character in the round order queue
                        bool isNextTurnAlive = false;
                        do 
                        {
                            // If current order number reaches to the size of the round order queue, reset it to zero
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


                        // Check end state for battle if all enemies are dead or all players are dead 
                        bool isPlayerTeamAlive = isTeamAlive(combatParticipants, false);
                        bool isEnemyTeamAlive = isTeamAlive(combatParticipants, true);

                        // If all enemies are dead, transition to victory screen
                        if (isEnemyTeamAlive == false) 
                        {
                            // Saves player team's stats and its positions
                            playerTeam = {combatParticipants[0], combatParticipants[1], combatParticipants[2], combatParticipants[3]};
                            for (int i = 0; i < 4; i++)
                            {
                                    if (combatParticipants[i].getName() == flute.getName())
                                        flute = combatParticipants[i];
                                    if (combatParticipants[i].getName() == drum.getName())
                                        drum = combatParticipants[i];
                                    if (combatParticipants[i].getName() == bass.getName())
                                        bass = combatParticipants[i];
                                    if (combatParticipants[i].getName() == conductor.getName())
                                        conductor = combatParticipants[i];                                
                            }

                            // Updates player HP for statMenu page
                            conductorHPName.changeText(std::to_string(conductor.getHp()) + "-");
                            statMenuConductorRow[1] = conductorHPName;
                            bassHPName.changeText(std::to_string(bass.getHp()) + "-");
                            statMenuBassRow[1] = bassHPName;
                            drumHPName.changeText(std::to_string(drum.getHp()) + "-");
                            statMenuDrumRow[1] = drumHPName;
                            fluteHPName.changeText(std::to_string(flute.getHp()) + "-");
                            statMenuFluteRow[1] = fluteHPName;
                            // Reset combat states to its initial values once combat ends
                            STATE_combatMenuTargetSelected = false;
                            STATE_battle = false;
                            STATE_enemiesSet = false;
                            STATE_roundsSet = false;
                            STATE_timerStarted = false;
                            STATE_timerAnimationStarted = false;
                            // Special case when boss is defeated
                            if (combatParticipants[4].getName()=="Carl")
                                STATE_isBossDead = true;
    
                            currTarget = 0;
                            currOrderNum = 0;
                            // Transition to victory screen
                            STATE_youWin = true;
                            screen = WIN;
                            break;
                        }
                        // If all player characters are dead, transition to defeat screen
                        else if (isPlayerTeamAlive == false)
                        {
                            // Reset player characters' HP and its positions
                            flute.setHp(flute.getMaxHp());
                            conductor.setHp(conductor.getMaxHp());
                            bass.setHp(bass.getMaxHp());
                            drum.setHp(drum.getMaxHp());
                            playerTeam = {flute, conductor, bass, drum};
                            // Update player HP in stat menu
                            conductorHPName.changeText(std::to_string(conductor.getHp()) + "-");
                            statMenuConductorRow[1] = conductorHPName;
                            bassHPName.changeText(std::to_string(bass.getHp()) + "-");
                            statMenuBassRow[1] = bassHPName;
                            drumHPName.changeText(std::to_string(drum.getHp()) + "-");
                            statMenuDrumRow[1] = drumHPName;
                            fluteHPName.changeText(std::to_string(flute.getHp()) + "-");
                            statMenuFluteRow[1] = fluteHPName;
                            // Reset combat states to its initial values once combat ends
                            STATE_combatMenuTargetSelected = false;
                            STATE_newGameSelected = false;
                            STATE_enemiesSet = false;
                            STATE_battle = false;
                            STATE_roundsSet = false;
                            STATE_timerStarted = false;
                            STATE_timerAnimationStarted = false;
                            currTarget = 0;
                            currOrderNum = 0;
                            // Transition to defeat screen
                            STATE_youLoose = true;
                            screen = DEFEAT;
                            break;
                        }
                        // Reset target selected state once action has been carried out
                        STATE_combatMenuTargetSelected = false;
                        currTarget = 0;
                    }
                }
                // Stat menu state
                case STATUS_MENU:
                {
                    if (event.type == SDL_KEYDOWN)
                    {
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_e:
                            {
                                STATE_statMenu = false;
                                STATE_mapScreenOpenForTransition = true;
                                break;
                            }
                        }
                    } 

                    break;
                }
                // Victory state
                case WIN:
                {     
                    if(STATE_youWin == true){
                        if (event.type == SDL_KEYDOWN)
                        {
                            switch (event.key.keysym.sym)
                            {
                                case SDLK_RETURN:
                                {
                                    STATE_bossFightBegin = false;
                                    STATE_youWin = false;
                                    // If boss is defeated, go back to main menu
                                    if (STATE_isBossDead)
                                        screen = INTRO;
                                    // If not a boss encounter, go back to the map
                                    else
                                        screen = MAP;
                                    break;
                                }
                            }
                        } 
                    }
                }
                // Defeat state
                case DEFEAT:
                {
                    // GO back to main menu
                    if(STATE_youLoose == true){
                        if (event.type == SDL_KEYDOWN)
                        {
                            switch (event.key.keysym.sym)
                            {
                                case SDLK_ESCAPE:
                                {
                                    STATE_bossFightBegin = false;
                                    screen = INTRO;
                                    break;
                                }
                            }
                        } 
                    }      
                }
            }
        }
        //////END USER INPUT EVENTS//////

        //////SCREEN RENDERING///////////

        //Clear screen
        SDL_SetRenderDrawColor(getRenderer(), 255, 255, 255, 255);
        SDL_RenderClear(getRenderer());

        switch (screen)
        {
            // Intro screen rendering
            case INTRO:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());
                introScreen.render(getRenderer(), 0, 0);
                introMenu.render(getRenderer()); 

                //Screen transition turning background opaque 
                if(STATE_postTransition == true)
                {
                    alphaValueScreenTransition -= 5;
                    blackScreenTransition.setAlpha(alphaValueScreenTransition);
                    if(alphaValueScreenTransition == 0)
                    {
                        STATE_postTransition = false;  
                    }
                    blackScreenTransition.render(getRenderer(), 0, 0);
                }
                //Screen transition turning background transparent
                else if(STATE_preTransition == true)
                {
                    alphaValueScreenTransition += 5;
                    blackScreenTransition.setAlpha(alphaValueScreenTransition);
                    if(alphaValueScreenTransition == 255)
                    {
                        STATE_preTransition = false;
                    }
                    blackScreenTransition.render(getRenderer(), 0, 0);
                }
                
                break;
            }
            // Credits screen rendering
            case CREDITS:
            {
                statusBg.render(getRenderer(), 0, 0);
                SDL_Rect rect = {470, 400, 300, 200};
                SDL_RenderCopy(getRenderer(), maxwell.getTexture(), nullptr, &rect);
                credit1.render(getRenderer());
                credit2.render(getRenderer()); 
                credit3.render(getRenderer()); 
                credit4.render(getRenderer());
                credit5.render(getRenderer());
                break;  
            }
            // Map screen rendering
            case MAP:
            {
                if (STATE_debug)
                {
                    debugCont.move(camera); 
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
                if (!STATE_debug)
                {
                    //hard coded directions, should be a sprite-sheet had time allowed
                    switch (STATE_characterDirection)
                    {
                        case UP:
                        {
                            characterController.render(getRenderer(), camera, characterInMapTextureUp);
                            break;
                        }
                        case LEFT:
                        {
                            characterController.render(getRenderer(), camera, characterInMapTextureLeft);
                            break;
                        }
                        case RIGHT:
                        {
                            characterController.render(getRenderer(), camera, characterInMapTextureRight);
                            break;
                        }
                        case DOWN:
                        {
                            characterController.render(getRenderer(), camera, characterInMapTextureDown);
                            break;
                        }
                        
                    }
                    
                }
                // Item notification textbox rendering
                if (STATE_mapEventboxOpen)
                {   
                    if (nextMapEvent == "ITEM")
                    {
                        std::string textNotification = STATE_itemFound + " was found!";
                        
                        itemNotification.changeText(textNotification);
                        itemNotification.render(getRenderer());
                        if (STATE_isWorseItem)
                        {
                            extra.render(getRenderer());
                            
                            extra2.render(getRenderer());
                        }
                    }
                    // Joke notification textbox rendering
                    else if (nextMapEvent == "JOKE")
                    {
                        if(STATE_didGetRandNumForJoke == true)
                        {
                            std::uniform_int_distribution<> jokeRandomPick(1,6);
                            jokeNumber = jokeRandomPick(gen);
                            STATE_didGetRandNumForJoke = false;
                        }
                        jokeNotification.changeText(jokeList[jokeNumber]);
                        jokeNotification.render(getRenderer()); 
                        
                    }
                    // Healing notification textbox rendering
                    else if (nextMapEvent == "HEAL")
                    {
                        std::string healText = std::to_string(STATE_amountHealed) + "hp healed for all team members";
                        healNotification.changeText(healText);
                        healNotification.render(getRenderer());
                    }
                }
                // Transitions rendering
                //Screen transition turning background opaque 
                if(STATE_postTransition == true)
                {
                    alphaValueScreenTransition -= 5;
                    blackScreenTransition.setAlpha(alphaValueScreenTransition);
                    if(alphaValueScreenTransition == 0)
                    {
                        STATE_postTransition = false;  
                    }
                    blackScreenTransition.render(getRenderer(), 0, 0);
                }
                //Screen transition turning background transparent
                else if(STATE_preTransition == true)
                {
                    alphaValueScreenTransition += 5;
                    blackScreenTransition.setAlpha(alphaValueScreenTransition);
                    if(alphaValueScreenTransition == 255)
                    {
                        STATE_preTransition = false;
                    }
                    blackScreenTransition.render(getRenderer(), 0, 0);
                }

                break;
            } 
            // Combat screen rendering
            case COMBAT: 
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());

                combatScreenTexture.render(getRenderer(), 0, 0);
                
                combatStatBg.render(getRenderer(), 0, 600);

                //Order Pane
                orderBg.render(getRenderer(), 720, 0);
                
                basemenuBg.render(getRenderer(), 720, 480);
                
                combatMenu.render(getRenderer());
                // Regular combat encounter 
                if(STATE_bossFightBegin != true){
                    // Render players at their positions
                    for (int i = 0; i < 4; i++)
                    {

                        if (combatParticipants[i].isAlive())
                        {
                            if (combatParticipants[i].getName() == flute.getName())
                                flutistTexture.render(getRenderer(), charRendering[i], 400);
                            if (combatParticipants[i].getName() == drum.getName())
                                drummerTexture.render(getRenderer(), charRendering[i], 400);
                            if (combatParticipants[i].getName() == bass.getName())
                                bassistTexture.render(getRenderer(), charRendering[i], 400);
                            if (combatParticipants[i].getName() == conductor.getName())
                                conductorTexture.render(getRenderer(), charRendering[i], 400);
                        }

                    }
                
                    // Render enemies at their positions
                    for (int i = 0; i < 4; i++)
                    {
                        if (combatParticipants[i+4].isAlive())
                        {
                            if (combatParticipants[i+4].getName() == enemies[0].getName())
                                enemyBatTexture.render(getRenderer(), charRendering[i+4], 400);
                            if (combatParticipants[i+4].getName() == enemies[1].getName())
                                enemyBellTexture.render(getRenderer(), charRendering[i+4], 400);
                            if (combatParticipants[i+4].getName() == enemies[2].getName())
                                pizzaheadTexture.render(getRenderer(), charRendering[i+4], 400);
                            if (combatParticipants[i+4].getName() == enemies[3].getName())
                                carlTexture.render(getRenderer(), charRendering[i+4], 400); 
                        }
                    }
                }
                // Boss fight encounter
                else 
                {
                    // Render players at their positions
                    for (int i = 0; i < 4; i++)
                    {
                        if (combatParticipants[i].isAlive())
                        {
                            if (combatParticipants[i].getName() == flute.getName())
                                flutistTexture.render(getRenderer(), charRendering[i], 400);
                            if (combatParticipants[i].getName() == drum.getName())
                                drummerTexture.render(getRenderer(), charRendering[i], 400);
                            if (combatParticipants[i].getName() == bass.getName())
                                bassistTexture.render(getRenderer(), charRendering[i], 400);
                            if (combatParticipants[i].getName() == conductor.getName())
                                conductorTexture.render(getRenderer(), charRendering[i], 400);
                        }
                    }
                
                    // Render boss at their positions
                    for (int i = 0; i < 4; i++)
                    {
                        if (combatParticipants[i+4].isAlive())
                        {
                            if (combatParticipants[i+4].getName() == enemies[0].getName())
                                bossTexture.render(getRenderer(), charRendering[i+4], 400);
                            if (combatParticipants[i+4].getName() == enemies[1].getName())
                                enemyBellTexture.render(getRenderer(), charRendering[i+4], 400);
                            if (combatParticipants[i+4].getName() == enemies[2].getName())
                                pizzaheadTexture.render(getRenderer(), charRendering[i+4], 400);
                            if (combatParticipants[i+4].getName() == enemies[3].getName())
                                carlTexture.render(getRenderer(), charRendering[i+4], 400); 
                        }
                    }
                }
                
                // Battle notification textbox rendering
                if (STATE_timerStarted && timer->deltaTime() < STATE_timerCount)
                {  
                    battleNotification.render(getRenderer());
                }
                if (timer->deltaTime() > STATE_timerCount)
                {
                    STATE_timerStarted = false; 
                }
                // Damage animation popup sprite rendering
                if(STATE_timerAnimationStarted && timer->deltaTime() < STATE_timerAnimationCount)
                {
                    getHitEffect.setAlpha(alphaDamageON);
                    getHitEffect.render(getRenderer(), whichTargetXValueForDamageAnimation, 398);
                }
                if (timer->deltaTime() > STATE_timerAnimationCount)
                {
                    getHitEffect.setAlpha(alphaDamageOFF);
                    STATE_timerAnimationStarted = false; 
                }
                // Target Boxes display for each action
                if (STATE_combatSelectedOption == "Attack")
                {
                    STATE_enemyTimerStarted = true;
                    STATE_enemyTimerCount = timer->deltaTime() + 3;
                    for (int i = 0; i < combatParticipants.size();i++)
                    {
                        if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                        {
                            validMoves = combatParticipants[i].getValidMoves(ATTACK, combatParticipants[i].getParticipantsIndex(),combatParticipants);
                            for (auto target: validMoves[currTarget])
                            {
                                targetTexture.render(getRenderer(), charBoxes[target].x, charBoxes[target].y);
                                
                            }
                        }
                    }
                    
                }
                if (STATE_combatSelectedOption == "Buff")
                {
                    STATE_enemyTimerStarted = true;
                    STATE_enemyTimerCount = timer->deltaTime() + 3;
                    for (int i = 0; i < combatParticipants.size();i++)
                    {
                        if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                        {
                            validMoves = combatParticipants[i].getValidMoves(BUFF, combatParticipants[i].getParticipantsIndex(),combatParticipants);
                            for (auto target: validMoves[currTarget])
                            {
                                targetTexture.render(getRenderer(), charBoxes[target].x, charBoxes[target].y);
                                
                            }
                        }
                    }
                    
                    
                }

                if (STATE_combatSelectedOption == "Debuff")
                {
                    STATE_enemyTimerStarted = true;
                    STATE_enemyTimerCount = timer->deltaTime() + 3;
                    SDL_SetRenderDrawColor(getRenderer(), 150, 0, 0, 255);
                    for (int i = 0; i < combatParticipants.size();i++)
                    {
                        if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                        {
                            validMoves = combatParticipants[i].getValidMoves(DEBUFF, combatParticipants[i].getParticipantsIndex(),combatParticipants);
                            for (auto target: validMoves[currTarget])
                            {
                                targetTexture.render(getRenderer(), charBoxes[target].x, charBoxes[target].y);
                            }
                        }
                    }
                }

                if (STATE_combatSelectedOption == "Move")
                {
                    STATE_enemyTimerStarted = true;
                    STATE_enemyTimerCount = timer->deltaTime() + 3;
                    SDL_SetRenderDrawColor(getRenderer(), 0, 150, 0, 255);
                    for (int i = 0; i < combatParticipants.size();i++)
                    {
                        if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                        {
                            validMoves = combatParticipants[i].getValidMoves(MOVE, combatParticipants[i].getParticipantsIndex(),combatParticipants);
                            for (auto target: validMoves[currTarget])
                            {
                                targetTexture.render(getRenderer(), charBoxes[target].x, charBoxes[target].y);
                            }
                        }
                    }
                }
                
                SDL_SetRenderDrawColor(getRenderer(), 0, 170, 0, 255);
                
                int currPlayer;
                for (int i = 0; i < combatParticipants.size(); i++)
                {
                    if (combatParticipants[i].getName()==roundOrder[currOrderNum])
                    {
                        currPlayer = combatParticipants[i].getParticipantsIndex();
                    }
                }
                
                currPlayerTexture.render(getRenderer(), charBoxes[currPlayer].x, charBoxes[currPlayer].y);

                // Update order display
                for (int i = 0; i < combatParticipants.size(); i++)
                {
                    for (int j = 0; j < roundOrder.size(); j++)
                    {
                        if (combatParticipants[i].getName() == roundOrder[j])
                        {
                            // If character in round order is alive, write its name in the order box
                            if (combatParticipants[i].isAlive())
                                orderBoxes[j].changeText(roundOrder[j]);
                            // If character in round order is dead, overwrite its name with an empty string
                            else 
                                orderBoxes[j].changeText("");
                        }
                    }
                }
                // If round order size less than order boxes size, replace dead character names with empty string
                for (int i = roundOrder.size(); i < orderBoxes.size(); i++)
                {
                    orderBoxes[i].changeText("");
                }
                
                
                for (int i = currOrderNum; i < orderBoxes.size(); i++)
                {
                    orderBoxes[i].render(getRenderer());
                }
                
                //Update health and position text renderings
                for (int i = 0; i < 4; i++)
                {
                    combatStatusRow[i].changeText(combatParticipants[i].getName());
                    combatStatusRow[i+4].changeText(prefixHP + std::to_string(combatParticipants[i].getHp()));
                }
               
                for (auto el: combatStatusRow)
                {
                    el.render(getRenderer());
                } 

                //Screen transition turning background opaque 
                if(STATE_postTransition == true)
                {
                    alphaValueScreenTransition -= 5;
                    blackScreenTransition.setAlpha(alphaValueScreenTransition);
                    if(alphaValueScreenTransition == 0)
                    {
                        STATE_postTransition = false;  
                    }
                    blackScreenTransition.render(getRenderer(), 0, 0);
                }
                //Screen transition turning background transparent
                else if(STATE_preTransition == true)
                {
                    alphaValueScreenTransition += 5;
                    blackScreenTransition.setAlpha(alphaValueScreenTransition);
                    if(alphaValueScreenTransition == 255)
                    {
                        STATE_preTransition = false;
                    }
                    blackScreenTransition.render(getRenderer(), 0, 0);
                }
                break;
            }
            case STATUS_MENU:
            {
                std::string statMenuDisplayStr;

                statusBg.render(getRenderer(), 0, 0);

            
                ///////Background under the text////////
                SDL_SetRenderDrawBlendMode(getRenderer(), SDL_BLENDMODE_BLEND); 
                SDL_Rect StatMenuBackground1 = {50, 95, 861, 100};
                SDL_Color StatMenuColor1 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor1.r, StatMenuColor1.g, StatMenuColor1.b, 128);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground1);

                SDL_Rect StatMenuBackground2 = {50, 245, 861, 100};
                SDL_Color StatMenuColor2 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor2.r, StatMenuColor2.g, StatMenuColor2.b, 128);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground2);

                SDL_Rect StatMenuBackground3 = {50, 395, 861, 100};
                SDL_Color StatMenuColor3 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor3.r, StatMenuColor3.g, StatMenuColor3.b, 128);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground3);

                SDL_Rect StatMenuBackground4 = {50, 545, 861, 100};
                SDL_Color StatMenuColor4 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor4.r, StatMenuColor4.g, StatMenuColor4.b, 128);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground4);
                SDL_SetRenderDrawBlendMode(getRenderer(), SDL_BLENDMODE_NONE); 
                
                ///////End Background under the text////////

                ///////Display Names-Stats////////
                
                for (int i = 0; i < 4; i++)
                {
                    characterStatsHP[i].render(getRenderer());
                    characterStatsSpeed[i].render(getRenderer());
                    characterStatsHit[i].render(getRenderer());
                    characterStatsArmor[i].render(getRenderer());
                    characterStatsDodge[i].render(getRenderer());
                    characterStatsItemMod[i].render(getRenderer());

                }
                
                for (int statLoopTemp = 0; statLoopTemp < statMenuConductorRow.size(); statLoopTemp++)
                {
                    statMenuBassRow[statLoopTemp].render(getRenderer());
                    statMenuDrumRow[statLoopTemp].render(getRenderer());
                    statMenuFluteRow[statLoopTemp].render(getRenderer());
                    statMenuConductorRow[statLoopTemp].render(getRenderer());
                } 

                ///////Display Names-Stats//////// 

                //Screen transition turning background opaque 
                if(STATE_postTransition == true)
                {
                    alphaValueScreenTransition -= 5;
                    blackScreenTransition.setAlpha(alphaValueScreenTransition);
                    if(alphaValueScreenTransition == 0)
                    {
                        STATE_postTransition = false;  
                    }
                    blackScreenTransition.render(getRenderer(), 0, 0);
                }
                //Screen transition turning background transparent
                else if(STATE_preTransition == true)
                {
                    alphaValueScreenTransition += 5;
                    blackScreenTransition.setAlpha(alphaValueScreenTransition);
                    if(alphaValueScreenTransition == 255)
                    {
                        STATE_preTransition = false;
                    }
                    blackScreenTransition.render(getRenderer(), 0, 0);
                }
                
                
                break;
            }
            // Victory screen rendering
            case WIN:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 150, 0, 255);
                SDL_RenderClear(getRenderer());

                congradsWinText.render(getRenderer());
                moveOnWin.render(getRenderer());

                break;
            }
            // Defeat screen rendering
            case DEFEAT:
            {
                
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());

                congradsYouDiedText.render(getRenderer());
                moveOnLoss.render(getRenderer());

                break;
            }   
        }

        //Update screen
        SDL_RenderPresent(getRenderer());
       
        //////END SCREEN RENDERING///////

    }
}
