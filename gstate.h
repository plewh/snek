#pragma once

#include "render.h"
#include "frt.h"
#include "snk.h"
#include "event.h"

typedef enum {

	GAME,
	DEATH,
	TITLE

} gstate_e;

typedef struct {

	snk_t* snk;
	frt_t* frt;

} gfield_t;

typedef struct gstate {

	gstate_e state;
	void     (*Tick)(struct gstate* gstate, gfield_t* gfield);
	void     (*Responder)(struct gstate* gstate, gfield_t* gfield, event_t* ev);
	rend_t*  (*GetRends)(struct gstate* gstate, gfield_t* gfield);
	double   ticks;
	int      isPaused;
	int      isHidden;
	int      isInputLocked;
	double   inputLockTimeout;

} gstate_t;

void         gs_Init();
void         gs_Cleanup();
void         gs_Tick();
void         gs_Responder();
rend_t*      gs_GetRenderables();
