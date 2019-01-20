#include <stdio.h>

#include "LWindow.h"
#include "LTexture.h"
#include "LPrints.h"

#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 600

int main(int argc, char **argv) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ErrorSDL("Failed to initialize SDL!");
        return 1;
    }

    LWindow window;
    LTexture texture;

    
    if (! LW_init(&window, 1, SCREEN_WIDTH, SCREEN_HEIGHT)) {
        ErrorSDL("Could not initialize window");
    }
    if (! LT_init(&texture)) {
        ErrorSDL("Could not initialize texture");
    }

    LT_loadImage(&texture, &window, "../colors.png");

    SDL_Rect renderClippingRectangle = { 0, 0, texture.width, texture.height };
    double renderAngle = 0.0;
    SDL_Point *renderCenter = NULL;
    SDL_RendererFlip rendererFlip = SDL_FLIP_NONE;

    LT_setDrawParameters(&texture,
                         &renderClippingRectangle,
                         renderAngle,
                         renderCenter,
                         rendererFlip);


    SDL_Color wipeColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    bool quittingTime = false;
    SDL_Event inputEvent;
    while (! quittingTime) {
        while (SDL_PollEvent(&inputEvent) != 0) {
            if (inputEvent.type == SDL_QUIT) {
                quittingTime = true;
            }
        }
        if (LW_prepareForRender(&window, wipeColor) < 0) {
            ErrorSDL("Failed to prepare render!");
            return 1;
        }
        
        LT_draw(&texture, &window,
                SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4,
                SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        
        LW_render(&window);
    }

    PRINT("success!");

    LW_free(&window);
    LT_free(&texture);
    SDL_Quit();
    
    
    return 0;
}
