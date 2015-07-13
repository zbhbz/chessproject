#include "CComputer.h"
#include "FChess.h"

int CComputer::evaluateState(int points[19][19], int type){
	int value, lineP;
	int** Line = new int*[6];
	Line[0] = new int[21];
	Line[1] = new int[21];
	Line[2] = new int[21];
	Line[3] = new int[21];
	Line[4] = new int[21];
	Line[5] = new int[21];
	value = 0;
	for(int i=0; i<6; i++)
		Line[i][0] = Line[i][20] = Global::EVA_CLOSE;
	for(int i=0; i<19; i++){
		lineP=1;
		for(int j=0; j<19; j++){
			Line[0][lineP] = Global::getType(points, i, j);//|
			Line[1][lineP] = Global::getType(points, j, i);//-
			Line[2][lineP] = Global::getType(points, i+j, j);
			Line[3][lineP] = Global::getType(points, Global::BOARD_SIZE-1-i-j, j);
			Line[4][lineP] = Global::getType(points, j, j+i);
			Line[5][lineP] = Global::getType(points, Global::BOARD_SIZE-1-j, i+j);
			lineP++;
		}
		int special = i == 0 ? 4 : 6;
		for (int p = 0; p < special; ++p)
		{
			value += evaluateLine(Line[p], type);
		}
	}
	return value;
}
int CComputer::evaluateLine(int Line[], int type){
	int value = 0;
	int cnt = 0;
	int death = 0;
	for(int i=1; i<=Global::BOARD_SIZE; i++){
		if(Line[i]==type){
			cnt = 1;
			death = 0;
			if(Line[i-1]!=Global::EVA_OPEN)
				death++;
			for(i=i+1; i<=Global::BOARD_SIZE && Line[i]==type; i++)
				cnt++;
			if(Line[i]!=Global::EVA_OPEN)
				death++;
			value += getValue(cnt, death);
		}
	}
	return value;
}
int CComputer::getValue(int cnt, int death){
	if(death==0){
		switch(cnt){
		case 1: return Global::VAL_ONE;
		case 2: return Global::VAL_TWO;
		case 3: return Global::VAL_THREE;
		case 4: return Global::VAL_FOUR;
		default: return Global::VAL_FIVE;
		}
	}
	if(death==1){
		switch(cnt){
		case 1: return Global::VAL_ONE_S;
		case 2: return Global::VAL_TWO_S;
		case 3: return Global::VAL_THREE_S;
		case 4: return Global::VAL_FOUR_S;
		default: return Global::VAL_FIVE;
		}
	}
	if(death==2){
		if(cnt>=5)
			return Global::VAL_FIVE;
		else 
			return Global::VAL_ZERO;
	}
}

void CComputer::CopyBoard(int pre[19][19],int last[19][19]){
	for(int i=0; i<19; i++)
		for(int j=0; j<19; j++)
			last[i][j] = pre[i][j];
}

POINT CComputer::AI(int points[19][19]){
	POINT p, q;
	int copy_board[19][19];
	int MIN=10000000, temp;
	CopyBoard(points, copy_board);
	SelectStep(copy_board);
	for(int i=0; i<19; i++)
		for(int j=0; j<19; j++){
			if(copy_board[i][j]==0){
				p.x=i; p.y=j;
				temp = SearchDepth(copy_board, p, Global::COLOR_AI, 0, MIN);
				if(MIN>temp){
					MIN = temp;
					q.x = i; q.y=j;
				}
			}
		}
	return q;
}

int CComputer::SearchDepth(int points[19][19], POINT p, int color, int depth, int AL_BT){
	depth++;
	int MAX=-10000000, MIN=10000000, temp, value;
	POINT q;
	points[p.x][p.y] = color;
	if(depth<Global::EVA_DEPTH){
		if(color==Global::COLOR_AI){
			for(int i=0; i<19; i++)
				for(int j=0; j<19; j++){
					if(points[i][j]==0){
						q.x=i; q.y=j;
						temp=SearchDepth(points, q, Global::COLOR_PER, depth, MAX);
						if(temp>AL_BT){ 
							points[p.x][p.y] = 0;
							return temp;
						}
						MAX = MAX<temp ? temp : MAX;
					}
				}
			points[p.x][p.y] = 0;
			return MAX;
		}
		if(color==Global::COLOR_PER){
			for(int i=0; i<19; i++)
				for(int j=0; j<19; j++){
					if(points[i][j]==0){
						q.x=i; q.y=j;
						temp = SearchDepth(points, q, Global::COLOR_AI, depth, MIN);
						if(temp<AL_BT){
							points[p.x][p.y] = 0;
							return temp;
						}
						MIN = MIN>temp ? temp : MIN;
					}
				}
			points[p.x][p.y] = 0;
			return MIN;
		}
	}
	value = evaluateState(points, Global::COLOR_PER)-evaluateState(points, Global::COLOR_AI);
	points[p.x][p.y] = 0;
	return value;
}

bool CComputer::AllDirection(int x, int y){
	int dx[8] = {0,2,2, 2, 0,-2,-2,-2};
	int dy[8] = {2,2,0,-2,-2,-2, 0, 2};
	int dx1[8] = {0,1,1, 1, 0,-1,-1,-1};
	int dy1[8] = {1,1,0,-1,-1,-1, 0, 1};

	int direct, x1, y1;

	for(direct=0; direct<8; direct++){
		x1 = x-dx[direct]; 
		y1 = y-dy[direct];
		if(x1>=0 && x1<=18 && y1>=0 && y1<=18 && a[x1][y1]!=0){
			return true;
		}
		x1 = x-dx1[direct]; 
		y1 = y-dy1[direct];
		if(x1>=0 && x1<=18 && y1>=0 && y1<=18 && a[x1][y1]!=0){
			return true;
		}
	}
	return false;
}

void CComputer::SelectStep(int points[19][19]){
	for(int i=0; i<19; i++)
		for(int j=0; j<19; j++){
			if(points[i][j]==0){
				if(!AllDirection(i, j)){
					points[i][j] = Global::EVA_CLOSE;
				}
			}
		}
}
