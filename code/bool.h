//
// bool.h
//
// Copyright (c) 2018 Fletcher Porter
//

// include guards
#ifndef BOOL_H
#define BOOL_H

// C doesn't have bool, so gotta make it myself
typedef enum boolean {
    true = 1,
    false = 0
} bool;

#endif
