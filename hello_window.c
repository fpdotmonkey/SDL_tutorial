#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

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

// Wrapper struct for textures, allows for easy access to width and height
typedef struct {
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
} LTexture;


//////////////////////
// Global Variables //
//////////////////////

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The window renderer
SDL_Renderer* gRenderer = NULL;

// Scene Texture
LTexture gFooTexture;
LTexture gBackgroundTexture;

/* // Current Displayed texture */
/* SDL_Texture* gTexture = NULL; */

/* // The surface contained by the window */
/* SDL_Surface* gScreenSurface = NULL; */

/* // The image that correspons to a keypress */
/* SDL_Surface* gKeyPressSurface[KEY_PRESS_SURFACE_TOTAL]; */

/* // Current displayed image */
/* SDL_Surface* gCurrentSurface = NULL; */

/* //Current displayed PNG image */
/* SDL_Surface* gPNGSurface = NULL; */


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
                                           SDL_RENDERER_ACCELERATED);
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
                /* } else { */
                /*     // Get Window Surface */
                /*     gScreenSurface = SDL_GetWindowSurface(gWindow); */
                /* }     */                               
            }
        }
    }
    
    return success;
}

// Loads textures for the window
SDL_Texture* loadTexture(char* path) {
    // The final texture
    SDL_Texture* newTexture = NULL;
    
    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path);
    
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n",
               path,
               IMG_GetError);
    } else {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n",
                   path,
                   SDL_GetError());
        }
        
        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    
    return newTexture;
}

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

void freeLTexture(LTexture toFree) {
    if (toFree.mTexture != NULL) {
        SDL_DestroyTexture(toFree.mTexture);
        toFree.mWidth = 0;
        toFree.mHeight = 0;
    }
}

// Renders texture at given point
void render(LTexture toRender, int x, int y) {
    // Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, toRender.mWidth, toRender.mHeight };
    SDL_RenderCopy(gRenderer, toRender.mTexture, NULL, &renderQuad);
}

/* // Loads the individual image */
/* SDL_Surface* loadSurface(char* path) { */
/*     // The final optimized image */
/*     SDL_Surface* optimizedSurface = NULL; */
    
/*     // Load image at specified path */
/*     SDL_Surface* loadedSurface = IMG_Load(path); */
/*     if (loadedSurface == NULL) { */
/*         printf("Unable to laod image %s! SDL_image Error: %s\n", */
/*                path, */
/*                IMG_GetError()); */
/*     } else {  // Convert the surface to screen format */
/*         optimizedSurface = SDL_ConvertSurface(loadedSurface, */
/*                                               gScreenSurface->format, */
/*                                               0); */
/*         if (optimizedSurface == NULL) { */
/*             printf("Unable to optimize image %s! SDL Error: %s\n", */
/*                    path, */
/*                    SDL_GetError()); */
/*         } */
        
/*         // Get rid of old loaded surface */
/*         SDL_FreeSurface(loadedSurface); */
/*     } */
    
/*     return optimizedSurface; */
/* } */

/* // Load the images that will be displayed **FOR BUTTONS** */
/* bool loadMedia() { */
/*     // Loading Success Flag */
/*     bool success = true; */

/*     // Load default image */
/*     gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("press.bmp"); */
/*     if (gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT] == NULL) { */
/*         printf("Unable to load default image!\n"); */
/*         success = false; */
/*     } */

/*     gKeyPressSurface[KEY_PRESS_SURFACE_UP] = loadSurface("up.bmp"); */
/*     if (gKeyPressSurface[KEY_PRESS_SURFACE_UP] == NULL) { */
/*         printf("Unable to load up image!\n"); */
/*         success = false; */
/*     } */

/*     gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] = loadSurface("down.bmp"); */
/*     if (gKeyPressSurface[KEY_PRESS_SURFACE_DOWN] == NULL) { */
/*         printf("Unable to load down image!\n"); */
/*         success = false; */
/*     } */

