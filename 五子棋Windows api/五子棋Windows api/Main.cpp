#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")
#include "resource.h"
#include "FChess.h"
#include "CComputer.h"
#include "string"
#include "Config.h"
#include "Record.h"

#define WM_COMPUTER (WM_USER + 100)

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SetDlgProc(HWND, UINT, WPARAM, LPARAM);
 
Record record;
FChess theApp(&record);
CComputer com(&theApp);

struct Button{
	int iStyle;
	TCHAR * szText;
}
button[]={
	BS_PUSHBUTTON, TEXT("开始"),
	BS_PUSHBUTTON, TEXT("设置"),
	BS_PUSHBUTTON, TEXT("退出"),
	BS_PUSHBUTTON, TEXT("悔棋"),
};
struct Point{
	int x;
	int y;
}
point[]={
	10, 242,
	10, 332,
	10, 422,
	630,50,
};

int Person = 2;
bool Start = false;
int Level = 1;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("FiveChess"); 
	HWND hwnd;	
	MSG msg;
	WNDCLASS wndclass;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;						
	wndclass.lpfnWndProc = WndProc;                                 
	wndclass.cbClsExtra = 0;										
	wndclass.cbWndExtra = 0;									   
	wndclass.hInstance = hInstance;									
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);					
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = szAppName;								
	wndclass.lpszClassName = szAppName;						
	
	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName,              
						TEXT("五子"),        
						WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,    
						400,         
						100,          
						730,           
						580,          
						NULL,                      
						NULL,
						hInstance,
						NULL);      

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	
	while(GetMessage(&msg,NULL,0,0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);  
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	int static Board_W, Board_H, Chess_Size;
	HBITMAP static hBitmap;
	HBITMAP static hBChess;
	BITMAP bitmap;
	HDC hdc, hdcMem;   
	static HINSTANCE hInstance;
	PAINTSTRUCT ps;      
	static HWND HwndButton[4];
	TCHAR buffer[10];
	int x1, x2, side;

	switch(message)
	{
	case WM_CREATE:
		for(int i=0; i<4; i++){
			HwndButton[i]=CreateWindow(
					TEXT("Button"),
					button[i].szText,
					WS_CHILD|WS_VISIBLE|button[i].iStyle,
					point[i].x,
					point[i].y,
					80,
					50,
					hwnd,
					(HMENU)i,
					((LPCREATESTRUCT)lParam)->hInstance,
					NULL
				);
		}
		hInstance = (LPCREATESTRUCT (lParam)) -> hInstance;
		hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(qi));
		GetObject(hBitmap, sizeof(BITMAP), &bitmap);
		Chess_Size=24;
		Board_W = bitmap.bmWidth;
		Board_H = bitmap.bmHeight-Chess_Size*4;
		return 0;

	case WM_COMMAND:
		hdc = GetDC(hwnd);
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, hBitmap);
		switch(LOWORD(wParam)){
		case 0:
			if(Start)
				break;
			theApp.ClearBoard();
			if(Person==2){
				Global::COLOR_PER = 2;
				Global::COLOR_AI = 1;
				theApp.SetChess(9,9,1);
				record.Add(9,9);
			}
			else{
				Global::COLOR_PER = 1;
				Global::COLOR_AI = 2;
			}
			Start = true;
			theApp.ShowBoard(hdc, hdcMem, Board_W, Board_H, Chess_Size, 1);
			theApp.ShowMe(hdc, hdcMem, Board_W, Board_H, Chess_Size, 1);	
			break;
		case 1:
			DialogBox(hInstance, TEXT("SETBOX"), hwnd, SetDlgProc);
			break;
		case 2:
			exit(0);
			break;
		case 3:
			if(Start==false)
				break;
			if(record.Pop(&x1, &x2, Person)){
				theApp.ClearChess(x1, x2);
			}
			if(record.Pop(&x1, &x2, Person)){
				theApp.ClearChess(x1, x2);
			}
			theApp.ShowBoard(hdc, hdcMem, Board_W, Board_H, Chess_Size, 1);
			theApp.ShowMe(hdc, hdcMem, Board_W, Board_H, Chess_Size, 1);
			break;
		case IDM_HELP:
			DialogBox(hInstance, TEXT("HELPBOX"), hwnd, AboutDlgProc);
			break;
		case IDM_ABOUT:
			DialogBox(hInstance, TEXT("ABOUTBOX"), hwnd, AboutDlgProc);
			break;
		case IDM_LO:
			break;
		case IDM_SA:
			break;
		case IDM_EXIT:
			exit(0);
			break;
		default:
			break;
		}
		DeleteDC(hdcMem);
		ReleaseDC(hwnd,hdc);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);   	                             
		hdcMem = CreateCompatibleDC(hdc);

		SelectObject(hdcMem, hBitmap);

		BitBlt(hdc, 0, 0, Board_W, Board_H, hdcMem, 0, 0, SRCCOPY);
		
		theApp.ShowMe(hdc, hdcMem, Board_W, Board_H, Chess_Size, 1);	

		DeleteDC(hdcMem);
		EndPaint(hwnd, &ps);
		return 0; 

	case WM_LBUTTONDOWN:
		if(!Start)
			return 0;
		x = LOWORD(lParam);
		y = HIWORD(lParam); 
		if(!TransForm(&x, &y))
			break;
		if(theApp.SetChess(x, y , Global::COLOR_PER)){
			hdc = GetDC(hwnd);   	                             
			hdcMem = CreateCompatibleDC(hdc);
			SelectObject(hdcMem, hBitmap);
			record.Add(x, y);
			if(theApp.ShowMe(hdc, hdcMem, Board_W, Board_H, Chess_Size, Global::COLOR_PER)){
				MessageBox (NULL, TEXT ("你赢了！"), TEXT ("结局"), 0);
				Start = false;
			}
			DeleteDC(hdcMem);
			ReleaseDC(hwnd, hdc);
			SendMessage(hwnd,  WM_COMPUTER, wParam, lParam);
		}
		return 0;

	/*case WM_RBUTTONDOWN:
		hdc = GetDC(hwnd);   	
		swprintf(buffer, TEXT("人的棋子：%i   电脑水平：%i"), Person, Level);
		MessageBox(hwnd, buffer, TEXT(""), MB_OK);
		ReleaseDC(hwnd, hdc);
		return 0;
		*/
		
	case WM_COMPUTER:
		com.Think(&record);

		hdc = GetDC(hwnd);   	                             
		hdcMem = CreateCompatibleDC(hdc);
		SelectObject(hdcMem, hBitmap);
		if(theApp.ShowMe(hdc, hdcMem, Board_W, Board_H, Chess_Size, Global::COLOR_AI)){
			MessageBox (NULL, TEXT ("电脑赢了！"), TEXT ("结局"), 0) ;
			Start = false;
		}	
		DeleteDC(hdcMem);
		ReleaseDC(hwnd, hdc);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch(message){
	case WM_INITDIALOG:
		return true;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDOK:
			EndDialog(hwnd, 0);
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

BOOL CALLBACK SetDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	static int tPerson;
	static int tLevel;
	switch(message){
	case WM_INITDIALOG:
		tPerson = Person;
		tLevel = Level;
		CheckRadioButton(hwnd, IDC_RADIO1, IDC_RADIO2, Person==2?IDC_RADIO1:IDC_RADIO2);
		CheckRadioButton(hwnd, IDC_RADIO3, IDC_RADIO5, Level==1? IDC_RADIO3:(Level==2?IDC_RADIO4:IDC_RADIO5));
		return true;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case IDB_CAL:
			EndDialog(hwnd, 0);
			return true;
		case IDB_OK:
			EndDialog(hwnd, 0);
			Start = false;
			Person = tPerson;
			Level = tLevel;
			return true;
		case IDC_RADIO1:
			tPerson = 2;
			return true;
		case IDC_RADIO2:
			tPerson = 1;
			return true;
		case IDC_RADIO3:
			tLevel = 1;
			return true;
		case IDC_RADIO4:
			tLevel = 2;
			return true;
		case IDC_RADIO5:
			tLevel = 3;
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}