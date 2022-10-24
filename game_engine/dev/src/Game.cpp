#include "Game.h"

EscapeFromCapstone::EscapeFromCapstone(Uint32 flags, 
                       const char* title, 
                       int x, int y, 
                       int w, int h) : 
    Phoenix(flags, title, x, y, w, h){}

void EscapeFromCapstone::runGameLoop()
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

    SDL_Rect camera = {0,0, 640, 480};

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
                    characterController.onInput(event, coordinateToTileTypeMap);
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
        SDL_SetRenderDrawColor(getRenderer(), 255, 255, 255, 255);
        SDL_RenderClear(getRenderer());

        switch (screen)
        {
            case INTRO:
            {
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
                SDL_RenderClear(getRenderer());
                SDL_Rect rect1 = { 100, 140, 400, 100 };
                SDL_Rect rect2 = { 100, 240, 400, 100 };
                SDL_Rect rect3 = { 100, 340, 300, 100 };
                SDL_Color color = { 255, 0, 0, 255 };
                SDL_Surface *surface1 = TTF_RenderText_Solid(font, "New Game", color);
                SDL_Surface *surface2 = TTF_RenderText_Solid(font, "Load Game", color);
                SDL_Surface *surface3 = TTF_RenderText_Solid(font, "Credits", color);
                SDL_Texture *texture1 = SDL_CreateTextureFromSurface(getRenderer(), surface1);
                SDL_Texture *texture2 = SDL_CreateTextureFromSurface(getRenderer(), surface2);
                SDL_Texture *texture3 = SDL_CreateTextureFromSurface(getRenderer(), surface3);
                SDL_FreeSurface(surface1);
                SDL_FreeSurface(surface2);
                SDL_FreeSurface(surface3);
                SDL_Surface *surfaceCursor = TTF_RenderText_Solid(font, ">", color);
                SDL_Texture *textureCursor = SDL_CreateTextureFromSurface(getRenderer(), surfaceCursor);
                SDL_FreeSurface(surfaceCursor);
                SDL_RenderCopy(getRenderer(), texture1, nullptr, &rect1);
                SDL_RenderCopy(getRenderer(), texture2, nullptr, &rect2);
                SDL_RenderCopy(getRenderer(), texture3, nullptr, &rect3);
                SDL_RenderCopy(getRenderer(), textureCursor, nullptr, &cursor);
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
                SDL_Rect rectBlue = { 320, 240, 100, 100 };
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 255, 255); // blue
                SDL_RenderFillRect(getRenderer(), &rectBlue);
                SDL_Rect textRec = { 320, 240, 100, 100 };
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