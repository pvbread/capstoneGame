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

    Timer* timer = Timer::instance();

    ////////// START CHARACTER INIT ////////
    BasePlayer conductor = BasePlayer("conductor    ", 30, 3, 3, 0, 3, 3, 3);
    BasePlayer drum = BasePlayer("drummer      ", 50, 2, 1, 0, 3, 3, 3);
    BasePlayer flute = BasePlayer("flutist      ", 20, 6, 1, 0, 3, 3, 3);
    Bass bass = Bass("bassist      ", 60, 1, 3, 0, 3, 3, 3);
    flute.setNewParticipantsIndex(0);
    conductor.setNewParticipantsIndex(1);
    bass.setNewParticipantsIndex(2);
    drum.setNewParticipantsIndex(3);
    std::vector<BaseCharacter> playerTeam{flute, conductor, bass, drum};
    std::vector<BaseCharacter> enemies;
    std::vector<BaseCharacter> combatParticipants;
    // set player index
    

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
    bool STATE_timerStarted = false;
    float STATE_timerCount;
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

    ////////// START BATTLE NOTIFICATION //////////

    TextBox battleNotification = TextBox("", 200, 5, 400, 700, 100);

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


    //////// DEBUGGER /////

    TextBox statusRoundOrderSize = TextBox(".", 100, 20, 20, 50, 50, Font::satisfy, Color::red, Color::white);
    TextBox statusCurrOrderNum = TextBox(".", 100, 80, 20, 50, 50, Font::satisfy, Color::red, Color::white);
    TextBox statusGetParticipantsIndex = TextBox(".", 100, 140, 20, 50, 50, Font::satisfy, Color::red, Color::white);

    ///////////////////////
    

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
                        screen = COMBAT;
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
                        // update setRoundTurns display
                        for(int i = 0; i < roundOrder.size(); i++)
                            tempCharNames[i] = roundOrder[i]->getName();


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
                    
                    
                    // create new round and set round turns
                    if(!STATE_roundsSet && STATE_roundOver)
                    {
                        // get new round turn order
                        roundOrder.clear();
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
                            STATE_combatSelectedOption = "NONE";
                            std::vector<int> attackDamage;
                            validMoves = roundOrder[currOrderNum]->getValidMoves(ATTACK, roundOrder[currOrderNum]->getParticipantsIndex(),combatParticipants);
                            // in case when a character dies, preserve target index's name before performing action 
                            std::string targetNotification;
                            targetNotification += combatParticipants[validMoves[currTarget][0]].getName();
                            roundOrder[currOrderNum]->doAction(ATTACK, attackDamage, validMoves[currTarget], combatParticipants); 
                            //TODO Set 8 to be the current size of alive characters (player and enemies) 'livingCharacters'
                            
                            do 
                            {
                                currOrderNum = (currOrderNum + 1) % roundOrder.size();
                            }
                            while (roundOrder[currOrderNum]->isAlive()==false);
                            std::string attackNotification;
                            for (int i = 0; i < validMoves[currTarget].size(); i++)
                            {
                                if (i == 0)
                                {
                                    attackNotification += std::to_string(attackDamage[i]);
                                    attackNotification += " dmg dealt to ";
                                    attackNotification += targetNotification;
                                    continue;
                                }
                                attackNotification += " *** ";
                                attackNotification += std::to_string(attackDamage[i]);
                                attackNotification += " dmg dealt to ";
                                attackNotification += combatParticipants[validMoves[currTarget][i]].getName();

                            }
                            battleNotification.changeText(attackNotification);
                            STATE_timerStarted = true;
                            STATE_timerCount = timer->deltaTime() + 3;
                        }

                        if (STATE_combatSelectedOption == "Buff")
                        {

                            //WAS the round order properly set??
                            
                            STATE_combatSelectedOption = "NONE";
                            //look at roundOrder
                            std::vector<int> healAmount;
                            validMoves = roundOrder[currOrderNum]->getValidMoves(BUFF,roundOrder[currOrderNum]->getParticipantsIndex(),combatParticipants);
                            roundOrder[currOrderNum]->doAction(BUFF, healAmount, validMoves[currTarget], combatParticipants); 
                            //TODO Set 8 to be the current size of alive characters (player and enemies) 'livingCharacters'
                            do 
                            {
                                currOrderNum = (currOrderNum + 1) % roundOrder.size();
                            }
                            while (roundOrder[currOrderNum]->isAlive()==false);
                            std::string healNotification;
                            for (int i = 0; i < validMoves[currTarget].size(); i++)
                            {
                                if (i == 0)
                                {
                                    healNotification += std::to_string(healAmount[i]);
                                    healNotification += " healed for ";
                                    healNotification += combatParticipants[validMoves[currTarget][i]].getName();
                                    continue;
                                }
                                healNotification += " *** ";
                                healNotification += std::to_string(healAmount[i]);
                                healNotification += " healed for ";
                                healNotification += combatParticipants[validMoves[currTarget][i]].getName();

                            }
                            battleNotification.changeText(healNotification);
                            STATE_timerStarted = true;
                            STATE_timerCount = timer->deltaTime() + 3;

                        }

                        if (STATE_combatSelectedOption == "Debuff")
                        {
                            //WAS the round order properly set??
                            STATE_combatSelectedOption = "NONE";
                            //look at roundOrder
                            std::vector<int> newSpeed;
                            validMoves = roundOrder[currOrderNum]->getValidMoves(DEBUFF,roundOrder[currOrderNum]->getParticipantsIndex(),combatParticipants);
                            roundOrder[currOrderNum]->doAction(DEBUFF, newSpeed, validMoves[currTarget], combatParticipants); 
                            //TODO Set 8 to be the current size of alive characters (player and enemies) 'livingCharacters'
                            do 
                            {
                                currOrderNum = (currOrderNum + 1) % roundOrder.size();
                            }
                            while (roundOrder[currOrderNum]->isAlive()==false);
                            std::string healNotification;
                            for (int i = 0; i < validMoves[currTarget].size(); i++)
                            {
                                if (i == 0)
                                {
                                    healNotification += std::to_string(combatParticipants[validMoves[currTarget][i]].getSpeed() + newSpeed[i]);
                                    healNotification += " is the new speed for ";
                                    healNotification += combatParticipants[validMoves[currTarget][i]].getName();
                                    continue;
                                }
                                healNotification += " *** ";
                                healNotification += std::to_string(combatParticipants[validMoves[currTarget][i]].getSpeed() + newSpeed[i]);
                                healNotification += " is the new speed for ";
                                healNotification += combatParticipants[validMoves[currTarget][i]].getName();

                            }
                            battleNotification.changeText(healNotification);
                            STATE_timerStarted = true;
                            STATE_timerCount = timer->deltaTime() + 3;
                        }

                        if (STATE_combatSelectedOption == "Move")
                        {
                            int x = 5;
                            //WAS the round order properly set??
                            STATE_combatSelectedOption = "NONE";
                            //look at roundOrder
                            std::vector<int> nothing;
                            validMoves = roundOrder[currOrderNum]->getValidMoves(MOVE,roundOrder[currOrderNum]->getParticipantsIndex(),combatParticipants);
                            roundOrder[currOrderNum]->doAction(MOVE, nothing, validMoves[currTarget], combatParticipants); 
                            //TODO Set 8 to be the current size of alive characters (player and enemies) 'livingCharacters'
                            do 
                            {
                                currOrderNum = (currOrderNum + 1) % roundOrder.size();
                            }
                            while (roundOrder[currOrderNum]->isAlive()==false);
                            std::string healNotification;
                            int charIndex;
                            std::string currName;
                            for (int i = 0; i < validMoves[currTarget].size(); i++)
                            {
                                if (i == 0)
                                {
                                    charIndex = roundOrder[currOrderNum]->getParticipantsIndex();
                                    currName = combatParticipants[charIndex].getName();
                                    healNotification += currName;
                                    healNotification += " switch places with ";
                                    healNotification += combatParticipants[validMoves[currTarget][i]].getName();
                                }
                
                            }
                            battleNotification.changeText(healNotification);
                            STATE_timerStarted = true;
                            STATE_timerCount = timer->deltaTime() + 3;
                        }

                        //TODO get end state for battle
                        STATE_combatMenuTargetSelected = false;
                        currTarget = 0;
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
                    SDL_FreeSurface(surfaceTesting);
                    SDL_DestroyTexture(textureTesting);
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
                //TODO FIX THIS BEFORE IT MELTS DAVID'S COMPUTER
                //TODO Something wrong with the rendering of currPlayer
                int currPlayer = roundOrder[currOrderNum]->getParticipantsIndex();
                SDL_RenderFillRect(getRenderer(), &charBoxes[currPlayer]);

                //hpBoxes
                SDL_Surface* surface;
                SDL_Texture* texture;
                for (int i = 0;  i < hpBoxes.size(); i++)
                {
                    SDL_SetRenderDrawColor(getRenderer(), 0, 0, 170, 255);
                    SDL_RenderFillRect(getRenderer(), &hpBoxes[i]); 
                    SDL_Color textColor = { 255, 0, 0, 255 };
                    std::stringstream hpStream;
                    hpStream << combatParticipants[i].getHp();
                    surface = TTF_RenderText_Solid(orderFont, hpStream.str().c_str(), textColor); //ttf surface  
                    texture = SDL_CreateTextureFromSurface(getRenderer(), surface); 
                    SDL_RenderCopy(getRenderer(), texture, nullptr, &hpBoxes[i]); 
                }
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);

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
                    SDL_SetRenderDrawColor(getRenderer(), 150, 0, 0, 255);
                    // rerender bug if I don't update validMoves
                    validMoves = roundOrder[currOrderNum]->getValidMoves(ATTACK, roundOrder[currOrderNum]->getParticipantsIndex(),combatParticipants);
                    for (auto target: validMoves[currTarget])
                    {
                        SDL_RenderFillRect(getRenderer(), &charBoxes[target]);
                    }
                }
                if (STATE_combatSelectedOption == "Buff")
                {
                    
                    // rerender bug if I don't update validMoves
                    //statusRoundOrderSize.changeText(std::to_string(roundOrder.size()));
                    //statusCurrOrderNum.changeText(std::to_string(currOrderNum));
                    //statusGetParticipantsIndex.changeText(std::to_string(roundOrder[currOrderNum]->getParticipantsIndex()));
                    //statusCurrOrderNum.render(getRenderer());
                    //statusGetParticipantsIndex.render(getRenderer());
                    //statusRoundOrderSize.render(getRenderer());
                    SDL_SetRenderDrawColor(getRenderer(), 0, 150, 0, 255);
                    
                    validMoves = roundOrder[currOrderNum]->getValidMoves(BUFF,roundOrder[currOrderNum]->getParticipantsIndex(),combatParticipants);
                    for (auto target: validMoves[currTarget])
                    {
                        SDL_RenderFillRect(getRenderer(), &charBoxes[target]);
                    }
                    
                }

                if (STATE_combatSelectedOption == "Debuff")
                {
                    SDL_SetRenderDrawColor(getRenderer(), 150, 0, 0, 255);
                    // rerender bug if I don't update validMoves
                    validMoves = roundOrder[currOrderNum]->getValidMoves(DEBUFF, roundOrder[currOrderNum]->getParticipantsIndex(),combatParticipants);
                    for (auto target: validMoves[currTarget])
                    {
                        SDL_RenderFillRect(getRenderer(), &charBoxes[target]);
                    }
                }

                if (STATE_combatSelectedOption == "Move")
                {
                    SDL_SetRenderDrawColor(getRenderer(), 0, 150, 0, 255);
                    // rerender bug if I don't update validMoves
                    validMoves = roundOrder[currOrderNum]->getValidMoves(MOVE, roundOrder[currOrderNum]->getParticipantsIndex(),combatParticipants);
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
                SDL_FreeSurface(surfaceTesting);
                SDL_DestroyTexture(textureTesting);

                // update turn order for rendering
                for(int i = 0; i < roundOrder.size(); i++)
                {
                    tempCharNames[i] = roundOrder[i]->getName();
                }
                // TODO: figure out how to remove names when character dies
                for (int i = roundOrder.size(); i < orderBoxes.size();i++)
                {
                    orderBoxes.pop_back();
                }

                for (int i = currOrderNum; i < orderBoxes.size(); i++)
                {
                    std::stringstream charNameStream;
                    SDL_RenderFillRect(getRenderer(), &orderBoxes[i]);
                    charNameStream << tempCharNames[i];
                    surfaceTesting = TTF_RenderText_Solid(orderFont, charNameStream.str().c_str(), textColor); //ttf surface  
                    textureTesting = SDL_CreateTextureFromSurface(getRenderer(), surfaceTesting);  
                    SDL_RenderCopy(getRenderer(), textureTesting, nullptr, &orderBoxes[i]); 
                }
                SDL_FreeSurface(surfaceTesting);
                SDL_DestroyTexture(textureTesting);
                break;
            }
        }

        
        //Update screen
        SDL_RenderPresent(getRenderer());
       
 
    }
}
