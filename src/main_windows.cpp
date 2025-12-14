#include "general.h"

#define UNICODE
#define _UNICODE

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>

#include "stb_image.h"


#if COMPILER_CL
#pragma comment(lib, "Gdi32.lib")
#endif

#define GL_PROC(ident) CONCAT(ident,PROC)


typedef unsigned int  GLenum;
typedef unsigned int  GLbitfield;
typedef int           GLint;
typedef int           GLsizei;
typedef unsigned char GLubyte;
typedef unsigned int  GLuint;
typedef float         GLfloat;
typedef float         GLclampf;
typedef double        GLclampd;
typedef void          GLvoid;


#define GL_BLEND      0x0BE2
#define GL_DEPTH_TEST 0x0B71
#define GL_TEXTURE_2D 0x0DE1

/* AlphaFunction */
#define GL_NEVER    0x0200
#define GL_LESS     0x0201
#define GL_EQUAL    0x0202
#define GL_LEQUAL   0x0203
#define GL_GREATER  0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL   0x0206
#define GL_ALWAYS   0x0207

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

/* MatrixMode */
#define GL_MODELVIEW  0x1700
#define GL_PROJECTION 0x1701

/* PixelFormat */
#define GL_RGB  0x1907
#define GL_RGBA 0x1908

/* texture */
#define GL_RGB8  0x8051
#define GL_RGBA8 0x8058

/* TextureEnvMode */
#define GL_MODULATE 0x2100
#define GL_DECAL    0x2101

/* TextureEnvParameter */
#define GL_TEXTURE_ENV_MODE  0x2200
#define GL_TEXTURE_ENV_COLOR 0x2201

/* TextureEnvTarget */
#define GL_TEXTURE_ENV 0x2300

/* TextureMagFilter */
#define GL_NEAREST 0x2600
#define GL_LINEAR  0x2601

/* TextureMinFilter */
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST  0x2701
#define GL_NEAREST_MIPMAP_LINEAR  0x2702
#define GL_LINEAR_MIPMAP_LINEAR   0x2703

/* TextureParameterName */
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S     0x2802
#define GL_TEXTURE_WRAP_T     0x2803

/* DataType */
#define GL_UNSIGNED_BYTE 0x1401


#define GL_VENDOR     0x1F00
#define GL_RENDERER   0x1F01
#define GL_VERSION    0x1F02
#define GL_EXTENSIONS 0x1F03


#define GL_TRIANGLES 0x0004

#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_COLOR_BUFFER_BIT 0x00004000


