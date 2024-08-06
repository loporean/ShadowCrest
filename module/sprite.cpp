// sprite.cpp

#include "../header/sprite.h"

// extern allows variables defined in main to be used here
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern Sprite sprite;

Sprite::Sprite() {
    State = nullptr;

    spriteRect.x = 54; // top X
    spriteRect.y = 30; // 48 - top Y
    spriteRect.w = 135; // Width
    spriteRect.h = 135; // Height

    // destRect.x = 200; // x-axis render destination
    destRect.x = (WINDOW_WIDTH / 2) - spriteRect.w + 30; // x-axis render destination
    destRect.y = WINDOW_HEIGHT - 250; // y-axis render destination
    // destRect.w = 200;
    // destRect.h = 200;
    destRect.w = WINDOW_WIDTH / 4.7;
    destRect.h = WINDOW_HEIGHT / 2.9;

    numIdleSprites =  1350 / spriteRect.w;
    numRunSprites = 810 / spriteRect.w;
    numJumpSprites = 270 / spriteRect.w;
    numFallSprites = 270 / spriteRect.w;
    numAttack1Sprites = 540 / spriteRect.w;

    direction = Direction::RIGHT;

    vx = 0;
    vy = 0;

    ground = false;
    jumping = false;
}
void Sprite::reset() {
    spriteRect.x = 54; // top X
    spriteRect.y = 30; // 48 - top Y
    spriteRect.w = 135; // Width
    spriteRect.h = 135; // Height

    // destRect.x = 200; // x-axis render destination
    destRect.x = (WINDOW_WIDTH / 2) - spriteRect.w + 30; // x-axis render destination
    destRect.y = WINDOW_HEIGHT - 250; // y-axis render destination
    // destRect.w = 200;
    // destRect.h = 200;
    destRect.w = WINDOW_WIDTH / 4.7;
    destRect.h = WINDOW_HEIGHT / 2.9;

    // Window
    // width 940
    // height 580

    numIdleSprites =  1350 / spriteRect.w;
    numRunSprites = 810 / spriteRect.w;
    numJumpSprites = 270 / spriteRect.w;
    numFallSprites = 270 / spriteRect.w;
    numAttack1Sprites = 540 / spriteRect.w;
}