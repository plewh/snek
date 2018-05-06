#pragma once

#include "defs.h"

#define SNK_HEAD 'X'
#define SNK_BODY 0
#define SNK_MAX_LEN 100

typedef struct {

    vect_t headPos;
    vect_t body[SNK_MAX_LEN];
    int    length;
    dir_e  dir;

} snk_t;

snk_t* snk_init();
void   snk_addLength(snk_t* snk);
int    snk_hasCollided(snk_t* snk, vect_t* vect);
void   snk_Free(snk_t* snk);
void   snk_chdir(snk_t* snk, dir_e dir);
void   snk_Tick(snk_t* snk);
