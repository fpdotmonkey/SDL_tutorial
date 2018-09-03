//
// bool.h
//
// Copyright (c) 2018 Fletcher Porter
//

#ifndef __BOOL_H__
#define __BOOL_H__

#include <SDL.h>

// C doesn't have bool, so gotta make it myself
typedef enum boolean {
    true = SDL_TRUE,
    false = SDL_FALSE
} bool;


#endif // !__BOOL_H__
