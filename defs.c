#include "defs.h"
#include <stdlib.h>
#include <stdio.h>

char isEqual(vect_t* a, vect_t* b) {

    if (a->x == b->x && a->y == b->y) {

        return true;

    }

    return false;

}

vect_t getRandomVect(int xMin, int xMax, int yMin, int yMax) {

    vect_t v;

    v.x = xMin + (rand() % xMax);
    v.y = xMin + (rand() % yMax);

    return v;

    fprintf(stderr, "%d:%d\n", v.x, v.y);

}