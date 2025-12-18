#include "general.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <dlfcn.h>

#include "stb_image.h"


// #include <GL/glx.h>
#include <GL/gl.h>

typedef struct __GLXcontextRec *GLXContext;
typedef XID GLXDrawable;

/*
 * Tokens for glXChooseVisual and glXGetConfig:
 */
#define GLX_USE_GL           1
#define GLX_BUFFER_SIZE      2
#define GLX_LEVEL            3
#define GLX_RGBA             4
#define GLX_DOUBLEBUFFER     5
#define GLX_STEREO           6
#define GLX_AUX_BUFFERS      7
#define GLX_RED_SIZE         8
#define GLX_GREEN_SIZE       9
#define GLX_BLUE_SIZE        10
#define GLX_ALPHA_SIZE       11
#define GLX_DEPTH_SIZE       12
#define GLX_STENCIL_SIZE     13
#define GLX_ACCUM_RED_SIZE   14
#define GLX_ACCUM_GREEN_SIZE 15
#define GLX_ACCUM_BLUE_SIZE  16
#define GLX_ACCUM_ALPHA_SIZE 17

/*
 * GLX 1.4 and later:
 */
#define GLX_SAMPLE_BUFFERS 0x186a0 /*100000*/
#define GLX_SAMPLES        0x186a1 /*100001*/

#define GL_PROC(ident) CONCAT(ident,PROC)

typedef Bool GL_PROC(glXQueryVersion)( Display *dpy, int *maj, int *min );
typedef XVisualInfo* GL_PROC(glXChooseVisual)( Display *dpy, int screen, int *attribList );
typedef GLXContext GL_PROC(glXCreateContext)( Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct );
typedef void GL_PROC(glXDestroyContext)( Display *dpy, GLXContext ctx );
typedef Bool GL_PROC(glXMakeCurrent)( Display *dpy, GLXDrawable drawable, GLXContext ctx);
typedef void GL_PROC(glXSwapBuffers)( Display *dpy, GLXDrawable drawable );

static GL_PROC(glXQueryVersion)   *glXQueryVersion;
static GL_PROC(glXChooseVisual)   *glXChooseVisual;
static GL_PROC(glXCreateContext)  *glXCreateContext;
static GL_PROC(glXDestroyContext) *glXDestroyContext;
static GL_PROC(glXMakeCurrent)    *glXMakeCurrent;
static GL_PROC(glXSwapBuffers)    *glXSwapBuffers;

#define GLX_LOAD_PROC(ident) ident = (GL_PROC(ident) *)dlsym(glx_module, #ident)

void glx_load(void) {
    void *glx_module = dlopen("./libGLX.so", RTLD_LAZY);
    if (!glx_module) {
        glx_module = dlopen("libGLX.so", RTLD_LAZY);
    }

    if (!glx_module) {
        write_string("Failed to load GLX.\n", true);
        return;
    }

    GLX_LOAD_PROC(glXQueryVersion);
    GLX_LOAD_PROC(glXChooseVisual);
    GLX_LOAD_PROC(glXCreateContext);
    GLX_LOAD_PROC(glXDestroyContext);
    GLX_LOAD_PROC(glXMakeCurrent);
    GLX_LOAD_PROC(glXSwapBuffers);
}

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

GLuint last_bound_texture_id;

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

    // @Todo: Add more formats.

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
    glLoadIdentity();
}

bool should_quit = false;

