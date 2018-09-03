//
// LWindow.h
//
// Copyright (c) 2018 Fletcher Porter
//

// Handles events for resizing windows, is a wrapper for SDL_Window

#ifndef __LWINDOW_H__
#define __LWINDOW_H__

#include <SDL.h>

#include "bool.h"

typedef struct {
    // Window data
    SDL_Window* mWindow;
    int mWindowID;

    // Renderer
    SDL_Renderer* mRenderer;

    // Window dimensions
    int mWidth, mHeight;

    // Window focus
    bool mMouseFocus, mKeyboardFocus, mFullScreen, mMinimized, mShown;
} LWindow;

// Initialize a window, macros SCREEN_WIDTH and SCREEN_HEIGHT must be
// defined prior to this function being called
bool LW_init(LWindow* window, int screenWidth, int screenHeight);

// Generates an SDL_Renderer* for an LWindow
SDL_Renderer* LW_createRenderer(LWindow window);

// Handles window events for the window
void LW_handleEvent(LWindow* window, SDL_Event* e);

// Handles the perpendicular-to-screen position of the window
void LW_focus(LWindow* window);

// Renders a new frame to the window
void LW_render(LWindow* window);

// Returns whether the current window is visible
bool LW_isShown(LWindow* window);

// Deallocate all the memory
void LW_free(LWindow* window);



#endif // !__LWINDOW_H__
