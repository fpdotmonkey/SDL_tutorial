//
// Particle.h
//
// Copyright (c) 2018 Fletcher Porter
//


#ifndef __LPARTICLE_H__
#define __LPARTICLE_H__


#include "bool.h"
#include "LTexture.h"

typedef struct {
    int positionX, positionY;
    int currentAnimationFrame;
    LTexture *texture;
    LTexture *shimmer;
} LParticle;


bool Particle_init(Dot *dot, int x, int y);
void Particle_free(Dot *dot);
void Particle_draw(Dot *dot);
bool Particle_isDead(Dot *dot);



#endif // __LPARTICLE_H__
