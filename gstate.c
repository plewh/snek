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
static rend_t*   gs_GameGetRends(gstate_t* gstate, gfield_t* gfield);

static void      gs_DeathTick(gstate_t* gstate, gfield_t* gfield);
static void      gs_DeathResponder(gstate_t* gstate, gfield_t* gfield, event_t* ev);
static rend_t*   gs_DeathGetRends(gstate_t* gstate, gfield_t* gfield);

static void      gs_TitleTick(gstate_t* gstate, gfield_t* gfield);
static void      gs_TitleResponder(gstate_t* gstate, gfield_t* gfield, event_t* ev);
static rend_t*   gs_TitleGetRends(gstate_t* gstate, gfield_t* gfield);

// data structs
static gstate_t* currState;
static gfield_t* currField;

/* * * PUBLIC FUNCS * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void gs_Init() {

	currState = gs_NewState(TITLE);
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
		
		case KEY_PRESS:
			if (ev->data == 'p') {
				if (currState->isPaused)
					currState->isPaused = false;
				else
					currState->isPaused = true;
				break;
			}
			break;

		default:
			break;

	}

	currState->Responder(currState, currField, ev);

}

rend_t* gs_GetRenderables() {

	return currState->GetRends(currState, currField);

}

/* * * PRIVATE FUNCS  * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

gstate_t* gs_NewState(gstate_e state) {

	gstate_t* new = malloc(sizeof(gstate_t));

	switch (state) {

		case GAME:
			new->state            = state;
			new->Tick             = gs_GameTick;
			new->Responder        = gs_GameResponder;
			new->GetRends         = gs_GameGetRends;
			new->ticks            = 0.0;
			new->isPaused         = false;
			new->isHidden         = false;
			new->isInputLocked    = false;
			new->inputLockTimeout = 5.0;
			break;

		case DEATH:
			new->state            = state;
			new->Tick             = gs_DeathTick;
			new->Responder        = gs_DeathResponder;
			new->GetRends         = gs_DeathGetRends;
			new->ticks            = 0.0;
			new->isPaused         = false;
			new->isHidden         = false;
			new->isInputLocked    = true;
			new->inputLockTimeout = 5.0;
			break;

		case TITLE:
			new->state            = state;
			new->Tick             = gs_TitleTick;
			new->Responder        = gs_TitleResponder;
			new->GetRends         = gs_TitleGetRends;
			new->ticks            = 0.0;
			new->isPaused         = false;
			new->isHidden         = false;
			new->isInputLocked    = false;
			new->inputLockTimeout = 5.0;
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

	gstate->ticks += (1.5 / GS_TICK_DEVISOR);

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

		ev_PushEvent(ev_NewEvent(SNK_DEAD, ' '));

	}

	// snek eat self :O
	for (int j = 0; j < gfield->snk->length; ++j) {

		if (IsEqual(&gfield->snk->headPos, &gfield->snk->body[j]))
			ev_PushEvent(ev_NewEvent(SNK_DEAD, ' '));

	}

}

void gs_GameResponder(gstate_t* gstate, gfield_t* gfield, event_t* ev) {

	switch (ev->type) {

		case KEY_PRESS:

			switch (ev->data) {

				case 'w':
					snk_Move(gfield->snk, UP);
					break;

				case 'a':
					snk_Move(gfield->snk, LEFT);
					break;

				case 's':
					snk_Move(gfield->snk, DOWN);
					break;

				case 'd':
					snk_Move(gfield->snk, RIGHT);
					break;

				case ' ':
					snk_Grow(gfield->snk);
					break;

				default:
					break;

			}
			break;

		case SNK_DEAD:
			currState = gs_NewState(DEATH);
			break;

		default:
			break;

	}

}

rend_t* gs_GameGetRends(gstate_t* gstate, gfield_t* gfield) {

	rend_t* rends = r_NewRends();

	snk_t* snk = gfield->snk;
	frt_t* frt = gfield->frt;

	for (int j = 0; j < snk->length; ++j) {
		r_PushRend(rends, RUNE, snk->bodyRune, NULL, snk->body[j]);
	}

	r_PushRend(rends, RUNE, snk->headRune, NULL, snk->headPos);
	r_PushRend(rends, RUNE, frt->rune, NULL, frt->pos);

	return rends;

}

void gs_DeathTick(gstate_t* gstate, gfield_t* gfield) {

	gstate->ticks += (1.0 / GS_TICK_DEVISOR);

	if (gstate->ticks >= 3.0) {
		gstate->ticks = 0.0;

		if ((gstate->inputLockTimeout -= 1.0) <= 0.0) {
			
			gstate->isInputLocked = false;

		}

		if (gstate->isHidden) {
			gstate->isHidden = false;
		} else {
			gstate->isHidden = true;
		}

	}

}

void gs_DeathResponder(gstate_t* gstate, gfield_t* gfield, event_t* ev) {

	if (gstate->isInputLocked)
		return;

	switch (ev->type) {

		case KEY_PRESS:
			ev_PushEvent(ev_NewEvent(APP_QUIT, '~'));
			break;

		default:
			break;

	}

}

rend_t* gs_DeathGetRends(gstate_t* gstate, gfield_t* gfield) {

	rend_t* rends = r_NewRends();

	if (!gstate->isHidden) {

		snk_t* snk = gfield->snk;
		frt_t* frt = gfield->frt;

		for (int j = 0; j < snk->length; ++j) {
			r_PushRend(rends, RUNE, snk->bodyRune, NULL, snk->body[j]);
		}

		r_PushRend(rends, RUNE, snk->headRune, NULL, snk->headPos);
		r_PushRend(rends, RUNE, frt->rune, NULL, frt->pos);
	
	}

	coord_t p = {(WIN_X - 38) / 2, (WIN_Y - 6) / 2};
	r_PushRend(rends, TEXT, ' ', "__   __           ______ _          _", p);
	++p.y;
	r_PushRend(rends, TEXT, ' ', "\\ \\ / /           |  _  (_)        | |", p);
	++p.y;
	r_PushRend(rends, TEXT, ' ', " \\ V /___  _   _  | | | |_  ___  __| |", p);
	++p.y;
	r_PushRend(rends, TEXT, ' ', "  \\ // _ \\| | | | | | | | |/ _ \\/ _` |", p);
	++p.y;
	r_PushRend(rends, TEXT, ' ', "  | | (_) | |_| | | |/ /| |  __/ (_| |", p);
	++p.y;
	r_PushRend(rends, TEXT, ' ', "  \\_/\\___/ \\__,_| |___/ |_|\\___|\\__,_|", p);

	return rends;

}
			                                         
void gs_TitleTick(gstate_t* gstate, gfield_t* gfield) {

	gstate->ticks += (1.0 / GS_TICK_DEVISOR);

}

void gs_TitleResponder(gstate_t* gstate, gfield_t* gfield, event_t* ev) {

	switch (ev->type) {

		case KEY_PRESS:
			currState = gs_NewState(GAME);
			break;

		default:
			break;

	}

}

rend_t* gs_TitleGetRends(gstate_t* gstate, gfield_t* gfield) {

	rend_t* rends = r_NewRends();

	coord_t p = {(WIN_X - 38) / 2, (WIN_Y - 6) / 2};
	r_PushRend(rends, TEXT, ' ', "      _____            _    ", p);
	++p.y;
	r_PushRend(rends, TEXT, ' ', "     /  ___|          | |   ", p);
	++p.y;
	r_PushRend(rends, TEXT, ' ', "	  \\ `--. _ __   ___| | __", p);
	++p.y;
	r_PushRend(rends, TEXT, ' ', "	   `--. \\ '_ \\ / _ \\ |/ /", p);
	++p.y;
	r_PushRend(rends, TEXT, ' ', "	  /\\__/ / | | |  __/   < ", p);
	++p.y;
	r_PushRend(rends, TEXT, ' ', "	  \\____/|_| |_|\\___|_|\\_\\", p);

	return rends;

}
