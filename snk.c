#include "snk.h"
#include <stdlib.h>

#define SNK_DEF_XPOS 10
#define SNK_DEF_YPOS 10

static void snk_shuffBody(snk_t* snk);

snk_t* snk_NewSnk() {

	snk_t* snk = malloc(sizeof(snk_t));
    snk->headPos.x = SNK_DEF_XPOS;
    snk->headPos.y = SNK_DEF_YPOS;

    for (int j = 0; j < SNK_MAX_LEN; ++j) {

        snk->body[j].x = 0;
        snk->body[j].y = 0;

    }

	snk->bodyRune  = '0';
	snk->headRune  = 'X';
    snk->length    = 0;
    snk->actualDir = RIGHT;
	snk->nextDir   = RIGHT;

	return snk;

}

void snk_Free(snk_t* snk) {

	free(snk);

}

void snk_Tick(snk_t* snk) {

	// multiple direction keys can be pressed prior to tick
	// we buffer the most recent direction, and set 
	// the actual direction snek moves only when we tick
	snk->actualDir = snk->nextDir;

	// move each segment of snake body
	snk_shuffBody(snk);

	// move head
	switch (snk->actualDir) {

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

void snk_shuffBody(snk_t* snk) {

    for (int j = snk->length; j > 0; --j) {

        snk->body[j].x = snk->body[j - 1].x;
        snk->body[j].y = snk->body[j - 1].y;

    }

    snk->body[0].x = snk->headPos.x;
    snk->body[0].y = snk->headPos.y;

}

int snk_IsCollided(snk_t* snk, coord_t* coord) {

	char cFlag = false;

	// has head collided?
	if (IsEqual(&snk->headPos, coord)) {

		cFlag = true;

	}

	// has body collided?
	for (int j = 0; j < snk->length; ++j) {
		if (IsEqual(&snk->body[j], coord)) {

			cFlag = true;

		}
	}

	return cFlag;

}

void snk_Move(snk_t* snk, dir_e dir) {

	switch (dir) {

		case UP:
			if (snk->actualDir != DOWN) {
				snk->nextDir = dir;
			}
			break;

		case LEFT:
			if (snk->actualDir != RIGHT) {
				snk->nextDir = dir;
			}
			break;

		case DOWN:
			if (snk->actualDir != UP) {
				snk->nextDir = dir;
			}
			break;

		case RIGHT:
			if (snk->actualDir != LEFT) {
				snk->nextDir = dir;
			}
			break;

	}

}

void snk_Grow(snk_t* snk) {

    ++snk->length;
    snk->body[snk->length].x = snk->headPos.x;
    snk->body[snk->length].y = snk->headPos.y;

}

