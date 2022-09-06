//to run
//clang++ -std=c++11 -shared -undefined dynamic_lookup -I./pybind11/include/ `python3 -m pybind11 --includes` test.cpp -o mymodule.so `python3-config --ldflags` -I/Library/Frameworks/SDL2.framework -F/Library/Frameworks -framework SDL2

#include <pybind11/pybind11.h>
#include <SDL2/SDL.h>

#include <string>

namespace py = pybind11;

// will test opening an SDL window with a red square and white background
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

}