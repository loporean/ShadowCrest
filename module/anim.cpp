// anim.cpp

#include "../header/anim.h"

// extern allows variables defined in main to be used here
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
// extern int BUTTON_WIDTH;
// extern int BUTTON_HEIGHT;

// Define extern instance of Anim
extern Anim fire;


Anim::Anim() {
    spriteRect.x = 6; // top X
    spriteRect.y = 9; // top Y
    spriteRect.w = 24; // Width -> 24
    spriteRect.h = 22; // Height

    // Left flame
    destRect1.x = 200; // x-axis render destination
    destRect1.y = WINDOW_HEIGHT - 150; // y-axis render destination
    destRect1.w = 80;
    destRect1.h = 80;

    // Right flame
    destRect2.x = WINDOW_WIDTH - 290; // x-axis render destination
    destRect2.y = WINDOW_HEIGHT - 150; // y-axis render destination
    destRect2.w = 80;
    destRect2.h = 80;

    numSprites = 6;
    fireStartIndex = 0;
    currentSpriteIndex = 0;
    frameDelay = 7; // Adjust the delay between frames
    frameCounter = 0;
}
void Anim::reset() {
    spriteRect.x = 6; // top X
    spriteRect.y = 9; // top Y
    spriteRect.w = 24; // Width -> 24
    spriteRect.h = 22; // Height

    // Left flame
    destRect1.x = 200; // x-axis render destination
    destRect1.y = WINDOW_HEIGHT - 150; // y-axis render destination
    destRect1.w = 80;
    destRect1.h = 80;

    // Right flame
    destRect2.x = WINDOW_WIDTH - 290; // x-axis render destination
    destRect2.y = WINDOW_HEIGHT - 150; // y-axis render destination
    destRect2.w = 80;
    destRect2.h = 80;

    numSprites = 6;
    fireStartIndex = 0;
    currentSpriteIndex = 0;
    frameDelay = 7; // Adjust the delay between frames
    frameCounter = 0;
}
