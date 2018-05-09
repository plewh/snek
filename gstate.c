#include "gstate.h"
#include <stdlib.h>
#include "defs.h"
#include "app.h"

static int doCollisions(gState_t* gState);

gState_t* gs_NewState() {

	gState_t* gState = malloc(sizeof(gState_t));

	gState->snek = snk_init();
	gState->fruit = getRandomVect(1, WIN_X - 2, 1, WIN_Y - 2);

	return gState;
}

void gs_Free(gState_t* gState) {

	snk_Free(gState->snek);
	free(gState);

}

void gs_HandleInput(gState_t* gState, char ch) {

	snk_t* snk = gState->snek;

	switch (ch) {
			
		case 'w':
			snk_chdir(snk, UP);
			break;
		case 'a':
			snk_chdir(snk, LEFT);
			break;		
		case 's':
			snk_chdir(snk, DOWN);
			break;
		case 'd':
			snk_chdir(snk, RIGHT);
			break;
		case ' ':
			snk_addLength(snk);
			break;

		default:
			break;

	}

}

void gs_Tick(gState_t* gState) {

	snk_Tick(gState->snek);

	if (doCollisions(gState)) {

            req_death();
            //running = false;
            return;

    }

}

void placeFruit(gState_t* gState) {

	vect_t v;
	
	// roll a random coord
	do {

		v = getRandomVect(1, WIN_X - 2, 1, WIN_Y - 2);

	// does coord collide with snek?
	} while (!snk_hasCollided(gState->snek, &gState->fruit));

	gState->fruit.x = v.x; 
	gState->fruit.y = v.y;

}

int doCollisions(gState_t* gState) {

	snk_t* snek = gState->snek;
	vect_t* fruit = &gState->fruit;

	// Check if fruit has been eaten
	if (snk_hasCollided(snek, fruit)) {

		placeFruit(gState);
		snk_addLength(snek);

	}

	// Check if snek has hit the field edge
	if (snek->headPos.x < 1 || snek->headPos.x > (WIN_X - 2)) {

		return true;

	}

	if (snek->headPos.y < 1 || snek->headPos.y > (WIN_Y - 2)) {

		return true;

	}

	// has snek eaten itself?
	for (int j = 0; j < snek->length; ++j) {
		if (isEqual(&snek->body[j], &snek->headPos)) {

			return true;

		}
	}

	return false;
}
