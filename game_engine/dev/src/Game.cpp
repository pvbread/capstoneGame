#include "Game.h"

EscapeFromCapstone::EscapeFromCapstone(Uint32 flags, 
                       const char* title, 
                       int x, int y, 
                       int w, int h) : 
    Phoenix(flags, title, x, y, w, h){}

void EscapeFromCapstone::runGameLoop()
{
    // temporary place for this
    Screen screen = INTRO;
    //temporary 
    TTF_Font *font = TTF_OpenFont("./Raleway-Medium.ttf", 100);
    //temp
    SDL_Rect cursor = { 45, 160, 50, 50 };
    //incredibly temp
    int testCounter = 0;
    const std::vector<const char*> introOptions = {
        "New Game ",
        "Load Game",
        "Credits  "
    };
    const char* railwayFontPath = "./Raleway-Medium.ttf";
    SDL_Color introMenuColor = { 255, 0, 0, 255 };
    BaseMenu introMenu = BaseMenu(100, 140, 400, 100, 100, 
                                  introOptions.size(), 
                                  introOptions, 
                                  railwayFontPath, 
                                  introMenuColor, 
                                  getRenderer()
    );

    const std::vector<const char*> combatOptions = {
        "Attack ",
        "Buff   ",
        "Debuff ",
        "Move   "
    };

    SDL_Color combatMenuColor = { 0, 0, 255, 255 };
    BaseMenu combatMenu = BaseMenu(25, 520, 200, 50, 100, 
                                   combatOptions.size(), 
                                   combatOptions, 
                                   railwayFontPath, 
                                   combatMenuColor, 
                                   getRenderer()
    );

    Mix_Music *SelectOST = Mix_LoadMUS("./bgmusic1.wav");
    Mix_Chunk *SelectMusic = Mix_LoadWAV("./MenuSelect.wav");
    Mix_PlayMusic(SelectOST, -1);
    int played;

    TextureWrapper tileTexture;
    TextureWrapper characterInMapTexture;
    TextureWrapper debugControllerTexture;
    std::unordered_map<TextureWrapper*, std::string> textureFilePaths = {
        {&tileTexture, "../../assets/image/tilesDraft.png"},
        {&characterInMapTexture, "../../assets/image/dot.bmp"},
        {&debugControllerTexture, "../../assets/image/dot.bmp"}
    };
    
    const int TILE_TYPE_COUNT = 24;
    const int TILE_COUNT = 192;
    std::vector<Tile*> tileMap(TILE_COUNT);
    std::vector<SDL_Rect> tilesClipped(TILE_COUNT);// this is the total tiles
    std::map<std::pair<int, int>, TileType> coordinateToTileTypeMap;
    //TODO in the destructor, clean this up
    //maybe better to have a dedicated function to map the coordinate tiles?
    bool didTexturesLoad = loadImageAssets(getRenderer(), 
                                           tileMap, 
                                           tilesClipped, 
                                           textureFilePaths, 
                                           coordinateToTileTypeMap);
    if (!didTexturesLoad)
    {
        SDL_Log("error loading image assets");
        setToQuit();
    }

    //set this to 0 whenever we want a clear debug controller
    debugControllerTexture.setAlpha(0);

    SDL_Rect debugHitbox = {0, 0, 1, 1};
    SDL_Rect characterControllerHitbox = {30, 30, 80, 80};
    MapDebugController debugController(10, 0, 0, debugHitbox);
    CharacterInMap characterController(80, 0, 0, characterControllerHitbox);

    const int SCREEN_WIDTH = 960;
    const int SCREEN_HEIGHT = 720;
    SDL_Rect camera = {0,0, SCREEN_WIDTH, SCREEN_HEIGHT};

    double degrees = 0;
    SDL_RendererFlip flipType = SDL_FLIP_NONE;

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
                }
            }
            switch (screen)
            {
                case INTRO:
                {
                    introMenu.onInput(event, SelectMusic);
                    break;
                }
                case MAP:
                {
                    //this has a bug where movement
                    //keeps being read if key is not unpressed
                    debugController.onInput(event);
                    characterController.onInput(event, coordinateToTileTypeMap);
                    break;
                }
                case COMBAT:
                {
                    combatMenu.onInput(event, SelectMusic);
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
                break;
            } 
            case COMBAT:
            {
                combatMenu.render(getRenderer());   
                SDL_Rect rectBlue = { 600, 240, 100, 100 };
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 255, 255); // blue
                SDL_RenderFillRect(getRenderer(), &rectBlue);
                SDL_Rect textRec = { 600, 240, 100, 100 };
                SDL_Color textColor = { 255, 0, 0, 255 };
                std::stringstream myFavoriteStream;
                myFavoriteStream << testCounter;
                SDL_Surface *surfaceTesting = TTF_RenderText_Solid(font, myFavoriteStream.str().c_str(), textColor); //ttf surface  
                SDL_Texture *textureTesting = SDL_CreateTextureFromSurface(getRenderer(), surfaceTesting);  
                SDL_FreeSurface(surfaceTesting); 
                SDL_RenderCopy(getRenderer(), textureTesting, nullptr, &rectBlue);
                break;
            }
        }

        
        //Update screen
        SDL_RenderPresent(getRenderer());
        /*
        SDL_Delay(2000);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // blue
        SDL_RenderPresent(renderer);
        SDL_Delay(2000);*/
        
    }
}

