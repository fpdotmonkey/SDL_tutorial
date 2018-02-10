//
// LTexture.c
//
// Copyright (c) 2018 Fletcher Porter
//

#include "LTexture.h"

// Loads image at specified path
LTexture loadLTexture(char* path, SDL_Renderer* renderer) {
    // Prepare output variable
    LTexture newLTexture = { NULL, 0, 0 };

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n",
               path,
               IMG_GetError());
    } else {
        // Color key image
        SDL_SetColorKey(loadedSurface,
                        SDL_TRUE,
                        SDL_MapRGB(loadedSurface->format,
                                   0,
                                   0xFF,
                                   0xFF)
            );

        // Create texture from surface pixels
        newLTexture.mTexture = SDL_CreateTextureFromSurface(renderer,
                                                            loadedSurface);
        if (newLTexture.mTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n",
                   path, SDL_GetError());
        } else {
            // Get image dimensions
            newLTexture.mWidth = loadedSurface->w;
            newLTexture.mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
        
    return newLTexture;;
}

// Creates image from font string
LTexture loadFromRenderedText(char* textureText,
                              SDL_Color textColor,
                              TTF_Font* font,
                              SDL_Renderer* renderer) {
    // Prepare output
    LTexture newLTexture; 

    // TTF_RenderText_Solid is not the only text renderer available
    SDL_Surface* textSurface = TTF_RenderText_Solid(font,
                                                    textureText,
                                                    textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n",
               TTF_GetError());        
    } else {
        //Create texture from surface pixels
        newLTexture.mTexture = SDL_CreateTextureFromSurface(renderer,
                                                            textSurface);
        if (newLTexture.mTexture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n",
                   SDL_GetError());
        } else {
            // Get image dimensions
            newLTexture.mWidth = textSurface->w;
            newLTexture.mHeight = textSurface->h;
        }

        // Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    return newLTexture;    
}

void freeLTexture(LTexture toFree) {
    if (toFree.mTexture != NULL) {
        SDL_DestroyTexture(toFree.mTexture);
        toFree.mWidth = 0;
        toFree.mHeight = 0;
    }
}

// Renders texture at given point
void render(SDL_Renderer* renderer, LTexture toRender, int x, int y) {    
    // Set rendering space
    SDL_Rect renderQuad = { x, y, toRender.mWidth, toRender.mHeight };

    // Render to screen
    SDL_RenderCopyEx(renderer, toRender.mTexture, NULL, &renderQuad,
                     0.0, NULL, SDL_FLIP_NONE);
}

// Renders texture at given point
void render_c(SDL_Renderer* renderer, LTexture toRender, int x, int y,
               SDL_Rect* clip) {
    // USEFUL DEFAULT VALUES:
    // clip = NULL
    
    // Set rendering space
    SDL_Rect renderQuad = { x, y, toRender.mWidth, toRender.mHeight };

    // Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopyEx(renderer, toRender.mTexture, clip, &renderQuad,
                     0.0, NULL, SDL_FLIP_NONE);
}

// Renders texture at given point
void render_ct(SDL_Renderer* renderer, LTexture toRender, int x, int y,
               SDL_Rect* clip,
               double angle, SDL_Point* center, SDL_RendererFlip flip) {
    // USEFUL DEFAULT VALUES:
    // clip = NULL
    // angle = 0.0
    // center = NULL
    // flip = SDL_FLIP_NONE
    
    // Set rendering space
    SDL_Rect renderQuad = { x, y, toRender.mWidth, toRender.mHeight };

    // Set clip rendering dimensions
    if (clip != NULL) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopyEx(renderer, toRender.mTexture, clip, &renderQuad,
                     angle, center, flip);
}
