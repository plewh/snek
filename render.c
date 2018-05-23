#include "render.h"
#include <ncurses.h>
#include <stdlib.h>
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
        ev_PushEvent(ev_NewEvent(APP_QUIT, ' '));

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

	rend_t* rendables = gs_GetRenderables();

	for (int j = 0; j < rendables->length; ++j) {

		switch (rendables->obj[j].type) {
			
			case RUNE:
				mvwaddch(win, 
						 rendables->obj[j].pos.y, 
						 rendables->obj[j].pos.x, 
						 rendables->obj[j].rune);
				break;

			case TEXT:
				mvwprintw(win,
						  rendables->obj[j].pos.y,
						  rendables->obj[j].pos.x,
						  rendables->obj[j].text);

			default:
				break;

		}
	}

	wrefresh(win);

	free(rendables);

	/*
	snk_t const* snk = gs_GetSnake();
	frt_t const* frt = gs_GetFruit();

	werase(win);
	box(win, 0, 0);

	if (gs_IsHidden()) {
		wrefresh(win);
		return;
	}

	// fruit
	mvwprintw(win, 0, 0, "%d:%d", frt->pos.x, frt->pos.y);
	mvwaddch(win, frt->pos.y, frt->pos.x, 'F');

	// snake body
	for (int j = 0; j < snk->length; ++j) {

		mvwaddch(win, snk->body[j].y, snk->body[j].x, '0');

	}

	// snake head
	mvwaddch(win, snk->headPos.y, snk->headPos.x, 'X');

	//pause
	if (gs_IsPaused())
		mvwprintw(win, 0, 10, "PAUSE");

	wrefresh(win);
	*/

}

void r_Tick() {

	;

}

rend_t* r_NewRends() {

	rend_t* new = malloc(sizeof(rend_t));

	new->length = 0;
	
	return new;

}

void r_PushRend(rend_t* rend, 
                obj_type_e type, 
 		   		char rune, 
				char* text, 
				coord_t pos) {
	
	rend->obj[rend->length].type = type;
	rend->obj[rend->length].rune = rune;
	rend->obj[rend->length].text = text;
	rend->obj[rend->length].pos  = pos;

	++rend->length;

}
