#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "bool.h"
#include "LWindow.h"
#include "LTexture.h"
#include "LTimer.h"
#include "dot.h"

#define LEVEL_WIDTH 1280
#define LEVEL_HEIGHT 960

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define JOYSTICK_DEAD_ZONE 8000


//////////////////////
// Global Variables //
//////////////////////

// The window we'll be rendering to
LWindow gWindow;

// The window renderer
SDL_Renderer* gRenderer = NULL;

// The data that we'll be reading
const size_t TOTAL_DATA = 10;
int32_t gData[TOTAL_DATA];
LTexture gDataTextures[TOTAL_DATA];

// The global font
TTF_Font* gFont = NULL;

// Text
LTexture gInputTextTexture;
LTexture gBGTexture;
LTexture gPromptTexture;
LTexture gTimeTextTexture;

///////////////
// Functions //
///////////////

// Initialize SDL and load the window
bool init() {
    // Initialization flag
    bool success = true;
    
    // Initialize SDL
    if ((SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    } else {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled");
        }

        if (!LW_init(&gWindow, SCREEN_WIDTH, SCREEN_HEIGHT)) {
            printf("Window could not be created! SDL_Error: %s\n",
                   SDL_GetError());
            success = false;
        } else {
            // Create renderer for window
            gRenderer = LW_createRenderer(gWindow);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n",
                       SDL_GetError());
                success = false;
            } else {
                // Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
                
                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image error: %s\n",
                           IMG_GetError());
                    success = false;
                }

                //Initialize SDL_ttf
                if( TTF_Init() == -1 ) {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                            TTF_GetError() );
                    success = false;
                }
            }
        }
    }
    
    return success;
}

void pop_back(char* string) {
    if (!(*string)) {
        // Do nothing
    } else {
        size_t stringLen = strlen(string);
        string[stringLen - 1] = '\0';
    }
}

void toString(int32_t i, char buffer[12]) {
    // void toString(int32_t i, char buffer[12])
    // Converts i to a string and writes it to buffer, which is allocated
    // externally.  buffer must be of size 12 in order to hold the largest
    // values of int32_t in base 10
    sprintf(buffer, "%i", i);
}

bool loadMedia() {
    bool success = true;
    
    // Load font
    gFont = TTF_OpenFont("../lazy.ttf", 28);
    if (gFont == NULL) {
        printf("Failed to lod lazy font! TTF Error: %s\n", TTF_GetError());
        success = false;
    }

    gPromptTexture = loadLTexture("../colors.png", gRenderer);
    
    return success;
}

// Deallocate memory and shut down SDL
void closeWindow() {
    freeLTexture(gPromptTexture);
    freeLTexture(gTimeTextTexture);

    // Close window
    SDL_DestroyRenderer(gRenderer);
    LW_free(&gWindow);
    
    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}


int main(int argc, char* args[]) {
    // Start up SDL and create a window
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        // Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            // Main loop flag
            bool quit = false;
        
            // Event Handler
            SDL_Event e;
            while (!quit) {
                // printf("0\n");
                // Render text flag
                bool renderText = false;
                while (SDL_PollEvent(&e) != 0) { // Pulls events off the event
                                                 // queue until the event queue
                                                 // is empty
                    
                    // User requests a quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                    LW_handleEvent(&gWindow, gRenderer, &e);
                }

                    
                // Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                render(gRenderer,
                       gPromptTexture,
                       (SCREEN_WIDTH-gPromptTexture.mWidth)/2, 0);
                
                // Update screen
                SDL_RenderPresent(gRenderer);
               
            }
        }
    }
    /* // Disable text input */
    /* SDL_StopTextInput(); */

    // Free up resources and close SDL
    closeWindow();
    
    return 0;
}
