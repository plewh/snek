#include "defs.h"

char isEqual(vect_t* a, vect_t* b) {

    if (a->x == b->x && a->y == b->y) {

        return true;

    }

    return false;

}

