//
// dot.h
//
// Copyright (c) 2018 Fletcher Porter
//

#ifndef DOT_H
#define DOT_H


#include <SDL.h>
#include <SDL_image.h>

#include "bool.h"
#include "LTexture.h"


typedef struct {
    int mWidth, mHeight;
    int mPosX, mPosY;
    int mVelX, mVelY;
    LTexture mLTexture;
    SDL_Rect mCollider;
} Dot;


void initDot(SDL_Renderer* renderer, Dot *dot, int w, int h, int xi, int yi);

void closeDot(Dot *dot);

void handleEventDot(Dot *dot, SDL_Event* e);

bool checkCollision(SDL_Rect a, SDL_Rect b);

void updatePositionDot(Dot *dot, SDL_Rect wall,
                       int levelWidth, int levelHeight);

void renderDot(SDL_Renderer* renderer, Dot *dot, int camX, int camY);


#endif
