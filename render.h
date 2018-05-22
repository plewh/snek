#pragma once

#include "event.h"
#include "defs.h"

#define MAX_OBJ 100

typedef enum {

	RUNE,
	TEXT

} obj_type_e;

typedef struct {

	obj_type_e type;
	coord_t    pos;
	char       rune;
	char*      text;

} obj_t;

typedef struct {

	int   length;
	obj_t obj[MAX_OBJ];

} rend_t;

void    r_Init();
void    r_Cleanup();
void    r_Responder(event_t* ev);
void    r_DoFrame();
void    r_Tick();
rend_t* r_NewRends();
void    r_PushRend(rend_t* rend, 
	               obj_type_e type, 
		   		   char rune, 
				   char* text, 
				   coord_t pos);
