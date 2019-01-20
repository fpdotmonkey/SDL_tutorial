#include <stdio.h>

#include "LWindow.h"
#include "LTexture.h"
#include "LPrints.h"

#define SCREEN_HEIGHT 400
#define SCREEN_WIDTH 600

int main(int argc, char **argv) {
    
    // Initialize the window and textures
    LWindow window;
    LTexture image;
    LTexture text;
    
    if (! LW_init(&window, 1, SCREEN_WIDTH, SCREEN_HEIGHT)) {
        ErrorSDL("Could not initialize window");
    }
    if (! LT_init(&image)) {
        ErrorSDL("Could not initialize texture");
    }
    if (! LT_init(&text)) {
        ErrorSDL("Could not initialize texture");
    }


    // Load the assets to be rendered
    LT_loadImage(&image, &window, "../colors.png");
    LT_loadFont(&text, "../lazy.ttf", 20);

    SDL_Color textColor = { 0xFF, 0, 0, 0xFF };
    LT_setTextColor(&text, textColor);
    LT_generateText(&text, &window, "Hello World!");


    // Set the preferences for rendering
    SDL_Rect imageClippingRectangle = { 0, 0, image.width, image.height };
    double imageDrawAngle = 0.0;
    SDL_Point *imageDrawCenter = NULL;
    SDL_RendererFlip imageDrawFlip = SDL_FLIP_NONE;

    SDL_Rect textClippingRectangle = { 0, 0, text.width, text.height };
    double textDrawAngle = 0.0;
    SDL_Point *textDrawCenter = NULL;
    SDL_RendererFlip textDrawFlip = SDL_FLIP_NONE;

    LT_setDrawParameters(&image,
                         &imageClippingRectangle,
                         imageDrawAngle,
                         imageDrawCenter,
                         imageDrawFlip);

    LT_setDrawParameters(&text,
                         &textClippingRectangle,
                         textDrawAngle,
                         textDrawCenter,
                         textDrawFlip);


    // Create the rendering loop
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
        
        LT_draw(&image, &window,
                SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4,
                SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        
        LT_draw(&text, &window,
                SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4,
                SCREEN_WIDTH / 4, SCREEN_HEIGHT / 8);
        
        LW_render(&window);
    }

    LW_free(&window);
    LT_free(&image);
    LT_free(&text);
    
    SDL_Quit();
    IMG_Quit();
    TTF_Quit();

    PRINT("success!");

    
    return 0;
}
