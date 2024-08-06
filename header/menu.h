// menu.h

#ifndef MENU_H
#define MENU_H

#include "../SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

struct Menu {
    SDL_Texture* titleBackground;
    SDL_Texture* titleName;
    SDL_Texture* start; 
    SDL_Texture* cont;
    SDL_Texture* quit;
    SDL_Texture* settings;
    SDL_Texture* back;
    SDL_Texture* apply;
    SDL_Rect titleNameRect;
    SDL_Rect startButtonRect;
    SDL_Rect contButtonRect;
    SDL_Rect quitButtonRect;
    SDL_Rect settingsButtonRect;
    SDL_Rect backButtonRect;
    SDL_Rect applyButtonRect;
    // Font
    TTF_Font* font;
    SDL_Color textColor;
    // *** DISPLAY MODE ***
    SDL_Surface* dispModeSurface;
    SDL_Texture* dispModeTexture;
    SDL_Rect dispModeRect;
    // windowed
    SDL_Surface* windowedSurface;
    SDL_Texture* windowedTexture;
    SDL_Rect windowedRect;
    // fullscreen
    SDL_Surface* fullscreenSurface;
    SDL_Texture* fullscreenTexture;
    SDL_Rect fullscreenRect;
    // *** TOGGLE FPS ***
    SDL_Surface* fpsSurface;
    SDL_Texture* fpsTexture;
    SDL_Rect fpsRect;
    // fps on
    SDL_Surface* fpsOnSurface;
    SDL_Texture* fpsOnTexture;
    SDL_Rect fpsOnRect;
    // fps off
    SDL_Surface* fpsOffSurface;
    SDL_Texture* fpsOffTexture;
    SDL_Rect fpsOffRect;
    // *** SCREEN RESOLUTION ***
    SDL_Surface* resSurface;
    SDL_Texture* resTexture;
    SDL_Rect resRect;
    // display screen resolution
    SDL_Surface* dispResSurface;
    SDL_Texture* dispResTexture;
    SDL_Rect dispResRect;
    int textWidth, textHeight;
    bool showFPS;
    bool setWindow;

    // Constructor 
    Menu();

    void reset();

    // Initialize fonts
    void init_fonts();
    // Cleanup fonts
    void cleanup_fonts();
};

#endif // MENU_H