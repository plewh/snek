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
					ev_PushEvent(ev_NewEvent(APP_QUIT));
                    break;

                case 'w':
					ev_PushEvent(ev_NewEvent(SNK_MOVE_UP));
                    break;

                case 'a':
					ev_PushEvent(ev_NewEvent(SNK_MOVE_LEFT));
                    break;

                case 's':
					ev_PushEvent(ev_NewEvent(SNK_MOVE_DOWN));
                    break;

                case 'd':
					ev_PushEvent(ev_NewEvent(SNK_MOVE_RIGHT));
                    break;

                case ' ':
					ev_PushEvent(ev_NewEvent(SNK_INC_LEN));
                    break;

                case 'x':
					ev_PushEvent(ev_NewEvent(APP_TICKRATE_DEC));
                    break;

                case 'c':
					ev_PushEvent(ev_NewEvent(APP_TICKRATE_INC));
                    break;

				case 'p':
					ev_PushEvent(ev_NewEvent(GS_PAUSE));

                default:
                    break;

            }

    }

}
