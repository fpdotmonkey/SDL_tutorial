#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <math.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


//////////////
// Typedefs //
//////////////

// C doesn't have bool, so gotta make it myself
typedef enum boolean {
    true = 1,
    false = 0
} bool;

// Key press surfaces constants
enum KeypressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT = 0, // enums count up by 1
    KEY_PRESS_SURFACE_UP,          // default initial value is 0
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

enum LButtonSprite {
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION,
    BUTTON_SPRITE_MOUSE_DOWN,
    BUTTON_SPRITE_MOUSE_UP,
    BUTTON_SPRITE_TOTAL
};

// Wrapper struct for textures, allows for easy access to width and height
typedef struct {
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
} LTexture;

// Wrapper struct for mouse input
typedef struct {
    // Top-left position
    SDL_Point mPosition;

    // Currently used global sprite
    enum LButtonSprite mCurrentSprite;
} LButton;


//////////////////////
// Global Variables //
//////////////////////

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The window renderer
SDL_Renderer* gRenderer = NULL;

// Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

// This week's sprite sheet texture
LTexture gButtonSpriteSheetTexture;

// Buttons
LButton gButtons[4];

// The globally used font
TTF_Font *gFont = NULL;

// Rendered text texture
LTexture gTextTexture;

/* // Rotation and flipping lesson */
/* LTexture gArrowTexture; */

/* // Walking animation */
/* const int WALKING_ANIMATION_FRAMES = 4; */
/* SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES]; */
/* LTexture gSpriteSheetTexture; */

/* // Color modulation textures */
/* LTexture gModulatedTexture; */

/* // Background Texture */
/* LTexture gBackgroundTexture; */

/* // Scene Sprites */
SDL_Rect gSpriteClips[4];
/* LTexture gSpriteSheetTexture; */


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
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
                           TTF_GetError());
                    success = false;
                }
            }
        }
    }
    
    return success;
}


//////////////////////
// LTexture "class" //
//////////////////////

