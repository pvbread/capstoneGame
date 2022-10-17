#include "gameEngine.h"
#include "TextureWrapper.h"
#include "Tile.h"
#include "MapDebugController.h"
#include "loadMedia.h"
#include "Screen.h"


#include <iostream>
#include <sstream>

Phoenix::Phoenix(Uint32 flags, const char* title, int x, int y, int w, int h)
{
    this->height = h;
    this->width = w;

    if (SDL_Init(flags) < 0)
    {
        SDL_Log("Init error!");
    }

    if(SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) == SDL_FALSE)
    {
        SDL_Log("Linear texture filtering wasn't enabled");
    }

    window = SDL_CreateWindow(title, x, y, w, h, SDL_WINDOW_SHOWN);
    if (window == nullptr)
    {
        SDL_Log("Error creating window");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr)
    {
        SDL_Log("Renderer wasn't created properly");
    }

    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_Log("Error initializing SDL_image");
    }

    if (TTF_Init() < 0)
        SDL_Log("TTF Init error!");

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        SDL_Log("Load Mixer Error: %s", Mix_GetError());  
}

Phoenix::~Phoenix()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Phoenix::runGameLoop()
{
    // temporary place for this
    Screen screen = COMBAT;
    //temporary 
    TTF_Font *font = TTF_OpenFont("./Raleway-Medium.ttf", 100);
    //temp
    SDL_Rect cursor = { 45, 160, 50, 50 };
    //incredibly temp
    int testCounter = 0;

    Mix_Music *SelectOST = Mix_LoadMUS("./bgmusic1.wav");
    Mix_Chunk *SelectMusic = Mix_LoadWAV("./MenuSelect.wav");
    Mix_PlayMusic(SelectOST, -1);
    int played;

    TextureWrapper tileTexture;
    TextureWrapper debugControllerTexture;
    std::vector<TextureWrapper*> textureWrappers{&tileTexture, &debugControllerTexture};
    
    const int TILE_TYPE_COUNT = 24;
    const int TILE_COUNT = 192;
    std::vector<Tile*> tileSet(TILE_COUNT);
    std::vector<SDL_Rect> tilesClipped(TILE_COUNT);// this is the total tiles

    if (!loadImageAssets(renderer, textureWrappers, tileSet, tilesClipped))
    {
        SDL_Log("error loading image assets");
        quit = true;
    }

    //set this to 0 whenever we want a clear debug controller
    debugControllerTexture.setAlpha(0);

    MapDebugController debugController;

    SDL_Rect camera = {0,0, 640, 480};

    double degrees = 0;
    SDL_RendererFlip flipType = SDL_FLIP_NONE;

    SDL_Event event;
    while (!quit)
    {
        while(SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
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
                    if (event.type == SDL_KEYDOWN)
                    {
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_UP:
                            {
                                cursor.y -= 100;
                                played = Mix_PlayChannel(-1, SelectMusic, 0);
                                if (played == -1){
                                    SDL_Log("audio error");
                                }
                                //if cursor is off the top of the screen, move it to the bottom
                                if (cursor.y < 160)
                                {
                                    cursor.y = 360;
                                }
                                break;
                            }
                            case SDLK_DOWN:
                            {
                                cursor.y += 100;
                                played = Mix_PlayChannel(-1, SelectMusic, 0);
                                if (played == -1){
                                    SDL_Log("audio error");
                                }
                                //if cursor is off the bottom of the screen, move it to the top
                                if (cursor.y > 360)
                                {
                                    cursor.y = 160;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
                case MAP:
                {
                    //this has a bug where movement
                    //keeps being read if key is not unpressed
                    debugController.onInput(event);
                    break;
                }
                case COMBAT:
                {
                    if (event.type == SDL_KEYDOWN)
                    {
                        switch (event.key.keysym.sym)
                        {
                            case SDLK_4:
                            {
                                testCounter++;
                                break;
                            }
                        }
                    }
                }
            }
            
        }
        
        //here have to poll event maybe in a loop?
        

        

        //Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        switch (screen)
        {
            case INTRO:
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                SDL_Rect rect1 = { 100, 140, 400, 100 };
                SDL_Rect rect2 = { 100, 240, 400, 100 };
                SDL_Rect rect3 = { 100, 340, 300, 100 };
                SDL_Color color = { 255, 0, 0, 255 };
                SDL_Surface *surface1 = TTF_RenderText_Solid(font, "New Game", color);
                SDL_Surface *surface2 = TTF_RenderText_Solid(font, "Load Game", color);
                SDL_Surface *surface3 = TTF_RenderText_Solid(font, "Credits", color);
                SDL_Texture *texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
                SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
                SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
                SDL_FreeSurface(surface1);
                SDL_FreeSurface(surface2);
                SDL_FreeSurface(surface3);
                SDL_Surface *surfaceCursor = TTF_RenderText_Solid(font, ">", color);
                SDL_Texture *textureCursor = SDL_CreateTextureFromSurface(renderer, surfaceCursor);
                SDL_FreeSurface(surfaceCursor);
                SDL_RenderCopy(renderer, texture1, nullptr, &rect1);
                SDL_RenderCopy(renderer, texture2, nullptr, &rect2);
                SDL_RenderCopy(renderer, texture3, nullptr, &rect3);
                SDL_RenderCopy(renderer, textureCursor, nullptr, &cursor);
                break;
            }
            case MAP:
            {
                debugController.move(1280, 960);
                debugController.centerScreen(camera);
                for(int i = 0; i < tileSet.size(); i++)
                {
                    tileSet[i]->render(renderer, tileTexture, camera, tilesClipped);
                }
                debugController.render(renderer, camera, debugControllerTexture);
                break;
            } 
            case COMBAT:
            {
                SDL_Rect rectBlue = { 320, 240, 100, 100 };
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // blue
                SDL_RenderFillRect(renderer, &rectBlue);
                SDL_Rect textRec = { 320, 240, 100, 100 };
                SDL_Color textColor = { 255, 0, 0, 255 };
                std::stringstream myFavoriteStream;
                myFavoriteStream << testCounter;
                SDL_Surface *surfaceTesting = TTF_RenderText_Solid(font, myFavoriteStream.str().c_str(), textColor); //ttf surface  
                SDL_Texture *textureTesting = SDL_CreateTextureFromSurface(renderer, surfaceTesting);  
                SDL_FreeSurface(surfaceTesting); 
                SDL_RenderCopy(renderer, textureTesting, nullptr, &rectBlue);
                break;
            }
        }

        
        //Update screen
        SDL_RenderPresent(renderer);
/*
        SDL_Delay(2000);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // blue
        SDL_RenderPresent(renderer);
        SDL_Delay(2000);*/
        
    }
    
}

void Phoenix::stopGameLoop()
{
    quit = true;
}