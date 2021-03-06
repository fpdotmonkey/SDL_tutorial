//
// LWindow.c
//
// Copyright (c) 2018 Fletcher Porter
//

#include "LWindow.h"

bool LW_init(LWindow* window,
             int numberOfDisplays,
             int screenWidth,
             int screenHeight) {

    
    // Initialize the LWindow
    window->mWindow = SDL_CreateWindow("SDL Tutorial",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       screenWidth,
                                       screenHeight,
                                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window->mWindow != NULL) {
        window->mWidth = screenWidth;
        window->mHeight = screenHeight;
        
        window->mMouseFocus = false;
        window->mKeyboardFocus = false;

        window->mRenderer = SDL_CreateRenderer(window->mWindow,
                                               -1,
                                               SDL_RENDERER_ACCELERATED |
                                               SDL_RENDERER_PRESENTVSYNC);
        if (window->mRenderer != NULL) {
            // Initialize renderer color
            /* prepareForRender(window); */

            // Grab window identifier
            window->mWindowID = SDL_GetWindowID(window->mWindow);
            window->mWindowDisplayID = SDL_GetWindowDisplayIndex(window->mWindow);
            window->mNumberOfDisplays = numberOfDisplays;

            // Flag as opened
            window->mShown = true;
            LW_setFullScreenStateTo(window, false);
            window->mMinimized = false;
        } else {
            ErrorSDL("Renderer coudl not be created");
            SDL_DestroyWindow(window->mWindow);
            window->mWindow = NULL;
        }
    } else {
        ErrorSDL("Window could not be created");
    }

    return window->mWindow != NULL && window->mRenderer != NULL;
}

void LW_free(LWindow* window) {
    // Destroy Window
    SDL_DestroyWindow(window->mWindow);
    SDL_DestroyRenderer(window->mRenderer);
    window->mWindow = NULL;
    window->mRenderer = NULL;

    window = NULL;
}

void LW_handleEvent(LWindow* window, SDL_Event* event) {
    // Window Event Occured
    if (event->type == SDL_WINDOWEVENT
        && event->window.windowID == window->mWindowID) {
        // Caption update flag
        bool updateCaption = false;

        switch (event->window.event) {
            // Window moves
        case SDL_WINDOWEVENT_MOVED:
            window->mWindowDisplayID = SDL_GetWindowDisplayIndex(window->mWindow);
            updateCaption = true;
            break;
            
            // Get new dimensions and repaint on window size changed
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            window->mWidth = event->window.data1;
            window->mHeight = event->window.data2;
            SDL_RenderPresent(window->mRenderer);
            break;

            // Repaint on exposure
        case SDL_WINDOWEVENT_EXPOSED:
            SDL_RenderPresent(window->mRenderer);
            break;

            // Mouse enters window
        case SDL_WINDOWEVENT_ENTER:
            window->mMouseFocus = true;
            updateCaption = true;
            break;

            // Mouse left window
        case SDL_WINDOWEVENT_LEAVE:
            window->mMouseFocus = false;
            updateCaption = true;
            break;

            // Window has keyboard focus
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            window->mKeyboardFocus = true;
            updateCaption = true;
            break;

            // Window has lost keyboard focus
        case SDL_WINDOWEVENT_FOCUS_LOST:
            if (window->mFullScreen) {
                LW_setFullScreenStateTo(window, false);
            }
            window->mKeyboardFocus = false;
            updateCaption = true;
            break;

            // window->minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            window->mMinimized = true;
            break;

            // window->maximized
        case SDL_WINDOWEVENT_MAXIMIZED:
            if (window->mKeyboardFocus) {
            }
            break;

            // Window restored
        case SDL_WINDOWEVENT_RESTORED:
            if (window->mFullScreen) {
                /* setFullScreenTo(window, false); */
            }
            window->mMinimized = false;
            break;

            // Hide on close
        case SDL_WINDOWEVENT_CLOSE:
            LW_setFullScreenStateTo(window, false);
            SDL_HideWindow(window->mWindow);
            window->mShown = false;
            break;
        }
 
        // Update window caption with new data
        if (updateCaption) {
            char* caption = NULL;

            asprintf(&caption,
                     "SDL Tutorial ID: %i Display: %i - MouseFocus: %s, KeyboardFocus: %s",
                     window->mWindowID,
                     window->mWindowDisplayID,
                     (window->mMouseFocus) ? "On" : "Off",
                     (window->mKeyboardFocus) ? "On" : "Off");
            SDL_SetWindowTitle(window->mWindow, caption);
            
            free(caption);
        }
        
    } else if (event->type == SDL_KEYDOWN) {
        bool switchDisplay = false;
        switch (event->key.keysym.sym) {
        case SDLK_UP:
            ++window->mWindowDisplayID;
            switchDisplay = true;
            break;

        case SDLK_DOWN:
            --window->mWindowDisplayID;
            switchDisplay = true;
            break;

        case SDLK_RETURN:
            if (window->mKeyboardFocus) {
                PRINT(".");
                if (window->mFullScreen) {
                    LW_setFullScreenStateTo(window, false);
                } else {
                    LW_setFullScreenStateTo(window, true);
                    window->mMinimized = false;
                }
            }
            break;
        }
        // @FIX windows do not switch displays on button presses (or ever!)
        if (switchDisplay) {
            // Bound the display index
            if (window->mWindowDisplayID < 0) {
                window->mWindowDisplayID = window->mNumberOfDisplays - 1;
            } else if (window->mWindowDisplayID >= window->mNumberOfDisplays) {
                window->mWindowDisplayID = 0;
            }

            // Move the window to the center of the next display
        }
    }
}

void LW_setFullScreenStateTo(LWindow* window, bool active) {
    SDL_SetWindowFullscreen(window->mWindow, active);
    window->mFullScreen = active;
}

void LW_focus(LWindow* window) {
    // Restore window if needed
    if (!window->mShown) {
        SDL_ShowWindow(window->mWindow);
    }

    // Move the window forward
    SDL_RaiseWindow(window->mWindow);
}

int LW_prepareForRender(LWindow* window, SDL_Color wipeColor) {
    SDL_SetRenderDrawColor(window->mRenderer,
                           wipeColor.r,
                           wipeColor.g,
                           wipeColor.b,
                           wipeColor.a);
    
    return SDL_RenderClear(window->mRenderer);
}

void LW_render(LWindow* window) {
    if (!window->mMinimized) {
        // Update Screen
        SDL_RenderPresent(window->mRenderer);
    }
}


// Accessors
int LW_windowIDOf(LWindow* window){
    return window->mWindowID;
}
int LW_windowDisplayIDOf(LWindow* window) {
    return window->mWindowDisplayID;
}
int LW_widthOf(LWindow* window) {
    return window->mWidth;
}
int LW_heightOf(LWindow* window) {
    return window->mHeight;
}
bool LW_hasMouseFocus(LWindow* window) {
    return window->mMouseFocus;
}
bool LW_hasKeyboardFocus(LWindow* window) {
    return window->mKeyboardFocus;
}
bool LW_isFullScreen(LWindow* window) {
    return window->mFullScreen;
}
bool LW_isMinimized(LWindow* window) {
    return window->mMinimized;
}
bool LW_isShown(LWindow* window) {
    return window->mShown;
}



