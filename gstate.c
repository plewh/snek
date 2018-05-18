#include "gstate.h"
#include <stdlib.h>
#include <stdio.h>
#include "frt.h"
#include "snk.h"

#define GS_TICK_DEVISOR 6

static snk_t* snk;
static frt_t* frt;

static double ticks;
static frt_t* gs_PlaceFruit();

void gs_Init() {

	snk = snk_NewSnk();
	frt = gs_PlaceFruit();

	ticks = 0.0;

}

void gs_Cleanup() {

	snk_Free(snk);
	frt_Free(frt);

}

void gs_Tick() {

	ticks += (1.0 / GS_TICK_DEVISOR);

	if (ticks >= 1.0) {
		ticks = 0.0;

		snk_Tick(snk);
		frt_Tick(frt);

	}

	if (snk_IsCollided(snk, &frt->pos)) {

		frt_Free(frt);
		gs_PlaceFruit();
		snk_Grow(snk);

	}

}

void gs_Responder(event_t* ev) {

	switch (ev->type) {
		
		case SNK_MOVE_UP:
			snk_Move(snk, UP);
			break;

		case SNK_MOVE_LEFT:
			snk_Move(snk, LEFT);
			break;

		case SNK_MOVE_DOWN:
			snk_Move(snk, DOWN);
			break;

		case SNK_MOVE_RIGHT:
			snk_Move(snk, RIGHT);
			break;

		case SNK_INC_LEN:
			snk_Grow(snk);
			break;

		default:
			break;

	}

}

snk_t const* gs_GetSnake() {

	return snk;

}

frt_t const* gs_GetFruit() {

	return frt;

}

frt_t* gs_PlaceFruit() {

	// dont place new fruit inside snek

	frt_t* pFrt;

	do {

		pFrt = frt_NewFrt(WIN_X, WIN_Y);

	} while(snk_IsCollided(snk, &(pFrt->pos)));

	return pFrt;

}
