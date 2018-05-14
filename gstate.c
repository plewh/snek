#include "gstate.h"
#include <stdlib.h>
#include "frt.h"
#include "snk.h"

#include <stdio.h>

#define GS_TICK_DEVISOR 6

static snk_t* snk;
static frt_t* frt;

static double ticks;

void gs_Init() {

	snk = snk_NewSnk();
	frt = frt_NewFrt();

	ticks = 0.0;

}

void gs_Cleanup() {

	snk_Free(snk);
	frt_Free(frt);

}

void gs_Tick() {

	snk_Tick(snk);
	frt_Tick(frt);

}

void gs_Responder() {

	;

}

snk_t const* gs_GetSnake() {

	return snk;

}

frt_t const* gs_GetFruit() {

	return frt;

}

