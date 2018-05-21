#pragma once

#include "render.h"
#include "frt.h"
#include "snk.h"
#include "event.h"

typedef enum {

	GAME

} gstate_e;

typedef struct {

	gstate_e state;
	void (*Tick)();
	void (*Responder)();
	snk_t* snk;
	frt_t* frt;
	double ticks;
	int    isPaused;

} gstate_t;

void      gs_Init();
void      gs_Cleanup();
void      gs_Responder();
gstate_t* gs_NewState(gstate_e state);
void      gs_Free(gstate_t* gstate);
snk_t const* gs_GetSnake();
frt_t const* gs_GetFruit();
int          gs_IsPaused();
