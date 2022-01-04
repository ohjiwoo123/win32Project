// win32Project.cpp : ���ø����̼ǿ� ���� �������� �����մϴ�.
//

#include "framework.h"
#include "win32Project.h"

#define MAX_LOADSTRING 100
#define WM_CHECKBINGO WM_USER + 10

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


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        //�¾ ��

    case WM_CREATE:
    {
        // �θ�� hWnd �̴�. (�θ���ǥ ����)
        CreateWindow(_T("�������"), _T("Click Me"),
            WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 120, 100, 100, hWnd, NULL, hInst, NULL);
        CreateWindow(_T("�������"), _T("Click You"), WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 220, 100, 100, hWnd, NULL, hInst, NULL);
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // �޴� ������ ���� �м��մϴ�:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
        EndPaint(hWnd, &ps);

    }
    break;

    // ���� üũ�϶�� �޼��� 
    case WM_CHECKBINGO: //WM_USER + 10
        TCHAR str[100];
        // FALSE == X , TRUE == O �̴�. 
        if (lParam == FALSE)
        {
            wsprintf(str, _T("�ڽ�, %d�� X"), wParam);
        }
        else 
        {
            wsprintf(str, _T("�ڽ�, %d�� O"), wParam);
        }
        SetWindowText(hWnd, str);
        break;


    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK BingoProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static BOOL bEllipse = FALSE;
    switch (message)
    {

    case WM_CREATE:
        // FALSE TRUE �����ϴ� ���� �̸� "���׶��"
        SetProp(hWnd, __T("���׶��"), FALSE);
        break;
    case WM_LBUTTONDOWN:
    {

        //SendMessage(hWnd, WM_CLOSE, 0, 0);
        //if (bEllipse)
        //    bEllipse = FALSE;
        //else
        //    bEllipse = TRUE
       
        BOOL mode = (BOOL)GetProp(hWnd, __T("���׶��"));
        if (mode) mode = FALSE;
        else
            mode = TRUE;
        SetProp(hWnd, __T("���׶��"), (HANDLE)mode);

        SendMessage(GetParent(hWnd), WM_CHECKBINGO, (WPARAM)hWnd, mode);

        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
        // Rectangle(hdc, 0, 0, 100, 100);
        BOOL mode = (BOOL)GetProp(hWnd, __T("���׶��"));
        if (mode)
        {
            Ellipse(hdc, 10, 10, 90, 90);
        }
        else 
        {
            MoveToEx(hdc, 10, 10, NULL); LineTo(hdc, 90, 90);
            MoveToEx(hdc, 90, 10, NULL); LineTo(hdc, 10, 90);
        }

        TextOut(hdc, 10, 10, _T("����"), 2);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
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

