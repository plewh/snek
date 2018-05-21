#pragma once

#include "render.h"
#include "frt.h"
#include "snk.h"
#include "event.h"

typedef enum {

	GAME,
	DEATH

} gstate_e;

typedef struct {

	snk_t* snk;
	frt_t* frt;

} gfield_t;

typedef struct gstate {

	gstate_e state;
	void (*Tick)(struct gstate* gstate, gfield_t* gfield);
	void (*Responder)(struct gstate* gstate, gfield_t* gfield, event_t* ev);
	double ticks;
	int    isPaused;
	int    isHidden;

} gstate_t;

void         gs_Init();
void         gs_Cleanup();
void         gs_Tick();
void         gs_Responder();
snk_t const* gs_GetSnake();
frt_t const* gs_GetFruit();
int          gs_IsPaused();
int          gs_IsHidden();
