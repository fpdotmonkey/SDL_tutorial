#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

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

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// The image that correspons to a keypress
SDL_Surface* gKeyPressSurface[KEY_PRESS_SURFACE_TOTAL];

// Current displayed image
SDL_Surface* gCurrentSurface = NULL;

//Current displayed PNG image
SDL_Surface* gPNGSurface = NULL;


// Initialize SDL and load the window
bool init() {
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if ((SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    } else {
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
            // Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)) {
                printf("SDL_image could not initialize! SDL_image error: %s\n",
                       IMG_GetError());
                success = false;
            } else {
                // Get Window Surface
                gScreenSurface = SDL_GetWindowSurface(gWindow);
            }                                   
        }
    }
    return success;
}

// Loads the individual image
SDL_Surface* loadSurface(char* path) {
    // The final optimized image
    SDL_Surface* optimizedSurface = NULL;
    
    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to laod image %s! SDL_image Error: %s\n",
               path,
               IMG_GetError());
    } else {  // Convert the surface to screen format
        optimizedSurface = SDL_ConvertSurface(loadedSurface,
                                              gScreenSurface->format,
                                              0);
        if (optimizedSurface == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s\n",
                   path,
                   SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    
    return optimizedSurface;
}

/* // Load the images that will be displayed */
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

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG surface
	gPNGSurface = loadSurface( "06_extension_libraries_and_loading_other_image_formats/loaded.png" );
	if( gPNGSurface == NULL )
	{
		printf( "Failed to load PNG image!\n" );
		success = false;
	}

	return success;
}

// Deallocate memory and shut down SDL
void closeWindow() {
    // Deallocate surface
    SDL_FreeSurface(gCurrentSurface);
    SDL_FreeSurface(gPNGSurface);
    gCurrentSurface = NULL;
    gPNGSurface = NULL;

    // Destroy Window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    // Quit SDL subsystems
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

                // Apply the PNG image
				SDL_BlitSurface( gPNGSurface, NULL, gScreenSurface, NULL );

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
                
                // Update the surface
                SDL_UpdateWindowSurface(gWindow);
            }
        }
    }
    // Free up resources and close SDL
    closeWindow();

    return 0;
}
