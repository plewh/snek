#include "defs.h"

int IsEqual(coord_t* a, coord_t* b) {

	if (a->x == b->x && a->y == b->y)
		return true;
	return false;

}
