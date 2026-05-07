#include <windows.h>
#include <string>

#define ID_number0 1001
#define ID_number1 1002
#define ID_number2 1003
#define ID_number3 1004
#define ID_number4 1005
#define ID_number5 1006
#define ID_number6 1007
#define ID_number7 1008
#define ID_number8 1009
#define ID_number9 1010
#define ID_number10 1011
#define ID_number11 1012
#define ID_number12 1013
#define ID_number13 1014
#define ID_number14 1015
#define ID_number15 1016
#define ID_number16 1017
#define ID_number17 1018
#define ID_number18 1019
#define ID_number19 1020
#define ID_number20 1021
#define ID_number21 1022
#define ID_number22 1023
#define ID_number23 1024
#define ID_number24 1025
#define ID_number25 1026
#define ID_number26 1027
#define ID_number27 1028
#define ID_number28 1029
#define ID_number29 1030
#define ID_number30 1031
#define ID_number31 1032
#define ID_number32 1033
#define ID_number33 1034
#define ID_number34 1035
#define ID_number35 1036

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = "FormClass";
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowA(
        "FormClass", "Untitled",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL, NULL, hInst, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
