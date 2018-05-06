#pragma once

#define true 1
#define false 0

typedef enum {

    UP,
    LEFT,
    DOWN,
    RIGHT

} dir_e;

typedef struct {

    int x;
    int y;

} vect_t;

char isEqual(vect_t* a, vect_t* b);
