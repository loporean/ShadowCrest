// sprite.h

#ifndef SPRITE_H
#define SPRITE_H

#include "../SDL2/SDL.h"

enum struct Direction { LEFT, RIGHT };

struct Sprite {
    SDL_Texture* State;
    SDL_Texture* Idle;
    SDL_Texture* Run;
    SDL_Texture* Jump;
    SDL_Texture* Fall;
    SDL_Texture* Attack1;
    SDL_Rect spriteRect;
    SDL_Rect destRect;
    Direction direction;
    float vx;
    float vy;
    bool ground;
    bool jumping;

    int numSprites;
    int numIdleSprites;
    int numRunSprites;
    int numJumpSprites;
    int numFallSprites;
    int numAttack1Sprites;

    // Constructor
    Sprite();
    // reset
    void reset();
};

#endif // SPRITE_H