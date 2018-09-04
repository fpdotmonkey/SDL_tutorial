#include <stdio.h>

#include "bool.h"
#include "LPrints.h"
#include "LWindow.h"


#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 680
#define NUMBER_OF_WINDOWS 3

int main(int argc, char **argv) {
    // initialize
    LWindow windows[NUMBER_OF_WINDOWS];
    int numberOfDisplays = 0;
    SDL_Rect* displayBounds = NULL;
    for (int i = 0; i < NUMBER_OF_WINDOWS; ++i) {
        if (! LW_init(&windows[i], numberOfDisplays, SCREEN_WIDTH, SCREEN_HEIGHT)) {
            ErrorSDL("Could not initialize window");
            printf("Could not initialize window %i! SDL_Error: %s\n", i, SDL_GetError());
        }
    }
    

    // 
    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            for (int i = 0; i < NUMBER_OF_WINDOWS; ++i) {
                LW_handleEvent(&windows[i], &event);
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_1:
                    LW_focus(&windows[0]);
                        break;

                case SDLK_2:
                    LW_focus(&windows[1]);
                    break;

                case SDLK_3:
                    LW_focus(&windows[2]);
                    break;
                }
            }
        }

        // Render all the windows
        for (int i = 0; i < NUMBER_OF_WINDOWS; ++i) {
            LW_render(&windows[i]);
        }

        // Check to see if all the windows are closed
        bool allWindowsAreClosed = true;
        for (int i = 0; i < NUMBER_OF_WINDOWS; ++i) {
            if (LW_isShown(&windows[i])) {
                allWindowsAreClosed = false;
                break;
            }
        }
        if (allWindowsAreClosed) { // Quit the application if all the windows are closed
            quit = true;
        }
    }

    for (int i = 0; i < NUMBER_OF_WINDOWS; ++i) {
        LW_free(&windows[i]);
    }
    SDL_Quit();
    return 0;
}
