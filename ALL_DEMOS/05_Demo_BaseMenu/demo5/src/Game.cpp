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

   
    Screen screen = INTRO;

    //////////// START MENUS INIT ///////////////

    const std::vector<std::string> introOptions = {
        "New Tab",
        "New Window",
        "New Priv Win",
        "Close File",
        "Open File",
        "Close Tab"
    };

    //const char* railwayFontPath = "./Raleway-Medium.ttf";
    
    BaseMenu introMenu2 = BaseMenu(100, 70, 70, 400, 100,  
                                  introOptions, 
                                  Font::satisfy, 
                                  Color::green,
                                  Color::darkGreen,
                                  getRenderer()
    );
    BaseMenu introMenu3 = BaseMenu(100, 70, 70, 400, 100,  
                                  introOptions, 
                                  Font::sono, 
                                  Color::lightNavy,
                                  Color::navy,
                                  getRenderer()
    );
    BaseMenu introMenu = BaseMenu(100, 70, 70, 400, 100, 
                                  introOptions, 
                                  Font::inter, 
                                  Color::cyan,
                                  Color::teal,
                                  getRenderer()
    );
    BaseMenu introMenu4 = BaseMenu(100, 70, 70, 400, 100,  
                                  introOptions, 
                                  Font::raleway, 
                                  Color::red,
                                  Color::maroon,
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
    bool STATE_updateHP = false;
    bool STATE_youWin = false;
    bool STATE_youLoose = false;
    bool STATE_didGetRandNumForJoke = true;
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

  
    //////////// START.TEXTURE LOADING /////////////
    
    TextureWrapper blackScreenTransition;
    

    std::unordered_map<TextureWrapper*, std::string> textureFilePaths = {
        
        {&blackScreenTransition, "../../assets/image/blackScreen.png"}
        
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
    
    

    ////////////START SCREEN TRANSITION INIT ///////////

    int alphaValueScreenTransition = 255;

    ////////////END START SCREEN TRANSITION INIT ///////////

    

    

    SDL_Event event;
    while (!getQuit())
    {
        /// POST-PROCESSOR ///

        timer->update();

        //////SCREEN TRANSITIONS////////
        if (STATE_introSelectedOption == "New Game")
        {
            STATE_preTransition = true;
            if(alphaValueScreenTransition >= 255)
            {
                STATE_preTransition = false;
                screen = MAP;
                STATE_postTransition = true;
                STATE_introSelectedOption = "NONE";
            }
            
        }
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
                        screen = INTRO2;
                        break;
                    }
                    case SDLK_3:
                    {
                        screen = INTRO3;
                        break;
                    }
                    case SDLK_4:
                    {
                        screen = INTRO4;
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
                            STATE_mapScreenOpenForTransition = true;
                        }
                    }
                    
                    break;
                }
                case INTRO2:
                { 
                    
                    introMenu2.onInput(event, SelectMusic, STATE_introSelectedOption);
                    if (STATE_introSelectedOption != "NONE")
                    {
                        if (STATE_introSelectedOption == "New Game")
                        {
                            
                            STATE_newGameSelected = true;
                            STATE_gameOver = false; 
                            STATE_mapScreenOpenForTransition = true;
                        }
                    }
                    
                    break;
                }
                case INTRO3:
                { 
                    
                    introMenu3.onInput(event, SelectMusic, STATE_introSelectedOption);
                    if (STATE_introSelectedOption != "NONE")
                    {
                        if (STATE_introSelectedOption == "New Game")
                        {
                            
                            STATE_newGameSelected = true;
                            STATE_gameOver = false; 
                            STATE_mapScreenOpenForTransition = true;
                        }
                    }
                    
                    break;
                }
                case INTRO4:
                { 
                    
                    introMenu4.onInput(event, SelectMusic, STATE_introSelectedOption);
                    if (STATE_introSelectedOption != "NONE")
                    {
                        if (STATE_introSelectedOption == "New Game")
                        {
                            
                            STATE_newGameSelected = true;
                            STATE_gameOver = false; 
                            STATE_mapScreenOpenForTransition = true;
                        }
                    }
                    
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
            case INTRO2:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());
                introMenu2.render(getRenderer()); 

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
            case INTRO3:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());
                introMenu3.render(getRenderer()); 

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
            case INTRO4:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());
                introMenu4.render(getRenderer()); 

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

        }

        
        //Update screen
        SDL_RenderPresent(getRenderer());
       
 
    }
}
