#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

int main(int argc, char* argv[]) {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event event;
    bool quit = false;
    std::cout << "Doesnt work" << std::endl;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
	std::cout << "Error SDL2 Initialization : " << SDL_GetError();//SDL2 initialization error check
    }
	
    if (IMG_Init(IMG_INIT_PNG) == 0) {
	std::cout << "Error SDL2_image Initialization";//SDL2 immage not loading check
    }

    SDL_Surface* lettuce_sur = IMG_Load("lettuce.png");
    if (lettuce_sur == NULL) {
	    std::cout << "Error loading image: " << IMG_GetError();//trys to load lettuce.png
	
    }

    SDL_Texture* lettuce_tex = SDL_CreateTextureFromSurface(renderer, lettuce_sur);
    if (lettuce_tex == NULL) {
	    std::cout << "Error creating texture";//now trys to texture it
	
    }

    SDL_FreeSurface(lettuce_sur);

    while (true) {
	SDL_Event e;
	if (SDL_WaitEvent(&e)) {
		if (e.type == SDL_QUIT) {
			break;
            std::cout << "Doesnt work" << std::endl;
		    }
	    }

        SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, lettuce_tex, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

    //---

    SDL_DestroyTexture(lettuce_tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}