/*     gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] = loadSurface("left.bmp"); */
/*     if (gKeyPressSurface[KEY_PRESS_SURFACE_LEFT] == NULL) { */
/*         printf("Unable to load left image!\n"); */
/*         success = false; */
/*     } */

/*     gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] = loadSurface("right.bmp"); */
/*     if (gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT] == NULL) { */
/*         printf("Unable to load default right!\n"); */
/*         success = false; */
/*     } */

/*     return success; */
/* } */

/* bool loadMedia() */
/* { */
/* 	//Loading success flag */
/* 	bool success = true; */
    
/* 	//Load PNG surface */
/* 	gTexture = loadTexture( "texture.png" ); */
/* 	if(gTexture == NULL) */
/* 	{ */
/* 		printf( "Failed to load texture image!\n" ); */
/* 		success = false; */
/* 	} */
    
/* 	return success; */
/* } */

/* // Load media (nothing) for the primitives example */
/* bool loadMedia() { */
/*     // Loading success flag */
/*     bool success = true; */

/*     // Nothing to load */
/*     return success; */
/* } */

/* // Load media for texture example
/* bool loadMedia() { */
/* 	//Loading success flag */
/* 	bool success = true; */
    
/* 	//Load texture surface */
/* 	gTexture = loadTexture("viewport.png"); */
/* 	if(gTexture == NULL) { */
/* 		printf( "Failed to load texture image!\n" ); */
/* 		success = false; */
/* 	} */

/*     return success; */
/* } */

bool loadMedia() {
    // Loading success flag
    bool success = true;

    // Load foo texture
    gFooTexture = loadLTexture("foo.png");
    if (gFooTexture.mTexture == NULL) {
        printf("Failed to load foo texture image!\n");
        success = false;
    }

    gBackgroundTexture = loadLTexture("background.png");
    if (gBackgroundTexture.mTexture == NULL) {
        printf("Failed to load background texture image\n");
        success = false;
    }

    return success;
}

