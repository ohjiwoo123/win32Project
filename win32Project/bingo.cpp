#define ON_MAIN
#ifdef ON_MAIN

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND g_hWndMain;
LPCTSTR lpszClass = TEXT("Bingo");


#define WM_CHECKBINGO WM_USER + 10

BOOL g_bMyTurn = TRUE;
enum modeBINGO { bingoNONE, bingoMINE, bingoMyBINGO, bingoYOURS, bingoYourBINGO };
enum modeCHECK { HORIZONTAL, VERTICAL, FDIAGONAL, BDIAGONAL };

#define MAX 5
HWND g_hBingoChild[MAX][MAX];

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	WndClass.hCursor = LoadCursor(NULL, IDC_CROSS);
	WndClass.lpfnWndProc = ChildProc;
	WndClass.lpszClassName = TEXT("BingoChild");
	WndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	RegisterClass(&WndClass);

	g_hWndMain = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(g_hWndMain, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

void CheckBingo(int nCheck, int x, int y, int nCurMode)
{
	HWND hChild;
	int nMode;
	int nBingoCount;

	//Count bingo count
	nBingoCount = 0;
	for (int temp = 0; temp < MAX; temp++)
	{
		if (nCheck == HORIZONTAL)
			hChild = g_hBingoChild[x][temp];
		else if (nCheck == VERTICAL)
			hChild = g_hBingoChild[temp][y];
		else if (nCheck == FDIAGONAL)
			hChild = g_hBingoChild[temp][temp];
		else
			hChild = g_hBingoChild[temp][MAX - 1 - temp];

		nMode = (int)GetProp(hChild, TEXT("nBingoMode"));
		if (nCurMode == nMode || nCurMode + 1 == nMode)
		{
			nBingoCount++;
		}
	}
	// !!!!!!! BINGO !!!!!!!
	if (nBingoCount == MAX)
	{
		for (int temp = 0; temp < MAX; temp++)
		{
			if (nCheck == HORIZONTAL)
				hChild = g_hBingoChild[x][temp];
			else if (nCheck == VERTICAL)
				hChild = g_hBingoChild[temp][y];
			else if (nCheck == FDIAGONAL)
				hChild = g_hBingoChild[temp][temp];
			else
				hChild = g_hBingoChild[temp][MAX - 1 - temp];

			SetProp(hChild, TEXT("nBingoMode"), (HANDLE)(nCurMode + 1));//bingo
			InvalidateRect(hChild, NULL, TRUE);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	HDC hdc;
	PAINTSTRUCT ps;
	static TCHAR str[256];

	switch (iMessage) {
	case WM_CREATE:
		for (x = 0; x < MAX; x++)
			for (y = 0; y < MAX; y++) {
				g_hBingoChild[x][y] =
					CreateWindow(TEXT("BingoChild"), NULL, WS_CHILD | WS_VISIBLE,
						x * 100, y * 100, 100, 100, hWnd, (HMENU)NULL, g_hInst, NULL);
			}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (g_bMyTurn)
			TextOut(hdc, 500, 100, TEXT("나의 차례"), 5);
		else
			TextOut(hdc, 500, 100, TEXT("너의 차례"), 5);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_CHECKBINGO:
	{
		//---------------------------------------------
		// Toggle turns
		g_bMyTurn = !g_bMyTurn;

		if (g_bMyTurn)
			SetWindowText(hWnd, TEXT("나의 차례"));
		else
			SetWindowText(hWnd, TEXT("너의 차례"));
		InvalidateRect(hWnd, NULL, TRUE);

		//---------------------------------------------
		// Find my info	
		HWND hCurChild = (HWND)wParam;
		int nCurMode = (int)lParam;
		int x = -1;
		int y = -1;

		for (int i = 0; i < MAX; i++) {
			for (int j = 0; j < MAX; j++) {
				if (g_hBingoChild[i][j] == hCurChild)
				{
					x = i;
					y = j;
					break;
				}
			}
		}
		if (x < 0 || y < 0) return 0;

		//---------------------------------------------
		// Check Bingo	
		CheckBingo(HORIZONTAL, x, 0, nCurMode);	// 가로체크 **********	
		CheckBingo(VERTICAL, 0, y, nCurMode);	// 세로체크 **********
		if (x == y)
			CheckBingo(FDIAGONAL, x, y, nCurMode);	// 대각선체크 1 **********
		if (x + y == MAX - 1)
			CheckBingo(BDIAGONAL, 0, 0, nCurMode);	// 대각선체크 2 **********
	}
	return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

LRESULT CALLBACK ChildProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush, OldBrush;

	int mode;
	switch (iMessage) {
	case WM_CREATE:
		SetProp(hWnd, TEXT("nBingoMode"), (HANDLE)bingoNONE);//init
		return 0;
	case WM_LBUTTONDOWN:
		mode = (int)GetProp(hWnd, TEXT("nBingoMode"));
		if (mode == bingoNONE)
		{
			if (g_bMyTurn)
				mode = bingoMINE;
			else
				mode = bingoYOURS;

			SetProp(hWnd, TEXT("nBingoMode"), (HANDLE)mode);
			InvalidateRect(hWnd, NULL, TRUE);

			PostMessage(g_hWndMain, WM_CHECKBINGO, (WPARAM)hWnd, (LPARAM)mode);
		}
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		mode = (int)GetProp(hWnd, TEXT("nBingoMode"));
		switch (mode)
		{
		case bingoNONE:
			Rectangle(hdc, 10, 10, 90, 90);
			break;
		case bingoMINE:
			Ellipse(hdc, 10, 10, 90, 90);
			break;
		case bingoYOURS:
			MoveToEx(hdc, 10, 10, NULL); LineTo(hdc, 90, 90);
			MoveToEx(hdc, 10, 90, NULL); LineTo(hdc, 90, 10);
			break;
		case bingoMyBINGO:
			MyBrush = CreateSolidBrush(RGB(255, 0, 255));
			OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
			Rectangle(hdc, 10, 10, 90, 90);
			SelectObject(hdc, OldBrush);
			DeleteObject(MyBrush);
			TextOut(hdc, 10, 10, TEXT("나의 빙고"), 5);
			break;
		case bingoYourBINGO:
			MyBrush = CreateSolidBrush(RGB(0, 255, 255));
			OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
			Rectangle(hdc, 10, 10, 90, 90);
			SelectObject(hdc, OldBrush);
			DeleteObject(MyBrush);
			TextOut(hdc, 10, 10, TEXT("너의 빙고"), 5);
			break;
		}

		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		RemoveProp(hWnd, TEXT("nBingoMode"));
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

#endif