#include "gstate.h"
#include <stdlib.h>
#include <stdio.h>
#include "frt.h"
#include "snk.h"

#define GS_TICK_DEVISOR 6

// create/free data structs
static gstate_t* gs_NewState(gstate_e state);
static void      gs_FreeState(gstate_t* gstate);
static gfield_t* gs_NewField();
static void      gs_FreeField(gfield_t* gfield);

// helper funcs
static frt_t*    gs_PlaceFruit(snk_t* snk);

// state funcs
static void      gs_GameTick(gstate_t* gstate, gfield_t* gfield);
static void      gs_GameResponder(gstate_t* gstate, gfield_t* gfield, event_t* ev);

// data structs
static gstate_t* currState;
static gfield_t* currField;

/* * * PUBLIC FUNCS * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void gs_Init() {

	currState = gs_NewState(GAME);
	currField = gs_NewField();

}

void gs_Cleanup() {

	;

}

void gs_Tick() {

	currState->Tick(currState, currField);

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

	currState->Responder(currState, currField, ev);

}

snk_t const* gs_GetSnake() {

	return currField->snk;

}

frt_t const* gs_GetFruit() {

	return currField->frt;

}

int gs_IsPaused() {

	return currState->isPaused;

}

/* * * PRIVATE FUNCS  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

gstate_t* gs_NewState(gstate_e state) {

	gstate_t* new = malloc(sizeof(gstate_t));

	switch (state) {

		case GAME:
			new->state     = state;
			new->Tick      = gs_GameTick;
			new->Responder = gs_GameResponder;
			new->ticks     = 0.0;
			new->isPaused  = false;
			break;

		default:
			break;

	}

	return new;

}

void gs_FreeState(gstate_t* gstate) {

	//snk_Free(gstate->snk);
	//frt_Free(gstate->frt);
	free(gstate);

}

gfield_t* gs_NewField() {

	gfield_t* new = malloc(sizeof(gfield_t));

	new->snk       = snk_NewSnk();
	new->frt       = gs_PlaceFruit(new->snk);

	return new;

}

void gs_FreeField(gfield_t* gfield) {

	free(gfield);

}

frt_t* gs_PlaceFruit(snk_t* snk) {

	// dont place new fruit inside snek

	frt_t* pFrt;

	do {

		pFrt = frt_NewFrt(WIN_X, WIN_Y);

	} while(snk_IsCollided(snk, &(pFrt->pos)));

	return pFrt;

}


void gs_GameTick(gstate_t* gstate, gfield_t* gfield) {

	if (gstate->isPaused)
		return;

	gstate->ticks += (1.0 / GS_TICK_DEVISOR);

	if (gstate->ticks >= 1.0) {
		gstate->ticks = 0.0;

		snk_Tick(gfield->snk);
		frt_Tick(gfield->frt);

	}

	// snek eat frt :3
	if (snk_IsCollided(gfield->snk, &gfield->frt->pos)) {

		frt_Free(gfield->frt);
		gfield->frt = gs_PlaceFruit(gfield->snk);
		snk_Grow(gfield->snk);

	}

	// snek bang wall >.<
	if (gfield->snk->headPos.x < 1 || 
		gfield->snk->headPos.x > WIN_X - 2 ||
		gfield->snk->headPos.y < 1 ||
		gfield->snk->headPos.y > WIN_Y - 2) {

		ev_PushEvent(ev_NewEvent(SNK_DEAD));

	}

	// snek eat self :O
	for (int j = 0; j < gfield->snk->length; ++j) {

		if (IsEqual(&gfield->snk->headPos, &gfield->snk->body[j]))
			ev_PushEvent(ev_NewEvent(SNK_DEAD));

	}

}

void gs_GameResponder(gstate_t* gstate, gfield_t* gfield, event_t* ev) {

	switch (ev->type) {

		case SNK_MOVE_UP:
			snk_Move(gfield->snk, UP);
			break;

		case SNK_MOVE_LEFT:
			snk_Move(gfield->snk, LEFT);
			break;

		case SNK_MOVE_DOWN:
			snk_Move(gfield->snk, DOWN);
			break;

		case SNK_MOVE_RIGHT:
			snk_Move(gfield->snk, RIGHT);
			break;

		case SNK_INC_LEN:
			snk_Grow(gfield->snk);
			break;

		case SNK_DEAD:
			gstate->isPaused = true;
			break;

		default:
			break;

	}

}
