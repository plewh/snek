#pragma once
#include "defs.h"

#define SNK_MAX_LEN 1000

typedef enum {

	UP,
	LEFT,
	DOWN,
	RIGHT

} dir_e;

typedef struct {

    coord_t headPos;
    coord_t body[SNK_MAX_LEN];
    int     length;
    dir_e   nextDir;
	dir_e   actualDir;

} snk_t;


snk_t* snk_NewSnk();
void   snk_Free(snk_t* snk);
void   snk_Tick(snk_t* snk);
