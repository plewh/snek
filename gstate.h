#pragma once

#include "snek.h"

typedef struct {

    snk_t*  snek;
    vect_t  fruit;

} gState_t;

gState_t* gs_NewState();
void      gs_Free(gState_t* gState);
void      gs_HandleInput(gState_t* gState, char ch);
void      gs_Tick(gState_t* gState);