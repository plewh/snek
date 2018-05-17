#pragma once

#define WIN_X 80
#define WIN_Y 24

#define true 1
#define false 0

typedef struct {

	int x;
	int y;

} coord_t;

int IsEqual(coord_t* a, coord_t* b);
