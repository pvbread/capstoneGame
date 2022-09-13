#include <Python.h>

// A python module that opens a window and draws a red square

#include <SDL2/SDL.h>

static PyObject* _helloSDL_game(PyObject* self) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Hello, SDL!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(3000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return PyUnicode_FromString("Hello, SDL!");
}

static struct PyMethodDef methods[] = {
    {"helloSDL_game", (PyCFunction)_helloSDL_game, METH_NOARGS, "Opens a window and draws a red square"},
    {NULL, NULL}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "_helloSDL",
    "A module that opens a window and draws a red square",
    -1,
    methods
};

PyMODINIT_FUNC PyInit__helloSDL(void) {
    return PyModule_Create(&module);
}
