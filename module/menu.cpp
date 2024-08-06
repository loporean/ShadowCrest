// menu.cpp

#include "../header/menu.h"

// extern allows variables defined in main to be used here
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern int BUTTON_WIDTH;
extern int BUTTON_HEIGHT;

// Define extern instance of Menu
extern Menu menu;

//Constructor
Menu::Menu() {
    // title name
    titleNameRect.x = (WINDOW_WIDTH - 390) / 2;
    titleNameRect.y =  WINDOW_HEIGHT / 3 - 125;
    titleNameRect.w = 390;
    titleNameRect.h = 41;

    // start button
    startButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    startButtonRect.y =  WINDOW_HEIGHT / 3;
    startButtonRect.w = BUTTON_WIDTH;
    startButtonRect.h = BUTTON_HEIGHT;

    // continue button
    contButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    contButtonRect.y = (WINDOW_HEIGHT / 3);
    contButtonRect.w = BUTTON_WIDTH;
    contButtonRect.h = BUTTON_HEIGHT;

    // settings button
    settingsButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    settingsButtonRect.y = (WINDOW_HEIGHT / 3) + BUTTON_HEIGHT + 20;
    settingsButtonRect.w = BUTTON_WIDTH;
    settingsButtonRect.h = BUTTON_HEIGHT;

    // quit button
    quitButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    quitButtonRect.y = settingsButtonRect.y + BUTTON_HEIGHT + 20;
    quitButtonRect.w = BUTTON_WIDTH;
    quitButtonRect.h = BUTTON_HEIGHT;

    // back button
    backButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2 - 120;
    backButtonRect.y = settingsButtonRect.y + BUTTON_HEIGHT + 80;
    backButtonRect.w = (BUTTON_WIDTH/2);
    backButtonRect.h = (BUTTON_HEIGHT/2);

    // apply button
    applyButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2 + 180;
    applyButtonRect.y = settingsButtonRect.y + BUTTON_HEIGHT + 80;
    applyButtonRect.w = (BUTTON_WIDTH/4);
    applyButtonRect.h = (BUTTON_HEIGHT/2);

    // fps
    showFPS = false;
    // Window settings
    setWindow = false;

    // Fonts
    textColor = { 255, 255, 255 }; // white
}

void Menu::reset() {
    // title name
    titleNameRect.x = (WINDOW_WIDTH - 390) / 2;
    titleNameRect.y =  WINDOW_HEIGHT / 3 - 125;
    titleNameRect.w = 390;
    titleNameRect.h = 41;

    // start button
    startButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    startButtonRect.y =  WINDOW_HEIGHT / 3;
    startButtonRect.w = BUTTON_WIDTH;
    startButtonRect.h = BUTTON_HEIGHT;

    // continue button
    contButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    contButtonRect.y = (WINDOW_HEIGHT / 3);
    contButtonRect.w = BUTTON_WIDTH;
    contButtonRect.h = BUTTON_HEIGHT;

    // settings button
    settingsButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    settingsButtonRect.y = (WINDOW_HEIGHT / 3) + BUTTON_HEIGHT + 20;
    settingsButtonRect.w = BUTTON_WIDTH;
    settingsButtonRect.h = BUTTON_HEIGHT;

    // quit button
    quitButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    quitButtonRect.y = settingsButtonRect.y + BUTTON_HEIGHT + 20;
    quitButtonRect.w = BUTTON_WIDTH;
    quitButtonRect.h = BUTTON_HEIGHT;

    // back button
    backButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2 - 120;
    backButtonRect.y = settingsButtonRect.y + BUTTON_HEIGHT + 80;
    backButtonRect.w = (BUTTON_WIDTH/2);
    backButtonRect.h = (BUTTON_HEIGHT/2);

    // apply button
    applyButtonRect.x = (WINDOW_WIDTH - BUTTON_WIDTH) / 2 + 180;
    applyButtonRect.y = settingsButtonRect.y + BUTTON_HEIGHT + 80;
    applyButtonRect.w = (BUTTON_WIDTH/4);
    applyButtonRect.h = (BUTTON_HEIGHT/2);
}

// Initialize fonts
void Menu::init_fonts() {

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        printf("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        return;
    }

    // font = TTF_OpenFont("./fonts/SketchyNight.ttf", 16);
    font = TTF_OpenFont("./fonts/Arial.ttf", 16);
    if (!font) {
        // Handle font loading error
        printf("Failed to load font: %s\n", TTF_GetError());
        TTF_Quit();
        return;
    }
}
// Cleanup fonts
void Menu::cleanup_fonts() {
    // Clean up font resources
    SDL_FreeSurface(dispModeSurface);
    SDL_DestroyTexture(dispModeTexture);
    TTF_CloseFont(font);
    TTF_Quit();
}