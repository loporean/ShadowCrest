// anim.h

#ifndef ANIM_H
#define ANIM_H

#include "../SDL2/SDL.h"

struct Anim {
    SDL_Texture* fire[5];
    SDL_Texture* fireStart[5];
    SDL_Texture* Orange;
    SDL_Texture* OrangeStart;
    SDL_Texture* Blue;
    SDL_Texture* BlueStart;
    SDL_Texture* Green;
    SDL_Texture* GreenStart;
    SDL_Texture* Purple;
    SDL_Texture* PurpleStart;
    SDL_Texture* White;
    SDL_Texture* WhiteStart;
    SDL_Rect spriteRect;
    SDL_Rect destRect1;
    SDL_Rect destRect2;

    int numSprites;
    int fireStartIndex;
    int currentSpriteIndex;
    int frameDelay;
    int frameCounter;

    // Constructor
    Anim();
    // Reset
    void reset();
};

#endif // ANIM_H