typedef const GLubyte * APIENTRY GL_PROC(glGetString) (GLenum name);
typedef void APIENTRY GL_PROC(glClear) (GLbitfield mask);
typedef void APIENTRY GL_PROC(glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void APIENTRY GL_PROC(glBlendFunc) (GLenum sfactor, GLenum dfactor);
typedef void APIENTRY GL_PROC(glDisable) (GLenum cap);
typedef void APIENTRY GL_PROC(glEnable) (GLenum cap);
typedef void APIENTRY GL_PROC(glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void APIENTRY GL_PROC(glGenTextures) (GLsizei n, GLuint *textures);
typedef void APIENTRY GL_PROC(glBindTexture) (GLenum target, GLuint texture);
typedef void APIENTRY GL_PROC(glTexImage2D) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void APIENTRY GL_PROC(glTexParameteri) (GLenum target, GLenum pname, GLint param);
typedef void APIENTRY GL_PROC(glDepthFunc) (GLenum func);
typedef void APIENTRY GL_PROC(glClearDepth) (GLclampd depth);

GL_PROC(glGetString)     *glGetString;
GL_PROC(glClear)         *glClear;
GL_PROC(glClearColor)    *glClearColor;
GL_PROC(glBlendFunc)     *glBlendFunc;
GL_PROC(glDisable)       *glDisable;
GL_PROC(glEnable)        *glEnable;
GL_PROC(glViewport)      *glViewport;
GL_PROC(glGenTextures)   *glGenTextures;
GL_PROC(glBindTexture)   *glBindTexture;
GL_PROC(glTexImage2D)    *glTexImage2D;
GL_PROC(glTexParameteri) *glTexParameteri;
GL_PROC(glDepthFunc)     *glDepthFunc;
GL_PROC(glClearDepth)    *glClearDepth;


/***************** Legacy functions *********************/
typedef void APIENTRY GL_PROC(glBegin) (GLenum mode);
typedef void APIENTRY GL_PROC(glEnd) (void);
typedef void APIENTRY GL_PROC(glVertex3f) (GLfloat x, GLfloat y, GLfloat z);
typedef void APIENTRY GL_PROC(glColor3f) (GLfloat red, GLfloat green, GLfloat blue);
typedef void APIENTRY GL_PROC(glColor4f) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void APIENTRY GL_PROC(glMatrixMode) (GLenum mode);
typedef void APIENTRY GL_PROC(glLoadMatrixf) (const GLfloat *m);
typedef void APIENTRY GL_PROC(glTexEnvf) (GLenum target, GLenum pname, GLfloat param);
typedef void APIENTRY GL_PROC(glTexCoord2f) (GLfloat s, GLfloat t);

GL_PROC(glBegin)       *glBegin;
GL_PROC(glEnd)         *glEnd;
GL_PROC(glVertex3f)    *glVertex3f;
GL_PROC(glColor3f)     *glColor3f;
GL_PROC(glColor4f)     *glColor4f;
GL_PROC(glMatrixMode)  *glMatrixMode;
GL_PROC(glLoadMatrixf) *glLoadMatrixf;
GL_PROC(glTexEnvf)     *glTexEnvf;
GL_PROC(glTexCoord2f)  *glTexCoord2f;
/***************** Legacy functions *********************/


// WGL.
typedef HGLRC WINAPI GL_PROC(wglCreateContext)(HDC);
typedef BOOL  WINAPI GL_PROC(wglDeleteContext)(HGLRC);
typedef BOOL  WINAPI GL_PROC(wglMakeCurrent)(HDC, HGLRC);

static GL_PROC(wglCreateContext) *W32_wglCreateContext;
static GL_PROC(wglDeleteContext) *W32_wglDeleteContext;
static GL_PROC(wglMakeCurrent)   *W32_wglMakeCurrent;


#define W32_LOAD_GL_1_1_PROC(ident) ident = (GL_PROC(ident) *)GetProcAddress(gl_module, #ident)
#define W32_LOAD_WGL_PROC(ident) CONCAT(W32_,ident) = (GL_PROC(ident) *)GetProcAddress(gl_module, #ident)


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
    W32_LOAD_GL_1_1_PROC(glGenTextures);
    W32_LOAD_GL_1_1_PROC(glBindTexture);
    W32_LOAD_GL_1_1_PROC(glTexImage2D);
    W32_LOAD_GL_1_1_PROC(glTexParameteri);
    W32_LOAD_GL_1_1_PROC(glDepthFunc);
    W32_LOAD_GL_1_1_PROC(glClearDepth);


    W32_LOAD_GL_1_1_PROC(glBegin);
    W32_LOAD_GL_1_1_PROC(glEnd);
    W32_LOAD_GL_1_1_PROC(glVertex3f);
    W32_LOAD_GL_1_1_PROC(glColor3f);
    W32_LOAD_GL_1_1_PROC(glColor4f);
    W32_LOAD_GL_1_1_PROC(glMatrixMode);
    W32_LOAD_GL_1_1_PROC(glLoadMatrixf);
    W32_LOAD_GL_1_1_PROC(glTexEnvf);
    W32_LOAD_GL_1_1_PROC(glTexCoord2f);
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
int back_buffer_width;
int back_buffer_height;

GLuint last_bound_texture_id;

struct Vector3 {
    float x, y, z;
};

struct Vector4 {
    float x, y, z, w;
};

struct Texture {
    GLuint id;
    int width;
    int height;
    int channels;
};

void render_update_texture(Texture *texture, unsigned char *data) {
    if (!texture->id) {
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &texture->id);
    }

    if (!texture->id) return;

    // https://learn.microsoft.com/en-us/windows/win32/opengl/glteximage2d
    GLenum gl_source_format = GL_RGBA8; // Texture format.
    GLenum gl_dest_format   = GL_RGBA;  // The format of the pixel data.
    if (texture->channels == 3) {
        gl_source_format = GL_RGB8;
        gl_dest_format   = GL_RGB;
    }

    // @Todo: Add more format.

    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, gl_source_format, 
                 texture->width, texture->height, 
                 0, gl_dest_format, GL_UNSIGNED_BYTE, 
                 data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture texture_load_from_file(const char *file_path) {
    Texture result = {};

    stbi_uc *data = stbi_load(file_path, &result.width, &result.height, &result.channels, 0);
    if (data) {
        render_update_texture(&result, data);

        stbi_image_free(data);
    }

    return result;
}

void set_texture(Texture *texture) {
    if (last_bound_texture_id != texture->id) {
        glBindTexture(GL_TEXTURE_2D, texture->id);
    }

    last_bound_texture_id = texture->id;
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

void rendering_2d(int w, int h) {
    glMatrixMode(GL_PROJECTION);

    float proj[16] = {
        2.0f/w,  0,       0,   0,
        0,       2.0f/h,  0,   0,
        0,       0,       1,   0,
       -1,      -1,       0,   1
    };
    glLoadMatrixf(proj);

    glMatrixMode(GL_MODELVIEW);
    float identity[16] = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    glLoadMatrixf(identity);
}

void draw_quad(float x0, float y0, float x1, float y1, Vector4 c) {
    glBegin(GL_TRIANGLES);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(0, 1);
    glVertex3f(x0, y0, 0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(1, 1);
    glVertex3f(x1, y0, 0);
    
    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(1, 0);
    glVertex3f(x1, y1,  0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(0, 1);
    glVertex3f(x0, y0, 0);
    
    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(1, 0);
    glVertex3f(x1, y1,  0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(0, 0);
    glVertex3f(x0, y1,  0);

    glEnd();
}

void draw_quad(float x0, float y0, float x1, float y1, 
               float u0, float v0, float u1, float v1,
               Vector4 c) {
    glBegin(GL_TRIANGLES);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u0, v1);
    glVertex3f(x0, y0, 0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u1, v1);
    glVertex3f(x1, y0, 0);
    
    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u1, v0);
    glVertex3f(x1, y1,  0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u0, v1);
    glVertex3f(x0, y0, 0);
    
    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u1, v0);
    glVertex3f(x1, y1,  0);

    glColor4f(c.x, c.y, c.z, c.w);
    glTexCoord2f(u0, v0);
    glVertex3f(x0, y1,  0);

    glEnd();
}

void draw_quad(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
               Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3) {
    glBegin(GL_TRIANGLES);

    glColor4f(c0.x, c0.y, c0.z, c0.w);
    glTexCoord2f(0, 1);
    glVertex3f(p0.x, p0.y, p0.z);

    glColor4f(c1.x, c1.y, c1.z, c1.w);
    glTexCoord2f(1, 1);
    glVertex3f(p1.x, p1.y, p1.z);
    
    glColor4f(c2.x, c2.y, c2.z, c2.w);
    glTexCoord2f(1, 0);
    glVertex3f(p2.x, p2.y, p2.z);

    glColor4f(c0.x, c0.y, c0.z, c0.w);
    glTexCoord2f(0, 1);
    glVertex3f(p0.x, p0.y, p0.z);

    glColor4f(c2.x, c2.y, c2.z, c2.w);
    glTexCoord2f(1, 0);
    glVertex3f(p2.x, p2.y, p2.z);

    glColor4f(c3.x, c3.y, c3.z, c3.w);
    glTexCoord2f(0, 0);
    glVertex3f(p3.x, p3.y, p3.z);

    glEnd();
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

    Texture test = texture_load_from_file("data/textures/Texturtest planar.png");
    Texture cat  = texture_load_from_file("data/textures/cat.png");

    // Display the window.
    UpdateWindow(hwnd);
    ShowWindow(hwnd, SW_SHOW);

    HDC hdc = GetDC(hwnd);

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

    while (!should_quit) {
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

        draw_quad(500, 300, 650, 400, 
                  0, 0, 1, 0.5f,
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
        x0 = 400;
        y0 = 300;
        x1 = 500;
        y1 = 400;
        draw_quad(Vector3{x0,y0,depth}, Vector3{x1,y0,depth}, Vector3{x1,y1,depth}, Vector3{x0,y1,depth},
                  Vector4{1,1,1,1},   Vector4{1,1,1,1},   Vector4{1,1,1,1},   Vector4{1,1,1,1});

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