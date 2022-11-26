#include <Windows.h>

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow ) {

    const auto CLASS_NAME = "Direct3D11 practice project";
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof( wc );
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = DefWindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm = nullptr;
    RegisterClassEx( &wc );

    HWND hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "This is a lovely window",
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
        200,
        200,
        640,
        480,
        nullptr,
        nullptr,
        hInstance,
        nullptr );

    ShowWindow( hWnd, SW_SHOW );
    while ( true );

    return 0;
}