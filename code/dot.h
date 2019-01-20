//
// dot.h
//
// Copyright (c) 2018 Fletcher Porter
//

#ifndef __DOT_H__
#define __DOT_H__


#include <SDL.h>
#include <SDL_image.h>

#include "bool.h"
#include "LTexture.h"
#include "Particle.h"


typedef struct {
    int width, height;
    int positionX, positionY;
    int velocityX, velocityY;
    LTexture texture;
    SDL_Rect collider;

    int numberOfParticles;
    Particle *particles;
} Dot;


void Dot_init(Dot *dot, LWindow *window, int w, int h, int xi, int yi);

void Dot_free(Dot *dot);

void Dot_handleEvent(Dot *dot, SDL_Event* inputEvent);

bool Dot_checkCollision(SDL_Rect a, SDL_Rect b);

void Dot_updatePosition(Dot *dot, int levelWidth, int levelHeight);

void Dot_draw(Dot *dot, LWindow *window, int cameraX, int cameraY);


#endif  // __DOT_H__