// Loads image at specified path
LTexture loadLTexture(char* path) {
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
        newLTexture.mTexture = SDL_CreateTextureFromSurface(gRenderer,
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

#ifdef _SDL_TTF_H
// Creates image from font string
LTexture loadFromRenderedText(char* textureText, SDL_Color textColor) {
    // Prepare output
    LTexture newLTexture; 
    
    // TTF_RenderText_Solid is not the only text renderer available
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont,
                                                    textureText,
                                                    textColor);
    if (textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n",
               TTF_GetError());        
    } else {
        //Create texture from surface pixels
        newLTexture.mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
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
#endif

void freeLTexture(LTexture toFree) {
    if (toFree.mTexture != NULL) {
        SDL_DestroyTexture(toFree.mTexture);
        toFree.mWidth = 0;
        toFree.mHeight = 0;
    }
}

// Renders texture at given point
void render(LTexture toRender, int x, int y,
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
    SDL_RenderCopyEx(gRenderer, toRender.mTexture, clip, &renderQuad,
                     angle, center, flip);
}


/////////////////////
// LButton "class" //
/////////////////////

void handleEvent(LButton button, SDL_Event* e) {
    if (e->type == SDL_MOUSEMOTION) {
        printf("x: %i\n", e->motion.x);
    }
    // If a mouse event happens
    if (e->type == SDL_MOUSEMOTION ||  // TODO: WHY ISN'T THIS WORKING???
        e->type == SDL_MOUSEBUTTONDOWN ||
        e->type == SDL_MOUSEBUTTONUP) {
        // Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        // Check if mouse is in button
        bool inside = true;
        
        if (x < button.mPosition.x) {
            // Mouse is left of the button
            inside = false;
        } else if (x > button.mPosition.x + BUTTON_WIDTH) {
            // Mouse is right of the button
            inside = false;
        } else if (y < button.mPosition.y) {
            // Mouse is above the button
            inside = false;
        } else if (y > button.mPosition.y + BUTTON_HEIGHT) {
            // Mouse is below the button
            inside = false;
        }

        // Mouse is outside the button
        if (!inside) {
            button.mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        } else { // Mouse is inside the button            
            // Set mouse over sprite
            switch (e->type) {
            case SDL_MOUSEMOTION:
                button.mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

            case SDL_MOUSEBUTTONDOWN:
                button.mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

            case SDL_MOUSEBUTTONUP:
                button.mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;                
            }
        }
    }
}

void renderLButton(LButton button) {
    // Show current button sprite
    render(gButtonSpriteSheetTexture, button.mPosition.x, button.mPosition.y,
           &gSpriteClips[button.mCurrentSprite],
           0.0, NULL, SDL_FLIP_NONE);
}


/* // Load media with SDL_TTF */
/* bool loadMedia() { */
/*     // Loading success flag */
/*     bool success = true; */

/*     // Open the font */
/*     gFont = TTF_OpenFont("lazy.ttf", 28); */
/*     if (gFont == NULL) { */
/*         printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError()); */
/*         success = false; */
/*     } else { */
/*         // Render text */
/*         SDL_Color textColor = { 0, 0, 0 }; */
/*         gTextTexture = */
/*             loadFromRenderedText("The quick brown fox jumped over the lazy dog" */
/*                                  , textColor); */
/*         if (gTextTexture.mTexture == NULL) { */
/*             printf("Failed to render text texture!\n"); */
/*             success = false; */
/*         } */
/*     } */

/*     return success; */
/* } */

/* // Semi-generic load media function */
/* bool loadMedia() { */
/*     // Loading success flag */
/*     bool success = true; */

/*     // Load texture from path */
/*     gArrowTexture = loadLTexture("arrow.png"); */
/*     if (gArrowTexture.mTexture == NULL) { */
/*         printf("Failed to load texture image!\n"); */
/*         success = false; */
/*     } */

/*     return success; */
/* } */

/* // Load media for VSync lesson */
/* bool loadMedia() { */
/*     // Load success flag */
/*     bool success = true; */

/*     // Load sprite sheet texture */
/*     gSpriteSheetTexture = loadLTexture("foo.png"); */
/*     if (gSpriteSheetTexture.mTexture == NULL) { */
/*         printf("Failed to load walking animation texture!\n"); */
/*         success = false; */
/*     } else { */
/*         // Set sprite clips */
/*         gSpriteClips[0].x = 0; */
/*         gSpriteClips[0].y = 0; */
/*         gSpriteClips[0].w = 64; */
/*         gSpriteClips[0].h = 205; */

/*         gSpriteClips[1].x = 64; */
/*         gSpriteClips[1].y = 0; */
/*         gSpriteClips[1].w = 64; */
/*         gSpriteClips[1].h = 205; */
        
/*         gSpriteClips[2].x = 128; */
/*         gSpriteClips[2].y = 0; */
/*         gSpriteClips[2].w = 64; */
/*         gSpriteClips[2].h = 205; */
        
/*         gSpriteClips[3].x = 196; */
/*         gSpriteClips[3].y = 0; */
/*         gSpriteClips[3].w = 64; */
/*         gSpriteClips[3].h = 205; */
/*     } */

/*     return success; */
/* } */

/* // Loading for alpha textures (with SDL2's fancy hardware accelerated blending */
/* bool loadMedia() { */
/*     // Loading success flag */
/*     bool success = true; */

/*     // Load front alpha texture */
/*     gModulatedTexture = loadLTexture("fadeout.png"); */
/*     if (gModulatedTexture.mTexture == NULL) { */
/*         printf("Failed to load texture image!\n"); */
/*         success = false; */
/*     } else { */
/*         // Set standard alpha blending */
/*         SDL_SetTextureBlendMode(gModulatedTexture.mTexture, */
/*                                 SDL_BLENDMODE_BLEND); */
/*     } */

/*     // Load Background texture */
/*     gBackgroundTexture = loadLTexture("fadein.png"); */
/*     if (gBackgroundTexture.mTexture == NULL) { */
/*         printf("Failed to load background texture!\n"); */
/*         success = false; */
/*     } */

/*     return success; */
/* } */

/* // Semi-generic load media function */
/* bool loadMedia() { */
/*     // Loading success flag */
/*     bool success = true; */

/*     // Load texture from path */
/*     gModulatedTexture = loadLTexture("colors.png"); */
/*     if (gModulatedTexture.mTexture == NULL) { */
/*         printf("Failed to load texture image!\n"); */
/*         success = false; */
/*     } */

/*     return success; */
/* } */

/* bool loadMedia() { */
/*     // Loading success flag */
/*     bool success = true; */

/*     // Load sprite sheet textures */
/*     gSpriteSheetTexture = loadLTexture("dots.png"); */
/*     if (gSpriteSheetTexture.mTexture == NULL) { */
/*         printf("Failed to load foo texture image!\n"); */
/*         success = false; */
/*     } else { */
/*         // Top-left sprite */
/*         gSpriteClips[0].x = 0; */
/*         gSpriteClips[0].y = 0; */
/*         gSpriteClips[0].w = 100; */
/*         gSpriteClips[0].h = 100; */
        
/*         // Top-right sprite */
/*         gSpriteClips[1].x = 100; */
/*         gSpriteClips[1].y = 0; */
/*         gSpriteClips[1].w = 100; */
/*         gSpriteClips[1].h = 100; */

/*         // Bottom-left sprite */
/*         gSpriteClips[2].x = 0; */
/*         gSpriteClips[2].y = 100; */
/*         gSpriteClips[2].w = 100; */
/*         gSpriteClips[2].h = 100; */

/*         // Bottom-right sprite */
/*         gSpriteClips[3].x = 100; */
/*         gSpriteClips[3].y = 100; */
/*         gSpriteClips[3].w = 100; */
/*         gSpriteClips[3].h = 100; */
/*     } */

/*     return success; */
/* } */

// load media for LButtons tutorial
bool loadMedia() {
	//Loading success flag
    bool success = true;

    //Load sprites
    gButtonSpriteSheetTexture = loadLTexture("button.png");
    if (gButtonSpriteSheetTexture.mTexture == NULL) {
        printf( "Failed to load button sprite texture!\n" );
        success = false;
    } else {
        //Set sprites
        for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
        {
            gSpriteClips[i].x = 0;
            gSpriteClips[i].y = i * 200;
            gSpriteClips[i].w = BUTTON_WIDTH;
            gSpriteClips[i].h = BUTTON_HEIGHT;
        }

        //Set buttons in corners
        gButtons[0].mPosition.x = 0;
        gButtons[0].mPosition.y = 0;          
        gButtons[1].mPosition.x = SCREEN_WIDTH - BUTTON_WIDTH;
        gButtons[1].mPosition.y = 0;
        gButtons[2].mPosition.x = 0;
        gButtons[2].mPosition.y = SCREEN_HEIGHT - BUTTON_HEIGHT;
        gButtons[3].mPosition.x = SCREEN_WIDTH - BUTTON_WIDTH;
        gButtons[3].mPosition.y = SCREEN_HEIGHT - BUTTON_HEIGHT;
    }

    return success;
}

// Deallocate memory and shut down SDL
void closeWindow() {
    // Deallocate surface
    freeLTexture(gTextTexture);
    /* freeLTexture(gModulatedTexture);  */
    /* freeLTexture(gSpriteSheetTexture); */

    // Free global font
    TTF_CloseFont(gFont);
    gFont = NULL;
    
    // Destroy Window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
    
    // Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


int main(int argc, char* args[]) {
    // Start up SDL and create a window
    if (!init()) {
        printf("Failed to initialize!\n");
    } else {
        // Main loop flag
        bool quit = false;
        
        // Event Handler
        SDL_Event e;

        // Angle of rotation
        double degrees = 0.0;

        // Flip type
        SDL_RendererFlip flipType = SDL_FLIP_NONE;
        
        /* // Current animation frame */
        /* int frame = 0; */

        /* // Modulation components */
        /* uint8_t r = 255; */
        /* uint8_t g = 255; */
        /* uint8_t b = 255; */
        /* uint8_t a = 255; */
        
        // Load media
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        } else {
            
            while (!quit) {
                while (SDL_PollEvent(&e) != 0) { // Pulls events off the event
                                                 // queue until the event queue
                                                 // is empty
                    // User requests a quit
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {

                        // Handle button events
                        for (int i = 0; i < TOTAL_BUTTONS; ++i) {
                            handleEvent(gButtons[i], &e);
                        }

                        /* switch (e.key.keysym.sym) { */
                        /* case SDLK_a: */
                        /*     degrees -= 90; */
                        /*     break; */

                        /* case SDLK_d: */
                        /*     degrees +=60; */
                        /*     break; */

                        /* case SDLK_q: */
                        /*     flipType = SDL_FLIP_HORIZONTAL; */
                        /*     break; */

                        /* case SDLK_w: */
                        /*     flipType = SDL_FLIP_NONE; */
                        /*     break; */

                        /* case SDLK_e: */
                        /*     flipType = SDL_FLIP_VERTICAL; */
                        /*     break; */
                        /* } */
                        
                        /* switch (e.key.keysym.sym) { */
                        /*     // Increase red on q */
                        /* case SDLK_q: */
                        /*     r += 32; */
                        /*     break; */

                        /*     // Increase green on w */
                        /* case SDLK_w: */
                        /*     g += 32; */
                        /*     break; */
                            
                        /*     // Increase blue on e */
                        /* case SDLK_e: */
                        /*     b += 32; */
                        /*     break; */

                        /*     // Increase alpha on r */
                        /* case SDLK_r: */
                        /*     a += 32; */
                        /*     break; */
                            
                        /*     // Decrease red on a */
                        /* case SDLK_a: */
                        /*     r -= 32; */
                        /*     break; */
                            
                        /*     // Decrease green on s */
                        /* case SDLK_s: */
                        /*     g -= 32; */
                        /*     break; */
                            
                        /*     // Decrease blue on d */
                        /* case SDLK_d: */
                        /*     b -= 32; */
                        /*     break; */

                        /*     // Decrease alpha on f */
                        /* case SDLK_f: */
                        /*     a -= 32; */
                        /*     break; */

                        /* } */
                    }
                }
                
                
                // Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render buttons
                for (int i = 0; i < TOTAL_BUTTONS; ++i) {
                    renderLButton(gButtons[i]);
                }
                
                /* // Render current frame */
                /* render(gTextTexture, */
                /*        (SCREEN_WIDTH - gTextTexture.mWidth)/2, */
                /*        (SCREEN_HEIGHT - gTextTexture.mHeight)/2, */
                /*        NULL, */
                /*        0, */
                /*        NULL, */
                /*        SDL_FLIP_NONE); */
                
                
                /* // Render arrow */
                /* render(gArrowTexture, */
                /*        (SCREEN_WIDTH - gArrowTexture.mWidth)/2, */
                /*        (SCREEN_HEIGHT - gArrowTexture.mHeight)/2, */
                /*        NULL, */
                /*        degrees, */
                /*        NULL, */
                /*        flipType); */
                
                /* // Render current frame */
                /* SDL_Rect* currentClip = &gSpriteClips[frame/4]; */
                /* render(gSpriteSheetTexture, */
                /*        (SCREEN_WIDTH - currentClip->w)/2, */
                /*        (SCREEN_HEIGHT - currentClip->h)/2, */
                /*        currentClip); */
                
                /* // Cycle animation */
                /* ++frame; */
                /* if (frame/4 >= WALKING_ANIMATION_FRAMES) { */
                /*     frame = 0; */
                /* } */

                /* // Render Stuff for alpha and blending example */
                /* // Render Background */
                /* render(gBackgroundTexture, 0, 0, NULL); */

                /* // Render front blended */
                /* SDL_SetTextureAlphaMod(gModulatedTexture.mTexture, a); */
                /* render(gModulatedTexture, 0, 0, NULL); */

                /* // Modulate and render texture for color modulate example */
                /* SDL_SetTextureColorMod(gModulatedTexture.mTexture, r, g, b); */
                /* render(gModulatedTexture, 0, 0, NULL); */
                
                /* // Render stuff for sprite sheet example */
                /* render(gSpriteSheetTexture, */
                /*        0, */
                /*        0, */
                /*        &gSpriteClips[0]); // Top left */
                /* render(gSpriteSheetTexture, */
                /*        SCREEN_WIDTH-gSpriteClips[1].w, */
                /*        0, */
                /*        &gSpriteClips[1]);  // Top right */
                /* render(gSpriteSheetTexture, */
                /*        0, */
                /*        SCREEN_HEIGHT-gSpriteClips[2].h, */
                /*        &gSpriteClips[2]); // Bottom left */
                /* render(gSpriteSheetTexture, */
                /*        SCREEN_WIDTH-gSpriteClips[3].w, */
                /*        SCREEN_HEIGHT-gSpriteClips[2].h, */
                /*        &gSpriteClips[3]);  // Bottom right */
                
                // Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    // Free up resources and close SDL
    closeWindow();
    
    return 0;
}
