#include "frt.h"
#include <stdlib.h>
#include "app.h"

frt_t* frt_NewFrt(int winX, int winY) {

	frt_t* f = malloc(sizeof(frt_t));

	f->pos = app_GetRandomCoord(winX, winY);

	return f;

}

void frt_Free(frt_t* frt) {

	free(frt);

}

void frt_Tick(frt_t* frt) {

	;

}
