#pragma once
#include "defs.h"

typedef struct {

	coord_t pos;
	char    rune;

} frt_t;

frt_t* frt_NewFrt(int winX, int winY);
void   frt_Free(frt_t* frt);
void   frt_Tick(frt_t* frt);
