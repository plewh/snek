#include "gstate.h"
#include <stdlib.h>
#include <stdio.h>
#include "frt.h"
#include "snk.h"

#define GS_TICK_DEVISOR 6

static frt_t* gs_PlaceFruit(snk_t* snk);
static void   gs_GameTick(gstate_t* gstate);
static void   gs_GameResponder(gstate_t* gstate, event_t* ev);

static gstate_t* currState;

void gs_Init() {

	currState = gs_NewState(GAME);

}

void gs_Cleanup() {

	;

}

void gs_Responder(event_t* ev) {

	switch (ev->type) {
		
		case GS_PAUSE:
			if (currState->isPaused)
				currState->isPaused = false;
			else
				currState->isPaused = true;
			break;

		default:
			break;

	}

	currState->Responder(ev);

}

gstate_t* gs_NewState(gstate_e state) {

	gstate_t* new = malloc(sizeof(gstate_t));

	switch (state) {

		case GAME:
			new->state     = state;
			new->Tick      = gs_GameTick;
			new->Responder = gs_GameResponder;
			new->snk       = snk_NewSnk();
			new->frt       = gs_PlaceFruit(new->snk);
			new->ticks     = 0.0;
			new->isPaused  = false;
			break;

		default:
			break;

	}

	return new;

}

void gs_Free(gstate_t* gstate) {

	snk_Free(gstate->snk);
	frt_Free(gstate->frt);
	free(gstate);

}

void gs_GameTick(gstate_t* gstate) {

	if (gstate->isPaused)
		return;

	gstate->ticks += (1.0 / GS_TICK_DEVISOR);

	if (gstate->ticks >= 1.0) {
		gstate->ticks = 0.0;

		snk_Tick(gstate->snk);
		frt_Tick(gstate->frt);

	}

	// snek eat frt :3
	if (snk_IsCollided(gstate->snk, &gstate->frt->pos)) {

		frt_Free(gstate->frt);
		gstate->frt = gs_PlaceFruit(gstate->snk);
		snk_Grow(gstate->snk);

	}

	// snek bang wall >.<
	if (gstate->snk->headPos.x < 1 || 
		gstate->snk->headPos.x > WIN_X - 2 ||
		gstate->snk->headPos.y < 1 ||
		gstate->snk->headPos.y > WIN_Y - 2) {

		ev_PushEvent(ev_NewEvent(SNK_DEAD));

	}

	// snek eat self :O
	for (int j = 0; j < gstate->snk->length; ++j) {

		if (IsEqual(&gstate->snk->headPos, &gstate->snk->body[j]))
			ev_PushEvent(ev_NewEvent(SNK_DEAD));

	}

}

void gs_GameResponder(gstate_t* gstate, event_t* ev) {

	switch (ev->type) {

		case SNK_MOVE_UP:
			snk_Move(gstate->snk, UP);
			break;

		case SNK_MOVE_LEFT:
			snk_Move(gstate->snk, LEFT);
			break;

		case SNK_MOVE_DOWN:
			snk_Move(gstate->snk, DOWN);
			break;

		case SNK_MOVE_RIGHT:
			snk_Move(gstate->snk, RIGHT);
			break;

		case SNK_INC_LEN:
			snk_Grow(gstate->snk);
			break;

		case SNK_DEAD:
			gstate->isPaused = true;
			break;

	}

}

frt_t* gs_PlaceFruit(snk_t* snk) {

	// dont place new fruit inside snek

	frt_t* pFrt;

	do {

		pFrt = frt_NewFrt(WIN_X, WIN_Y);

	} while(snk_IsCollided(snk, &(pFrt->pos)));

	return pFrt;

}

snk_t const* gs_GetSnake() {

	return currState->snk;

}

frt_t const* gs_GetFruit() {

	return currState->frt;

}

int gs_IsPaused() {

	return currState->isPaused;

}
