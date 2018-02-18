#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "bool.h"
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
SDL_Window* gWindow = NULL;

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
        
        // Create a window
        gWindow = SDL_CreateWindow("SDL Tutorial",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SCREEN_WIDTH,
                                   SCREEN_HEIGHT,
                                   SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n",
                   SDL_GetError());
            success = false;
        } else {
            // Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow,
                                           -1,
                                           SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_PRESENTVSYNC);
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
    // values of int32_t
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
    
    // Open a file for reading in binary
    SDL_RWops* file = SDL_RWFromFile("../nums.bin", "r+b");
    
    if (file == NULL) {
        printf("Warning! Unable to open file.  SDL Error: %s\n",
               SDL_GetError());
        
        // Create a file for writing
        file = SDL_RWFromFile("../nums.bin", "w+b");
        
        if (file != NULL) {
            printf("New file created!\n");
            
            // Initialize data
            for (int i = 0; i < TOTAL_DATA; ++i) {
                gData[i] = 0;
                SDL_RWwrite(file, &gData[1], sizeof(int32_t), 1);
            }
            
            // Close file handler
            SDL_WRclose(file);
        } else {
            printf("Error: Unable to create file! SDL Erroe: %s\n",
                   SDL_GetError());
            success = false;
        }
    } else {
        // Load data
        printf("Reading file....\n");
        for (int i = 0; i < TOTAL_DATA; ++i) {
            SDL_RWread(file, &gData, sizeof(int32_t), 1);
        }
        
        // Close file handler
        SDL_RWclose(file);
    }
    
    // Initialize data textures
    char* buffer[12];
    toString(gData[0], buffer);
    gDataTextures[0] = loadFromRenderedText(buffer,
                                            highlightColor);
    for (int i = 1; i < TOTAL_DATA; ++i) {
        char* buffer[12];
        toString(gData[i], buffer);
        gDataTexture[i] = loadFromRenderedText(buffer, textColor);
    }

    
    
    return success;
}

