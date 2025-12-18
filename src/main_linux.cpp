#include "general.h"

#include <X11/Xlib.h>

#include <GL/glx.h>
#include <GL/gl.h>

bool should_quit = false;

int main(void) {
    Display *display = XOpenDisplay(null);
    if (!display) {
        write_string("Failed to XOpenDisplay.\n", true);
        return 0;
    }

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

/*
    Window window = XCreateSimpleWindow(display,
                                        XDefaultRootWindow(display),
                                        0, 0,
                                        800, 600,
                                        0,
                                        white_color,
                                        black_color);
*/

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
                                  0, 0,
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

/*
    XWindowAttributes window_attributes = {};
    XGetWindowAttributes(display, window, &window_attributes);
    GC gc = XCreateGC(display, window, 0, null);
*/

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

    // XSelectInput(display, window, ExposureMask|KeyPressMask|StructureNotifyMask);
    
    XClearWindow(display, window);
    XMapRaised(display, window);
    // XMapWindow(display, window);

    // XSync(display, False);

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
                    break;

                case KeyPress:
                    write_string("Key press event.\n");
                    break;
            }
        }

        glClearColor(0.2f, 0.38f, 0.72f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glXSwapBuffers(display, window);
    }

    glXDestroyContext(display, gl_context);

    XFree(visual_info);
    XFreeColormap(display, colormap);
    // XFreeGC(display, gc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}





#define GENERAL_IMPLEMENTATION
#include "general.h"