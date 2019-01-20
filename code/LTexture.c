//
// LTexture.c
//
// Copyright (c) 2018 Fletcher Porter
//

#include "LTexture.h"


bool LT_init(LTexture *texture) {
    texture->texture = NULL;
    
    texture->clippingRectangle = NULL;
    texture->drawAngle = 0.0;
    texture->drawCenter = NULL;
    texture->drawFlip = SDL_FLIP_NONE;

    texture->textFont = NULL;
    /* texture->textColor = { 0, 0, 0, 0xFF };  // Black */

    texture->width = 0;
    texture->height = 0;
    
    return true;
}

void LT_free(LTexture *texture) {
    if (texture->texture != NULL) {
        SDL_DestroyTexture(texture->texture);
        texture->width = 0;
        texture->height = 0;
    }

    texture->clippingRectangle = NULL;
    texture->drawCenter = NULL;
    
    if (texture->textFont != NULL) {
        TTF_CloseFont(texture->textFont);
    }

    texture = NULL;
}

// Loads image at specified path
bool LT_loadImage(LTexture *texture, LWindow *window, char* fromPath) {
    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(fromPath);
    if (loadedSurface == NULL) {
        ErrorIMG_path("Unable to load image!", fromPath);

        return false;
    } else {
        // Color key image
        SDL_SetColorKey(loadedSurface,
                        true,
                        SDL_MapRGB(loadedSurface->format,
                                   0,
                                   0xFF,
                                   0xFF)
            );

        // Create texture from surface pixels
        texture->texture = SDL_CreateTextureFromSurface(window->mRenderer,
                                                        loadedSurface);
        if (texture->texture == NULL) {
            ErrorSDL_path("Unable to create texture!", fromPath);
        } else {
            // Get image dimensions
            texture->width = loadedSurface->w;
            texture->height = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);

        return true;
    }
}

bool LT_loadFont(LTexture *texture, char* fromPath, int pointSize) {
    texture->textFont = TTF_OpenFont(fromPath, pointSize);

    if (texture->textFont != NULL) {
        
        return true;
    } else {
        ErrorTTF_path("Could not load font!", fromPath);
    }

    return false;
}

void LT_setTextColor(LTexture *texture, SDL_Color toColor) {
    texture->textColor = toColor;
}

// Creates image from font string
bool LT_generateText(LTexture *texture,
                     LWindow *window,
                     char *textureText) {

    // TTF_RenderText_Solid is not the only text renderer available
    SDL_Surface* textSurface = TTF_RenderText_Solid(texture->textFont,
                                                    textureText,
                                                    texture->textColor);
    if (textSurface == NULL) {
        ErrorTTF("Unable to render text surface!");
        
        return false;
    } else {
        //Create texture from surface pixels
        texture->texture = SDL_CreateTextureFromSurface(window->mRenderer,
                                                        textSurface);
        if (texture->texture == NULL) {
            ErrorSDL("Unable to create texture from rendered text!");

            return false;
        } else {
            // Get image dimensions
            texture->width = textSurface->w;
            texture->height = textSurface->h;
        }

        // Get rid of old surface
        SDL_FreeSurface(textSurface);

        return true;
    }
}

void LT_setDrawParameters(LTexture *texture,
                          SDL_Rect *clippingRectangle,
                          double drawAngle,
                          SDL_Point *drawCenter,
                          SDL_RendererFlip drawFlip) {
    if (clippingRectangle != NULL) {
        texture->clippingRectangle = clippingRectangle;
    }

    texture->drawAngle = drawAngle;
    texture->drawCenter = drawCenter;
    texture->drawFlip = drawFlip;
}

// Renders texture at given point
void LT_draw(LTexture *texture, LWindow *window,
             int x, int y, int width, int height) {    
    // Set rendering space
    SDL_Rect renderQuad = { x, y, width, height };

    /* if (texture->clippingRectangle != NULL) { */
    /*     renderQuad.w = texture->clippingRectangle->w; */
    /*     renderQuad.h = texture->clippingRectangle->h; */
    /* } */
    
    // Render to screen
    SDL_RenderCopyEx(window->mRenderer,
                     texture->texture,
                     texture->clippingRectangle,
                     &renderQuad,
                     texture->drawAngle,
                     texture->drawCenter,
                     texture->drawFlip);
}

/* // Renders texture at given point */
/* void render_c(SDL_Renderer* renderer, LTexture toRender, int x, int y, */
/*                SDL_Rect* clip) { */
/*     // USEFUL DEFAULT VALUES: */
/*     // clip = NULL */
    
/*     // Set rendering space */
/*     SDL_Rect renderQuad = { x, y, toRender.width, toRender.height }; */

/*     // Set clip rendering dimensions */
/*     if (clip != NULL) { */
/*         renderQuad.w = clip->w; */
/*         renderQuad.h = clip->h; */
/*     } */

/*     // Render to screen */
/*     SDL_RenderCopyEx(renderer, toRender.texture, clip, &renderQuad, */
/*                      0.0, NULL, SDL_FLIP_NONE); */
/* } */

/* // Renders texture at given point */
/* void render_ct(SDL_Renderer* renderer, LTexture toRender, int x, int y, */
/*                SDL_Rect* clip, */
/*                double angle, SDL_Point* center, SDL_RendererFlip flip) { */
/*     // USEFUL DEFAULT VALUES: */
/*     // clip = NULL */
/*     // angle = 0.0 */
/*     // center = NULL */
/*     // flip = SDL_FLIP_NONE */
    
/*     // Set rendering space */
/*     SDL_Rect renderQuad = { x, y, toRender.width, toRender.height }; */

/*     // Set clip rendering dimensions */
/*     if (clip != NULL) { */
/*         renderQuad.w = clip->w; */
/*         renderQuad.h = clip->h; */
/*     } */

/*     // Render to screen */
/*     SDL_RenderCopyEx(renderer, toRender.texture, clip, &renderQuad, */
/*                      angle, center, flip); */
/* } */
