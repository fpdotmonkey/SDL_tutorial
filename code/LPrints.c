//
// LPrints.c
//
// Copyright (c) 2018 Fletcher Porter
//

#include "LPrints.h"

int ErrorSDL(char *description) {
    const char *errorMessage = SDL_GetError();
    return fprintf(stderr,
                   "Error! %s, SDL Error: %s\n",
                   description,
                   errorMessage);
}

int ErrorSDL_path(char *description, char *path) {
    const char *errorMessage = SDL_GetError();
    return fprintf(stderr,
                   "Error! %s, path: %s, SDL Error: %s\n",
                   description,
                   path,
                   errorMessage);
}

int ErrorTTF(char *description) {
    const char *errorMessage = TTF_GetError();
    return fprintf(stderr,
                   "Error! %s, TTF Error: %s\n",
                   description,
                   errorMessage);
}

int ErrorTTF_path(char *description, char* path) {
    const char *errorMessage = TTF_GetError();
    return fprintf(stderr,
                   "Error! %s, path: %s, TTF Error: %s\n",
                   description,
                   path,
                   errorMessage);
}

int ErrorIMG(char *description) {
    const char *errorMessage = IMG_GetError();
    return fprintf(stderr,
                   "Error! %s, IMG Error: %s\n",
                   description,
                   errorMessage);
}

int ErrorIMG_path(char *description, char* path) {
    const char *errorMessage = IMG_GetError();
    return fprintf(stderr,
                   "Error! %s, path: %s, IMG Error: %s\n",
                   description,
                   path,
                   errorMessage);
}
