#include "Config.h"
#include "windows.h"

int Global::BOARD_SIZE = 19;
int Global::EVA_CLOSE = 3;
int Global::EVA_OPEN = 0;
int Global::EVA_DEPTH = 2;
int Global::COLOR_AI = 1;
int Global::COLOR_PER = 2;

int Global::VAL_ONE =10;
int Global::VAL_TWO =100;
int Global::VAL_THREE =1000;
int Global::VAL_FOUR =10000;
int Global::VAL_FIVE =100000;

int Global::VAL_ONE_S =1;
int Global::VAL_TWO_S =10;
int Global::VAL_THREE_S =100;
int Global::VAL_FOUR_S =1000;

int Global::VAL_ZERO = 0;

int Global::getType(int points[19][19], int x, int y){
		if(x>18 || x<0 || y>18 || y<0)
			return Global::EVA_CLOSE;
		else
			return points[x][y];
}

bool TransForm(int *x, int *y){
	if(*x<123 || *x>=598 || *y<23 || *y>=498)
		return false;
	*x = (*x-123)/25;
	*y = (*y-23)/25;
	return true;
}