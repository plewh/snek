#include "defs.h"
#include <stdlib.h>

coord_t GetRandomCoord(int winX, int winY) {

	int x = (1 + rand()) % (winX - 1);
	int y = (1 + rand()) % (winY - 1);

	coord_t c = {x, y};

	return c;

}
