// sdl_init.h

#ifndef SDL_INIT_H
#define SDL_INIT_H

#include "../SDL2/SDL.h"

struct Window {
    SDL_Window* window;
    SDL_Renderer* renderer;
};

bool init_SDL(void);
void toggleFullScreen(void);
void cleanup_SDL(void);

#endif // SDL_INIT_H