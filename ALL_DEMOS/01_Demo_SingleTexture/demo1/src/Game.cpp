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
    BaseCharacter conductor = BaseCharacter("Conductor", 30, 3, 3, 0, 3, 3, 3,false);
    BaseCharacter drum = BaseCharacter("Drummer", 50, 2, 1, 0, 3, 3, 3,false);
    BaseCharacter flute = BaseCharacter("Flutist", 20, 6, 1, 0, 3, 3, 3,false);
    BaseCharacter bass = BaseCharacter("Bassist", 60, 1, 3, 0, 3, 3, 3,false);
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
    
    /////////// COMBAT SCREEN THINGS //////////
    std::string prefixHP = "HP: ";

    

    TextBox combatStatFlute = TextBox("flute", 25, 50, 630, 150, 30, Font::inter, Color::black, Color::cyan);
    TextBox combatStatFluteHP = TextBox(prefixHP + std::to_string(flute.getHp()), 25, 50, 660, 150, 30, Font::inter, Color::black, Color::cyan);
    
    TextBox combatStatConductor = TextBox("conductor", 25, 200, 630, 150, 30, Font::inter, Color::black, Color::cyan);
    TextBox combatStatConductorHP = TextBox(prefixHP + std::to_string(conductor.getHp()), 25, 200, 660, 150, 30, Font::inter, Color::black, Color::cyan);

    TextBox combatStatBass = TextBox("bass", 25, 350, 630, 150, 30, Font::inter, Color::black, Color::cyan);
    TextBox combatStatBassHP = TextBox(prefixHP + std::to_string(bass.getHp()), 25, 350, 660, 150, 30, Font::inter, Color::black, Color::cyan);
    
    TextBox combatStatDrum = TextBox("drum", 25, 500, 630, 150, 30, Font::inter, Color::black, Color::cyan);
    TextBox combatStatDrumHP = TextBox(prefixHP + std::to_string(drum.getHp()), 25, 500, 660, 150, 30, Font::inter, Color::black, Color::cyan);

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
    const int TEST_CHAR_SHEET_COLS = 4;
    const int ANIMATION_FRAME_COUNT = 4;
    int currFrameNum = 0;
    int currRectNum = 0;
    
    std::vector<SDL_Rect> spriteClipped(ANIMATION_FRAME_COUNT);
    std::unordered_map<TextureWrapper*, int> textureFrameCount = {
        {&characterInMapTexture, ANIMATION_FRAME_COUNT}
    };
    std::unordered_map<TextureWrapper*, std::vector<SDL_Rect>> texturePtrToSpriteMap = {
        {&characterInMapTexture, spriteClipped}
    };

    didClip = clipSheet(TEST_CHAR_SHEET_ROWS, TEST_CHAR_SHEET_COLS, 51, 107, ANIMATION_FRAME_COUNT, spriteClipped);
    if (!didClip)
    {
        setToQuit();
    }
    */
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
        TextBox temp = TextBox("", 25, 750, 30+(i*50), 200, 30, Font::raleway, Color::black, Color::gray);
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

    ///battle damage/death 
    int alphaDamageON = 255;
    int alphaDamageOFF = 0;
    int alphaDeathON = 255;
    int alphaDeathOFF = 0;
    int whichTargetXValueForDamageAnimation = 0;
    std::vector<int> damageTakenPosition = {0, 90, 180, 270, 360, 450, 540, 630};

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
    TextBox baseName = TextBox("   Bassist", 40, 50, 95, 50, 30, Font::roboto, Color::blue, Color::cyan);
    TextBox bassHPName = TextBox(std::to_string(bass.getHp()) + "-", 40, 130, 145, 40, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox bassMaxHPName = TextBox(std::to_string(bass.getMaxHp()), 40, 185, 145, 40, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox bassSpeedName = TextBox(std::to_string(bass.getSpeed()), 40, 315, 148, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox bassHitName = TextBox(std::to_string(bass.getHit()), 40, 435, 148, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox bassArmorName = TextBox(std::to_string(bass.getArmor()), 40, 550, 148, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox bassDodgeName = TextBox(std::to_string(bass.getDodgeModifier()), 40, 670, 148, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox bassItemModName = TextBox(std::to_string(bass.getItemModifier()) + tempItemModAddStat, 40, 750, 148, 30, 50, Font::roboto, Color::blue, Color::cyan);
    
    std::vector<TextBox> statMenuBassRow { baseName , bassHPName, bassMaxHPName, bassSpeedName, bassHitName, bassArmorName, bassDodgeName, bassItemModName };
    ///////End Base Stats/////////
    ///////Drum Stats/////////
    TextBox drumName = TextBox(" Drummer", 40, 50, 245, 50, 30, Font::roboto, Color::blue, Color::cyan);
    TextBox drumHPName = TextBox(std::to_string(drum.getHp()) + "-", 40, 130, 295, 40, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox drumMaxHPName = TextBox(std::to_string(drum.getMaxHp()), 40, 185, 295, 40, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox drumSpeedName = TextBox(std::to_string(drum.getSpeed()), 40, 315, 298, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox drumHitName = TextBox(std::to_string(drum.getHit()), 40, 435, 298, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox drumArmorName = TextBox(std::to_string(drum.getArmor()), 40, 550, 298, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox drumDodgeName = TextBox(std::to_string(drum.getDodgeModifier()), 40, 670, 298, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox drumItemModName = TextBox(std::to_string(drum.getItemModifier()) + tempItemModAddStat, 40, 750, 298, 30, 50, Font::roboto, Color::blue, Color::cyan);

    std::vector<TextBox> statMenuDrumRow { drumName , drumHPName, drumMaxHPName, drumSpeedName, drumHitName, drumArmorName, drumDodgeName, drumItemModName };
    ///////End Drum Stats/////////
    ///////Flute Stats/////////
    TextBox fluteName = TextBox("    Flutist", 40, 50, 395, 50, 30, Font::roboto, Color::blue, Color::cyan);
    TextBox fluteHPName = TextBox(std::to_string(flute.getHp()) + "-", 40, 130, 445, 40, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox fluteMaxHPName = TextBox(std::to_string(flute.getMaxHp()), 40, 185, 445, 40, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox fluteSpeedName = TextBox(std::to_string(flute.getSpeed()), 40, 315, 448, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox fluteHitName = TextBox(std::to_string(flute.getHit()), 40, 435, 448, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox fluteArmorName = TextBox(std::to_string(flute.getArmor()), 40, 550, 448, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox fluteDodgeName = TextBox(std::to_string(flute.getDodgeModifier()), 40, 670, 448, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox fluteItemModName = TextBox(std::to_string(flute.getItemModifier()) + tempItemModAddStat, 40, 750, 448, 30, 50, Font::roboto, Color::blue, Color::cyan);

    std::vector<TextBox> statMenuFluteRow { fluteName , fluteHPName, fluteMaxHPName, fluteSpeedName, fluteHitName, fluteArmorName, fluteDodgeName, fluteItemModName };
    ///////End Flute Stats/////////
    ///////Conductor Stats/////////
    TextBox conductorName = TextBox("Conductor", 40, 50, 545, 50, 30, Font::roboto, Color::blue, Color::cyan);
    TextBox conductorHPName = TextBox(std::to_string(conductor.getHp()) + "-", 40, 130, 595, 40, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox conductorMaxHPName = TextBox(std::to_string(conductor.getMaxHp()), 40, 185, 595, 40, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox conductorSpeedName = TextBox(std::to_string(conductor.getSpeed()), 40, 315, 598, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox conductorHitName = TextBox(std::to_string(conductor.getHit()), 40, 435, 598, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox conductorArmorName = TextBox(std::to_string(conductor.getArmor()), 40, 550, 598, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox conductorDodgeName = TextBox(std::to_string(conductor.getDodgeModifier()), 40, 670, 598, 30, 50, Font::roboto, Color::blue, Color::cyan);
    TextBox conductorItemModName = TextBox(std::to_string(conductor.getItemModifier()) + tempItemModAddStat, 40, 750, 598, 30, 50, Font::roboto, Color::blue, Color::cyan);

    std::vector<TextBox> statMenuConductorRow { conductorName , conductorHPName, conductorMaxHPName, conductorSpeedName, conductorHitName, conductorArmorName, conductorDodgeName, conductorItemModName };
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
        TextBox statsHP = TextBox(" HP", 40, 50, (startBoxWidth + (i*150) + 50),
         50, 20, Font::roboto, Color::blue, Color::cyan);
        characterStatsHP.push_back(statsHP);
        
        TextBox statsSpeed = TextBox("Speed", 35, 276, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan);
        characterStatsSpeed.push_back(statsSpeed);

        TextBox statsHit = TextBox("   Hit   ", 35, 393, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan);
        characterStatsHit.push_back(statsHit);

        TextBox statsArmor = TextBox("Armor", 35, 510, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan);
        characterStatsArmor.push_back(statsArmor);
        
        TextBox statsDodge = TextBox("Dodge", 35, 627, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan);
        characterStatsDodge.push_back(statsDodge);

        TextBox statsItemMod = TextBox("Item Mod", 35, 748, (startBoxWidth + (i*150)),
         100, 50, Font::roboto, Color::blue, Color::cyan);
        characterStatsItemMod.push_back(statsItemMod);
        
        
    }   
    ///////End Display Stat Names/////////
    
    ////// END CHARACTER STATS //////

    //BaseItem test("some item", "normal", 5);

    //double degrees = 0;
    //SDL_RendererFlip flipType = SDL_FLIP_NONE;
    int STATUS_show = 1;
    float STATE_angle = 0.0;
    SDL_Event event;
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
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_1:
                    {
                        STATUS_show = 1;
                        drummerTexture.setAlpha(255);
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
                        STATUS_show = 1;
                        drummerTexture.setAlpha(150);
                        break;
                    }
                    case SDLK_9:
                    {
                        STATUS_show = 1;
                        drummerTexture.setAlpha(60);
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
            switch (screen)
            {
                case INTRO:
                { 
                    /*
                    introMenu.onInput(event, SelectMusic, STATE_introSelectedOption);
                    if (STATE_introSelectedOption != "NONE")
                    {
                        if (STATE_introSelectedOption == "New Game")
                        {
                            
                            STATE_newGameSelected = true;
                            STATE_gameOver = false; 
                            STATE_mapScreenOpenForTransition = true;
                        }
                    }*/
                    
                    break;
                }
                case WIN:
                {
                    if (event.type == SDL_KEYDOWN)
                    {
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_RETURN:
                            {
                                
                                screen = MAP;
                                break;
                            }
                        }
                    } 
                    
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
                SDL_Point point = {200, 200};
                if (STATUS_show == 1)
                    drummerTexture.render(getRenderer(), 0, 0, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
                if (STATUS_show == 2)
                    drummerTexture.render(getRenderer(), 0, 0, nullptr, 0.0, nullptr, SDL_FLIP_HORIZONTAL);
                if (STATUS_show == 3)
                    drummerTexture.render(getRenderer(), 0, 0, nullptr, 0.0, nullptr, SDL_FLIP_VERTICAL);
                if (STATUS_show == 4)
                    drummerTexture.render(getRenderer(), 0, 0, nullptr, 45.0, nullptr, SDL_FLIP_VERTICAL);
                if (STATUS_show == 5)
                    drummerTexture.render(getRenderer(), 0, 0, nullptr, 125.0, nullptr, SDL_FLIP_VERTICAL);
                if (STATUS_show == 6)
                {
                    drummerTexture.render(getRenderer(), 0, 0, nullptr, 0.0, &point, SDL_FLIP_NONE);
                    bassistTexture.render(getRenderer(), 200, 200); 
                }
                if (STATUS_show == 7)
                {
                    drummerTexture.render(getRenderer(), 0, 0, nullptr, STATE_angle, &point, SDL_FLIP_NONE);
                    bassistTexture.render(getRenderer(), 200, 200); 
                }
                
                
                
                /*introMenu.render(getRenderer()); 

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
                */
                break;
            }
            case MAP:
            {
                //write macro for this eventually
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

                // render players at their positions
                for (int i = 0; i < 4; i++)
                {
                    if (combatParticipants[i].isAlive())
                    {
                        if (combatParticipants[i].getName() == "Flutist")
                            flutistTexture.render(getRenderer(), charRendering[i], 400);
                        if (combatParticipants[i].getName() == "Drummer")
                            drummerTexture.render(getRenderer(), charRendering[i], 400);
                        if (combatParticipants[i].getName() == "Bassist")
                            bassistTexture.render(getRenderer(), charRendering[i], 400);
                        if (combatParticipants[i].getName() == "Conductor")
                            flutistTexture.render(getRenderer(), charRendering[i], 400);
                    }
                }
                
                // render enemies at their positions
                for (int i = 0; i < 4; i++)
                {
                    if (combatParticipants[i+4].isAlive())
                    {
                        if (combatParticipants[i+4].getName() == enemies[0].getName())
                            linebackerTexture.render(getRenderer(), charRendering[i+4], 400);
                        if (combatParticipants[i+4].getName() == enemies[1].getName())
                            drummerTexture.render(getRenderer(), charRendering[i+4], 400);
                        if (combatParticipants[i+4].getName() == enemies[2].getName())
                            bassistTexture.render(getRenderer(), charRendering[i+4], 400);
                        if (combatParticipants[i+4].getName() == enemies[3].getName())
                            flutistTexture.render(getRenderer(), charRendering[i+4], 400); 
                    }
                }
                
                if (STATE_timerStarted && timer->deltaTime() < STATE_timerCount)
                {  
                    battleNotification.render(getRenderer());
                }
                if (timer->deltaTime() > STATE_timerCount)
                {
                    STATE_timerStarted = false; 
                }

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
                
                //---

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

                conductorStats.render(getRenderer());
                testToHitStats.render(getRenderer());
                SDL_Rect x = {20, 620, 226, 100};
                SDL_Color y = Color::gray;
                SDL_SetRenderDrawColor(getRenderer(), y.r, y.g, y.b, 0);
                SDL_RenderFillRect(getRenderer(), &x);
                SDL_Rect a = {666, 620, 226, 100};
                SDL_Color b = Color::gray;
                SDL_SetRenderDrawColor(getRenderer(), b.r, b.g, b.b, 0);
                SDL_RenderFillRect(getRenderer(), &a);
                SDL_Rect box1 = {246, 620, 84, 100};
                SDL_Color color1 = Color::gray;
                SDL_SetRenderDrawColor(getRenderer(), color1.r, color1.g, color1.b, 0);
                SDL_RenderFillRect(getRenderer(), &box1);
                SDL_Rect box2 = {330, 620, 84, 100};
                SDL_Color color2 = Color::navy;
                SDL_SetRenderDrawColor(getRenderer(), color2.r, color2.g, color2.b, 0);
                SDL_RenderFillRect(getRenderer(), &box2);
                SDL_Rect box3 = {414, 620, 84, 100};
                SDL_Color color3 = Color::black;
                SDL_SetRenderDrawColor(getRenderer(), color3.r, color3.g, color3.b, 0);
                SDL_RenderFillRect(getRenderer(), &box3);
                SDL_Rect box4 = {498, 620, 84, 100};
                SDL_Color color4 = Color::olive;
                SDL_SetRenderDrawColor(getRenderer(), color4.r, color4.g, color4.b, 0);
                SDL_RenderFillRect(getRenderer(), &box4);
                SDL_Rect box5 = {582, 620, 84, 100};
                SDL_Color color5 = Color::white;
                SDL_SetRenderDrawColor(getRenderer(), color5.r, color5.g, color5.b, 0);
                SDL_RenderFillRect(getRenderer(), &box5);
                conductorStats.render(getRenderer());
                drummerStats.render(getRenderer());
                for (auto stat: conductorStatsRow)
                {
                    stat.render(getRenderer());
                }
                for (auto stat: drummerStatsRow)
                {
                    stat.render(getRenderer());
                }
                //TextBox conductorStats = TextBox("Conductor", 40, 20, 620, 100, 100, Font::sono, Color::white, Color::black);
                //TextBox testToHitStats = TextBox("x+ to hit", 40, 690, 620, 100, 100, Font::sono, Color::white, Color::black);  
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
                
                SDL_Rect StatMenuBackground1 = {50, 95, 861, 100};
                SDL_Color StatMenuColor1 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor1.r, StatMenuColor1.g, StatMenuColor1.b, 0);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground1);

                SDL_Rect StatMenuBackground2 = {50, 245, 861, 100};
                SDL_Color StatMenuColor2 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor2.r, StatMenuColor2.g, StatMenuColor2.b, 0);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground2);

                SDL_Rect StatMenuBackground3 = {50, 395, 861, 100};
                SDL_Color StatMenuColor3 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor3.r, StatMenuColor3.g, StatMenuColor3.b, 0);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground3);

                SDL_Rect StatMenuBackground4 = {50, 545, 861, 100};
                SDL_Color StatMenuColor4 = Color::cyan;
                SDL_SetRenderDrawColor(getRenderer(), StatMenuColor4.r, StatMenuColor4.g, StatMenuColor4.b, 0);
                SDL_RenderFillRect(getRenderer(), &StatMenuBackground4);

                ///////End Background under the text////////

                ///////Lines that split the screen//////////
                
                SDL_Rect splitLine1 = {239, 95, 3, 550};
                SDL_Color splitLineColor1 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor1.r, splitLineColor1.g, splitLineColor1.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine1);

                SDL_Rect splitLine7 = {252, 95, 3, 550};
                SDL_Color splitLineColor7 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor7.r, splitLineColor7.g, splitLineColor7.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine7);
                
                SDL_Rect splitLine2 = {265, 95, 3, 550};
                SDL_Color splitLineColor2 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor2.r, splitLineColor2.g, splitLineColor2.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine2);
                
                SDL_Rect splitLine3 = {385, 95, 3, 550};
                SDL_Color splitLineColor3 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor3.r, splitLineColor3.g, splitLineColor3.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine3);

                SDL_Rect splitLine4 = {499, 95, 3, 550};
                SDL_Color splitLineColor4 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor4.r, splitLineColor4.g, splitLineColor4.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine4);

                SDL_Rect splitLine5 = {617, 95, 3, 550};
                SDL_Color splitLineColor5 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor5.r, splitLineColor5.g, splitLineColor5.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine5);

                SDL_Rect splitLine6 = {737, 95, 3, 550};
                SDL_Color splitLineColor6 = Color::teal;
                SDL_SetRenderDrawColor(getRenderer(), splitLineColor6.r, splitLineColor6.g, splitLineColor6.b, 0);
                SDL_RenderFillRect(getRenderer(), &splitLine6);
               
                ///////End Lines that split the screen//////////

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
                
                for (int statLoopTemp = 0; statLoopTemp < 8; statLoopTemp++)
                {
                    statMenuBassRow[statLoopTemp].render(getRenderer());
                    statMenuDrumRow[statLoopTemp].render(getRenderer());
                    statMenuFluteRow[statLoopTemp].render(getRenderer());
                    statMenuConductorRow[statLoopTemp].render(getRenderer());
                } 

                ///////Display Names-Stats//////// 

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
            case WIN:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 150, 0, 255);
                SDL_RenderClear(getRenderer());

                TextBox congrads = TextBox("Congratulations  ", 100, 100, 100, 500, 200, Font::roboto, Color::black, Color::darkGreen);
                congrads.render(getRenderer());
                TextBox moveOn = TextBox("Press ''Enter'' to continue", 65, 100, 550, 500, 200, Font::roboto, Color::black, Color::darkGreen);
                moveOn.render(getRenderer());

                break;
            }
            case DEFEAT:
            {
                
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());

                TextBox congrads = TextBox("  You Died", 100, 200, 100, 500, 200, Font::roboto, Color::maroon, Color::black);
                congrads.render(getRenderer());

                break;
            }
            
        }

        
        //Update screen
        SDL_RenderPresent(getRenderer());
       
 
    }
}
