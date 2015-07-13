#ifndef _RECORD_H_
#define _RECORD_H_

struct item{
	int x; 
	int y;
};

class Record{
private:
	item Line[200];
	int cnt;
public:
	Record(){
		cnt = -1;
	}
	void Add(int x, int y){
		cnt++;
		Line[cnt].x = x;
		Line[cnt].y = y;
	}
	bool Pop(int *x, int *y, int Person){
		if(Person==1){
			if(cnt<0)
				return false;
		}
		else{
			if(cnt<1)
				return false;
		}
		*x = Line[cnt].x;
		*y = Line[cnt].y;
		cnt--;
		return true;
	}
	bool getCur(int *x, int *y){
		if(cnt<0)
			return false;
		*x = Line[cnt].x;
		*y = Line[cnt].y;
		return true;
	}
};

#endif