// Deallocate memory and shut down SDL
void closeWindow() {
    // Deallocate surface
    freeLTexture(gFooTexture);
    freeLTexture(gBackgroundTexture);
    /* SDL_FreeSurface(gCurrentSurface); */
    /* SDL_FreeSurface(gPNGSurface); */
    /* SDL_DestroyTexture(gTexture); */
    /* gCurrentSurface = NULL; */
    /* gPNGSurface = NULL; */
    /* gTexture = NULL; */
    
    
    // Destroy Window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = NULL;
    gWindow = NULL;
    
    // Quit SDL subsystems
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
        
        /* // Set default current surface */
        /* gCurrentSurface = gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT]; */
        
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
                    }
                }
                
                // Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                // Render stuff for color keying example
                render(gBackgroundTexture, 0, 0);
                render(gFooTexture, 240, 190);

                /* // Set up viewports for viewport example */
                /* // Top-left corner viewport */
                /* SDL_Rect topLeftViewport = { 0, */
                /*                              0, */
                /*                              SCREEN_WIDTH / 2, */
                /*                              SCREEN_HEIGHT / 2 }; */
                /* SDL_RenderSetViewport(gRenderer, &topLeftViewport); */
                
                /* // Render texture to screen */
                /* SDL_RenderCopy(gRenderer, gTexture, NULL, NULL); */

                
                /* // Top-right corner viewport */
                /* SDL_Rect topRightViewport = { SCREEN_WIDTH / 2, */
                /*                               0, */
                /*                               SCREEN_WIDTH / 2, */
                /*                               SCREEN_HEIGHT / 2 }; */
                /* SDL_RenderSetViewport(gRenderer, &topRightViewport); */
                
                /* // Render texture to screen */
                /* SDL_RenderCopy(gRenderer, gTexture, NULL, NULL); */

                
                /* // Bottom viewport */
                /* SDL_Rect bottomViewport = { 0, */
                /*                             SCREEN_HEIGHT / 2, */
                /*                             SCREEN_WIDTH, */
                /*                             SCREEN_HEIGHT / 2 }; */
                /* SDL_RenderSetViewport(gRenderer, &bottomViewport); */

                /* // Render texture to screen */
                /* SDL_RenderCopy(gRenderer, gTexture, NULL, NULL); */

                /* // Render primitives for primitive example */
                /* // Render red-filled quadrilateral */
                /* SDL_Rect fillRect = { SCREEN_WIDTH/4, */
                /*                       SCREEN_HEIGHT/4, */
                /*                       SCREEN_WIDTH/2, */
                /*                       SCREEN_HEIGHT/2 }; */
                /* SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF); */
                /* SDL_RenderFillRect(gRenderer, &fillRect); */

                /* // Render green-outlined quadrilateral */
                /* SDL_Rect outlineRect = { SCREEN_WIDTH/6, */
                /*                          SCREEN_HEIGHT/6, */
                /*                          SCREEN_WIDTH*2/3, */
                /*                          SCREEN_HEIGHT*2/3 }; */
                /* SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF); */
                /* SDL_RenderDrawRect(gRenderer, &outlineRect); */

                /* // Render blue horizontal line */
                /* SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF); */
                /* SDL_RenderDrawLine(gRenderer, */
                /*                    0, */
                /*                    SCREEN_HEIGHT/2, */
                /*                    SCREEN_WIDTH, */
                /*                    SCREEN_HEIGHT/2); */

                /* // Draw vertical line of yellow dots */
                /* SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF); */
                /* for (int i = 0; i < SCREEN_HEIGHT; i += 4) { */
                /*     SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH/2, i); */
                /* } */
                
                /* // Render texture to screen */
                /* SDL_RenderCopy(gRenderer, gTexture, NULL, NULL); */
                
                // Update screen
                SDL_RenderPresent(gRenderer);
                
                /* // Apply the PNG image */
				/* SDL_BlitSurface( gPNGSurface, NULL, gScreenSurface, NULL ); */
                
                /* // Select surface based on button press */
                /* switch (e.key.keysym.sym) { */
                /* case SDLK_UP: */
                /*     gCurrentSurface = */
                /*         gKeyPressSurface[KEY_PRESS_SURFACE_UP]; */
                /*     break; */
                
                /* case SDLK_DOWN: */
                /*     gCurrentSurface = */
                /*         gKeyPressSurface[KEY_PRESS_SURFACE_DOWN]; */
                /*     break; */
                
                /* case SDLK_LEFT: */
                /*     gCurrentSurface = */
                /*         gKeyPressSurface[KEY_PRESS_SURFACE_LEFT]; */
                /*     break; */
                
                /* case SDLK_RIGHT: */
                /*     gCurrentSurface = */
                /*         gKeyPressSurface[KEY_PRESS_SURFACE_RIGHT]; */
                /*     break; */
                
                /* default: */
                /*     gCurrentSurface = */
                /*         gKeyPressSurface[KEY_PRESS_SURFACE_DEFAULT]; */
                /*     break; */
                /* } */
                
                
                /* // Apply the image stretched */
                /* // Blitting is applying a source image (gHelloWorld) */
                /* //  to a screen (gScreenSurface) */
                /* SDL_Rect stretchRect; */
                /* stretchRect.x = 0; */
                /* stretchRect.y = 0; */
                /* stretchRect.w = SCREEN_WIDTH; */
                /* stretchRect.h = SCREEN_HEIGHT; */
                /* SDL_BlitScaled(gCurrentSurface, */
                /*                NULL, */
                /*                gScreenSurface, */
                /*                &stretchRect); */
                
                /* // Update the surface */
                /* SDL_UpdateWindowSurface(gWindow); */
            }
        }
    }
    // Free up resources and close SDL
    closeWindow();
    
    return 0;
}
