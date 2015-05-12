#include <iostream>
#include <X11/Xlib.h>

int main() {
    Display *display;
    XEvent xevent;
    Window window;

    if ((display = XOpenDisplay(NULL)) == NULL) {
        return -1;
    }

    window = DefaultRootWindow(display);
    XAllowEvents(display, AsyncBoth, CurrentTime);

    XGrabKeyboard(display,  window, 1, GrabModeAsync, GrabModeAsync, CurrentTime);

    while (1) {
        XNextEvent(display, &xevent);
        std::cout << xevent.type << std::endl;
        switch (xevent.type) {
            case KeyPress:
                std::cout << "key press" << std::endl;
                break;
            case KeyRelease:
                std::cout << "key released" << std::endl;
        }
        std::cout << xevent.xkey.keycode << std::endl;
        if (xevent.xkey.keycode == 81) {
            break;
        }
    }

    return 0;
}
