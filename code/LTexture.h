//
// LTexture.h
//
// Copyright (c) 2018 Fletcher Porter
//

#ifndef __LTEXTURE_H__
#define __LTEXTURE_H__


#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "bool.h"
#include "LWindow.h"

typedef struct {
    SDL_Texture* texture;

    SDL_Rect *clippingRectangle;
    double drawAngle;
    SDL_Point *drawCenter;
    SDL_RendererFlip drawFlip;
    
    TTF_Font* textFont;
    SDL_Color textColor;

    int width;
    int height;
} LTexture;


bool LT_init(LTexture *texture);

void LT_free(LTexture *texture);

// Loads image at specified path
bool LT_loadImage(LTexture *texture, LWindow *window, char* path);

bool LT_loadFont(LTexture *texture, char* path);

void LT_setTextColor(LTexture *texture, SDL_Color toColor);

// Creates image from font string
bool LT_generateText(LTexture *texture,
                     LWindow *window,
                     char *textureText);

void LT_setDrawParameters(LTexture *texture,
                          SDL_Rect *renderClippingRectangle,
                          double renderAngle,
                          SDL_Point *renderCenter,
                          SDL_RendererFlip rendererFlip);

// Draws a texture with previously set draw parameters to window to be rendered
void LT_draw(LTexture *texture, LWindow *window,
             int x, int y, int width, int height);


#endif // __LTEXTURE_H__
