#include "general.h"

#define UNICODE
#define _UNICODE

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>
#include <math.h>

#include "stb_image.h"

#define GL_IMPLEMENTATION
#include "GL.h"


#include "framework.h"


extern "C" {
    // https://docs.nvidia.com/gameworks/content/technologies/desktop/optimus.htm
    // SHARED_EXPORT DWORD NvOptimusEnablement = 0x00000001;

    /* 
        This link https://gpuopen.com/learn/amdpowerxpressrequesthighperformance/
        defines the constant as a DWORD, but every code I saw defines it as an int.
    */
    // SHARED_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}

#if COMPILER_CL
#pragma comment(lib, "Gdi32.lib")
#endif


HGLRC gl_context;

bool opengl_init(HWND window) {
    // Create dummy window.
    HINSTANCE hInstance = GetModuleHandleW(null);

    WNDCLASSEXW wcex = {};
    wcex.cbSize = size_of(WNDCLASSEXW);
    wcex.hInstance     = hInstance;
    wcex.lpszClassName = L"Dummy Window";
    wcex.lpfnWndProc   = DefWindowProcW;

    // Register the window class.
    if (RegisterClassExW(&wcex) == 0) {
        write_string("RegisterClassExW returned 0.\n", true);
        return false;
    }

    HWND hwnd = CreateWindowExW(0,
                                L"Dummy Window",
                                L"Dummy Window",
                                0,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                null,
                                null,
                                hInstance,
                                null);

    if (!hwnd) {
        write_string("CreateWindowExW returned 0.\n", true);
        return false;
    }

    HDC dummy_dc = GetDC(hwnd);

    {
        PIXELFORMATDESCRIPTOR pixel_format = {};
        pixel_format.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
        pixel_format.nVersion     = 1;
        pixel_format.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pixel_format.iPixelType   = PFD_TYPE_RGBA;
        pixel_format.cColorBits   = 32;
        pixel_format.cDepthBits   = 24;
        pixel_format.cStencilBits = 8;
        pixel_format.iLayerType   = PFD_MAIN_PLANE;

        int pixel_format_index = ChoosePixelFormat(dummy_dc, &pixel_format);
        if (!pixel_format_index) {
            write_string("Failed to ChoosePixelFormat.\n", true);
            return false;
        }

        BOOL success = SetPixelFormat(dummy_dc, pixel_format_index, &pixel_format);
        if (success == FALSE) {
            write_string("Failed to SetPixelFormat.\n", true);
            return false;
        }
    }

    if (!gl_load()) return false;

    // Get dummy GL context.
    HGLRC hglrc = W32_wglCreateContext(dummy_dc);
    if (!hglrc) {
        write_string("Failed to wglCreateContext.\n", true);
        return false;
    }

    if (!W32_wglMakeCurrent(dummy_dc, hglrc)) {
        write_string("Failed to wglMakeCurrent.\n", true);
        return false;
    }

    // Load WGL functions using wglGetProcAddress.
    GL_LOAD_PROC(wglCreateContextAttribsARB);
    GL_LOAD_PROC(wglChoosePixelFormatARB);

    W32_wglDeleteContext(hglrc);
    ReleaseDC(hwnd, dummy_dc);
    DestroyWindow(hwnd);
    UnregisterClassW(L"Dummy Window", hInstance);


    // Create modern GL context.
    int pixel_attrib_list[] = {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
        WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,

        // WGL_COLOR_BITS_ARB,     32,
        // WGL_DEPTH_BITS_ARB,     24,
        // WGL_STENCIL_BITS_ARB,   8,
        // WGL_AUX_BUFFERS_ARB,    1,

        0,  // Terminator.
    };

    HDC hdc = GetDC(window);

    int pixel_format_index;
    UINT extended_pick;
    BOOL success = wglChoosePixelFormatARB(hdc, pixel_attrib_list, null, 1, 
        &pixel_format_index, &extended_pick);
    if ((success == FALSE) || (extended_pick == 0)) {
        write_string("Failed to wglChoosePixelFormatARB.\n", true);
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd = {};
    if (!DescribePixelFormat(hdc, pixel_format_index, 
                             size_of(pfd), &pfd)) {
        write_string("Failed to DescribePixelFormat.\n", true);
        return false;
    }

    if (!SetPixelFormat(hdc, pixel_format_index, &pfd)) {
        write_string("Failed to SetPixelFormat.\n", true);
        return false;
    }

    int gl_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
#if GL_USE_LEGACY_PROCS
        // WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
#else
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
#endif
        0,
    };

    gl_context = wglCreateContextAttribsARB(hdc, null, gl_attribs);
    if (!gl_context) {
        write_string("Failed to wglCreateContextAttribsARB.\n", true);
        return false;
    }

    if (!W32_wglMakeCurrent(hdc, gl_context)) {
        write_string("Failed to wglMakeCurrent.\n", true);
        return false;
    }

    // Load modern GL functions...
    gl_load_extensions();
    
    return true;
}

static LRESULT CALLBACK win32_main_window_callback(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    switch (msg) {
        case WM_ERASEBKGND:
            return 1;

        case WM_CLOSE:
            should_quit = true;
            break;

        case WM_SIZE:
            back_buffer_width  = LOWORD(lparam);
            back_buffer_height = HIWORD(lparam);
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
                                hInstance,
                                null);

    if (hwnd == null) {
        write_string("CreateWindowExW returned 0.\n", true);
        return 0;
    }

    opengl_init(hwnd);

    const GLubyte *gl_version  = glGetString(GL_VERSION);
    const GLubyte *gl_vendor   = glGetString(GL_VENDOR);
    const GLubyte *gl_renderer = glGetString(GL_RENDERER);
    const GLubyte *gl_shader_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
    
    print("OpenGL Version:  %s\n", gl_version);
    print("OpenGL Vendor:   %s\n", gl_vendor);
    print("OpenGL Renderer: %s\n", gl_renderer);
    print("OpenGL Shading Language Version: %s\n", gl_shader_version);

    Texture test = texture_load_from_file("data/textures/Texturtest planar.png");
    Texture cat  = texture_load_from_file("data/textures/cat.png");

    // Display the window.
    UpdateWindow(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    HDC hdc = GetDC(hwnd);


    init_framework();


    // glDisable(GL_DEPTH_TEST);
    // Depth is mapped as near=-1 and far 1.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    RECT client_rect;
    GetClientRect(hwnd, &client_rect);

    back_buffer_width  = client_rect.right  - client_rect.left;
    back_buffer_height = client_rect.bottom - client_rect.top;

    Vector2 cat_pos0 = {400, 300};
    Vector2 cat_pos1 = {500, 400};
    float cat_rot_angle = 0;

    Vector3 red_cat_p0 = {500, 300, 0};
    Vector3 red_cat_p1 = {650, 300, 0};
    Vector3 red_cat_p2 = {650, 400, 0};
    Vector3 red_cat_p3 = {500, 400, 0};

    float64 one_over_frequency = 1.0;
    LARGE_INTEGER large_frequency;
    if (QueryPerformanceFrequency(&large_frequency)) {
        one_over_frequency = 1.0 / (float64)large_frequency.QuadPart;
    }

    LARGE_INTEGER last_counter = {};
    QueryPerformanceCounter(&last_counter);

    while (!should_quit) {
        LARGE_INTEGER wall_counter;
        QueryPerformanceCounter(&wall_counter);
        float current_dt = (float)((wall_counter.QuadPart - last_counter.QuadPart) * one_over_frequency);
        last_counter = wall_counter;

        MSG msg;
        while (PeekMessageW(&msg, null, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        glViewport(0, 0, back_buffer_width, back_buffer_height);
        
        rendering_2d(back_buffer_width, back_buffer_height);

        // glClearColor(0.2f, 0.72f, 0.38f, 1);
        glClearColor(0.2f, 0.38f, 0.72f, 1);
        glClearDepth(1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        set_texture(&test);

        draw_quad(10, 10, 100, 100, Vector4{1,1,1,1});

        draw_quad(Vector3{100,100,0}, Vector3{250,100,0}, Vector3{250,250,0}, Vector3{100,250,0},
                  Vector4{1,0,0,1},   Vector4{0,1,0,1},   Vector4{0,0,1,1},   Vector4{0,1,0,1});

        set_texture(&cat);

        Vector3 center = {
            (red_cat_p0.x+red_cat_p1.x)/2,
            0,
            (red_cat_p0.z+red_cat_p3.z)/2,
        };

        draw_quad(red_cat_p0, 
                  red_cat_p1, 
                  red_cat_p2, 
                  red_cat_p3,
                  Vector2{0,0}, 
                  Vector2{1,0},
                  Vector2{1,0.5f},
                  Vector2{0,0.5f},
                  Vector4{1,0,0,1});

        float depth,x0,y0,x1,y1;

        set_texture(&test);
        depth = 0;
        x0 = 200;
        y0 = 200;
        x1 = 500;
        y1 = 500;
        draw_quad(Vector3{x0,y0,depth}, Vector3{x1,y0,depth}, Vector3{x1,y1,depth}, Vector3{x0,y1,depth},
                  Vector4{1,0,0,1},   Vector4{0,1,0,1},   Vector4{0,0,1,1},   Vector4{0,1,0,1});

        set_texture(&cat);
        depth = -0.2f;

        Vector2 center_2d = {
            (cat_pos0.x+cat_pos1.x)/2,
            (cat_pos0.y+cat_pos1.y)/2,
        };

        cat_rot_angle += current_dt * (float)TAU;
        if (cat_rot_angle >= 360.0f) cat_rot_angle = 0;

        Vector2 p0 = rotate_z(Vector2{cat_pos0.x,cat_pos0.y}, center_2d, cat_rot_angle);
        Vector2 p1 = rotate_z(Vector2{cat_pos1.x,cat_pos0.y}, center_2d, cat_rot_angle);
        Vector2 p2 = rotate_z(Vector2{cat_pos1.x,cat_pos1.y}, center_2d, cat_rot_angle);
        Vector2 p3 = rotate_z(Vector2{cat_pos0.x,cat_pos1.y}, center_2d, cat_rot_angle);

        draw_quad(Vector3{p0.x, p0.y ,depth},
                  Vector3{p1.x, p1.y ,depth}, 
                  Vector3{p2.x, p2.y ,depth}, 
                  Vector3{p3.x, p3.y ,depth}, 
                  Vector4{1,1,1,1},
                  Vector4{1,1,1,1},
                  Vector4{1,1,1,1},
                  Vector4{1,1,1,1});


        frame_flush();

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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "framework.cpp"
