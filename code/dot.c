//
// dot.c
//
// Copyright (c) 2018 Fletcher Porter
//

#include "dot.h"

void Dot_init(Dot *dot, LWindow *window, int xi, int yi, int w, int h) {
    dot->positionX = xi;
    dot->positionY = yi;
    dot->width = w;
    dot->height = h;
    dot->velocityX = 0;
    dot->velocityY = 0;

    LT_init(&dot->texture);
    LT_loadImage(&dot->texture, window, "../dot.png");

    dot->collider.x = dot->positionX;
    dot->collider.y = dot->positionY;
    dot->collider.w = dot->width;
    dot->collider.h = dot->height;
}

void Dot_free(Dot *dot) {
    LT_free(&dot->texture);

    dot = NULL;
}

void Dot_handleEvent(Dot *dot, SDL_Event* inputEvent) {
    const int DV = 10;
    if (inputEvent->type == SDL_KEYDOWN && inputEvent->key.repeat == 0) {
        // Adjust the velocity
        switch (inputEvent->key.keysym.sym) {
        case SDLK_UP:
            dot->velocityY -= DV;
            break;

        case SDLK_DOWN:
            dot->velocityY += DV;
            break;

        case SDLK_LEFT:
            dot->velocityX -= DV;
            break;

        case SDLK_RIGHT:
            dot->velocityX += DV;
            break;
        }
    }

    if (inputEvent->type == SDL_KEYUP && inputEvent->key.repeat == 0) {
        // Adjust the velocity
        switch (inputEvent->key.keysym.sym) {
        case SDLK_UP:
            dot->velocityY += DV;
            break;

        case SDLK_DOWN:
            dot->velocityY -= DV;
            break;

        case SDLK_LEFT:
            dot->velocityX += DV;
            break;

        case SDLK_RIGHT:
            dot->velocityX -= DV;
            break;
        }
    }
}

bool Dot_checkCollision(SDL_Rect a, SDL_Rect b) {
    // The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if (bottomA <= topB) {
        return false;
    }

    if (topA <= bottomB) {
        return false;
    }

    if (rightA <= leftB) {
        return false;
    }

    if (leftA >= rightB) {
        return false;
    }

    return true;
}

void Dot_updatePosition(Dot *dot, int levelWidth, int levelHeight) {    
    dot->positionX += dot->velocityX;
    dot->positionY += dot->velocityY;
    dot->collider.x = dot->positionX;
    dot->collider.y = dot->positionY;

    if ((dot->positionX < 0)
        || (dot->positionX + dot->width > levelWidth)) {
        // Move back
        dot->positionX -= dot->velocityX;
        dot->collider.x = dot->positionX;
    }

    if ((dot->positionY < 0)
        || (dot->positionY + dot->height > levelHeight)) {
        // Move back
        dot->positionY -= dot->velocityY;
        dot->collider.x = dot->positionY;
    }
 
}

void Dot_draw(Dot *dot, LWindow *window, int cameraX, int cameraY) {
    // Render dot relative to camera
    LT_draw(&dot->texture,
            window,
            dot->positionX - cameraX, dot->positionY - cameraY,
            dot->width, dot->height);
}


// PRIVATE

static void drawParticles(Dot *dot) {

}