// Deallocate memory and shut down SDL
void closeWindow() {
    // Open data for writing
    SDL_RWops* file = SDL_RWFromFile("../nums.bin");
    if (file != NULL) {
        // Save data
        for (int i = 0; i < TOTAL_DATA; ++i) {
            SDL_RWwrite(file, &gData[i], sizeof(int32_t), 1);
        }

        // Close file handler
        SDL_RWclose(file);
    } else {
        printf("Error: Unable to save file! SDL Error: %s\n", SDL_GetError());
    }

    
    // Deallocate surface
    for (int i = 0; i < TOTAL_DATA, ++i) {
        freeLTexture(gDataTexture[i]);
    }
    freeLTexture(gPromptTexture);
    freeLTexture(gTimeTextTexture);
    
    // Destroy Window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
    
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

            // Set text color to black
            SDL_Color textColor = { 0, 0, 0, 0xFF };
            SDL_Color highlightColor = { 0xFF, 0, 0, 0xFF };

            // Current input point
            int currentData = 0;
            
            // The current input text
            const size_t maxInputSize = 50;
            char inputText[maxInputSize] = "Some Text";
            gInputTextTexture = loadFromRenderedText(inputText,
                                                     textColor,
                                                     gFont,
                                                     gRenderer);

            SDL_StartTextInput();


            /* Dot dot; */

            /* initDot(&dot, SCREEN_WIDTH/2, SCREEN_HEIGHT/2); */

            /* SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; */

            /* SDL_Rect wall; */
            /* wall.x = 300; */
            /* wall.y = 40; */
            /* wall.w = 40; */
            /* wall.h = 400; */
            
            while (!quit) {
                // Render text flag
                bool renderText = false;
                while (SDL_PollEvent(&e) != 0) { // Pulls events off the event
                                                 // queue until the event queue
                                                 // is empty

                    
                    // User requests a quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                            // Previous data entry
                        case SDLK_UP:
                            // Rerender previous entry input point
                            gDataTextures[currentData] =
                                loadFromRenderedText(
                                    toString(gData[currentData]),
                                    textColor);
                            --currentData;
                            if (currentData < 0) {
                                currentData = TOTAL_DATA - 1;
                            }

                            // Rerender current entry input point
                            gDataTextures[currentData] =
                                loadFromRenderedText(
                                    toString(gData[currentData]),
                                    highlightColor);
                            break;

                            // Next data entry
                        case SDLK_DOWN:
                            // Rerender prfious entry input point
                            gDataTextures[currentData] =
                                loadFromRenderedText(
                                    toString(gData[currentData]),
                                    textColor);
                            ++currentData;
                            if (currentData == TOTAL_DATA) {
                                currentData = 0;
                            }

                            // Rerender current entry input point
                            gDataTextures[currentData] =
                                loadFromRenderedText(
                                    toString(gData[currentData]),
                                    highlightColor);
                            break;

                        case SDLK_LEFT:
                            --gData[currentData];
                            gDataTexture[currentData] =
                                loadFromRenderedText(
                                    toString(gData[currentData]),
                                    highlightColor);
                            break;

                        case SDLK_RIGHT:
                            ++gData[currentData];
                            gDataTexture[currentData] =
                                loadFromRenderedText(
                                    toString(gData[currentData]),
                                    highlightColor);
                            break;
                          
                                
                                
                            
                                
                        }
                    }
                    /* // Handle text input, backspace, and copy and paste
                    /* else if (e.type == SDL_KEYDOWN) { */
                    /*     if (e.key.keysym.sym == SDLK_c */
                    /*         && SDL_GetModState() & KMOD_LGUI) { */
                    /*         // Copy inputText to clipboard on l_cmd-c */
                    /*         SDL_SetClipboardText(inputText); */
                    /*     } else if (e.key.keysym.sym == SDLK_BACKSPACE */
                    /*                && strlen(inputText) > 0) { */
                    /*         // Delete last character */
                    /*         pop_back(inputText); */
                    /*         renderText = true; */
                    /*     } else if (e.key.keysym.sym == SDLK_v */
                    /*                && SDL_GetModState() & KMOD_LGUI) { */
                    /*         // Replace inputText with the clipboard on l_cmd-v */
                    /*         inputText[0] = '\0'; */
                    /*         strncpy(inputText, */
                    /*                 SDL_GetClipboardText(), */
                    /*                 maxInputSize); */
                    /*         inputText[maxInputSize-1] = '\0'; */
                    /*         renderText = true; */
                    /*     } */
                    /* } else if (e.type == SDL_TEXTINPUT */
                    /*            && strlen(inputText) < maxInputSize) { */
                    /*     // Handle text input */
                        
                    /*     // if not copy nor paste */
                    /*     if (!(SDL_GetModState() & KMOD_LGUI */
                    /*           && (e.text.text[0] == 'c' */
                    /*               || e.text.text[0] == 'C' */
                    /*               || e.text.text[0] == 'v' */
                    /*               || e.text.text[0] == 'V'))) { */
                    /*         // Append character */
                    /*         if (strlen(e.text.text) + 1 > */
                    /*             sizeof(inputText) - strlen(inputText)) { */
                    /*             // Do nothing */
                    /*         } else { */
                    /*             if (strncat(inputText, e.text.text, */
                    /*                         sizeof(inputText) */
                    /*                         - strlen(inputText) - 1) != NULL) { */
                    /*                 renderText = true; */
                    /*             } */
                    /*         } */
                    /*     } */
                    /* } */

                    /* handleEventDot(&dot, &e); */
                }
                /* // Render text if needed */
                /* if (renderText) { */
                /*     // if text is not empty */
                /*     if (strcmp(inputText, "")) { */
                /*         // Render the new text */
                /*         gInputTextTexture = loadFromRenderedText(inputText, */
                /*                                                  textColor, */
                /*                                                  gFont, */
                /*                                                  gRenderer); */
                /*     } else { // Text is empty */
                /*         // Render " " (SDL_ttf won't render an empty string) */
                /*         gInputTextTexture = loadFromRenderedText(" ", */
                /*                                                  textColor, */
                /*                                                  gFont, */
                /*                                                  gRenderer); */
                /*     } */
                /* } */
                
                /* updatePositionDot(&dot, wall); */

                /* // Center camera over the dot */
                /* camera.x = (dot.mPosX - SCREEN_WIDTH/2); */
                /* camera.y = (dot.mPosY - SCREEN_HEIGHT/2); */

                /* // Keep the camera inbounds */
                /* if (camera.x < 0) { */
                /*     camera.x = 0; */
                /* } */
                
                /* if (camera.y < 0) { */
                /*     camera.y = 0; */
                /* } */

                /* if (camera.x > LEVEL_WIDTH - camera.w) { */
                /*     camera.x = LEVEL_WIDTH - camera.w; */
                /* } */

                /* if (camera.y > LEVEL_HEIGHT - camera.h) { */
                /*     camera.y = LEVEL_HEIGHT - camera.h; */
                /* } */
                    
                // Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                render(gRenderer,
                       gPromptTexture,
                       (SCREEN_WIDTH-gPromptTexture.mWidth)/2, 0);

                for (int i = 0; i < TOTAL_DATA; ++i) {
                    render(gRenderer,
                           gDataTextures[i],
                           (SCREEN_WIDTH - gDataTextures[i].mWidth) / 2,
                           gPromptTextTextures.mHeight
                           + gDataTextures[0].mHeight * i);
                }
                
                /* render(gRenderer, */
                /*        gInputTextTexture, */
                /*        (SCREEN_WIDTH-gInputTextTexture.mWidth)/2, */
                /*        gPromptTexture.mHeight); */
                
                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    // Disable text input
    SDL_StopTextInput();

    // Free up resources and close SDL
    closeWindow();
    
    return 0;
}
