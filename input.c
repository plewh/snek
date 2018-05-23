#include "input.h"
#include <ncurses.h>
#include "event.h"

void in_Init() {

	;

}

void in_Cleanup() {

	;

}

void in_HandleInput() {

    char ch = '~';

    while ( (ch = getch()) != ERR ) {

            switch (ch) {
                
                case 'q':
					ev_PushEvent(ev_NewEvent(KEY_PRESS, 'q'));
                    break;

                case 'w':
					ev_PushEvent(ev_NewEvent(KEY_PRESS, 'w'));
                    break;

                case 'a':
					ev_PushEvent(ev_NewEvent(KEY_PRESS, 'a'));
                    break;

                case 's':
					ev_PushEvent(ev_NewEvent(KEY_PRESS, 's'));
                    break;

                case 'd':
					ev_PushEvent(ev_NewEvent(KEY_PRESS, 'd'));
                    break;

                case ' ':
					ev_PushEvent(ev_NewEvent(KEY_PRESS, ' '));
                    break;

                case 'x':
					ev_PushEvent(ev_NewEvent(KEY_PRESS, 'x'));
                    break;

                case 'c':
					ev_PushEvent(ev_NewEvent(KEY_PRESS, 'c'));
                    break;

				case 'p':
					ev_PushEvent(ev_NewEvent(KEY_PRESS, 'p'));

                default:
                    break;

            }

    }

}
