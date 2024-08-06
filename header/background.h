// background.h

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "../SDL2/SDL.h"

struct Background {
    SDL_Texture* gameBackground;
    SDL_Texture* clouds;
    SDL_Texture* ground;
    SDL_Texture* trees;
    SDL_Texture* far_trees;
    int groundX;
    int treesX;
    int farTreesX;
    int cloudsX;

    // Constructor
    Background();
};

#endif // BACKGROUND_H