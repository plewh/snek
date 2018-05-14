#pragma once
#include "defs.h"

typedef struct {

	coord_t pos;

} frt_t;

void   frt_Tick(frt_t* frt);
frt_t* frt_NewFrt();
void   frt_Free(frt_t* frt);
