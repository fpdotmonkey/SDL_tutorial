//
// LTimer.c
//
// Copyright (c) 2018 Fletcher Porter
//

#include "LTimer.h"

void startLTimer(LTimer *timer) {
    timer->mStarted = true;
    timer->mPaused = false;
    timer->mStartTicks = SDL_GetTicks();
    timer->mPausedTicks = 0;
}

void stopLTimer(LTimer *timer) {
    timer->mStarted = false;
    timer->mPaused = false;
    timer->mStartTicks = 0;
    timer->mPausedTicks = 0;
}

void pauseLTimer(LTimer *timer) {
    if (timer->mStarted && !timer->mPaused) {
        timer->mPaused = true;
        timer->mPausedTicks = SDL_GetTicks() - timer->mStartTicks;
        timer->mStartTicks = 0;
    }
}

void unpauseLTimer(LTimer *timer) {
    if (timer->mStarted && timer->mPaused) {
        timer->mPaused = false;
        timer->mStartTicks = SDL_GetTicks() - timer->mPausedTicks;
        timer->mPausedTicks = 0;
    }
}

uint32_t getTicks(LTimer timer) {
    uint32_t time;
    
    if (timer.mStarted) {
        if (timer.mPaused) {
            time = timer.mPausedTicks;
        } else {
            time = SDL_GetTicks() - timer.mStartTicks;
        }
    }

    return time;
}
