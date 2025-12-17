#include "general.h"

#include <X11/Xlib.h>

bool should_quit = false;

int main(void) {
    Display *display = XOpenDisplay(null);
    if (!display) {
        write_string("Failed to XOpenDisplay.\n", true);
        return 0;
    }

    int screen = XDefaultScreen(display);
    u64 black_color = BlackPixel(display, screen);
    u64 white_color = WhitePixel(display, screen);

    Window window = XCreateSimpleWindow(display,
                                        XDefaultRootWindow(display),
                                        0, 0,
                                        800, 600,
                                        0,
                                        white_color,
                                        black_color);

    const char *title = "Window Test (Linux)";
    Atom net_wm_name = XInternAtom(display, "_NET_WM_NAME", False);
    Atom utf8_string = XInternAtom(display, "UTF8_STRING", False);

    XChangeProperty(display, window, net_wm_name, utf8_string, 
                    8, PropModeReplace, 
                    (const u8 *)title, string_length(title));

    XStoreName(display, window, title);  // Legacy fallback.

    // Notify when the window manager attempts to delete the window.
    Atom wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", False);
    if (!XSetWMProtocols(display, window, &wm_delete_window, 1)) {
        write_string("Failed to XSetWMProtocols.\n", true);
    }

    Atom wm_protocols = XInternAtom(display, "WM_PROTOCOLS", False);

    XSelectInput(display, window, ExposureMask|KeyPressMask|StructureNotifyMask);
    XMapWindow(display, window);

    // XSync(display, False);

    while (!should_quit) {
        // Non-blocking event loop.
        while (XPending(display) > 0) {
            XEvent event;
            XNextEvent(display, &event);

            switch (event.type) {
                case ClientMessage:
                    if (event.xclient.message_type == wm_protocols &&
                        event.xclient.data.l[0] == wm_delete_window) {
                        should_quit = true;
                    }
                    break;

                case DestroyNotify:
                    should_quit = true;
                    break;

                case Expose:
                    break;

                case KeyPress:
                    write_string("Key press event.\n");
                    break;
            }
        }

    }

    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}




#define GENERAL_IMPLEMENTATION
#include "general.h"