int main(void) {
    Display *display = XOpenDisplay(null);
    if (!display) {
        write_string("Failed to XOpenDisplay.\n", true);
        return 0;
    }

    glx_load();

    // GLX version.
    GLint majorGLX, minorGLX;
    glXQueryVersion(display, &majorGLX, &minorGLX);
    if ((majorGLX == 1) && (minorGLX < 2)) {
        write_string("You need at least GLX 1.2.\n", true);
        return 0;
    }

    print("GLX Version: %d.%d\n", majorGLX, minorGLX);

    int screen_id   = XDefaultScreen(display);
    u64 black_color = BlackPixel(display, screen_id);
    u64 white_color = WhitePixel(display, screen_id);

    // Choose visual.
    GLint glx_attributes[] = {
        GLX_RGBA,
        GLX_DOUBLEBUFFER,
        GLX_DEPTH_SIZE,    24,
        GLX_STENCIL_SIZE,  8,
        GLX_RED_SIZE,      8,
        GLX_GREEN_SIZE,    8,
        GLX_BLUE_SIZE,     8,
        GLX_SAMPLE_BUFFERS,0,
        GLX_SAMPLES,       0,
        None
    };

    XVisualInfo *visual_info = glXChooseVisual(display, screen_id, glx_attributes);
    if (!visual_info) {
        write_string("Failed to glXChooseVisual.\n", true);
        return 0;
    }

    Window parent_window = RootWindow(display, screen_id);

    Colormap colormap = XCreateColormap(display, parent_window, 
                                        visual_info->visual, 
                                        AllocNone);

    XSetWindowAttributes window_attributes = {};
    window_attributes.border_pixel      = black_color;
    window_attributes.background_pixel  = white_color;
    window_attributes.override_redirect = True;
    window_attributes.colormap   = colormap;
    window_attributes.event_mask = ExposureMask|KeyPressMask|StructureNotifyMask;

    Window window = XCreateWindow(display, parent_window, 
                                  10, 10,
                                  800, 600,
                                  0, 
                                  visual_info->depth, 
                                  InputOutput,
                                  visual_info->visual,
                                  CWBackPixel|CWColormap|CWBorderPixel|CWEventMask,
                                  &window_attributes);

    const char *title = "Window Test (Linux)";
    Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
    Atom utf8_string = XInternAtom(display, "UTF8_STRING",  False);

    XChangeProperty(display, window, net_wm_name, utf8_string, 
                    8, PropModeReplace, 
                    (const u8 *)title, string_length(title));

    XStoreName(display, window, title);  // Legacy fallback.

    // Notify when the window manager attempts to delete the window.
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    if (!XSetWMProtocols(display, window, &wm_delete_window, 1)) {
        write_string("Failed to XSetWMProtocols.\n", true);
    }

    GLXContext gl_context = glXCreateContext(display, visual_info, null, True);
    if (!gl_context) {
        write_string("Failed to glXCreateContext.\n", true);
        return 0;
    }

    if (!glXMakeCurrent(display, window, gl_context)) {
        write_string("Failed to glXMakeCurrent.\n", true);
        return 0;
    }

    const GLubyte *gl_version  = glGetString(GL_VERSION);
    const GLubyte *gl_vendor   = glGetString(GL_VENDOR);
    const GLubyte *gl_renderer = glGetString(GL_RENDERER);

    print("OpenGL Version:  %s\n", gl_version);
    print("OpenGL Vendor:   %s\n", gl_vendor);
    print("OpenGL Renderer: %s\n", gl_renderer);

    Texture test = texture_load_from_file("data/textures/Texturtest planar.png");
    Texture cat  = texture_load_from_file("data/textures/cat.png");

    XClearWindow(display, window);
    XMapRaised(display, window);

    XWindowAttributes size_attributes;
    XGetWindowAttributes(display, window, &size_attributes);

    int back_buffer_width  = size_attributes.width;
    int back_buffer_height = size_attributes.height;

    // glDisable(GL_DEPTH_TEST);
    // Depth is mapped as near=-1 and far 1.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!should_quit) {
        // Non-blocking event loop.
        while (XPending(display) > 0) {
            XEvent event;
            XNextEvent(display, &event);

            switch (event.type) {
                case ClientMessage:
                    if ((Atom)event.xclient.data.l[0] == wm_delete_window) {
                        should_quit = true;
                    }
                    break;

                case DestroyNotify:
                    should_quit = true;
                    break;

                case Expose:
                    // Repaint.
                    break;

                case ConfigureNotify:
                    // Handle resize here.
                    back_buffer_width  = event.xconfigure.width;
                    back_buffer_height = event.xconfigure.height;
                    break;

                case KeyPress:
                    write_string("Key press event.\n");
                    break;
            }
        }

        glViewport(0, 0, back_buffer_width, back_buffer_height);

        rendering_2d(back_buffer_width, back_buffer_height);

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

        glXSwapBuffers(display, window);
    }

    glXDestroyContext(display, gl_context);

    XFree(visual_info);
    XFreeColormap(display, colormap);
    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}





#define GENERAL_IMPLEMENTATION
#include "general.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"