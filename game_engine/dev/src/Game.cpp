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
    TextureWrapper characterTestTexture;
    //add sprite sheet here
    std::unordered_map<TextureWrapper*, std::string> textureFilePaths = {
        {&tileTexture, "../../assets/image/tilesDraft.png"},
        {&characterInMapTexture, "../../assets/image/dot.bmp"},
        {&debugControllerTexture, "../../assets/image/dot.bmp"},
        {&characterTestTexture, "../../assets/image/char.png"}
    };
    
    //this is temporary
    
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

    const int TILE_COUNT = 192;
    const int TILE_LENGTH = 80;
    const int TILE_TYPE_COUNT = 12;

    std::vector<Tile*> tileMap(TILE_COUNT);
    std::vector<SDL_Rect> tilesClipped(TILE_TYPE_COUNT);// this is the total tiles
    std::map<std::pair<int, int>, TileType> coordinateToTileTypeMap;

    bool didTilesLoad = loadTiles(tileMap, coordinateToTileTypeMap, TILE_COUNT, TILE_TYPE_COUNT, TILE_LENGTH);
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
    //set this to 0 whenever we want a clear debug controller
    debugControllerTexture.setAlpha(0);

    SDL_Rect debugHitbox = {0, 0, 1, 1};
    SDL_Rect characterControllerHitbox = {30, 30, 80, 80};
    MapDebugController debugController(10, 0, 0, debugHitbox);
    CharacterInMap characterController(80, 0, 0, characterControllerHitbox);
    std::vector<SDL_Rect> charBoxes(8);
    for (int i = 0; i < charBoxes.size(); i++)
    {
        SDL_Rect temp = {(50+(i*100)), 200, 64, 64};
        charBoxes[i] = temp; 
    }
    std::vector<SDL_Rect> orderBoxes(9);
    for (int i = 0; i < orderBoxes.size(); i++)
    {
        SDL_Rect temp = {750, 280+(i*50), 200, 30};
        orderBoxes[i] = temp; 
    }

    std::vector<std::string> tempCharNames {
        "Order        ",
        "flute        ",
        "conductor    ",
        "drums        ",
        "bass         ",
        "coneheadAlpha",
        "coneheadBeta ",
        "coneheadTheta",
        "Carl         "
    };

    int currMove = 0;
    std::vector<std::vector<int>> validMoves = {
        {1, 3},
        {2, 4},
        {3, 5},
        {4, 6},
        {5, 7}
    };
    bool actionChosen = false;

    const int SCREEN_WIDTH = 960;
    const int SCREEN_HEIGHT = 720;
    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

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
                            case SDLK_LEFT:
                            {
                                currMove--;
                                if (currMove < 0)
                                    currMove = 0;
                                break;
                            }
                            case SDLK_RIGHT:
                            {
                                currMove++;
                                if (currMove == validMoves.size())
                                    currMove = validMoves.size()-1;
                                break;
                            }
                            case SDLK_RETURN:
                            {
                                if (!actionChosen)
                                    actionChosen = true;
                                else
                                    actionChosen = false;
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
                if (actionChosen)
                {
                    SDL_SetRenderDrawColor(getRenderer(), 150, 0, 0, 255);
                    for (auto target: validMoves[currMove])
                    {
                        SDL_RenderFillRect(getRenderer(), &charBoxes[target]);
                    }
                }
                SDL_SetRenderDrawColor(getRenderer(), 0, 0, 140, 255);
                for (int i = 0; i < orderBoxes.size(); i++)
                {
                    SDL_RenderFillRect(getRenderer(), &orderBoxes[i]);
                    SDL_Color textColor = { 255, 0, 0, 255 };
                    std::stringstream myFavoriteStream;
                    myFavoriteStream << tempCharNames[i];
                    SDL_Surface *surfaceTesting = TTF_RenderText_Solid(font, myFavoriteStream.str().c_str(), textColor); //ttf surface  
                    SDL_Texture *textureTesting = SDL_CreateTextureFromSurface(getRenderer(), surfaceTesting);  
                    SDL_RenderCopy(getRenderer(), textureTesting, nullptr, &orderBoxes[i]); 
                }
                

                //test drawing text on a rectangle

                /*
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
                */
                break;
            }
        }

        
        //Update screen
        SDL_RenderPresent(getRenderer());
       
 
    }
}
