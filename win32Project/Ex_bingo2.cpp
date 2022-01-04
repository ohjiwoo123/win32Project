// win32Project.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "win32Project.h"

#define WM_CHECKBINGO WM_USER + 10

BOOL g_bMyTurn = TRUE;
enum modeBINGO { bingoNONE, bingoMINE, bingoMyBINGO, bingoYOURS, bingoYourBINGO };
enum modeCHECK { HORIZONTAL, VERTICAL, FDIAGONAL, BDIAGONAL };

#define MAX 5
HWND g_hBingoChild[MAX][MAX];

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    BingoProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}


//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32PROJECT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // 여기다 return 하면 에러남, 뒤에서 빙고프락을 return 못하게 됨.
    RegisterClassExW(&wcex);

    //----------------------------------------
    // 나의 빙고 등록
    static HBRUSH hBingoBrush;
    hBingoBrush = CreateSolidBrush(RGB(255, 255, 0));

    wcex.hbrBackground = hBingoBrush;
    wcex.lpfnWndProc = BingoProc;
    wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
    wcex.lpszClassName = TEXT("빙고빙고");
    RegisterClassExW(&wcex);

    return 0;


}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int x, y;
    HDC hdc;
    PAINTSTRUCT ps;
    static TCHAR str[256];

    switch (message)
    {
        //태어날 때

    case WM_CREATE:
        // 부모는 hWnd 이다. (부모좌표 기준)
        for (int x = 0; x < MAX; x++)
        {
            for (int y = 0; y < MAX; y++)
                g_hBingoChild[x][y] = CreateWindow(_T("빙고빙고"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, x * 100, y * 100, 100, 100, hWnd, NULL, hInst, NULL);
        }
        return 0;


    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        if (g_bMyTurn)
            TextOut(hdc, 800, 100, TEXT("나의 차례"), 5);
        else
            TextOut(hdc, 800, 100, TEXT("너의 차례"), 5);
        EndPaint(hWnd, &ps);
        return 0;
    }

    // 빙고를 체크하라는 메세지 
    case WM_CHECKBINGO: //WM_USER + 10
    {
        // Toggle turns
        g_bMyTurn = !g_bMyTurn;

        if (g_bMyTurn)
            SetWindowText(hWnd, TEXT("나의 차례"));
        else
            SetWindowText(hWnd, TEXT("너의 차례"));
        InvalidateRect(hWnd, NULL, TRUE);

        // Find My Info
        HWND hCurChild = (HWND)wParam;
        int nCurMode = (int)lParam;
        int x = -1;
        int y = -1;

        for (int i = 0; i < MAX; i++) {
            for (int j = 0; j < MAX; j++) {
                if (g_hBingoChild[i][j] == hCurChild)
                {
                    // 찾는 칸이 나이면, 빠져나오라 !! 
                    x = i;
                    y = j;
                    break;
                }
            }
        }
        // 인덱스가 없으면 그만해라
        if (x < 0 || y < 0) return 0;

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
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK BingoProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HBRUSH MyBrush, OldBrush;
    int mode;
    switch (message)
    {

    case WM_CREATE:
    {
        // 태어나자마자 각 칸이 0 (빙고 논이다.)
        SetProp(hWnd, TEXT("nBingoMode"), (HANDLE)bingoNONE);//init
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        mode = (int)GetProp(hWnd, TEXT("nBingoMode"));

        if (mode == bingoNONE)
        {
            if (g_bMyTurn)
            {
                mode = bingoMINE;
            }
            else
            {
                mode = bingoYOURS;
            }
            SetProp(hWnd, __T("nBingoMode"), (HANDLE)mode);
            InvalidateRect(hWnd, NULL, TRUE);
            SendMessage(GetParent(hWnd), WM_CHECKBINGO, (WPARAM)hWnd, (LPARAM)mode);

        }
        return 0;
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        mode = (int)GetProp(hWnd, TEXT("nBingoMode"));
        // enum modeBINGO { bingoNONE, bingoMINE, bingoMyBINGO, bingoYOURS, bingoYourBINGO };
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
    }


    case WM_DESTROY:

        RemoveProp(hWnd, TEXT("nBingoMode"));
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);  
}


// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

