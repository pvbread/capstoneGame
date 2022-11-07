#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_timer.h>
#include <iostream>
#include <sstream>


int main(int argv, char** args)
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    
    SDL_Event event;
    bool quit = false;
    
    SDL_Init(SDL_INIT_VIDEO);

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("./Raleway-Medium.ttf", 24);
    SDL_Color color = { 255, 0, 0, 255 };

    //-----------------------------------------------------------------------------------
    int testCounter = 5;//is used to write 5,4,3,2,1,0 in decending order
    float frameTime= 0;//dont need
    int prevTime = 0;//dont need
    int currentTime = 0;
    float deltaTime = 0; //dont need //stands for change in time/////////////////////////////
    float testTime = 0;
    bool runTimer = false;

    //Im cheating here cause im tired but these are for our if check
    int fiveSecond = 0;
    int fourSecond = 0;
    int threeSecond = 0;
    int twoSecond = 0;
    int oneSecond = 0;
    int doneSecond = 0;
    //-----------------------------------------------------------------------------------

    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Rect rect1 = { 100, 40, 400, 100 };// {start (x,y) then length}

    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Surface *surface1 = TTF_RenderText_Solid(font, "Press 'DOWN KEY'", color);
    SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_FreeSurface(surface1);

    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture1, NULL, &rect1);
    SDL_RenderPresent(renderer);
    
    SDL_Rect rect = { 100, 140, 400, 10 };
    

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_Rect rectBlue = { 320, 240, 100, 100 };
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &rectBlue);
    SDL_Rect textRec = { 320, 240, 100, 100 };
    SDL_Color textColor = { 255, 0, 0, 255 };

    SDL_RenderPresent(renderer);

    while (!quit)
    {
        //where I began to best frame cap but most not needed
        prevTime = currentTime;
        currentTime = SDL_GetTicks();//I am needed
        deltaTime = (currentTime - prevTime) / 1000.0f;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                
                    case SDLK_DOWN:
                    {
                        //This is me being lazy and not writing a function that will do this automatically 
                        fiveSecond = currentTime + 5000.0f;
                        fourSecond = currentTime + 4000.0f; 
                        threeSecond = currentTime + 3000.0f;
                        twoSecond = currentTime + 2000.0f;
                        oneSecond = currentTime + 1000.0f;
                        doneSecond = currentTime;
                        //draws a blue box to be written on
                        SDL_Rect rectBlue = { 320, 240, 100, 100 };
                        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                        SDL_RenderFillRect(renderer, &rectBlue);

                        break;
                    }
                    case SDLK_UP:
                    {   
                        //check to do to see if this can appear when the loop is going
                        std::cout << "----------------------------------" << std::endl;
                        
                    }
                }

               
            }
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }
            
        }

        /// The next section can all be replaced with a timer function that will do all this work
        ///     rather then writting it in each box. 
        /// Need to figure out way to do this without multipul if statements cehcking for each sec
        /// 
        if (currentTime == fiveSecond)
        {
            SDL_Rect rectBlue = { 320, 240, 100, 100 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &rectBlue);
            SDL_Rect textRec = { 320, 240, 100, 100 };
            SDL_Color textColor = { 255, 0, 0, 255 };
            std::stringstream testC;
            testC << testCounter;
            SDL_Surface *testCounterSurface = TTF_RenderText_Solid(font, testC.str().c_str(), color);
            SDL_Texture *textureCounterTesting = SDL_CreateTextureFromSurface(renderer, testCounterSurface);
            testCounter--;
            std::cout << "-----------------" << testCounter + 1 << "------------------" << std::endl;
            SDL_RenderCopy(renderer, textureCounterTesting, nullptr, &rectBlue);
            SDL_FreeSurface(testCounterSurface);

        }
        if (currentTime == fourSecond)
        {
            SDL_Rect rectBlue = { 320, 240, 100, 100 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &rectBlue);
            SDL_Rect textRec = { 320, 240, 100, 100 };
            SDL_Color textColor = { 255, 0, 0, 255 };
            std::stringstream testC;
            testC << testCounter;
            SDL_Surface *testCounterSurface = TTF_RenderText_Solid(font, testC.str().c_str(), color);
            SDL_Texture *textureCounterTesting = SDL_CreateTextureFromSurface(renderer, testCounterSurface);
            testCounter--;
            std::cout << "-----------------" << testCounter + 1 << "------------------" << std::endl;
            SDL_RenderCopy(renderer, textureCounterTesting, nullptr, &rectBlue);
            SDL_FreeSurface(testCounterSurface);
        }
        if (currentTime == threeSecond)
        {
            SDL_Rect rectBlue = { 320, 240, 100, 100 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &rectBlue);
            SDL_Rect textRec = { 320, 240, 100, 100 };
            SDL_Color textColor = { 255, 0, 0, 255 };
            std::stringstream testC;
            testC << testCounter;
            SDL_Surface *testCounterSurface = TTF_RenderText_Solid(font, testC.str().c_str(), color);
            SDL_Texture *textureCounterTesting = SDL_CreateTextureFromSurface(renderer, testCounterSurface);
            testCounter--;
            std::cout << "-----------------" << testCounter + 1 << "------------------" << std::endl;
            SDL_RenderCopy(renderer, textureCounterTesting, nullptr, &rectBlue);
            SDL_FreeSurface(testCounterSurface);
        }
        if (currentTime == twoSecond)
        {
            SDL_Rect rectBlue = { 320, 240, 100, 100 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &rectBlue);
            SDL_Rect textRec = { 320, 240, 100, 100 };
            SDL_Color textColor = { 255, 0, 0, 255 };
            std::stringstream testC;
            testC << testCounter;
            SDL_Surface *testCounterSurface = TTF_RenderText_Solid(font, testC.str().c_str(), color);
            SDL_Texture *textureCounterTesting = SDL_CreateTextureFromSurface(renderer, testCounterSurface);
            testCounter--;
            std::cout << "-----------------" << testCounter + 1 << "------------------" << std::endl;
            SDL_RenderCopy(renderer, textureCounterTesting, nullptr, &rectBlue);
            SDL_FreeSurface(testCounterSurface);
        }
        if (currentTime == oneSecond)
        {
            SDL_Rect rectBlue = { 320, 240, 100, 100 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &rectBlue);
            SDL_Rect textRec = { 320, 240, 100, 100 };
            SDL_Color textColor = { 255, 0, 0, 255 };
            std::stringstream testC;
            testC << testCounter;
            SDL_Surface *testCounterSurface = TTF_RenderText_Solid(font, testC.str().c_str(), color);
            SDL_Texture *textureCounterTesting = SDL_CreateTextureFromSurface(renderer, testCounterSurface);
            testCounter--;
            std::cout << "-----------------" << testCounter + 1 << "------------------" << std::endl;
            SDL_RenderCopy(renderer, textureCounterTesting, nullptr, &rectBlue);
            SDL_FreeSurface(testCounterSurface);
        }
        if (currentTime == doneSecond)
        {
            SDL_Rect rectBlue = { 320, 240, 100, 100 };
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &rectBlue);
            SDL_Rect textRec = { 320, 240, 100, 100 };
            SDL_Color textColor = { 255, 0, 0, 255 };
            std::stringstream testC;
            testC << testCounter;
            SDL_Surface *testCounterSurface = TTF_RenderText_Solid(font, testC.str().c_str(), color);
            SDL_Texture *textureCounterTesting = SDL_CreateTextureFromSurface(renderer, testCounterSurface);
            testCounter--;
            std::cout << "-----------------" << testCounter + 1 << "------------------" << std::endl;
            SDL_RenderCopy(renderer, textureCounterTesting, nullptr, &rectBlue);
            SDL_FreeSurface(testCounterSurface);
        }
        

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
    
}