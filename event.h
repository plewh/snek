#pragma once

typedef enum {

	APP_QUIT,
	APP_TICKRATE_INC,
	APP_TICKRATE_DEC,
	APP_TICK,
	GS_PAUSE,
	SNK_MOVE_UP,
	SNK_MOVE_LEFT,
	SNK_MOVE_DOWN,
	SNK_MOVE_RIGHT,
	SNK_INC_LEN,
	SNK_DEAD,
	KEY_PRESS

} eventType_e;

typedef struct {

	eventType_e type;
	char        data;

} event_t;

void     ev_Init();
void     ev_Cleanup();
void     ev_RegisterResponder(void (*Resp)(event_t*));
void     ev_PushEvent(event_t* event);
void     ev_PumpEvents();
event_t* ev_NewEvent(eventType_e type, char data);
