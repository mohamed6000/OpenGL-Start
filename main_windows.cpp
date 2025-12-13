#include "general.h"

#define UNICODE
#define _UNICODE

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

#if COMPILER_CL
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Opengl32.lib") // @Todo: Should I load the gl functions dynamically?
#endif

#include <GL/GL.h>

bool opengl_init(HWND window) {
    HDC hdc = GetDC(window);

    PIXELFORMATDESCRIPTOR pixel_format = {};
    pixel_format.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
    pixel_format.nVersion     = 1;
    pixel_format.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixel_format.iPixelType   = PFD_TYPE_RGBA;
    pixel_format.cColorBits   = 24;
    pixel_format.cAlphaBits   = 8;
    pixel_format.cDepthBits   = 24;
    pixel_format.cStencilBits = 8;
    pixel_format.iLayerType   = PFD_MAIN_PLANE;

    int pixel_format_index = ChoosePixelFormat(hdc, &pixel_format);
    if (!pixel_format_index) {
        write_string("Failed to ChoosePixelFormat.\n", true);
        return false;
    }

    BOOL success = SetPixelFormat(hdc, pixel_format_index, &pixel_format);
    if (success == FALSE) {
        write_string("Failed to SetPixelFormat.\n", true);
        return false;
    }

    HGLRC gl_context = wglCreateContext(hdc);
    if (gl_context == null) {
        write_string("Failed to wglCreateContext.\n", true);
        return false;
    }

    if (!wglMakeCurrent(hdc, gl_context)) {
        write_string("Failed to wglMakeCurrent.\n", true);
        return false;
    }

    ReleaseDC(window, hdc);

    return true;
}

bool should_quit = false;

static LRESULT CALLBACK win32_main_window_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_CLOSE:
        case WM_QUIT:
            should_quit = true;
            break;

        default:
            return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    return 0;
}

int main(void) {
    HINSTANCE hInstance = GetModuleHandleW(null);

    WNDCLASSEXW wc = {};
    wc.cbSize = size_of(WNDCLASSEXW);
    wc.style                = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.cbClsExtra           = 0;
    wc.cbWndExtra           = 0;
    wc.hInstance            = hInstance;
    wc.hCursor              = LoadCursorW(null, IDC_ARROW);
    wc.hbrBackground        = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName         = null;
    wc.lpszClassName        = L"WindowClassName";

    wc.lpfnWndProc          = win32_main_window_callback;


    // Register the window class.
    if (RegisterClassExW(&wc) == 0) {
        write_string("RegisterClassExW returned 0.\n", true);
        return 0;
    }

    HWND hwnd = CreateWindowExW(0,
                                L"WindowClassName",
                                L"OpenGL",
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                null,
                                null,
                                (HINSTANCE)0,
                                null);

    if (hwnd == null) {
        write_string("CreateWindowExW returned 0.\n", true);
        return 0;
    }

    opengl_init(hwnd);

    // Display the window.
    UpdateWindow(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    HDC hdc = GetDC(hwnd);

    while (!should_quit) {
        MSG msg;
        while (PeekMessageW(&msg, null, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.38f, 0.72f, 1);

        BOOL ok = SwapBuffers(hdc);
        if (ok == FALSE) {
            write_string("Failed to SwapBuffers.\n", true);
        }
    }

    ReleaseDC(hwnd, hdc);
    wglMakeCurrent(null, null);

    return 0;
}






















#define GENERAL_IMPLEMENTATION
#include "general.h"