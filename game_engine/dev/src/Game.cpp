#include "Game.h"
#include "Game/Utility/isTeamAlive.h"
#include "Game/Utility/setRoundTurns.h"

EscapeFromCapstone::EscapeFromCapstone(Uint32 flags, 
                       const char* title, 
                       int x, int y, 
                       int w, int h) : 
    Phoenix(flags, title, x, y, w, h){}

void EscapeFromCapstone::runGameLoop()
{

    ////////// START CHARACTER INIT ////////
    BasePlayer conductor = BasePlayer("Conductor", 30, 3, 3, 0, 3, 3, 3);
    BasePlayer drum = BasePlayer("Drummer", 50, 2, 1, 0, 3, 3, 3);
    BasePlayer flute = BasePlayer("Flutist", 20, 6, 1, 0, 3, 3, 3);
    Bass bass = Bass("Bassist", 60, 1, 3, 0, 3, 3, 3);
    std::vector<BaseCharacter> playerTeam{flute, conductor, bass, drum};
    std::vector<BaseCharacter> enemies;
    std::vector<BaseCharacter> combatParticipants;

    ///////// END CHARACTER INIT //////

    

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
    BaseMenu introMenu = BaseMenu(100, 140, 400, 100, 100,  
                                  introOptions, 
                                  Font::raleway, 
                                  Color::red, 
                                  getRenderer()
    );

    const std::vector<std::string> combatOptionsStrings = {
        "Attack",
        "Buff",
        "Debuff",
        "Move"
    };

    SDL_Color combatMenuColor = { 0, 0, 255, 255 };
    BaseMenu combatMenu = BaseMenu(25, 520, 200, 50, 100, 
                                   combatOptionsStrings, 
                                   Font::raleway, 
                                   Color::red, 
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
    std::string STATE_introSelectedOption = "NONE";
    std::string STATE_helpMenuSelectedOption = "NONE";
    std::string STATE_combatSelectedOption = "NONE";

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
    //add sprite sheet here
    std::unordered_map<TextureWrapper*, std::string> textureFilePaths = {
        {&tileTexture, "../../assets/image/sprite_tiled.png"},
        {&characterInMapTexture, "../../assets/image/dot.bmp"},
        {&debugControllerTexture, "../../assets/image/dot.bmp"},
        {&characterTestTexture, "../../assets/image/char.png"}
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
    const int TILE_COUNT = 192;
    const int TILE_LENGTH = 80;
    const int TILE_TYPE_COUNT = 12;

    std::vector<Tile*> tileMap(TILE_COUNT);
    std::vector<SDL_Rect> tilesClipped(TILE_TYPE_COUNT);// this is the total tiles
    std::map<std::pair<int, int>, TileType> coordinateToTileTypeMap;
    std::map<std::pair<int, int>, std::string> coordinateToEventTypeMap;

    bool didTilesLoad = loadTiles(tileMap, 
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

    const int TILE_SHEET_ROWS = 3;
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
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0,10); 

    for (auto& [coordinate, event]: coordinateToEventTypeMap)
    {
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
        SDL_Rect temp = {(50+(i*100)), 200, 64, 64};
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
        SDL_Rect temp = {750, 330+(i*50), 200, 30};
        orderBoxes[i] = temp; 
    }

    SDL_Rect orderTitleBox = {750, 280, 200, 30};
    std::string orderTitle = "ORDER        ";
    int currOrderNum = 0;
    std::vector<BaseCharacter*> roundOrder;

    ////////// END BATTLE ORDER INIT ///////////////

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
    std::vector<std::vector<int>> validMoves = {
        {1, 3},
        {2, 4},
        {3, 5},
        {4, 6},
        {5, 7}
    };
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

    BaseMenu sandboxMenu = BaseMenu(750, 500, 200, 50, 100, 
                                   sandboxOptionsStrings, 
                                   Font::openSans, 
                                   Color::white, 
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
    TextBox statBass = TextBox("bass     ", 500, 50, 630, 150, 30);
    TextBox statBassHP = TextBox("Hp:       ", 500, 50, 660, 150, 30);

    TextBox statDrum = TextBox("drum     ", 500, 200, 630, 150, 30);
    TextBox statDrumHP = TextBox("Hp:       ", 500, 200, 660, 150, 30);

    TextBox statFlute = TextBox("flute   ", 500, 350, 630, 150, 30);
    TextBox statFluteHP = TextBox("Hp:        ", 500, 350, 660, 150, 30);
    
    TextBox statConductor = TextBox("conductor", 500, 500, 630, 150, 30);
    TextBox statConductorHP = TextBox("Hp:          ", 500, 500, 660, 150, 30);

    std::vector<TextBox> statusRow{statBass, statDrum, statFlute, statConductor};


    ///////// END SANDBOX ///////////
    

    //double degrees = 0;
    //SDL_RendererFlip flipType = SDL_FLIP_NONE;

    SDL_Event event;
    while (!getQuit())
    {
        
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
                        screen = COMBAT;
                        break;
                    }
                    case SDLK_4:
                    {
                        screen = SANDBOX;
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
                    //this has a bug where movement
                    //keeps being read if key is not unpressed
                    debugController.onInput(event);
                    characterController.onInput(event, nextMapEvent, STATE_mapEventboxOpen, coordinateToTileTypeMap, coordinateToEventTypeMap);
                    if (nextMapEvent == "BATTLE")
                    {
                        //init enemy characters
                        BaseCharacter e1 = BaseCharacter("ConeheadAlpha", 10, 2, 1, 0, 3, 3, 3, true);
                        BaseCharacter e2 = BaseCharacter("ConeheadBeta", 10, 6, 1, 0, 3, 3, 3, true);
                        BaseCharacter e3 = BaseCharacter("ConeheadKappa", 10, 2, 1, 0, 3, 3, 3, true);
                        BaseCharacter e4 = BaseCharacter("Carl", 20, 0, 1, 0, 3, 3, 3, true);
                        //normally this will just get enemies from a randomly selected "PACK"
                        std::vector<BaseCharacter> temp{e1, e2, e3, e4};
                        //this might not be necessary
                        enemies = temp;
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

                    }
                    if (event.type == SDL_KEYDOWN)
                    {
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_RETURN:
                            {
                                STATE_mapEventboxOpen = false;
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

                    if (STATE_combatSelectedOption == "NONE" && !STATE_combatMenuTargetSelected)
                        combatMenu.onInput(event, SelectMusic, STATE_combatSelectedOption);

                    if (STATE_combatMenuTargetSelected)
                    {
                        //do thing;
                        if (STATE_combatSelectedOption == "Attack")
                        {
                            //WAS the round order properly set??
                            STATE_combatSelectedOption = "NONE";
                            combatParticipants = roundOrder[currOrderNum]->doAction(ATTACK, validMoves[currTarget], combatParticipants);
                            //TODO Set 8 to be the current size of alive characters (player and enemies) 'livingCharacters'
                            currOrderNum = (currOrderNum + 1) % 8;
                        }
                        STATE_combatMenuTargetSelected = false;
                        currTarget = 0;
                    }
                    case SANDBOX:
                    {
                        break;
                    }
                   
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
                debugController.move(1280, 960);
                characterController.move(1280, 960);
                //debugController.centerScreen(camera);
                characterController.centerScreen(camera);
                for(int i = 0; i < tileMap.size(); i++)
                {
                    tileMap[i]->render(getRenderer(), tileTexture, camera, tilesClipped);
                }
                debugController.render(getRenderer(), camera, debugControllerTexture);
                characterController.render(getRenderer(), camera, characterInMapTexture);
                if (STATE_mapEventboxOpen)
                {
                    SDL_SetRenderDrawColor(getRenderer(), 0, 0, 200, 255);
                    SDL_RenderFillRect(getRenderer(), &mapEventBox);
                    SDL_Color textColor = { 255, 0, 0, 255 };
                    std::stringstream mapEventStream;
                    mapEventStream << nextMapEvent;
                    SDL_Surface* surfaceTesting = TTF_RenderText_Solid(font, mapEventStream.str().c_str(), textColor); //ttf surface  
                    SDL_Texture* textureTesting = SDL_CreateTextureFromSurface(getRenderer(), surfaceTesting); 
                    SDL_RenderCopy(getRenderer(), textureTesting, nullptr, &mapEventBox); 
                }
                break;
            } 
            case COMBAT:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());
                combatMenu.render(getRenderer());
                
                
                SDL_Rect* currFrameRect = &spriteClipped[currFrameNum];

                //will be for loop (eventually)
                characterTestTexture.render(getRenderer(), 50, 100, currFrameRect);
                characterTestTexture.render(getRenderer(), 150, 100, currFrameRect);
                characterTestTexture.render(getRenderer(), 250, 100, currFrameRect);
                characterTestTexture.render(getRenderer(), 350, 100, currFrameRect);
                characterTestTexture.render(getRenderer(), 450, 100, currFrameRect);
                characterTestTexture.render(getRenderer(), 550, 100, currFrameRect);
                characterTestTexture.render(getRenderer(), 650, 100, currFrameRect);
                characterTestTexture.render(getRenderer(), 750, 100, currFrameRect);

                SDL_SetRenderDrawColor(getRenderer(), 0, 170, 0, 255);
                SDL_RenderFillRect(getRenderer(), &charBoxes[0]);

                //hpBoxes
                for (int i = 0;  i < hpBoxes.size(); i++)
                {
                    SDL_SetRenderDrawColor(getRenderer(), 0, 0, 170, 255);
                    SDL_RenderFillRect(getRenderer(), &hpBoxes[i]); 
                    SDL_Color textColor = { 255, 0, 0, 255 };
                    std::stringstream hpStream;
                    hpStream << combatParticipants[i].getHp();
                    SDL_Surface* surface = TTF_RenderText_Solid(orderFont, hpStream.str().c_str(), textColor); //ttf surface  
                    SDL_Texture* texture = SDL_CreateTextureFromSurface(getRenderer(), surface); 
                    SDL_RenderCopy(getRenderer(), texture, nullptr, &hpBoxes[i]); 
                }
                
                //targetBoxes
                if (STATE_combatSelectedOption == "Attack")
                {
                    SDL_SetRenderDrawColor(getRenderer(), 150, 0, 0, 255);
                    for (auto target: validMoves[currTarget])
                    {
                        SDL_RenderFillRect(getRenderer(), &charBoxes[target]);
                    }
                }
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 140, 255);
                //TODO, render ORDER first
                //TODO, pop off when x happens
                SDL_RenderFillRect(getRenderer(), &orderTitleBox);
                SDL_Color textColor = { 255, 0, 0, 255 };
                std::stringstream titleStream;
                titleStream << orderTitle;
                SDL_Surface* surfaceTesting = TTF_RenderText_Solid(orderFont, titleStream.str().c_str(), textColor); //ttf surface  
                SDL_Texture* textureTesting = SDL_CreateTextureFromSurface(getRenderer(), surfaceTesting); 
                SDL_RenderCopy(getRenderer(), textureTesting, nullptr, &orderTitleBox); 
                
                for (int i = currOrderNum; i < orderBoxes.size(); i++)
                {
                    std::stringstream charNameStream;
                    SDL_RenderFillRect(getRenderer(), &orderBoxes[i]);
                    charNameStream << tempCharNames[i];
                    surfaceTesting = TTF_RenderText_Solid(orderFont, charNameStream.str().c_str(), textColor); //ttf surface  
                    textureTesting = SDL_CreateTextureFromSurface(getRenderer(), surfaceTesting);  
                    SDL_RenderCopy(getRenderer(), textureTesting, nullptr, &orderBoxes[i]); 
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
                
                break;
            }
        }

        
        //Update screen
        SDL_RenderPresent(getRenderer());
       
 
    }
}
