//
// LWindow.c
//
// Copyright (c) 2018 Fletcher Porter
//

#include "LWindow.h"

static int prepareForRender(LWindow* window) {
    SDL_SetRenderDrawColor(window->mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    return SDL_RenderClear(window->mRenderer);
}

bool LW_init(LWindow* window, int screenWidth, int screenHeight) {
    window->mWindow = SDL_CreateWindow("SDL Tutorial",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       screenWidth,
                                       screenHeight,
                                       SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window->mWindow != NULL) {
        window->mWidth = screenWidth;
        window->mHeight = screenHeight;
        
        window->mMouseFocus = true;
        window->mKeyboardFocus = true;

        window->mRenderer = SDL_CreateRenderer(window->mWindow,
                                               -1,
                                               SDL_RENDERER_ACCELERATED |
                                               SDL_RENDERER_PRESENTVSYNC);
        if (window->mRenderer != NULL) {
            // Initialize renderer color
            prepareForRender(window);

            // Grab window identifier
            window->mWindowID = SDL_GetWindowID(window->mWindow);

            // Flag as opened
            window->mShown = true;
        } else {
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            SDL_DestroyWindow(window->mWindow);
            window->mWindow = NULL;
        }
    } else {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    }

    return window->mWindow != NULL && window->mRenderer != NULL;
}

void LW_handleEvent(LWindow* window, SDL_Event* event) {
    // Window Event Occured
    if (event->type == SDL_WINDOWEVENT
        && event->window.windowID == window->mWindowID) {
        // Caption update flag
        bool updateCaption = false;

        switch (event->window.event) {
            // Get new dimensions and repaint on window size change
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
            window->mKeyboardFocus = false;
            updateCaption = true;
            break;

            // window->minimized
        case SDL_WINDOWEVENT_MINIMIZED:
            window->mMinimized = true;
            break;

            // window->maximized
        case SDL_WINDOWEVENT_MAXIMIZED:
            window->mMinimized = false;
            break;

            // Window restored
        case SDL_WINDOWEVENT_RESTORED:
            window->mMinimized = false;
            break;

            // Hide on close
        case SDL_WINDOWEVENT_CLOSE:
            SDL_HideWindow(window->mWindow);
            break;
        }
 
        // Update window caption with new data
        if (updateCaption) {
            char* caption = NULL;

            asprintf(&caption,
                     "SDL Tutorial %i - MouseFocus: %s, KeyboardFocus: %s",
                     window->mWindowID,
                     (window->mMouseFocus) ? "On" : "Off",
                     (window->mKeyboardFocus) ? "On" : "Off");
            SDL_SetWindowTitle(window->mWindow, caption);
            
            free(caption);
        }
        
    } else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_RETURN) {
        if (window->mFullScreen) {
            SDL_SetWindowFullscreen(window->mWindow, false);
        } else {
            SDL_SetWindowFullscreen(window->mWindow, true);
            window->mFullScreen = true;
            window->mMinimized = false;
        }
    }
}

void LW_focus(LWindow* window) {
    // Restore window if needed
    if (!window->mShown) {
        SDL_ShowWindow(window->mWindow);
    }

    // Move the window forward
    SDL_RaiseWindow(window->mWindow);
}

void LW_render(LWindow* window) {
    if (!window->mMinimized) {
        // Clear Screen
        prepareForRender(window);

        // Update Screen
        SDL_RenderPresent(window->mRenderer);
    }
}

bool LW_isShown(LWindow* window) {
    return window->mShown;
}

void LW_free(LWindow* window) {
    // Destroy Window
    SDL_DestroyWindow(window->mWindow);
    window->mWindow = NULL;
    window->mRenderer = NULL;
}


