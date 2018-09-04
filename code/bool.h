//
// bool.h
//
// Copyright (c) 2018 Fletcher Porter
//

#ifndef __BOOL_H__
#define __BOOL_H__

#include <SDL.h>

// Wraps around SDL_bool so it looks like a regular bool
typedef enum boolSDL {
    true = SDL_TRUE,
    false = SDL_FALSE
} bool;


#endif // !__BOOL_H__
