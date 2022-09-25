//terminal compile
//g++ helloOpenGl.cpp ./src/glad.c -I./include -I/Library/Frameworks/SDL2.framework -F/Library/Frameworks -framework SDL2

#include <SDL.h>
#include <glad/glad.h>
#include <iostream>

int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window* gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;
bool gQuit = false;

void getOpenGLVersionInfo()
{
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void initProgram()
{
    if (SDL_Init(SDL_INIT_VIDEO < 0))
    {
        SDL_Log("error initializing video");
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);//sets version to 4.1 (mac doesn't support beyong 4.1)

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //filters out deprecated functions
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    gGraphicsApplicationWindow = SDL_CreateWindow("test", 0, 0, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);
    if (gGraphicsApplicationWindow == nullptr)
    {
        SDL_Log("sdl window init error");
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);
    if (gGraphicsApplicationWindow == nullptr)
    {
        SDL_Log("GL context creation error");
        exit(1);
    }

    //initializes the Glad library
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
    {
        SDL_Log("glad failed to initialize");
        exit(1);
    }

    getOpenGLVersionInfo();

}

void Input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            gQuit = true;
        }
    }
}

void PreDraw()
{}

void Draw()
{}

void mainLoop()
{
    while(!gQuit)
    {
        Input();
        PreDraw();
        Draw();

        SDL_GL_SwapWindow(gGraphicsApplicationWindow);
    }
    
}

void cleanUp()
{
    SDL_DestroyWindow(gGraphicsApplicationWindow);
    SDL_Quit();
}

int main()
{
    initProgram();

    mainLoop();

    cleanUp();

    return 0;
}