#include "general.h"

#define UNICODE
#define _UNICODE

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

#if COMPILER_CL
#pragma comment(lib, "Gdi32.lib")
#endif

#define GL_PROC(ident) CONCAT(ident,PROC)


typedef unsigned int  GLenum;
typedef unsigned char GLubyte;
typedef unsigned int  GLbitfield;
typedef int           GLint;
typedef int           GLsizei;
typedef float         GLfloat;
typedef float         GLclampf;


#define GL_BLEND      0x0BE2
#define GL_DEPTH_TEST 0x0B71


/* BlendingFactorDest */
#define GL_ZERO                0
#define GL_ONE                 1
#define GL_SRC_COLOR           0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA           0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA           0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305

/* BlendingFactorSrc */
#define GL_DST_COLOR           0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE  0x0308


#define GL_VENDOR     0x1F00
#define GL_RENDERER   0x1F01
#define GL_VERSION    0x1F02
#define GL_EXTENSIONS 0x1F03


#define GL_TRIANGLES 0x0004


#define GL_COLOR_BUFFER_BIT 0x00004000


typedef const GLubyte * APIENTRY GL_PROC(glGetString) (GLenum name);
typedef void APIENTRY GL_PROC(glClear) (GLbitfield mask);
typedef void APIENTRY GL_PROC(glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void APIENTRY GL_PROC(glBlendFunc) (GLenum sfactor, GLenum dfactor);
typedef void APIENTRY GL_PROC(glDisable) (GLenum cap);
typedef void APIENTRY GL_PROC(glEnable) (GLenum cap);
typedef void APIENTRY GL_PROC(glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

GL_PROC(glGetString)  *glGetString;
GL_PROC(glClear)      *glClear;
GL_PROC(glClearColor) *glClearColor;
GL_PROC(glBlendFunc)  *glBlendFunc;
GL_PROC(glDisable)    *glDisable;
GL_PROC(glEnable)     *glEnable;
GL_PROC(glViewport)   *glViewport;


/***************** Legacy functions *********************/
typedef void APIENTRY GL_PROC(glBegin) (GLenum mode);
typedef void APIENTRY GL_PROC(glEnd) (void);
typedef void APIENTRY GL_PROC(glVertex3f) (GLfloat x, GLfloat y, GLfloat z);
typedef void APIENTRY GL_PROC(glColor3f) (GLfloat red, GLfloat green, GLfloat blue);
typedef void APIENTRY GL_PROC(glColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);

GL_PROC(glBegin)      *glBegin;
GL_PROC(glEnd)        *glEnd;
GL_PROC(glVertex3f)   *glVertex3f;
GL_PROC(glColor3f)    *glColor3f;
GL_PROC(glColor4f)    *glColor4f;
/***************** Legacy functions *********************/


// WGL.
typedef HGLRC WINAPI GL_PROC(wglCreateContext)(HDC);
typedef BOOL  WINAPI GL_PROC(wglDeleteContext)(HGLRC);
typedef BOOL  WINAPI GL_PROC(wglMakeCurrent)(HDC, HGLRC);

static GL_PROC(wglCreateContext) *W32_wglCreateContext;
static GL_PROC(wglDeleteContext) *W32_wglDeleteContext;
static GL_PROC(wglMakeCurrent)   *W32_wglMakeCurrent;


#define W32_LOAD_GL_1_1_PROC(ident) ident = (GL_PROC(ident) *)GetProcAddress(gl_module, #ident)
#define W32_LOAD_WGL_PROC(ident) CONCAT(W32_, ident) = (GL_PROC(ident) *)GetProcAddress(gl_module, #ident)


HGLRC gl_context;

void gl_load(void) {
    HMODULE gl_module = LoadLibraryA("OpenGL32.dll");
    if (!gl_module) {
        write_string("Failed to load Opengl32.\n", true);
        return;
    }

    W32_LOAD_WGL_PROC(wglCreateContext);
    W32_LOAD_WGL_PROC(wglDeleteContext);
    W32_LOAD_WGL_PROC(wglMakeCurrent);


    W32_LOAD_GL_1_1_PROC(glGetString);
    W32_LOAD_GL_1_1_PROC(glClear);
    W32_LOAD_GL_1_1_PROC(glClearColor);
    W32_LOAD_GL_1_1_PROC(glBlendFunc);
    W32_LOAD_GL_1_1_PROC(glDisable);
    W32_LOAD_GL_1_1_PROC(glEnable);
    W32_LOAD_GL_1_1_PROC(glViewport);


    W32_LOAD_GL_1_1_PROC(glBegin);
    W32_LOAD_GL_1_1_PROC(glEnd);
    W32_LOAD_GL_1_1_PROC(glVertex3f);
    W32_LOAD_GL_1_1_PROC(glColor3f);
    W32_LOAD_GL_1_1_PROC(glColor4f);
}

bool set_pixel_format(HWND window) {
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

    ReleaseDC(window, hdc);

    return true;
}

bool opengl_init(HWND window) {
    set_pixel_format(window);

    HDC hdc = GetDC(window);

    if (!gl_context) {
        gl_load();

        gl_context = W32_wglCreateContext(hdc);
        if (gl_context == null) {
            write_string("Failed to wglCreateContext.\n", true);
            return false;
        }
    }

    if (!W32_wglMakeCurrent(hdc, gl_context)) {
        write_string("Failed to wglMakeCurrent.\n", true);
        return false;
    }

    ReleaseDC(window, hdc);

    return true;
}

bool should_quit = false;

static LRESULT CALLBACK win32_main_window_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_ERASEBKGND:
            return 1;

        case WM_CLOSE:
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
    // wc.hbrBackground        = (HBRUSH)GetStockObject(WHITE_BRUSH);
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

    const GLubyte *gl_version  = glGetString(GL_VERSION);
    const GLubyte *gl_vendor   = glGetString(GL_VENDOR);
    const GLubyte *gl_renderer = glGetString(GL_RENDERER);
    
    print("OpenGL Version:  %s\n", gl_version);
    print("OpenGL Vendor:   %s\n", gl_vendor);
    print("OpenGL Renderer: %s\n", gl_renderer);

    // Display the window.
    UpdateWindow(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    HDC hdc = GetDC(hwnd);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    RECT client_rect;
    GetClientRect(hwnd, &client_rect);
    int width  = client_rect.right  - client_rect.left;
    int height = client_rect.bottom - client_rect.top;

    glViewport(0, 0, width, height);

    while (!should_quit) {
        MSG msg;
        while (PeekMessageW(&msg, null, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        // glClearColor(0.2f, 0.72f, 0.38f, 1);
        glClearColor(0.2f, 0.38f, 0.72f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);

        glColor4f(1.0f, 0, 0, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0);

        glColor4f(0, 1.0f, 0, 0.5f);
        glVertex3f(0.5f, -0.5f, 0);
        
        glColor4f(0, 0, 1.0f, 0.5f);
        glVertex3f(0, 0.5f,  0);

        glEnd();

        BOOL ok = SwapBuffers(hdc);
        if (ok == FALSE) {
            write_string("Failed to SwapBuffers.\n", true);
        }
    }

    ReleaseDC(hwnd, hdc);
    W32_wglMakeCurrent(null, null);
    W32_wglDeleteContext(gl_context);

    return 0;
}






















#define GENERAL_IMPLEMENTATION
#include "general.h"