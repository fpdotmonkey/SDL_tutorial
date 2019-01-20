//
// LPrints.h
//
// Copyright (c) 2018 Fletcher Porter
//

#ifndef __LPRINTS_H__
#define __LPRINTS_H__

#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#ifdef __DEBUG__
#define PRINT(...) do {           \
        fprintf(stdout, __VA_ARGS__);        \
        fputc('\n', stdout);                           \
    } while (0);
#else
#define PRINT(...)
#endif // !__DEBUG__


int ErrorSDL(char *description);
int ErrorSDL_path(char *description, char *path);
int ErrorIMG(char *description);
int ErrorIMG_path(char *description, char *path);
int ErrorTTF(char *description);
int ErrorTTF_path(char *description, char *path);


#endif // !__LPRINTS_H__
