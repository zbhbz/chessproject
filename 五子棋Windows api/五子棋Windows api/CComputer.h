#ifndef _COMMPUTER_H_
#define _COMMPUTER_H_

#include "FChess.h"
#include "Config.h"
#include "Record.h"

class CComputer{
private:
	FChess *chess;
	POINT p;
	int a[19][19];
public:
	CComputer(FChess *temp){
		chess = temp;
	}
	void Think(Record *record){
		chess->GetChess(a);
		p = AI(a);
		chess->SetChess(p.x, p.y, Global::COLOR_AI);
		record->Add(p.x, p.y);
	}
private:
	//评价当前局面，得出一方的分数
	int evaluateState(int points[19][19], int type);
    int evaluateLine(int Line[], int type);
	int getValue(int cnt, int death);
	POINT AI(int points[19][19]);
	void SelectStep(int points[19][19]);
	void CopyBoard(int pre[19][19],int last[19][19]);
	bool AllDirection(int x, int y);
	int SearchDepth(int points[19][19], POINT p, int color, int depth, int AL_BT);

//测试内部函数
public:
	void TestSelectStep(){
		int copy_board[19][19];
		chess->GetChess(a);
		CopyBoard(a, copy_board);
		SelectStep(copy_board);
		CopyBoard(copy_board, a);
		chess->SetBoard(a);
	}
};

#endif