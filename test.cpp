#include <stdio.h>
#include <iostream>
#include <string>
#include <X11/Xlib.h>

std::string key_name[] = {
    "first",
    "second (or middle)",
    "third",
    "fourth",
    "fifth"
};

int main() {
    Display *display;
    XEvent xevent;
    Window window;

    if ( (display = XOpenDisplay(NULL)) == NULL)
        return -1;

    window = DefaultRootWindow(display);
    XAllowEvents(display, AsyncBoth, CurrentTime);

    XGrabPointer(display,
                 window,
                 1,
                 PointerMotionMask | ButtonPressMask | ButtonReleaseMask ,
                 GrabModeAsync,
                 GrabModeAsync,
                 None,
                 None,
                 CurrentTime);

    while (1) {
        XNextEvent(display, &xevent);

        switch(xevent.type) {
            case MotionNotify:
                std::cout << "mouse moved: " << xevent.xmotion.x_root <<
                    " " << xevent.xmotion.y_root << std::endl;
                break;
            case ButtonPress:
                std::cout << "Button Pressed: " <<
                    key_name[xevent.xbutton.button - 1] << std::endl;
                break;
            case ButtonRelease:
                std::cout << "Button Released: " <<
                    key_name[xevent.xbutton.button - 1] << std::endl;
                break;
        }
    }

    return 0;

}
