//
// LWindow.h
//
// Copyright (c) 2018 Fletcher Porter
//

// Create a window

#ifndef __LWINDOW_H__
#define __LWINDOW_H__

#include <SDL.h>

#include "bool.h"
#include "LPrints.h"

typedef struct {
    // Window data
    SDL_Window* mWindow;
    int mWindowID, mWindowDisplayID, mNumberOfDisplays;

    // Renderer
    SDL_Renderer* mRenderer;

    // Window dimensions
    int mWidth, mHeight;

    // Window focus
    bool mMouseFocus, mKeyboardFocus, mFullScreen, mMinimized, mShown;
} LWindow;

// Initialize a window, macros SCREEN_WIDTH and SCREEN_HEIGHT must be
// defined prior to this function being called
bool LW_init(LWindow* window, int numberOfDisplays, int screenWidth, int screenHeight);

// Handles window events for the window
void LW_handleEvent(LWindow* window, SDL_Event* event);

// Change the fullscreen state of the window
void LW_setFullScreenStateTo(LWindow* window, bool active);

// Moves the window to the front of the screen
void LW_focus(LWindow* window);

// Sets up the next frame to be rendered to be a blanks slate of color
// wipeColor.  Returns 0 on succes and <0 on failure
int LW_prepareForRender(LWindow* window, SDL_Color wipeColor);

// Renders a new frame to the window
void LW_render(LWindow* window);

// Deallocate the LWindow's memory
void LW_free(LWindow* window);

// Accessors
int LW_windowIDOf(LWindow* window);
int LW_windowDisplayIDOf(LWindow* window);
int LW_numberOfDisplaysFor(LWindow* window);
int LW_widthOf(LWindow* window);
int LW_heightOf(LWindow* window);
bool LW_hasMouseFocus(LWindow* window);
bool LW_hasKeyboardFocus(LWindow* window);
bool LW_isFullScreen(LWindow* window);
bool LW_isMinimized(LWindow* window);
bool LW_isShown(LWindow* window);


#endif // !__LWINDOW_H__
