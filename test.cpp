/*
 * NAME: Spencer Denney
 */ 

// Libraries
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include <iostream>
#include <ctime>
#include <vector>
#include <bits/stdc++.h> 
// Header files
#include "header/sdl_init.h" 
#include "header/menu.h"
#include "header/anim.h"
#include "header/sprite.h"
#include "header/background.h"

int isFullScreen = false;
int WINDOW_WIDTH = 940;
int WINDOW_HEIGHT = 580;
int BUTTON_WIDTH = 150;
int BUTTON_HEIGHT = 50;
#define BOX_SIZE WINDOW_HEIGHT / 4
#define MOVE_STEP 5.0
#define JUMP_VELOCITY -10.0
#define GRAVITY 0.5

// struct for window
Window sdl;
// struct for menu
Menu menu;
// struct for fire animation
Anim fire;
// struct for sprite
Sprite sprite;
// struct for background
Background bg;

struct Box {
    float x;
    float y;
    float vx;
    float vy;
    float ax;
    float ay;
    int size;

    Box() {
        size = BOX_SIZE;
        x = WINDOW_HEIGHT/2;
        // y = WINDOW_HEIGHT/2;
        y = WINDOW_HEIGHT - BOX_SIZE;
        vx = 0;
        vy = 0;
    }

} box;

struct Global {
    bool hoverStartButton;
    bool hoverQuitButton;
    bool hoverContButton;
    bool hoverSettingsButton;
    bool hoverBackButton;
    bool hoverApplyButton;
    bool hoverSetWinButton;
    bool hoverFullButton;
    bool hoverWinButton;
    bool hoverFpsButton;
    bool titleMenuReady;
    int randFireGen;
    std::vector<float> fps;
    Uint64 startFPS;
    Uint64 endFPS;
    int avgFPS;
    bool fullscreen;
    Global() {
        hoverStartButton = false;
        hoverQuitButton = false;
        hoverContButton = false;
        hoverSettingsButton = false;
        hoverBackButton = false;
        hoverApplyButton = false;
        hoverSetWinButton = false;
        hoverFullButton = false;
        hoverWinButton = false;
        hoverFpsButton = false;
        titleMenuReady = false;
        randFireGen = 0;
        avgFPS = 0;
        fullscreen = false;
    };

} g;

// Prototypes
SDL_Texture* loadTexture(const std::string&);
void init_background(void); 
void init_sprite(void);
void renderSprite(int&, SDL_Texture*&);
void updateSprite(void);
void handleInput(bool&, bool&, int&, const int);
Uint32 timerCallback(Uint32, void*);
void init_title_screen(void);
void titleScreen(void);
void pauseScreen(void);
void menuHandleInput(bool&, bool&, bool&);
void calculate_fps(void);

/*  TO DO:
    -Implement attack2
    -Watch video on handling texture clipping
    -If sprite vx is 0 and on ground, then convert to running
*/

