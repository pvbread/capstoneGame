//to run
//clang++ -std=c++11 -shared -undefined dynamic_lookup -I./pybind11/include/ `python3 -m pybind11 --includes` test.cpp -o mymodule.so `python3-config --ldflags` -I/Library/Frameworks/SDL2.framework -F/Library/Frameworks -framework SDL2

#include <pybind11/pybind11.h>
#include <SDL2/SDL.h>

#include <string>

namespace py = pybind11;

// initialize SDL Video
void init()
{
    SDL_Init(SDL_INIT_VIDEO);
}

//define a class to hold the window and renderer
class Window
{
public:
    Window(const std::string &title, int width, int height)
    {
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
        renderer = SDL_CreateRenderer(window, -1, 0);
    }

    ~Window()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

    SDL_Window *window;
    SDL_Renderer *renderer;
};



// create a renderer and return a pointer to it
SDL_Renderer* create_renderer(SDL_Window* window)
{
    return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

// clear the renderer with the given color
void clear_renderer(SDL_Renderer* renderer, int r, int g, int b, int a)
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}

// draw a rectangle on the renderer
void draw_rect(SDL_Renderer* renderer, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderFillRect(renderer, &rect);
}

// present the renderer
void present_renderer(SDL_Renderer* renderer)
{
    SDL_RenderPresent(renderer);
}

// poll for event, return true if there is one
bool poll_event(SDL_Event* event)
{
    return SDL_PollEvent(event);
}

// quit SDL
void quit()
{
    SDL_Quit();
}

//destroy window
void destroy_window(SDL_Window* window)
{
    SDL_DestroyWindow(window);
}

//destroy renderer
void destroy_renderer(SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
}



void test(){
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Event event;
    bool quit = false;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Rect rect = { 320, 240, 100, 100 };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_RenderPresent(renderer);

    //wait for user to quit
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


PYBIND11_MODULE(mymodule, m){
    m.doc() = "example plugin"; // Optional docstring
    m.def("test", &test, "Test SDL.");
    m.def("init", &init, "Initialize SDL."); 
    m.def("quit", &quit, "Quit SDL.");
    
    //define a class
    py::class_<Window>(m, "Window")
        .def(py::init<const std::string &, int, int>());
    

    
  


}