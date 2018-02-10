//
// LTexture.h
//
// Copyright (c) 2018 Fletcher Porter
//

#ifndef LTEXTURE_H
#define LTEXTURE_H


#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// Wrapper struct for textures, allows for easy access to width and height
typedef struct {
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
} LTexture;

// Loads image at specified path
LTexture loadLTexture(char* path, SDL_Renderer* renderer);

// Creates image from font string
LTexture loadFromRenderedText(char* textureText,
                              SDL_Color textColor,
                              TTF_Font* font,
                              SDL_Renderer* renderer);

void freeLTexture(LTexture toFree);

// Renders texture at given point
void render(SDL_Renderer* renderer, LTexture toRender, int x, int y);

// Renders texture at a given point and clipping
void render_c(SDL_Renderer* renderer, LTexture toRender, int x, int y,
               SDL_Rect* clip);

// Renders texture at given point and clipping with a rotation,
// translation, and flip
void render_ct(SDL_Renderer* renderer, LTexture toRender, int x, int y,
               SDL_Rect* clip,
               double angle, SDL_Point* center, SDL_RendererFlip flip);


#endif