// Load sprite
SDL_Texture* loadTexture(const std::string& filePath) {
    SDL_Surface* surface = IMG_Load(filePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl.renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

// Render box as a floor
void renderBox() {
    SDL_Rect boxRect = {0, static_cast<int>(box.y), WINDOW_WIDTH, BOX_SIZE};
    SDL_SetRenderDrawColor(sdl.renderer, 0, 0, 0, 0); 
    SDL_RenderFillRect(sdl.renderer, &boxRect);
}

// Initialize game backgrounds
void init_background() {
     // Load the background image for game
    bg.gameBackground = loadTexture("./image/background assets/11_background.png");
    if (!bg.gameBackground) {
        std::cerr << "Failed to load background image." << std::endl;
        return;
    }

    // Load sky background
    bg.clouds = loadTexture("./image/background assets/08_clouds.png");
    if (!bg.clouds) {
        std::cerr << "Failed to load cloud image." << std::endl;
        return;
    }

    // Load ground background
    bg.ground = loadTexture("./image/background assets/01_ground.png");
    if (!bg.ground) {
        std::cerr << "Failed to load ground image." << std::endl;
        return;
    }

    // Load trees background
    bg.trees = loadTexture("./image/background assets/02_trees and bushes.png");
    if (!bg.trees) {
        std::cerr << "Failed to load trees image." << std::endl;
        return;
    }

    // Load far trees background
    bg.far_trees = loadTexture("./image/background assets/03_distant_trees.png");
    if (!bg.far_trees) {
        std::cerr << "Failed to load far trees image." << std::endl;
        return;
    }
}

// Initialize Sprite
void init_sprite() {

    // Load Idle sprite sheet
    sprite.Idle = loadTexture("./Sprites/Idle.png");
    if (!sprite.Idle) {
        std::cerr << "Failed to load sprite sheet." << std::endl;
        return;
    }

    // Load Run sprite sheet
    sprite.Run = loadTexture("./Sprites/Run.png");
    if (!sprite.Run) {
        std::cerr << "Failed to load sprite sheet." << std::endl;
        return;
    }

    // Load Jump sprite sheet
    sprite.Jump = loadTexture("./Sprites/Jump.png");
    if (!sprite.Jump) {
        std::cerr << "Failed to load sprite sheet." << std::endl;
        return;
    }

    // Load Fall sprite sheet
    sprite.Fall = loadTexture("./Sprites/Fall.png");
    if (!sprite.Fall) {
        std::cerr << "Failed to load sprite sheet." << std::endl;
        return;
    }

    // Load Attack1 sprite sheet
    sprite.Attack1 = loadTexture("./Sprites/Attack1.png");
    if (!sprite.Attack1) {
        std::cerr << "Failed to load sprite sheet." << std::endl;
        return;
    }

    // Default sprite state
    sprite.State = sprite.Idle;
    sprite.numSprites = sprite.numIdleSprites;
}

// Render Sprite
void renderSprite(int& currentSpriteIndex, SDL_Texture*& spriteSheet) {

    // Clear the screen
    SDL_RenderClear(sdl.renderer);

    // Ground
    // renderBox();

    // Display background image
    SDL_RenderCopy(sdl.renderer, bg.gameBackground, NULL, NULL);

    // clouds
    SDL_Rect cloudsRectLeft = { bg.cloudsX - WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect cloudsRect = { bg.cloudsX, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect cloudsRectRight = { bg.cloudsX + WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy(sdl.renderer, bg.clouds, NULL, &cloudsRectLeft);
    SDL_RenderCopy(sdl.renderer, bg.clouds, NULL, &cloudsRect);
    SDL_RenderCopy(sdl.renderer, bg.clouds, NULL, &cloudsRectRight);

    // far trees
    SDL_Rect far_treesRectLeft = { bg.farTreesX - WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect far_treesRect = { bg.farTreesX, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect far_treesRectRight = { bg.farTreesX + WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy(sdl.renderer, bg.far_trees, NULL, &far_treesRectLeft);
    SDL_RenderCopy(sdl.renderer, bg.far_trees, NULL, &far_treesRect);
    SDL_RenderCopy(sdl.renderer, bg.far_trees, NULL, &far_treesRectRight);

    // trees
    SDL_Rect treesRectLeft = { bg.treesX - WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect treesRect = { bg.treesX, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect treesRectRight = { bg.treesX + WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy(sdl.renderer, bg.trees, NULL, &treesRectLeft);
    SDL_RenderCopy(sdl.renderer, bg.trees, NULL, &treesRect);
    SDL_RenderCopy(sdl.renderer, bg.trees, NULL, &treesRectRight);

    // ground
    SDL_Rect groundRectLeft = { bg.groundX - WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect groundRect = { bg.groundX, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_Rect groundRectRight = { bg.groundX + WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    SDL_RenderCopy(sdl.renderer, bg.ground, NULL, &groundRectLeft);
    SDL_RenderCopy(sdl.renderer, bg.ground, NULL, &groundRect);
    SDL_RenderCopy(sdl.renderer, bg.ground, NULL, &groundRectRight);

    // Update the background position for the scrolling effect
    // bgX -= SCROLL_SPEED;
    // Background scrolling speed based on sprite movement
    // if (sprite.destRect.x <= 100 || sprite.destRect.x + sprite.spriteRect.w >= WINDOW_WIDTH + (-185)) {
    //     groundX -= sprite.vx;
    //     treesX -= sprite.vx * 0.5;
    //     farTreesX -= sprite.vx * 0.2;
    //     cloudsX -= sprite.vx * 0.2;
    // }

    bg.groundX -= sprite.vx * 1.2;
    bg.treesX -= sprite.vx * 0.5;
    bg.farTreesX -= sprite.vx * 0.2;
    bg.cloudsX -= sprite.vx * 0.2;

    // If the position of the image reaches the 
    // end of the image, then repeat the image
    // Ground
    if (bg.groundX <= -WINDOW_WIDTH || bg.groundX >= WINDOW_WIDTH) {
        bg.groundX = 0;
    }
    // Trees
    if (bg.treesX <= -WINDOW_WIDTH || bg.treesX >= WINDOW_WIDTH) {
        bg.treesX = 0;
    }
    // Far Trees
    if (bg.farTreesX <= -WINDOW_WIDTH || bg.farTreesX >= WINDOW_WIDTH) {
        bg.farTreesX = 0;
    }
    // clouds
    if (bg.cloudsX <= -WINDOW_WIDTH || bg.cloudsX >= WINDOW_WIDTH) {
        bg.cloudsX = 0;
    }

    if (menu.showFPS) {
        // show fps
        menu.textColor = { 255, 255, 255 }; // white
        std::stringstream fps;
        fps << "fps: " << g.avgFPS;
        menu.dispModeSurface = TTF_RenderText_Solid(menu.font, fps.str().c_str(), menu.textColor);
        if (!menu.dispModeSurface) {
            return;
        }
        menu.dispModeTexture = SDL_CreateTextureFromSurface(sdl.renderer, menu.dispModeSurface);
        if (!menu.dispModeTexture) {
            SDL_FreeSurface(menu.dispModeSurface);
        }
        SDL_QueryTexture(menu.dispModeTexture, NULL, NULL, &menu.textWidth, &menu.textHeight);
        menu.dispModeRect = { 10, 10, menu.textWidth, menu.textHeight };
        SDL_RenderCopy(sdl.renderer, menu.dispModeTexture, NULL, &menu.dispModeRect);
    }


    // Render the sprite
    sprite.spriteRect.x = currentSpriteIndex * sprite.spriteRect.w;
    SDL_RendererFlip flip = SDL_FLIP_NONE; //Initialize to default render

    // Determine flip
    if (sprite.direction == Direction::LEFT) {
        flip = SDL_FLIP_HORIZONTAL; // Mirror
    } else {
        flip = SDL_FLIP_NONE; // Default
    }

    SDL_RenderCopyEx(sdl.renderer, spriteSheet, &sprite.spriteRect, &sprite.destRect, 0, NULL, flip);
    SDL_RenderPresent(sdl.renderer);

}

// Update Sprite
void updateSprite() {
    // Gravity
    sprite.vy += GRAVITY;

    // Update sprite position based on velocity
    // sprite.destRect.x += sprite.vx;
    sprite.destRect.y += sprite.vy;

    // Offset (to handle texture clipping problem)
    // int offsetLeft = -80;
    // int offsetRight = 15;
    int offsetLeft = 100;
    int offsetRight = -185;
    // int offsetUp = 0;
    // int offsetDown = 48;
    // temporary fix to reposition sprite when in fullscreen
    int offsetDown = 0;
    if (!isFullScreen) {
        offsetDown = -105;
    } else {
        offsetDown = -300;
    }

    //+++++++++++++++++++++++
    // Put the offsets in the 
    // sprite struct to control
    // in the rendering for
    // background
    //+++++++++++++++++++++++

    // Collision with ground
    if (sprite.destRect.y >= WINDOW_HEIGHT - sprite.spriteRect.h + offsetDown) {
        sprite.destRect.y = WINDOW_HEIGHT - sprite.spriteRect.h + offsetDown;
        sprite.vy = 0; // Stop vertical velocity when touching the ground
        sprite.ground = true;

        // If jumping, switch to running or idle state based on hrizontal velocity
        if (sprite.jumping) {
            sprite.jumping = false;
            if (sprite.vx != 0) {
                sprite.State = sprite.Run;
                sprite.numSprites = sprite.numRunSprites;
            } else {
                sprite.State = sprite.Idle;
                sprite.numSprites = sprite.numIdleSprites;
            }   
        }
    }

    // If sprite is in air and not jumping, 
    // set jumping state and change sprite to jump state
    if (!sprite.ground && !sprite.jumping) {
        sprite.jumping = true;
        sprite.State = sprite.Jump;
        sprite.numSprites = sprite.numJumpSprites;
    }

    // If sprite is falling, set Fall state
    if (!sprite.ground && sprite.vy > 0) {
        sprite.State = sprite.Fall;
        sprite.numSprites = sprite.numFallSprites;
    }

    // Keep the box within the window bounds
    if (sprite.destRect.x < offsetLeft)
        sprite.destRect.x = offsetLeft;
    else if (sprite.destRect.x + sprite.spriteRect.w > WINDOW_WIDTH + offsetRight)
        sprite.destRect.x = WINDOW_WIDTH - sprite.spriteRect.w + offsetRight;

}

// Input handling
void handleInput(bool& done, bool& play, int& currentSpriteIndex, const int numSprites) {
    static bool spacePressed = false;
    static bool attackInProgress = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            done = true;
            break;
        // On Key Down
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                // Implement jumping
                // Only jump when on ground
                if (box.y == WINDOW_HEIGHT - box.size) {
                    box.vy += JUMP_VELOCITY;
                }
                // sprite.vy = MOVE_STEP;
                if (sprite.ground) {
                    sprite.vy += JUMP_VELOCITY;
                    // Sprite state
                    sprite.State = sprite.Jump;
                    sprite.numSprites = sprite.numJumpSprites;
                    sprite.ground = false;
                }
                break;
            case SDLK_DOWN:
                box.vy = MOVE_STEP;
                sprite.vy = MOVE_STEP;
                break;
            case SDLK_LEFT:
                box.vx = -MOVE_STEP;
                sprite.vx = -MOVE_STEP;
                sprite.direction = Direction::LEFT;
                // Sprite state
                sprite.State = sprite.Run;
                sprite.numSprites = sprite.numRunSprites;
                break;
            case SDLK_RIGHT:
                box.vx = MOVE_STEP;
                sprite.vx = MOVE_STEP;
                sprite.direction = Direction::RIGHT;
                // Sprite state
                sprite.State = sprite.Run;
                sprite.numSprites = sprite.numRunSprites;
                break;
            case SDLK_SPACE:
                // Sprite state
                // ***When space is pressed, need to perform a slower animation to show attack
                // and shouldnt be replicated by holding down space bar***
                if (!spacePressed && !attackInProgress) {
                    sprite.State = sprite.Attack1;
                    sprite.numSprites = sprite.numAttack1Sprites;
                    currentSpriteIndex = 0;
                    spacePressed = true;
                    attackInProgress = true;
                }
                break;
            case SDLK_ESCAPE:
                play = false;
                break;
            default:
                break;
            }
            break;
        // On Key Up
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                break;
            case SDLK_DOWN:
                break;
            case SDLK_LEFT:
                if (box.vx < 0)
                    box.vx = 0;
                if (sprite.vx < 0) {
                    sprite.vx = 0;
                    // Sprite state
                    sprite.State = sprite.Idle;
                    sprite.numSprites = sprite.numIdleSprites;
                }
                break;
            case SDLK_RIGHT:
                if (box.vx > 0)
                    box.vx = 0;
                if (sprite.vx > 0) {
                    sprite.vx = 0;
                    // Sprite state
                    sprite.State = sprite.Idle;
                    sprite.numSprites = sprite.numIdleSprites;
                }
                break;
            case SDLK_SPACE:
                // Sprite state
                sprite.State = sprite.Idle;
                sprite.numSprites = sprite.numIdleSprites;
                spacePressed = false;
                break;
            case SDLK_f:
                // toggle Fullscreen
                toggleFullScreen();
                // Update the Window width and height
                SDL_GetWindowSize(sdl.window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
                // Reinitialize menu, fire animation, sprite
                menu.reset();
                fire.reset();
                sprite.reset();
                break;
            case SDLK_ESCAPE:
                break;
            default:
                break;
            }
        case SDL_USEREVENT:
            // Check if attack animation is in progress
            if (attackInProgress) {
                // If it's the last sprite of the attack animation, transition to idle
                if (currentSpriteIndex == sprite.numAttack1Sprites - 1) {
                    sprite.State = sprite.Idle;
                    sprite.numSprites = sprite.numIdleSprites;
                    attackInProgress = false; // Reset attackInProgress flag
                }
            }
            currentSpriteIndex = (currentSpriteIndex + 1) % numSprites;
            break;
        default:
            break;
        }
    }
}

// Callback function for timer
Uint32 timerCallback(Uint32 interval, void* param) {
    SDL_Event event;
    event.type = SDL_USEREVENT;
    SDL_PushEvent(&event);
    return interval;
}

// Title Screen
void titleScreen() {
    static int alpha = 0;
    static int alphastart = 0;
    static int alphaquit = 0;
    static int alphasettings = 0;
    const float fadespeedName = 1.75;
    const float fadespeedButtons = 3.0;

    // Clear the screen
    SDL_RenderClear(sdl.renderer);

    // Display background image
    SDL_RenderCopy(sdl.renderer, menu.titleBackground, NULL, NULL);

    // Fade-in animation
    SDL_SetTextureAlphaMod(menu.titleName, alpha);
    SDL_SetTextureAlphaMod(menu.start, alphastart);
    SDL_SetTextureAlphaMod(menu.settings, alphasettings);
    SDL_SetTextureAlphaMod(menu.quit, alphaquit);

    // Fade-in handling
    // update alpha for title
    alpha += fadespeedName;
    if (alpha > 255) {
        alpha = 255;
    }
    // update alpha for start button
    if (alpha == 255) {
        alphastart += fadespeedButtons;
        if (alphastart > 255) {
            alphastart = 255;
        }
    }
    // update alpha for quit button
    if (alphastart == 255) {
        alphasettings += fadespeedButtons;
        if (alphasettings > 255) {
            alphasettings = 255;
        }
    }
    // update alpha for settings button
    if (alphasettings == 255) {
        alphaquit += fadespeedButtons;
        if (alphaquit > 255) {
            alphaquit = 255;
            // Menu ready
            g.titleMenuReady = true;
        }
    }
    // Skip intro if Enter is pressed before intro finishes
    if (g.titleMenuReady) {
        alpha = 255;
        alphastart = 255;
        alphasettings = 255;
        alphaquit = 255;
    }

    // display title name
    SDL_RenderCopy(sdl.renderer, menu.titleName, NULL, &menu.titleNameRect);

    // *** Render the menu ***
    // Start button
    SDL_RenderCopy(sdl.renderer, menu.start, NULL, &menu.startButtonRect);

    if (g.hoverStartButton) {
        // Render glowing version of the play button
        SDL_SetTextureColorMod(menu.start, 255, 255, 255); // Set color to white
        SDL_RenderCopy(sdl.renderer, menu.start, NULL, &menu.startButtonRect);
    } else {
        // Render normal version of the play button
        SDL_SetTextureColorMod(menu.start, 200, 200, 200); // Set color to gray
        SDL_RenderCopy(sdl.renderer, menu.start, NULL, &menu.startButtonRect);
    }

    // Quit button
    SDL_RenderCopy(sdl.renderer, menu.quit, NULL, &menu.quitButtonRect);

    if (g.hoverQuitButton) {
        // Render glowing version of the quit button
        SDL_SetTextureColorMod(menu.quit, 255, 255, 255); // Set color to white
        SDL_RenderCopy(sdl.renderer, menu.quit, NULL, &menu.quitButtonRect);
    } else {
        // Render normal version of the quit button
        SDL_SetTextureColorMod(menu.quit, 200, 200, 200); // Set color to gray
        SDL_RenderCopy(sdl.renderer, menu.quit, NULL, &menu.quitButtonRect);
    }

    // Settings button
    SDL_RenderCopy(sdl.renderer, menu.settings, NULL, &menu.settingsButtonRect);

    if (g.hoverSettingsButton) {
        // Render glowing version of the settings button
        SDL_SetTextureColorMod(menu.settings, 255, 255, 255); // Set color to white
        SDL_RenderCopy(sdl.renderer, menu.settings, NULL, &menu.settingsButtonRect);
    } else {
        // Render normal version of the settings button
        SDL_SetTextureColorMod(menu.settings, 200, 200, 200); // Set color to gray
        SDL_RenderCopy(sdl.renderer, menu.settings, NULL, &menu.settingsButtonRect);
    }

    // Fire start
    // After menu is done loading, start fire animation
    if (g.titleMenuReady) {
        if (fire.fireStartIndex < 6) {
            // Render the fire animation1
            fire.spriteRect.x = fire.fireStartIndex * fire.spriteRect.w;
            SDL_RenderCopy(sdl.renderer, fire.fireStart[g.randFireGen], &fire.spriteRect, &fire.destRect1);
            // Render the fire animation2
            SDL_RenderCopy(sdl.renderer, fire.fireStart[g.randFireGen], &fire.spriteRect, &fire.destRect2);
            fire.fireStartIndex++;
        } else {
            // Render the fire animation1
            fire.spriteRect.x = fire.currentSpriteIndex * fire.spriteRect.w;
            SDL_RenderCopy(sdl.renderer, fire.fire[g.randFireGen], &fire.spriteRect, &fire.destRect1);
            // Render the fire animation2
            SDL_RenderCopy(sdl.renderer, fire.fire[g.randFireGen], &fire.spriteRect, &fire.destRect2);
            // Update the frame counter and index
            fire.frameCounter++;
            if (fire.frameCounter >= fire.frameDelay) {
                fire.frameCounter = 0;
                fire.currentSpriteIndex = (fire.currentSpriteIndex + 1) % fire.numSprites;
            }
        }
    }

    // Present the rendered frame
    SDL_RenderPresent(sdl.renderer);
}

// Pause Screen
void pauseScreen() {
    // Clear the screen
    SDL_RenderClear(sdl.renderer);

    // Display background image
    SDL_RenderCopy(sdl.renderer, menu.titleBackground, NULL, NULL);

    // *** Render the menu ***
    // Continue button
    SDL_RenderCopy(sdl.renderer, menu.cont, NULL, &menu.contButtonRect);

    if (g.hoverContButton) {
        // Render glowing version of the continue button
        SDL_SetTextureColorMod(menu.cont, 255, 255, 255); // Set color to white
        SDL_RenderCopy(sdl.renderer, menu.cont, NULL, &menu.contButtonRect);
    } else {
        // Render normal version of the continue button
        SDL_SetTextureColorMod(menu.cont, 200, 200, 200); // Set color to gray
        SDL_RenderCopy(sdl.renderer, menu.cont, NULL, &menu.contButtonRect);
    }

    // Settings button
    SDL_RenderCopy(sdl.renderer, menu.settings, NULL, &menu.settingsButtonRect);
    if (g.hoverSettingsButton) {
        // Render glowing version of the settings button
        SDL_SetTextureColorMod(menu.settings, 255, 255, 255); // Set color to white
        SDL_RenderCopy(sdl.renderer, menu.settings, NULL, &menu.settingsButtonRect);
    } else {
        // Render normal version of the settings button
        SDL_SetTextureColorMod(menu.settings, 200, 200, 200); // Set color to gray
        SDL_RenderCopy(sdl.renderer, menu.settings, NULL, &menu.settingsButtonRect);
    }

    // Quit button
    SDL_RenderCopy(sdl.renderer, menu.quit, NULL, &menu.quitButtonRect);

    if (g.hoverQuitButton) {
        // Render glowing version of the play button
        SDL_SetTextureColorMod(menu.quit, 255, 255, 255); // Set color to white
        SDL_RenderCopy(sdl.renderer, menu.quit, NULL, &menu.quitButtonRect);
    } else {
        // Render normal version of the play button
        SDL_SetTextureColorMod(menu.quit, 200, 200, 200); // Set color to gray
        SDL_RenderCopy(sdl.renderer, menu.quit, NULL, &menu.quitButtonRect);
    }

    // Present the rendered frame
    SDL_RenderPresent(sdl.renderer);
}

// Menu input handling
void menuHandleInput(bool& play, bool& done, bool& set) {
    SDL_Event event;

    // Declare variables outside the switch statement
    int mouseX, mouseY, buttonStartX, buttonStartY, buttonQuitX, buttonQuitY, buttonContX, buttonContY, buttonSettingsX, buttonSettingsY;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            play = true;
            done = true;
            break;
        case SDL_MOUSEBUTTONDOWN:

            // check if button animation has finished before clicking
            if (g.titleMenuReady) {
                // check if mouse clicks on button
                // Start button
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseX = event.button.x;
                    mouseY = event.button.y;
                    buttonStartX = menu.startButtonRect.x;
                    buttonStartY = menu.startButtonRect.y;
                    if (mouseX >= buttonStartX && mouseX <= buttonStartX + BUTTON_WIDTH &&
                        mouseY >= buttonStartY && mouseY <= buttonStartY + BUTTON_HEIGHT) {
                            play = true;
                    }
                }
                // Settings button
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseX = event.button.x;
                    mouseY = event.button.y;
                    buttonSettingsX = menu.settingsButtonRect.x;
                    buttonSettingsY = menu.settingsButtonRect.y;
                    if (mouseX >= buttonSettingsX && mouseX <= buttonSettingsX + BUTTON_WIDTH &&
                        mouseY >= buttonSettingsY && mouseY <= buttonSettingsY + BUTTON_HEIGHT) {
                            set = false;
                    }
                }   
                // Quit button
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouseX = event.button.x;
                    mouseY = event.button.y;
                    buttonQuitX = menu.quitButtonRect.x;
                    buttonQuitY = menu.quitButtonRect.y;
                    if (mouseX >= buttonQuitX && mouseX <= buttonQuitX + BUTTON_WIDTH &&
                        mouseY >= buttonQuitY && mouseY <= buttonQuitY + BUTTON_HEIGHT) {
                            play = true;
                            done = true;
                    }
                }   
            } else {
                g.titleMenuReady = true;
            }
            break;
        case SDL_MOUSEMOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            buttonStartX = menu.startButtonRect.x;
            buttonStartY = menu.startButtonRect.y;
            buttonQuitX = menu.quitButtonRect.x;
            buttonQuitY = menu.quitButtonRect.y;
            buttonContX = menu.contButtonRect.x;
            buttonContY = menu.contButtonRect.y;
            buttonSettingsX = menu.settingsButtonRect.x;
            buttonSettingsY = menu.settingsButtonRect.y;

            // Check if the mouse cursor is over Start button
            g.hoverStartButton = (mouseX >= buttonStartX && mouseX <= buttonStartX + BUTTON_WIDTH &&
                                    mouseY >= buttonStartY && mouseY <= buttonStartY + BUTTON_HEIGHT);

            // Check if the mouse cursor is over Continue button
            g.hoverContButton = (mouseX >= buttonContX && mouseX <= buttonContX + BUTTON_WIDTH &&
                                    mouseY >= buttonContY && mouseY <= buttonContY + BUTTON_HEIGHT);

            // Check if the mouse cursor is over Quit button
            g.hoverQuitButton = (mouseX >= buttonQuitX && mouseX <= buttonQuitX + BUTTON_WIDTH &&
                                    mouseY >= buttonQuitY && mouseY <= buttonQuitY + BUTTON_HEIGHT);

            // Check if the mouse cursor is over Settings button
            g.hoverSettingsButton = (mouseX >= buttonSettingsX && mouseX <= buttonSettingsX + BUTTON_WIDTH &&
                                    mouseY >= buttonSettingsY && mouseY <= buttonSettingsY + BUTTON_HEIGHT);

            break;
	    case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                // play = true;
                break;
            case SDLK_RETURN:
                // play = true;
                if (!g.titleMenuReady) {
                    g.titleMenuReady = true;
                } else {
                    play = true;
                }
                break;
            case SDLK_f:
                // toggle Fullscreen
                toggleFullScreen();
                // Update the Window width and height
                SDL_GetWindowSize(sdl.window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
                // Reinitialize menu, fire animation, sprite 
                menu.reset();
                fire.reset();
                sprite.reset();
                break;
            case SDLK_ESCAPE:
                // play = true;
                // done = true;
                // If Esc pressed in pause menu
                // close pause menu
                if (g.titleMenuReady) {
                    play = true;
                }
                break;
            default:
                break;
	        }  
	    default:
	        break;
	    }
    }
}

// Render Settings screen
void settingsScreen() {

    // Clear the screen
    SDL_RenderClear(sdl.renderer);

    // Display background image
    SDL_RenderCopy(sdl.renderer, menu.titleBackground, NULL, NULL);

    // Render Settings text
    // Dispaly Mode 
    menu.dispModeSurface = TTF_RenderText_Solid(menu.font, "Display Mode", menu.textColor);
    if (!menu.dispModeSurface) {
        return;
    }
    menu.dispModeTexture = SDL_CreateTextureFromSurface(sdl.renderer, menu.dispModeSurface);
    if (!menu.dispModeTexture) {
        SDL_FreeSurface(menu.dispModeSurface);
    }
    SDL_QueryTexture(menu.dispModeTexture, NULL, NULL, &menu.textWidth, &menu.textHeight);
    menu.dispModeRect = { WINDOW_WIDTH/2 - 200, menu.titleNameRect.y + 60, menu.textWidth, menu.textHeight };
    SDL_RenderCopy(sdl.renderer, menu.dispModeTexture, NULL, &menu.dispModeRect);
    // display windowed or fullscreen
    if (menu.setWindow) {
        // Show both options and highlight when hovered
        // windowed
        menu.windowedSurface = TTF_RenderText_Solid(menu.font, "windowed", menu.textColor);
        if (!menu.windowedSurface) {
            return;
        }
        menu.windowedTexture = SDL_CreateTextureFromSurface(sdl.renderer, menu.windowedSurface);
        if (!menu.windowedTexture) {
            SDL_FreeSurface(menu.windowedSurface);
        }
        SDL_QueryTexture(menu.windowedTexture, NULL, NULL, &menu.textWidth, &menu.textHeight);
        menu.windowedRect = { WINDOW_WIDTH/2 + 100, menu.titleNameRect.y + 50, menu.textWidth, menu.textHeight };
        SDL_RenderCopy(sdl.renderer, menu.windowedTexture, NULL, &menu.windowedRect);

        // Define the dimensions and position for the box
        int boxPadding = 5; // Padding around the text
        SDL_Rect boxWinRect = {
            menu.windowedRect.x - boxPadding,
            menu.windowedRect.y - boxPadding,
            menu.windowedRect.w + 2 * boxPadding,
            menu.windowedRect.h + 2 * boxPadding
        };
        // Set the color of the box
        SDL_SetRenderDrawColor(sdl.renderer, 255, 255, 255, 255); // White color (you can change this)
        // Render the box
        SDL_RenderDrawRect(sdl.renderer, &boxWinRect);
        // Render the text
        SDL_RenderCopy(sdl.renderer, menu.windowedTexture, NULL, &menu.windowedRect);

        // Hover
        if (g.hoverWinButton) {
            // Render glowing version of the back button
            SDL_SetTextureColorMod(menu.windowedTexture, 255, 255, 255); // Set color to white
            SDL_RenderCopy(sdl.renderer, menu.windowedTexture, NULL, &menu.windowedRect);
        } else {
            // Render normal version of the back button
            SDL_SetTextureColorMod(menu.windowedTexture, 200, 200, 200); // Set color to gray
            SDL_RenderCopy(sdl.renderer, menu.windowedTexture, NULL, &menu.windowedRect);
        }
        // fullscreen
        menu.fullscreenSurface = TTF_RenderText_Solid(menu.font, "fullscreen", menu.textColor);
        if (!menu.fullscreenSurface) {
            return;
        }
        menu.fullscreenTexture = SDL_CreateTextureFromSurface(sdl.renderer, menu.fullscreenSurface);
        if (!menu.fullscreenTexture) {
            SDL_FreeSurface(menu.fullscreenSurface);
        }
        SDL_QueryTexture(menu.fullscreenTexture, NULL, NULL, &menu.textWidth, &menu.textHeight);
        menu.fullscreenRect = { WINDOW_WIDTH/2 + 100, menu.titleNameRect.y + 80, menu.textWidth, menu.textHeight };
        SDL_RenderCopy(sdl.renderer, menu.fullscreenTexture, NULL, &menu.fullscreenRect);

        // Define the dimensions and position for the box
        SDL_Rect boxFullRect = {
            menu.fullscreenRect.x - boxPadding,
            menu.fullscreenRect.y - boxPadding,
            menu.windowedRect.w + 2 * boxPadding,
            menu.fullscreenRect.h + 2 * boxPadding
        };
        // Set the color of the box
        SDL_SetRenderDrawColor(sdl.renderer, 255, 255, 255, 255); // White color (you can change this)
        // Render the box
        SDL_RenderDrawRect(sdl.renderer, &boxFullRect);
        // Render the text
        SDL_RenderCopy(sdl.renderer, menu.windowedTexture, NULL, &menu.windowedRect);
        
        if (g.hoverFullButton) {
            // Render glowing version
            SDL_SetTextureColorMod(menu.fullscreenTexture, 255, 255, 255); // Set color to white
            SDL_RenderCopy(sdl.renderer, menu.fullscreenTexture, NULL, &menu.fullscreenRect);
        } else {
            // Render normal version
            SDL_SetTextureColorMod(menu.fullscreenTexture, 200, 200, 200); // Set color to gray
            SDL_RenderCopy(sdl.renderer, menu.fullscreenTexture, NULL, &menu.fullscreenRect);
        }
    } else {
        if (!isFullScreen) {
            // show set option
            menu.windowedSurface = TTF_RenderText_Solid(menu.font, "windowed", menu.textColor);
            if (!menu.windowedSurface) {
                return;
            }
            menu.windowedTexture = SDL_CreateTextureFromSurface(sdl.renderer, menu.windowedSurface);
            if (!menu.windowedTexture) {
                SDL_FreeSurface(menu.windowedSurface);
            }
            SDL_QueryTexture(menu.windowedTexture, NULL, NULL, &menu.textWidth, &menu.textHeight);
            menu.windowedRect = { WINDOW_WIDTH/2 + 100, menu.titleNameRect.y + 60, menu.textWidth, menu.textHeight };
            SDL_RenderCopy(sdl.renderer, menu.windowedTexture, NULL, &menu.windowedRect);

            // Define the dimensions and position for the box
            int boxPadding = 5; // Padding around the text
            SDL_Rect boxSetWinRect = {
                menu.windowedRect.x - boxPadding,
                menu.windowedRect.y - boxPadding,
                menu.windowedRect.w + 2 * boxPadding,
                menu.windowedRect.h + 2 * boxPadding
            };
            // Set the color of the box
            SDL_SetRenderDrawColor(sdl.renderer, 255, 255, 255, 255); // White color (you can change this)
            // Render the box
            SDL_RenderDrawRect(sdl.renderer, &boxSetWinRect);
            // Render the text
            SDL_RenderCopy(sdl.renderer, menu.windowedTexture, NULL, &menu.windowedRect);

            // Hover set window
            if (g.hoverSetWinButton) {
                // render glow
                SDL_SetTextureColorMod(menu.windowedTexture, 255, 255, 255); // Set color to white
                SDL_RenderCopy(sdl.renderer, menu.windowedTexture, NULL, &menu.windowedRect);
            } else {
                // render normal
                SDL_SetTextureColorMod(menu.windowedTexture, 200, 200, 200); // Set color to gray
                SDL_RenderCopy(sdl.renderer, menu.windowedTexture, NULL, &menu.windowedRect);
            }
        } else {
            menu.fullscreenSurface = TTF_RenderText_Solid(menu.font, "fullscreen", menu.textColor);
            if (!menu.fullscreenSurface) {
                return;
            }
            menu.fullscreenTexture = SDL_CreateTextureFromSurface(sdl.renderer, menu.fullscreenSurface);
            if (!menu.fullscreenTexture) {
                SDL_FreeSurface(menu.fullscreenSurface);
            }
            SDL_QueryTexture(menu.fullscreenTexture, NULL, NULL, &menu.textWidth, &menu.textHeight);
            menu.fullscreenRect = { WINDOW_WIDTH/2 + 100, menu.titleNameRect.y + 60, menu.textWidth, menu.textHeight };
            SDL_RenderCopy(sdl.renderer, menu.fullscreenTexture, NULL, &menu.fullscreenRect);

            // Define the dimensions and position for the box
            int boxPadding = 5; // Padding around the text
            SDL_Rect boxSetWinRect = {
                menu.fullscreenRect.x - boxPadding,
                menu.fullscreenRect.y - boxPadding,
                menu.fullscreenRect.w + 2 * boxPadding,
                menu.fullscreenRect.h + 2 * boxPadding
            };
            // Set the color of the box
            SDL_SetRenderDrawColor(sdl.renderer, 255, 255, 255, 255); // White color (you can change this)
            // Render the box
            SDL_RenderDrawRect(sdl.renderer, &boxSetWinRect);
            // Render the text
            SDL_RenderCopy(sdl.renderer, menu.fullscreenTexture, NULL, &menu.fullscreenRect);

            // Hover set window
            if (g.hoverSetWinButton) {
                // Render glow
                SDL_SetTextureColorMod(menu.fullscreenTexture, 255, 255, 255); // Set color to white
                SDL_RenderCopy(sdl.renderer, menu.fullscreenTexture, NULL, &menu.fullscreenRect);
                
            } else {
                // Render normal
                SDL_SetTextureColorMod(menu.fullscreenTexture, 200, 200, 200); // Set color to gray
                SDL_RenderCopy(sdl.renderer, menu.fullscreenTexture, NULL, &menu.fullscreenRect);
            }
        }
    }

    // Toggle FPS
    menu.fpsSurface = TTF_RenderText_Solid(menu.font, "FPS", menu.textColor);
    if (!menu.fpsSurface) {
        return;
    }
    menu.fpsTexture = SDL_CreateTextureFromSurface(sdl.renderer, menu.fpsSurface);
    if (!menu.fpsTexture) {
        SDL_FreeSurface(menu.fpsSurface);
    }
    SDL_QueryTexture(menu.fpsTexture, NULL, NULL, &menu.textWidth, &menu.textHeight);
    menu.fpsRect = { WINDOW_WIDTH/2 - 200, menu.dispModeRect.y + 60, menu.textWidth, menu.textHeight };
    SDL_RenderCopy(sdl.renderer, menu.fpsTexture, NULL, &menu.fpsRect);

    // Set FPS
    if (menu.showFPS) {
        // fps On
        menu.fpsOnSurface = TTF_RenderText_Solid(menu.font, "ON", menu.textColor);
        if (!menu.fpsOnSurface) {
            return;
        }
        menu.fpsOnTexture = SDL_CreateTextureFromSurface(sdl.renderer, menu.fpsOnSurface);
        if (!menu.fpsOnTexture) {
            SDL_FreeSurface(menu.fpsOnSurface);
        }
        SDL_QueryTexture(menu.fpsOnTexture, NULL, NULL, &menu.textWidth, &menu.textHeight);
        menu.fpsOnRect = { WINDOW_WIDTH/2 + 100, menu.dispModeRect.y + 60, menu.textWidth, menu.textHeight };
        SDL_RenderCopy(sdl.renderer, menu.fpsOnTexture, NULL, &menu.fpsOnRect);

        // Hover
        if (g.hoverFpsButton) {
            // Render glowing version of the fps on button
            SDL_SetTextureColorMod(menu.fpsOnTexture, 255, 255, 255); // Set color to white
            SDL_RenderCopy(sdl.renderer, menu.fpsOnTexture, NULL, &menu.fpsOnRect);
        } else {
            // Render normal version of the fps on button
            SDL_SetTextureColorMod(menu.fpsOnTexture, 200, 200, 200); // Set color to gray
            SDL_RenderCopy(sdl.renderer, menu.fpsOnTexture, NULL, &menu.fpsOnRect);
        }

    } else {
        // fps Off
        menu.fpsOffSurface = TTF_RenderText_Solid(menu.font, "OFF", menu.textColor);
        if (!menu.fpsOffSurface) {
            return;
        }
        menu.fpsOffTexture = SDL_CreateTextureFromSurface(sdl.renderer, menu.fpsOffSurface);
        if (!menu.fpsOffTexture) {
            SDL_FreeSurface(menu.fpsOffSurface);
        }
        SDL_QueryTexture(menu.fpsOffTexture, NULL, NULL, &menu.textWidth, &menu.textHeight);
        menu.fpsOffRect = { WINDOW_WIDTH/2 + 100, menu.dispModeRect.y + 60, menu.textWidth, menu.textHeight };
        SDL_RenderCopy(sdl.renderer, menu.fpsOffTexture, NULL, &menu.fpsOffRect);

        // Hover
        if (g.hoverFpsButton) {
            // Render glowing version of the fps off button
            SDL_SetTextureColorMod(menu.fpsOffTexture, 255, 255, 255); // Set color to white
            SDL_RenderCopy(sdl.renderer, menu.fpsOffTexture, NULL, &menu.fpsOffRect);
        } else {
            // Render normal version of the fps off button
            SDL_SetTextureColorMod(menu.fpsOffTexture, 200, 200, 200); // Set color to gray
            SDL_RenderCopy(sdl.renderer, menu.fpsOffTexture, NULL, &menu.fpsOffRect);
        }

    }

    // Screen Resolution 
    menu.resSurface = TTF_RenderText_Solid(menu.font, "Window resolution", menu.textColor);
    if (!menu.resSurface) {
        return;
    }
    menu.resTexture = SDL_CreateTextureFromSurface(sdl.renderer, menu.resSurface);
    if (!menu.resTexture) {
        SDL_FreeSurface(menu.resSurface);
    }
    SDL_QueryTexture(menu.resTexture, NULL, NULL, &menu.textWidth, &menu.textHeight);
    menu.resRect = { WINDOW_WIDTH/2 - 200, menu.fpsRect.y + 60, menu.textWidth, menu.textHeight };
    SDL_RenderCopy(sdl.renderer, menu.resTexture, NULL, &menu.resRect);

    // Grab current screen resolution
    std::stringstream res;
    res << WINDOW_WIDTH << " x " << WINDOW_HEIGHT;

    // Display Current Screen Resolution
    menu.dispResSurface = TTF_RenderText_Solid(menu.font, res.str().c_str(), menu.textColor);
    if (!menu.dispResSurface) {
        return;
    }
    menu.dispResTexture = SDL_CreateTextureFromSurface(sdl.renderer, menu.dispResSurface);
    if (!menu.dispResTexture) {
        SDL_FreeSurface(menu.dispResSurface);
    }
    SDL_QueryTexture(menu.dispResTexture, NULL, NULL, &menu.textWidth, &menu.textHeight);
    menu.dispResRect = { WINDOW_WIDTH/2 + 100, menu.fpsRect.y + 60, menu.textWidth, menu.textHeight };
    SDL_RenderCopy(sdl.renderer, menu.dispResTexture, NULL, &menu.dispResRect);

    // Back button
    SDL_RenderCopy(sdl.renderer, menu.back, NULL, &menu.backButtonRect);
    if (g.hoverBackButton) {
        // Render glowing version of the back button
        SDL_SetTextureColorMod(menu.back, 255, 255, 255); // Set color to white
        SDL_RenderCopy(sdl.renderer, menu.back, NULL, &menu.backButtonRect);
    } else {
        // Render normal version of the back button
        SDL_SetTextureColorMod(menu.back, 200, 200, 200); // Set color to gray
        SDL_RenderCopy(sdl.renderer, menu.back, NULL, &menu.backButtonRect);
    }

    // Present the rendered frame
    SDL_RenderPresent(sdl.renderer);
}

// Settings input handling
void settingsHandleInput(bool& play, bool& done, bool& set) {
    SDL_Event event;

    // Declare variables outside the switch statement
    int mouseX, mouseY, buttonBackX, buttonBackY, buttonWinX, buttonWinY, buttonFullX, buttonFullY, buttonSetWinX, buttonSetWinY, buttonFpsX, buttonFpsY;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            play = true;
            done = true;
            set = true;
            break;
        case SDL_MOUSEBUTTONDOWN:

            // check if button animation has finished before clicking
            if (g.titleMenuReady) {
                 // check if mouse clicks on button
                 if (menu.setWindow) {
                     if (event.button.button == SDL_BUTTON_LEFT) {
                         mouseX = event.button.x;
                         mouseY = event.button.y;
                         buttonWinX = WINDOW_WIDTH/2 + 100;
                         buttonWinY = menu.titleNameRect.y + 50;
                         buttonFullX = WINDOW_WIDTH/2 + 100;
                         buttonFullY = menu.titleNameRect.y + 80;
                         // windowed option
                         if (mouseX >= buttonWinX && mouseX <= buttonWinX + menu.textWidth &&
                             mouseY >= buttonWinY && mouseY <= buttonWinY + menu.textHeight) {
                            printf("windowed\n");
                            menu.setWindow = !menu.setWindow;
                            if (isFullScreen) {
                                // toggle Fullscreen
                                toggleFullScreen();
                                // Update the Window width and height
                                SDL_GetWindowSize(sdl.window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
                                // Reinitialize menu, fire animation, sprite 
                                menu.reset();
                                fire.reset();
                                sprite.reset();                            
                            }
                         }
                         // fullscreen option
                         else if (mouseX >= buttonFullX && mouseX <= buttonFullX + menu.textWidth &&
                             mouseY >= buttonFullY && mouseY <= buttonFullY + menu.textHeight) {
                            printf("fullscreen\n");
                            menu.setWindow = !menu.setWindow;
                            if (!isFullScreen) {
                                // toggle Fullscreen
                                toggleFullScreen();
                                // Update the Window width and height
                                SDL_GetWindowSize(sdl.window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
                                // Reinitialize menu, fire animation, sprite 
                                menu.reset();
                                fire.reset();
                                sprite.reset();                          
                            }
                         }
                         // close window settings
                         else {
                            menu.setWindow = false;
                         }
                     }
                 } else {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouseX = event.button.x;
                        mouseY = event.button.y;
                        buttonSetWinX = WINDOW_WIDTH/2 + 100;
                        buttonSetWinY = menu.titleNameRect.y + 60;
                        buttonFpsX = WINDOW_WIDTH/2 + 100;
                        buttonFpsY = menu.dispModeRect.y + 60;
                        buttonBackX = menu.backButtonRect.x;
                        buttonBackY = menu.backButtonRect.y;
                        // set window
                        if (mouseX >= buttonSetWinX && mouseX <= buttonSetWinX + menu.textWidth &&
                            mouseY >= buttonSetWinY && mouseY <= buttonSetWinY + menu.textHeight) {
                        printf("Set window\n");
                        menu.setWindow = !menu.setWindow;
                        }
                        // toggle the fps display
                        if (mouseX >= buttonFpsX && mouseX <= buttonFpsX + menu.fpsOffRect.w &&
                             mouseY >= buttonFpsY && mouseY <= buttonFpsY + menu.fpsOffRect.h) {
                            printf("FPS toggle\n");
                            // toggle fps
                            menu.showFPS = !menu.showFPS;
                        }
                        // Back button
                        if (mouseX >= buttonBackX && mouseX <= buttonBackX + menu.backButtonRect.w &&
                            mouseY >= buttonBackY && mouseY <= buttonBackY + menu.backButtonRect.h) {
                                set = true;
                                menu.setWindow = false;
                        }
                    }
                 }
            } else {
                g.titleMenuReady = true;
            }
            break;
        case SDL_MOUSEMOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            buttonBackX = menu.backButtonRect.x;
            buttonBackY = menu.backButtonRect.y;
            buttonSetWinX = WINDOW_WIDTH/2 + 100;
            buttonSetWinY = menu.titleNameRect.y + 60;
            buttonWinX = WINDOW_WIDTH/2 + 100;
            buttonWinY = menu.titleNameRect.y + 50;
            buttonFullX = WINDOW_WIDTH/2 + 100;
            buttonFullY = menu.titleNameRect.y + 80;
            buttonFpsX = WINDOW_WIDTH/2 + 100;
            buttonFpsY = menu.dispModeRect.y + 60;

            // Check if the mouse cursor is over Set Window button
            g.hoverSetWinButton = (mouseX >= buttonSetWinX && mouseX <= buttonSetWinX + menu.textWidth &&
                                     mouseY >= buttonSetWinY && mouseY <= buttonSetWinY + menu.textHeight);

            // Check if the mouse cursor is over Windowed option button
            g.hoverWinButton = (mouseX >= buttonWinX && mouseX <= buttonWinX + menu.textWidth &&
                                     mouseY >= buttonWinY && mouseY <= buttonWinY + menu.textHeight);

            // Check if the mouse cursor is over FullScreen option button
            g.hoverFullButton = (mouseX >= buttonFullX && mouseX <= buttonFullX + menu.textWidth &&
                                     mouseY >= buttonFullY && mouseY <= buttonFullY + menu.textHeight);

            // Check if the mouse cursor is over Back button
            g.hoverBackButton = (mouseX >= buttonBackX && mouseX <= buttonBackX + menu.backButtonRect.w &&
                                     mouseY >= buttonBackY && mouseY <= buttonBackY + menu.backButtonRect.h);

            // Check if the mouse cursor is over fps toggle button
            g.hoverFpsButton = (mouseX >= buttonFpsX && mouseX <= buttonFpsX + menu.fpsOffRect.w &&
                                     mouseY >= buttonFpsY && mouseY <= buttonFpsY + menu.fpsOffRect.h);
            break;
	    case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                break;
            case SDLK_RETURN:
                break;
            case SDLK_f:
                // toggle Fullscreen
                toggleFullScreen();
                // Update the Window width and height
                SDL_GetWindowSize(sdl.window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
                // Reinitialize menu, fire animation, sprite 
                menu.reset();
                fire.reset();
                sprite.reset();
                break;
            case SDLK_p:
                menu.showFPS = !menu.showFPS;
                break;
            case SDLK_ESCAPE:
                set = true;
                break;
            default:
                break;
	        }  
	    default:
	        break;
	    }
    }
}

// Initialize title screen
void init_title_screen() {
    // *** MENU ***
    // Load the background image for title screen
    menu.titleBackground = loadTexture("./image/title2.jpg");
    if (!menu.titleBackground) {
	std::cerr << "Failed to load title image." << std::endl;
	return;
    }

    // Load Name for title screen
    menu.titleName = loadTexture("./image/titleName.png");
    if (!menu.titleName) {
	std::cerr << "Failed to load title image." << std::endl;
	return;
    }

    // Load start button for menu
    menu.start = loadTexture("./menu/Large Buttons/Large Buttons/Play Button.png");
    if (!menu.start) {
	std::cerr << "Failed to load start button image." << std::endl;
    }

    // Load continue button for menu
    menu.cont = loadTexture("./menu/Large Buttons/Large Buttons/Continue Button.png");
    if (!menu.cont) {
	std::cerr << "Failed to load continue button image." << std::endl;
    }

    // Load exit button for menu
    menu.quit = loadTexture("./menu/Large Buttons/Large Buttons/Quit Button.png");
    if (!menu.quit) {
	std::cerr << "Failed to load quit button image." << std::endl;
    }

    // Load settings button for menu
    menu.settings = loadTexture("./menu/Large Buttons/Large Buttons/Settings Button.png");
    if (!menu.settings) {
	std::cerr << "Failed to load settings button image." << std::endl;
    }

    // Load back button for settings
    menu.back = loadTexture("./menu/Large Buttons/Large Buttons/Back Button.png");
    if (!menu.back) {
	std::cerr << "Failed to load back button image." << std::endl;
    }

    // Load apply button for settings
    menu.apply = loadTexture("./menu/Square Buttons/Square Buttons/V Square Button.png");
    if (!menu.apply) {
	std::cerr << "Failed to load apply button image." << std::endl;
    }

    // *** FIRE ANIMATIONS ***
    // Load orange fire loop for intro
    fire.Orange = loadTexture("./Sprites/fire_fx/png/orange/loops/burning_loop_1.png");
    if (!fire.Orange) {
	std::cerr << "Failed to load fire sprite." << std::endl;
    }

    // Load orange fire start for intro
    fire.OrangeStart = loadTexture("./Sprites/fire_fx/png/orange/start/burning_start_1.png");
    if (!fire.OrangeStart) {
	std::cerr << "Failed to load fire sprite." << std::endl;
    }
    // Load blue fire loop for intro
    fire.Blue = loadTexture("./Sprites/fire_fx/png/blue/loops/burning_loop_1.png");
    if (!fire.Blue) {
	std::cerr << "Failed to load fire sprite." << std::endl;
    }

    // Load blue fire start for intro
    fire.BlueStart = loadTexture("./Sprites/fire_fx/png/blue/start/burning_start_1.png");
    if (!fire.BlueStart) {
	std::cerr << "Failed to load fire sprite." << std::endl;
    }
    // Load green fire loop for intro
    fire.Green = loadTexture("./Sprites/fire_fx/png/green/loops/burning_loop_1.png");
    if (!fire.Green) {
	std::cerr << "Failed to load fire sprite." << std::endl;
    }

    // Load green fire start for intro
    fire.GreenStart = loadTexture("./Sprites/fire_fx/png/green/start/burning_start_1.png");
    if (!fire.GreenStart) {
	std::cerr << "Failed to load fire sprite." << std::endl;
    }
    // Load purple fire loop for intro
    fire.Purple = loadTexture("./Sprites/fire_fx/png/purple/loops/burning_loop_1.png");
    if (!fire.Purple) {
	std::cerr << "Failed to load fire sprite." << std::endl;
    }

    // Load purple fire start for intro
    fire.PurpleStart = loadTexture("./Sprites/fire_fx/png/purple/start/burning_start_1.png");
    if (!fire.PurpleStart) {
	std::cerr << "Failed to load fire sprite." << std::endl;
    }
    // Load white fire loop for intro
    fire.White = loadTexture("./Sprites/fire_fx/png/white/loops/burning_loop_1.png");
    if (!fire.White) {
	std::cerr << "Failed to load fire sprite." << std::endl;
    }

    // Load white fire start for intro
    fire.WhiteStart = loadTexture("./Sprites/fire_fx/png/white/start/burning_start_1.png");
    if (!fire.WhiteStart) {
	std::cerr << "Failed to load fire sprite." << std::endl;
    }

    // intitialize fire and fireStart
    // Used for random fire color selection
    fire.fire[0] = fire.Orange;
    fire.fire[1] = fire.Blue;
    fire.fire[2] = fire.Green;
    fire.fire[3] = fire.Purple;
    fire.fire[4] = fire.White;

    fire.fireStart[0] = fire.OrangeStart;
    fire.fireStart[1] = fire.BlueStart;
    fire.fireStart[2] = fire.GreenStart;
    fire.fireStart[3] = fire.PurpleStart;
    fire.fireStart[4] = fire.WhiteStart;

    // Random fire selection
    srand(time(nullptr));
    g.randFireGen = std::rand() % 5;
}

// FPS Average (no limit)
void calculate_fps() {
    float elapsed = (g.endFPS - g.startFPS) / (float)SDL_GetPerformanceFrequency();
    g.fps.push_back(1.0f / elapsed);
    if (g.fps.size() >= 100) {
        g.avgFPS = (accumulate(g.fps.begin(), g.fps.end(), 0.0f)) / g.fps.size();
        printf("\rFPS: %i", g.avgFPS);
        fflush(stdout);
        g.fps.clear();
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (!init_SDL()) {
        return -1;
    }

    // Initialize fonts
    menu.init_fonts();

    // Initialize title screen
    init_title_screen();

    bool play = false;
    bool done = false;
    bool set = true;
    // Title Screen loop
    while(!play) {
        titleScreen();
        menuHandleInput(play, done, set);
        while(!set) {
            settingsScreen();
            settingsHandleInput(play, done, set);
        }
    }
    
    // Initialize Sprites
    init_sprite();

    // Initilize Background
    init_background();

    // Init timer
    SDL_TimerID timerID = SDL_AddTimer(100, timerCallback, nullptr); //adjust delay for sprite

    // Index of current rendered sprite
    int currentSpriteIndex = 0;

    // Game running
    while (!done) {

        // Start fps timer
        g.startFPS = SDL_GetPerformanceCounter();

        handleInput(done, play, currentSpriteIndex, sprite.numSprites);

        renderSprite(currentSpriteIndex, sprite.State);

        updateSprite();

        // End fps timer
        g.endFPS = SDL_GetPerformanceCounter();

        // Calculate fps
        calculate_fps();

        //SDL_Delay(16); //18

        // Pause menu
        while(!play) {
            pauseScreen();
            menuHandleInput(play, done, set);
            while(!set) {
                settingsScreen();
                settingsHandleInput(play, done, set);
            }
        }
    }

    // Remove timer
    SDL_RemoveTimer(timerID);

    // Cleanup SDL
    cleanup_SDL();
    // Cleanup Fonts
    menu.cleanup_fonts();

    return 0;
}
