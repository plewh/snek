#include "snek.h"
#include <stdlib.h>

static void snk_shuffBody(snk_t* snk);

snk_t* snk_init() {

	snk_t* snk = malloc(sizeof(snk_t));
    snk->headPos.x = 10;
    snk->headPos.y = 10;

    for (int j = 0; j < SNK_MAX_LEN; ++j) {

        snk->body[j].x = 0;
        snk->body[j].y = 0;

    }

    snk->length = 0;
    snk->dir = RIGHT;

	return snk;

}

void snk_addLength(snk_t* snk) {

    ++snk->length;
    snk->body[snk->length].x = snk->headPos.x;
    snk->body[snk->length].y = snk->headPos.y;

}

void snk_shuffBody(snk_t* snk) {

    for (int j = snk->length; j > 0; --j) {

        snk->body[j].x = snk->body[j - 1].x;
        snk->body[j].y = snk->body[j - 1].y;

    }

    snk->body[0].x = snk->headPos.x;
    snk->body[0].y = snk->headPos.y;

}

int snk_hasCollided(snk_t* snk, vect_t* vect) {

	char cFlag = false;

	// has head collided?
	if (isEqual(&snk->headPos, vect)) {

		cFlag = true;

	}

	// has body collided?
	for (int j = 0; j < snk->length; ++j) {
		if (isEqual(&snk->body[j], vect)) {

			cFlag = true;

		}
	}

	return cFlag;

}

void snk_Free(snk_t* snk) {

	free(snk);

}

void snk_chdir(snk_t* snk, dir_e dir) {

	snk->dir = dir;

	/*
	switch (dir) {
		
		case UP:
			snk->dir = dir;
			break;
	*/

}

void snk_Tick(snk_t* snk) {

	// shuffle snake body
	snk_shuffBody(snk);

	// move head
	switch (snk->dir) {

		case UP:
			snk->headPos.y -= 1;
			break;

		case LEFT:
			snk->headPos.x -= 1;
			break;

		case DOWN:
			snk->headPos.y += 1;
			break;

		case RIGHT:
			snk->headPos.x += 1;
			break;

		default:
			break;

	}

}
