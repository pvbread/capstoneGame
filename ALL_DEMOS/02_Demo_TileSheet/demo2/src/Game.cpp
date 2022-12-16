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
    Screen screen = INTRO;
    
    //////////// START.TEXTURE LOADING /////////////
    TextureWrapper tileTexture;

    std::unordered_map<TextureWrapper*, std::string> textureFilePaths = {
        {&tileTexture, "../../assets/image/newspritedraft.png"}
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

    
    
    int STATE_demo;
    SDL_Event event;
    while (!getQuit())
    {
        /// POST-PROCESSOR ///

        
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
                        
                        STATE_demo = 1; 
                        break;
                    }
                    case SDLK_2:
                    {
                        
                        STATE_demo = 2;
                        break;
                    }
                    case SDLK_3:
                    {
                       STATE_demo = 3; 
                        
                        break;
                    }
                    case SDLK_4:
                    {
                        STATE_demo = 4;
                        
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
                tileTexture.render(getRenderer(), 0,0);
                if (STATE_demo == 1)
                {
                    tileTexture.render(getRenderer(), 400, 400, &tilesClipped[0], 0.0, nullptr, SDL_FLIP_NONE); 
                }
                if (STATE_demo == 2)
                {
                    tileTexture.render(getRenderer(), 400, 400, &tilesClipped[1], 0.0, nullptr, SDL_FLIP_NONE); 
                }
                if (STATE_demo == 3)
                {
                    tileTexture.render(getRenderer(), 400, 400, &tilesClipped[2], 0.0, nullptr, SDL_FLIP_NONE); 
                }
                if (STATE_demo == 4)
                {
                    tileTexture.render(getRenderer(), 400, 400, &tilesClipped[3], 0.0, nullptr, SDL_FLIP_NONE); 
                }
                break;
            }      
        }

        
        //Update screen
        SDL_RenderPresent(getRenderer());
       
 
    }
}
