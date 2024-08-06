// sdl_init.cpp

#include "../header/sdl_init.h"

// extern allows variables defined in main to be used here
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

// extern for struct
extern Window sdl;

// extern for fullscreen
extern bool isFullScreen;


// Initialize SDL and render window
bool init_SDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }
    // Create window
    sdl.window = SDL_CreateWindow("ShadowCrest",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!sdl.window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    // Render window
    sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!sdl.renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    return true;
}

// toggle full screen
void toggleFullScreen() {
    Uint32 windowMode = isFullScreen ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP;
    SDL_SetWindowFullscreen(sdl.window, windowMode);
    isFullScreen = !isFullScreen;
}

// Cleanup SDL
void cleanup_SDL() {
    SDL_DestroyRenderer(sdl.renderer);
    SDL_DestroyWindow(sdl.window);
    SDL_Quit();
}
