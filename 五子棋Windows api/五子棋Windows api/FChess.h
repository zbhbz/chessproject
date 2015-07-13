#ifndef _FCHESS_H_
#define _FCHESS_H_

#include <windows.h>
#include "Record.h"

class FChess{
private:
	int points[19][19];
	Record *record;
public:
	FChess(Record *temp){
		for(int i=0; i<19; i++)
			for(int j=0; j<19; j++)
				points[i][j] = 0;
		record = temp;
	}
	bool SetChess(int x, int y, int side){
		if(points[x][y]!=0)
			return false;
		points[x][y] = side;
		return true;
	}
	void ClearBoard(){
		for(int i=0; i<19; i++)
			for(int j=0; j<19; j++)
				points[i][j] = 0;
	}
	void ClearChess(int x, int y){
		points[x][y]=0;
	}
	void SetBoard(int a[19][19]){
		for(int i=0; i<19; i++)
			for(int j=0; j<19; j++)
				points[i][j] = a[i][j];
	}
	void ShowBoard(HDC hdc, HDC hdcMem, int Board_W, int Board_H, int Chess_Size, int side){
		BitBlt(hdc, 123, 23, Board_W-246, Board_H-46, hdcMem, 123, 23, SRCCOPY);
	}
	int ShowMe(HDC hdc, HDC hdcMem, int Board_W, int Board_H, int Chess_Size, int side){
		int x, y;
		for(int i=0; i<19; i++){
			for(int j=0; j<19; j++){
				if(points[i][j]==1){
					BitBlt(hdc, i*25+123, j*25+23, Chess_Size, Chess_Size, hdcMem, 0, Board_H, SRCCOPY);
				}
				if(points[i][j]==2){
					BitBlt(hdc, i*25+123, j*25+23, Chess_Size, Chess_Size, hdcMem, 0, Board_H+Chess_Size, SRCCOPY);
				}
			}
		}
		if(record->getCur(&x, &y)){
			if(points[x][y]==1){
				BitBlt(hdc, x*25+123, y*25+23, Chess_Size, Chess_Size, hdcMem, 0, Board_H+Chess_Size*2, SRCCOPY);
			}
			if(points[x][y]==2){
				BitBlt(hdc, x*25+123, y*25+23, Chess_Size, Chess_Size, hdcMem, 0, Board_H+Chess_Size+Chess_Size*2, SRCCOPY);
			}
		}
		return IsOver(side);
	}
	void GetChess(int a[19][19]){
		for(int i=0; i<19; i++)
			for(int j=0; j<19; j++){
				a[i][j] = points[i][j];
			}
	}

	int IsOver(int chess_symbol){
		if(Game_Over1(chess_symbol) || Game_Over2(chess_symbol) || Game_Over3(chess_symbol)){
			return 1;
		}
		return 0;
	}




	int Game_Over1(int chess_symbol )/* "-"形或者"|"形呈龙(事实上"-"与"|"关于"\"对称) */
{
    int i, j, k, l, r;
    for( i=0; i < 19; i++ )
        for( k=0; k < 19-4; k++ )
        {
            l=r=0;
            for( j=k; j < k+5; j++ )/* algorithm error: "j < k+5;" not "j < j+5;" */
            {
                if( points[i][j] == chess_symbol )/* "-"形呈龙 */
                    l++;
                if( points[j][i] == chess_symbol )/* "|"形呈龙 */
                    r++;
            }
            if( l == 5 || r == 5 )/*连续的5个棋子在一条直线上时,游戏结束*/
                return 1;
        }
        return 0;
}
 
int Game_Over2( int chess_symbol )/* "\"形呈龙 */
{
    int i, j, k, m, LeftDown, RightUp;
    for( m=0; m < 19-4; m++ )
        for( k=0; k < 19-4-m; k++ )
        {
            RightUp=LeftDown=0;
            for( i=k , j=k+m;  i < k+5;  i++ , j++ )
            {
                if( points[i][j] == chess_symbol )/* 对角线"\"上及其右上方的"\"形呈龙 */
                    RightUp++;
                if( points[j][i] == chess_symbol )/* 对角线"\"上及其左下方的"\"形呈龙 */
                    LeftDown++;
            }
            if( RightUp == 5 || LeftDown == 5 )
                return 1;
        }
        return 0;
}

int Game_Over3(int chess_symbol )/* "/"形呈龙 */
{
    int i, j, k, m, LeftUp, RightDown;
        for( m=19-1; m >= 4; m-- )
        for( k=0; k < m + 1 - 4; k++ )
        {
            RightDown=LeftUp=0;
            for( i=k , j= m - k;  i < k+5;  i++ , j-- )
            {
                if( points[i][j] == chess_symbol )/* 对角线"/"上及其左上方的"/"形呈龙 */
                    LeftUp++;
                if( points[19-1-j][19-1-i] == chess_symbol )/* 对角线"/"上及其右下方的"/"形呈龙 */
                    RightDown++;/* 上面应该是 "a[N-1-j][N-1-i] " , 而不是 " a[N-j][N-i] " */
            }
            if( RightDown == 5 || LeftUp == 5 )
                return 1;
        }
        return 0;
}

};

#endif