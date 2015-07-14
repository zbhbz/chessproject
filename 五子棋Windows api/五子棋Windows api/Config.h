#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "windows.h"

class Global{
public:
	static int BOARD_SIZE;

	static int EVA_CLOSE;
	static int EVA_OPEN;
	static int EVA_DEPTH;

	static int COLOR_AI;
	static int COLOR_PER;

	static int VAL_ONE;
	static int VAL_TWO;
	static int VAL_THREE;
	static int VAL_FOUR;
	static int VAL_FIVE;

	static int VAL_ONE_S;
	static int VAL_TWO_S;
	static int VAL_THREE_S;
	static int VAL_FOUR_S;

	static int VAL_ZERO;

	static int getType(int points[19][19], int x, int y);
};

bool TransForm(int *x, int *y);
#endif