#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define true 1
#define false 0

#define SNK_HEAD 'X'
#define SNK_BODY 0
#define SNK_MAX_LEN 100

#define FRUIT_CHAR 'F'

#define TICK_RATE 1.0
#define MAX_TICKS 64.0

#define WIN_X 80
#define WIN_Y 24

typedef enum {

    UP,
    LEFT,
    DOWN,
    RIGHT

} dir_e;

typedef struct {

    int x;
    int y;

} vect_t;

typedef struct {

    vect_t headPos;
    vect_t body[SNK_MAX_LEN];
    int    length;
    dir_e  dir;

} snk_t;

void snk_init(snk_t* snk);
void snk_addLength(snk_t* snk);
void snk_shuffBody(snk_t* snk);
void snk_handleInput(snk_t* snk, char* running, double* maxTicks);
void doFrame(WINDOW* win, snk_t* snk, vect_t* fruit);
char isEqual(vect_t* a, vect_t* b);
void placeFruit(snk_t* snk, vect_t* fruit);
int  hasCollided(snk_t* snk, vect_t* vect);

int main() {

    // setup and inits
    initscr();
    cbreak();
    noecho();
    curs_set(false);
    nodelay(stdscr, 1);

    srand(time(NULL));

    double currTicks = 0.0;
    double maxTicks = MAX_TICKS;

    snk_t snk;
    snk_init(&snk);

    vect_t fruit = {rand() % 80, rand() % 24};

    char running = true;

    // screen size check
    int winX, winY;
    getmaxyx(stdscr, winY, winX);
    if (winX < WIN_X || winY < WIN_Y) {

        fprintf(stderr, "%d:%d\n", winX, winY);
        fprintf(stderr, "Terminal to small...\n");
        running = false;

    }

    // calc field offset
    int offX = (winX - WIN_X) / 2;
    int offY = (winY - WIN_Y) / 2;

    WINDOW* field = newwin(WIN_Y, WIN_X, offY, offX);

    // start main loop
    while (running) {

        snk_handleInput(&snk, &running, &maxTicks);

        // tick game state
        currTicks += TICK_RATE;
        if (currTicks >= maxTicks) {

            currTicks = 0.0;

            // shuffle snake body
            snk_shuffBody(&snk);

            // move head
            switch (snk.dir) {

            case UP:
                snk.headPos.y -= 1;
                break;

            case LEFT:
                snk.headPos.x -= 1;
                break;

            case DOWN:
                snk.headPos.y += 1;
                break;

            case RIGHT:
                snk.headPos.x += 1;
                break;

            }

        }

        // check for collisions
        char cFlag = false;

        // has head collided?
        if (isEqual(&snk.headPos, &fruit)) {

            cFlag = true;

        }

        // has body collided?
        for (int j = 0; j < snk.length; ++j) {
            if (isEqual(&snk.body[j], &fruit)) {

                cFlag = true;

            }
        }

        if (cFlag) {

			placeFruit(&snk, &fruit);
            ++snk.length;

        }

        // draw frame 
        doFrame(field, &snk, &fruit);

    }

    endwin();

    return 0;
}

void snk_init(snk_t* snk) {

    snk->headPos.x = 10;
    snk->headPos.y = 10;

    for (int j = 0; j < SNK_MAX_LEN; ++j) {

        snk->body[j].x = 0;
        snk->body[j].y = 0;

    }

    snk->length = 0;
    snk->dir = RIGHT;


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

void snk_handleInput(snk_t* snk, char* running, double* maxTicks) {

    char ch = '~';

    while ( (ch = getch()) != ERR ) {

            switch (ch) {
                
                case 'q':
                    *running = false;
                    break;

                case 'w':
                    snk->dir = UP;
                    break;

                case 'a':
                    snk->dir = LEFT;
                    break;

                case 's':
                    snk->dir = DOWN;
                    break;

                case 'd':
                    snk->dir = RIGHT;
                    break;

                case ' ':
                    snk_addLength(snk);
                    break;

                case 'x':
                    ++*maxTicks;
                    fprintf(stderr, "%f\n", *maxTicks);
                    break;

                case 'c':
                    fprintf(stderr, "%f\n", *maxTicks);
                    --*maxTicks;
                    break;

                default:
                    break;

            }

    }

}

void doFrame(WINDOW* win, snk_t* snk, vect_t* fruit) {

    // set delay period (in nano seconds)
    struct timespec tSpec = {0, 1000000};

    // clear current buffer
    werase(win);
    box(win, 0, 0);

    // draw fruit to buffer
    mvwaddch(win, fruit->y, fruit->x, FRUIT_CHAR);

    // draw snk body to buffer
    if (snk->length > -1) {

        for (int j = 0; j < snk->length; ++j) {
            
            //mvaddch(snk->body[j].y, snk->body[j].x, '0');
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

char isEqual(vect_t* a, vect_t* b) {

    if (a->x == b->x && a->y == b->y) {

        return true;

    }

    return false;

}

void placeFruit(snk_t* snk, vect_t* fruit) {

	int x, y;

	// roll a random coord
	do {

		x = 1 + rand() % 78;
		y = 1 + rand() % 22;

	// does coord collide with snek?
	} while (hasCollided(snk, fruit));

	fruit->x = x;
	fruit->y = y;

}

int hasCollided(snk_t* snk, vect_t* vect) {

	// check for collisions
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
