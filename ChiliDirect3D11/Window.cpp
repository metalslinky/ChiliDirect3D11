#include "Window.h"

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
    : hInstance( GetModuleHandle( nullptr ) ) {
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof( wc );
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = nullptr;
    RegisterClassEx( &wc );
}

Window::WindowClass::~WindowClass() {
    UnregisterClass( wndClassName, GetInstance() );
}

const char* Window::WindowClass::GetName() noexcept {
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
    return wndClass.hInstance;
}

Window::Window( int width, int height, const char* name ) noexcept {
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    AdjustWindowRect( &wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE );
    hWnd = CreateWindow(
        WindowClass::GetName(),
        name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        wr.right - wr.left,
        wr.bottom - wr.top,
        nullptr,
        nullptr,
        WindowClass::GetInstance(),
        this
    );
    ShowWindow( hWnd, SW_SHOWDEFAULT );
}

Window::~Window() {
    DestroyWindow( hWnd );
}

LRESULT CALLBACK Window::HandleMsgSetup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept {
    if ( msg == WM_NCCREATE ) {
        const CREATESTRUCTW* const CREATE = reinterpret_cast< CREATESTRUCTW* > ( lParam );
        Window* const wnd = static_cast< Window* >( CREATE->lpCreateParams );
        SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( wnd ) );
        SetWindowLongPtr( hWnd, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( &Window::HandleMsgThunk ) );
        return wnd->HandleMsg( hWnd, msg, wParam, lParam );
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}
LRESULT CALLBACK Window::HandleMsgThunk( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept {
    Window* const wnd = reinterpret_cast< Window* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
    return wnd->HandleMsg( hWnd, msg, wParam, lParam );
}

LRESULT Window::HandleMsg( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept {
    switch ( msg ) {
    case WM_CLOSE:
        PostQuitMessage( 0 );
        return 0;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}