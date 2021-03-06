#include "app.h"

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "defs.h"
#include "render.h"
#include "input.h"
#include "gstate.h"

#define TICK_RATE 60 //hz

static int running;

static struct timespec lastTicks;
static double CalcDelta();
static double acc;
static double tickRate;

void app_Init() {

	running = true;

	srand(time(NULL));

	ev_Init();
	in_Init();
	 r_Init();
	gs_Init();

	ev_RegisterResponder(app_Responder);
	ev_RegisterResponder(r_Responder);
	ev_RegisterResponder(gs_Responder);

	tickRate = (1.0 / TICK_RATE) * 1000;	
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &lastTicks);


}

void app_Run() {

	while (running) {

		in_HandleInput();
		ev_PumpEvents();

		acc += CalcDelta();

		while (acc >= tickRate) {

			acc -= tickRate;

			gs_Tick();
			r_Tick();


		}

		r_DoFrame();

	}

}

void app_Cleanup() {

	ev_Cleanup();
	in_Init();
	 r_Cleanup();

}

void app_RequestQuit() {

	;

}

void app_Responder(event_t* ev) {

	switch (ev->type) {
		
		case APP_QUIT:
			running = false;
			break;

		case KEY_PRESS:
			switch (ev->data) {

				case 'q':
					running = false;
					break;

				case 'x':
					tickRate += 0.5;
					break;

				case 'c':
					tickRate -= 0.5;
					break;

				default:
					break;

			}

		default:
			break;

	}

}

double CalcDelta() {

		struct timespec ticks;
		struct timespec temp;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ticks);

		if ( (ticks.tv_nsec - lastTicks.tv_nsec) < 0 ) {

			temp.tv_sec = ticks.tv_sec - lastTicks.tv_sec - 1;
			temp.tv_nsec = 1000000000 + ticks.tv_nsec - lastTicks.tv_nsec;

		} else {
			
			temp.tv_sec = ticks.tv_sec - lastTicks.tv_sec;
			temp.tv_nsec = ticks.tv_nsec - lastTicks.tv_nsec;

		}

		lastTicks = ticks;

		return (double)temp.tv_nsec / 1000000.0;

}

coord_t app_GetRandomCoord(int winX, int winY) {

	int x = 1 + (rand() % (winX - 2));
	int y = 1 + (rand() % (winY - 2));

	coord_t c = {x, y};

	return c;

}
