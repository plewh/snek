#include "event.h"
#include <stdlib.h>

#define MAX_EVENTS     10
#define MAX_RESPONDERS 10

static event_t* evBuffer[MAX_EVENTS];
static int evHead;
static int evTail;

static void (*RespArr[MAX_RESPONDERS])(event_t*);
static int respCount;

static void ev_FreeEvent(event_t* event);

void ev_Init() {

	evHead    = 0;
	evTail    = 0;
	respCount = 0;

}

void ev_Cleanup() {

	;

}

void ev_RegisterResponder(void (*Resp)(event_t*)) {

	RespArr[respCount] = Resp;
	++respCount;

}

void ev_PushEvent(event_t* event) {

	evBuffer[evHead] = event;
	evHead = ++evHead % MAX_EVENTS;

}

void ev_PumpEvents() {

	event_t* ev;

	while (evTail != evHead) {

		ev = evBuffer[evTail];

		for (int j = 0; j < respCount; ++j) {

			RespArr[j](ev);

		}

		ev_FreeEvent(evBuffer[evTail]);
		evTail = ++evTail % MAX_EVENTS;

	}

}

event_t* ev_NewEvent(eventType_e type, char data) {

	event_t* ev = malloc(sizeof(type));

	ev->type = type;
	ev->data = data;

	return ev;

}

void ev_FreeEvent(event_t* event) {

	free(event);

}
