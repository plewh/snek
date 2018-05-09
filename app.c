#include "app.h"

#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#include "defs.h"
#include "snek.h"
#include "gstate.h"

#define FRUIT_CHAR 'F'
#define TICK_RATE  1.0
#define MAX_TICKS  64.0
#define DELAY      1000000 //ns

// main loop timing controls
static double  currTicks;
static double  maxTicks;
static char    running;

// ncurses window and win size vars
static WINDOW* field;
static int     offX;
static int     offY;

// game state object
static gState_t* gState;

// private funcs
static void doFrame(WINDOW* win, gState_t* gState);
static void handleInput(gState_t* gState);
static void GameTick();
static void DeathTick();

// func pointers
static void(*TickFunc)(void);
static void(*DrawFunc)(void);

// public funcs
void req_death();

void app_Init() {

	running = true;

	// init ncurses library
    initscr();
    cbreak();
    noecho();
    curs_set(false);
    nodelay(stdscr, true);

	// init random routine
    srand(time(NULL));

	// init timing control vars
    currTicks = 0.0;
    maxTicks = MAX_TICKS;

    // screen size check
    int winX, winY;
    getmaxyx(stdscr, winY, winX);
    if (winX < WIN_X || winY < WIN_Y) {

        fprintf(stderr, "Term size is %d:%d chars, require %d:%d or larger", winX, winY, WIN_X, WIN_Y);
        running = false;

    }

    // calc field offset
    offX = (winX - WIN_X) / 2;
    offY = (winY - WIN_Y) / 2;

	// create ncurses window for the game field
    field = newwin(WIN_Y, WIN_X, offY, offX);

    gState = gs_NewState();
    TickFunc = GameTick;

}

void app_Run() {

    while (running) {

        handleInput(gState);
        
        TickFunc();

        doFrame(field, gState);

    }

}

void app_CleanUp() {

	gs_Free(gState);
    endwin();

}

void doFrame(WINDOW* win, gState_t* gState) {

    // set delay period (in nano seconds)
    struct timespec tSpec = {0, DELAY};

    // clear current buffer
    werase(win);
    box(win, 0, 0);

    // draw fruit to buffer
    mvwaddch(win, gState->fruit.y, gState->fruit.x, FRUIT_CHAR);

    // draw snk body to buffer
    if (gState->snek->length > -1) {

        for (int j = 0; j < gState->snek->length; ++j) {
            
            mvwaddch(win, gState->snek->body[j].y, gState->snek->body[j].x, '0');

        }

    }

    // draw snk head to buffer
    mvwaddch(win, gState->snek->headPos.y, gState->snek->headPos.x, SNK_HEAD);

    // put buffer on screen
    wrefresh(win);

    // make puter go slow
    nanosleep(&tSpec, NULL);

}

void handleInput(gState_t* gState) {

    char ch = '~';

    while ( (ch = getch()) != ERR ) {

            switch (ch) {
                
                case 'q':
                    req_death();
                    break;

                case 'w':
                case 'a':
                case 's':
                case 'd':
                case ' ':
					gs_HandleInput(gState, ch);
                    break;

                case 'x':
                    ++maxTicks;
                    break;

                case 'c':
                    --maxTicks;
                    break;

                default:
                    break;

            }

    }

}

void GameTick() {

    currTicks += TICK_RATE;
    if (currTicks >= maxTicks) {

        currTicks = 0.0;
        gs_Tick(gState);
        
	}

}

void req_death() {

    //TickFunc = DeathTick;
    running = false;

}

static void DeathTick() {

/*
    currTicks += TICK_RATE;
    if (currTicks >= maxTicks) {

        currTicks = 0.0;
        
        if (++deathTicks == 100) {

            deathTicks = 0;
            
            if (dChar == ' ') {
                dChar = '?';
            } else {
                dChar = ' ';
            }

        }

	}
    //running = false;
*/
}