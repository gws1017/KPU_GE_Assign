﻿#include "stdafx.h"
#include "LabProject07.h"
#include "GameFramework.h"

#define MAX_LOADSTRING 100

CGameFramework gGameFramework;


HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LABPROJECT07, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);


    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABPROJECT07));

    MSG msg;
    while (true) {
        if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
        }
        else {
            gGameFramework.FrameAdvance();
        }
    }
    gGameFramework.OnDestroy();

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABPROJECT07));
    wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    //주 윈도우의 메뉴가 나타나지 않도록 한다. 
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = ::LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));


    return ::RegisterClassEx(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {

    RECT rc = { 0, 0, FRAME_BUFFER_WIDTH, FRAME_BUFFER_HEIGHT };
    DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_BORDER;
    AdjustWindowRect(&rc, dwStyle, FALSE);

    HWND hMainWnd = CreateWindow(szWindowClass, szTitle, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);

    if (!hMainWnd)
        return(FALSE);

    gGameFramework.OnCreate(hInstance, hMainWnd);

    ::ShowWindow(hMainWnd, nCmdShow);
    ::UpdateWindow(hMainWnd);

#ifdef _WITH_SWAPCHAIN_FULLSCREEN_STATE
    gGameFramework.ChangeSwapChainState();
#endif

    return(TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_SIZE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MOUSEMOVE:
    case WM_KEYDOWN:
    case WM_KEYUP:
        gGameFramework.OnProcessingWindowMessage(hWnd, message, wParam, lParam);
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        break;
    default:
        return(::DefWindowProc(hWnd, message, wParam, lParam));
    }

    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);

    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }

    return (INT_PTR)FALSE;
}