/*
                SDL_Rect rect1 = { 100, 140, 400, 100 };
                SDL_Rect rect2 = { 100, 240, 400, 100 };
                SDL_Rect rect3 = { 100, 340, 300, 100 };
                SDL_Color color = { 255, 0, 0, 255 };
                //unnamed textures initd in vector?
                //same surface can just be overwritten
                SDL_Surface *surface1 = TTF_RenderText_Solid(font, "New Game", color);
                SDL_Surface *surface2 = TTF_RenderText_Solid(font, "Load Game", color);
                SDL_Surface *surface3 = TTF_RenderText_Solid(font, "Credits", color);
                std::vector<SDL_Texture*> myTextTextures(3);
                myTextTextures[0] = SDL_CreateTextureFromSurface(getRenderer(), surface1);
                myTextTextures[1] = SDL_CreateTextureFromSurface(getRenderer(), surface2);
                myTextTextures[2] = SDL_CreateTextureFromSurface(getRenderer(), surface3);
                //SDL_Texture *texture1 = SDL_CreateTextureFromSurface(getRenderer(), surface1);
                //SDL_Texture *texture2 = SDL_CreateTextureFromSurface(getRenderer(), surface2);
                //SDL_Texture *texture3 = SDL_CreateTextureFromSurface(getRenderer(), surface3);
                SDL_FreeSurface(surface1);
                SDL_FreeSurface(surface2);
                SDL_FreeSurface(surface3);
                SDL_Surface *surfaceCursor = TTF_RenderText_Solid(font, ">", color);
                SDL_Texture *textureCursor = SDL_CreateTextureFromSurface(getRenderer(), surfaceCursor);
                SDL_FreeSurface(surfaceCursor);
                SDL_RenderCopy(getRenderer(), myTextTextures[0], nullptr, &rect1);
                SDL_RenderCopy(getRenderer(), myTextTextures[1], nullptr, &rect2);
                SDL_RenderCopy(getRenderer(), myTextTextures[2], nullptr, &rect3);
                //SDL_RenderCopy(getRenderer(), texture1, nullptr, &rect1);
                //SDL_RenderCopy(getRenderer(), texture2, nullptr, &rect2);
                //SDL_RenderCopy(getRenderer(), texture3, nullptr, &rect3);
                SDL_RenderCopy(getRenderer(), textureCursor, nullptr, &cursor);
                */