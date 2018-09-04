//
// LPrints.c
//
// Copyright (c) 2018 Fletcher Porter
//

#include "LPrints.h"

int ErrorSDL(char *description) {
    const char *errorMessage = SDL_GetError();
    return fprintf(stderr, "Error! %s, SDL Error: %s\n", description, errorMessage);
}
