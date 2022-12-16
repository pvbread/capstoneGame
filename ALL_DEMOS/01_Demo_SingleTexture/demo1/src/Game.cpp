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
    
    TextureWrapper bassistTexture;
    TextureWrapper drummerTexture;
    

    //add sprite sheet here
    std::unordered_map<TextureWrapper*, std::string> textureFilePaths = {
        
        {&bassistTexture, "../../assets/image/chars/bassist.png"},
        {&drummerTexture, "../../assets/image/chars/drummer.png"}
        
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
    
    int STATUS_show = 1;
    float STATE_angle = 0.0;
    SDL_Event event;
    while (!getQuit())
    {
        /// POST-PROCESSOR ///

        

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
                break;
            } 
        }

        //Update screen
        SDL_RenderPresent(getRenderer());

    }
}
