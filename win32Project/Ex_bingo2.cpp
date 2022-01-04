// win32Project.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
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

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ���Ե� �Լ��� ������ �����մϴ�:
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WIN32PROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�:
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
//  �Լ�: MyRegisterClass()
//
//  �뵵: â Ŭ������ ����մϴ�.
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

    // ����� return �ϸ� ������, �ڿ��� ���������� return ���ϰ� ��.
    RegisterClassExW(&wcex);

    //----------------------------------------
    // ���� ���� ���
    static HBRUSH hBingoBrush;
    hBingoBrush = CreateSolidBrush(RGB(255, 255, 0));

    wcex.hbrBackground = hBingoBrush;
    wcex.lpfnWndProc = BingoProc;
    wcex.hCursor = LoadCursor(nullptr, IDC_HAND);
    wcex.lpszClassName = TEXT("�������");
    RegisterClassExW(&wcex);

    return 0;


}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   �뵵: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   �ּ�:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  �뵵: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���ø����̼� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
        //�¾ ��

    case WM_CREATE:
        // �θ�� hWnd �̴�. (�θ���ǥ ����)
        for (int x = 0; x < MAX; x++)
        {
            for (int y = 0; y < MAX; y++)
                g_hBingoChild[x][y] = CreateWindow(_T("�������"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, x * 100, y * 100, 100, 100, hWnd, NULL, hInst, NULL);
        }
        return 0;


    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
        if (g_bMyTurn)
            TextOut(hdc, 800, 100, TEXT("���� ����"), 5);
        else
            TextOut(hdc, 800, 100, TEXT("���� ����"), 5);
        EndPaint(hWnd, &ps);
        return 0;
    }

    // ���� üũ�϶�� �޼��� 
    case WM_CHECKBINGO: //WM_USER + 10
    {
        // Toggle turns
        g_bMyTurn = !g_bMyTurn;

        if (g_bMyTurn)
            SetWindowText(hWnd, TEXT("���� ����"));
        else
            SetWindowText(hWnd, TEXT("���� ����"));
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
                    // ã�� ĭ�� ���̸�, ���������� !! 
                    x = i;
                    y = j;
                    break;
                }
            }
        }
        // �ε����� ������ �׸��ض�
        if (x < 0 || y < 0) return 0;

        // Check Bingo	
        CheckBingo(HORIZONTAL, x, 0, nCurMode);	// ����üũ **********	
        CheckBingo(VERTICAL, 0, y, nCurMode);	// ����üũ **********
        if (x == y)
            CheckBingo(FDIAGONAL, x, y, nCurMode);	// �밢��üũ 1 **********
        if (x + y == MAX - 1)
            CheckBingo(BDIAGONAL, 0, 0, nCurMode);	// �밢��üũ 2 **********
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
        // �¾�ڸ��� �� ĭ�� 0 (���� ���̴�.)
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
            TextOut(hdc, 10, 10, TEXT("���� ����"), 5);
            break;
        case bingoYourBINGO:
            MyBrush = CreateSolidBrush(RGB(0, 255, 255));
            OldBrush = (HBRUSH)SelectObject(hdc, MyBrush);
            Rectangle(hdc, 10, 10, 90, 90);
            SelectObject(hdc, OldBrush);
            DeleteObject(MyBrush);
            TextOut(hdc, 10, 10, TEXT("���� ����"), 5);
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


// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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

