//
// LTimer.h
//
// Copyright (c) 2018 Fletcher Porter
//

#ifndef LTIMER_H
#define LTIMER_H


#include "bool.h"
#include <SDL.h>

// Wrapper struct for advanced timing
typedef struct {
    uint32_t mStartTicks;
    uint32_t mPausedTicks;
    bool mPaused;
    bool mStarted;
} LTimer;

// Start a timer
void startLTimer(LTimer *timer);

// End a timer
void stopLTimer(LTimer *timer);

// Pause a timer
void pauseLTimer(LTimer *timer);

// Unpause a timer
void unpauseLTimer(LTimer *timer);

// Get the current time from a timer
uint32_t getTicks(LTimer timer);


#endif
