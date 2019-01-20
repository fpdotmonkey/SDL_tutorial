//
// Particle.c
//
// Copyright (c) 2019 FLetcher Porter
//


#include <stdlib.h>

#include "Particle.h"

bool Particle_init(Dot *dot,
                   int x, int y,
                   char *redTexturePath,
                   char *blueTexturePath,
                   char *greenTexturePath,
                   char *shimmerTexturePath) {

    dot->positionX = x - 5 + (arc4random() % 25);
    dot->positionY = y - 5 + (arc4random() % 25);

    dot->currentAnimationFrame = arc4random() % 5;

    if (! LT_loadImage(&dot->shimmer, shimmerTexturePath)) {

        switch (rand() % 3) {
        case 0:
            
            return LT_loadImage(&dot->texture, redTexturePath);
            break;

        case 1:
            
            return LT_loadImage(&dot->texture, greenTexturePath);
            break;

        case 2:
            
            return LT_loadImage(&dot->texture, blueTexturePath);
            break;
        }
    }

    return false;
}

void Particle_free(Dot *dot) {
    LT_free(&dot->texture);

    dot = NULL;
}

void Particle_draw(Dot *dot) {
    LT_draw(&dot->texture,
            positionX, positionY,
            LT_width(&dot->texture), LT_height(&dot->height))
}

bool Particle_isDead(Dot *dot) {
    if (dot->currentAnimationFrame > 10) {

        return true;
    } else {

        return false;
    }
}
