#include "app.h"

#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include "defs.h"
#include "snek.h"

#define FRUIT_CHAR 'F'
#define TICK_RATE  1.0
#define MAX_TICKS  64.0
#define WIN_X      80
#define WIN_Y      24
#define DELAY      1000000 //ns

static double  currTicks;
static double  maxTicks; 

static snk_t*  snk;
static vect_t  fruit;

static char    running;

static int     offX;
static int     offY;

static WINDOW* field;

static void    doFrame(WINDOW* win, snk_t* snk, vect_t* fruit);
static void    placeFruit(snk_t* snk, vect_t* fruit);
static int     doCollisions(snk_t* snk, vect_t* fruit);
static void    handleInput();

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

	// create the snek object
    snk = snk_init();

	// initial random fruit placement
    fruit.x = 1 + (rand() % (WIN_X - 1));
	fruit.y = 1 + (rand() % (WIN_Y - 1));

	// create ncurses window for the game field
    field = newwin(WIN_Y, WIN_X, offY, offX);

}

void app_Run() {

    // game loop
    while (running) {

        handleInput();

        // tick game state
        currTicks += TICK_RATE;
        if (currTicks >= maxTicks) {

            currTicks = 0.0;
			snk_Tick(snk);

		}

		if (doCollisions(snk, &fruit)) {
			running = false;
			continue;
		}

        doFrame(field, snk, &fruit);

	}

}

void app_CleanUp() {

	snk_Free(snk);
    endwin();

}

void doFrame(WINDOW* win, snk_t* snk, vect_t* fruit) {

    // set delay period (in nano seconds)
    struct timespec tSpec = {0, DELAY};

    // clear current buffer
    werase(win);
    box(win, 0, 0);

    // draw fruit to buffer
    mvwaddch(win, fruit->y, fruit->x, FRUIT_CHAR);

    // draw snk body to buffer
    if (snk->length > -1) {

        for (int j = 0; j < snk->length; ++j) {
            
            mvwaddch(win, snk->body[j].y, snk->body[j].x, '0');

        }

    }

    // draw snk head to buffer
    mvwaddch(win, snk->headPos.y, snk->headPos.x, SNK_HEAD);

   

    // put buffer on screen
    wrefresh(win);

    // make puter go slow
    nanosleep(&tSpec, NULL);

}

void placeFruit(snk_t* snk, vect_t* fruit) {

	int x, y;

	// roll a random coord
	do {

		x = 1 + rand() % WIN_X - 2;
		y = 1 + rand() % WIN_Y - 2;

	// does coord collide with snek?
	} while (!snk_hasCollided(snk, fruit));

	fruit->x = x;
	fruit->y = y;

}

int doCollisions(snk_t* snk, vect_t* fruit) {

	// Check if fruit has been eaten
	if (snk_hasCollided(snk, fruit)) {

		placeFruit(snk, fruit);
		snk_addLength(snk);

	}

	// Check if snek has hit the field edge
	if (snk->headPos.x < 1 || snk->headPos.x > (WIN_X - 2)) {

		return true;

	}

	if (snk->headPos.y < 1 || snk->headPos.y > (WIN_Y - 2)) {

		return true;

	}

	// has snek eaten itself?
	for (int j = 0; j < snk->length; ++j) {
		if (isEqual(&snk->body[j], &snk->headPos)) {

			return true;

		}
	}

	return false;
}

void handleInput() {

    char ch = '~';

    while ( (ch = getch()) != ERR ) {

            switch (ch) {
                
                case 'q':
                    running = false;
                    break;

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

                case 'x':
                    ++maxTicks;
                    //fprintf(stderr, "%f\n", *maxTicks);
                    break;

                case 'c':
                    --maxTicks;
                    //fprintf(stderr, "%f\n", *maxTicks);
                    break;

                default:
                    break;

            }

    }

}
