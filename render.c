#include "render.h"
#include <ncurses.h>
#include "event.h"
#include "gstate.h"
#include "snk.h"
#include "frt.h"

static WINDOW* win;
static int offX;
static int offY;

void r_Init() {

    initscr();
    cbreak();
    noecho();
    curs_set(false);
    nodelay(stdscr, true);

    // screen size check
    int winX, winY;
    getmaxyx(stdscr, winY, winX);
    if (winX < WIN_X || winY < WIN_Y) {

        fprintf(stderr, "Term size is %d:%d chars, require %d:%d or larger", 
		        winX, winY, WIN_X, WIN_Y);
        ev_PushEvent(ev_NewEvent(APP_QUIT));

    }

    // calc field offset
    offX = (winX - WIN_X) / 2;
    offY = (winY - WIN_Y) / 2;

    win = newwin(WIN_Y, WIN_X, offY, offX);

}

void r_Cleanup() {

	endwin();

}

void r_Responder(event_t* ev) {

	;

}

void r_DoFrame() {

	werase(win);
	box(win, 0, 0);

	snk_t const* snk = gs_GetSnake();

	for (int j = 0; j < snk->length; ++j) {

		mvaddch(snk->body->y, snk->body->x, '0');

	}

	mvaddch(snk->headPos.y, snk->headPos.x, 'X');

	wrefresh(win);

}

void r_Tick() {

	;

}
