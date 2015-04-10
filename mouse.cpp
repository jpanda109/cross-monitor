#include <iostream>
#include <stdio.h>
#include <X11/Xlib.h>
#include <string>

std::string key_name[] = {
    "first",
    "second (or middle)",
    "third",
    "fourth",  // :D
    "fifth"    // :|
};

int main(int argc, char **argv)
{
    Display *display;
    XEvent xevent;
    Window window;

    if( (display = XOpenDisplay(NULL)) == NULL )
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

    while(1) {
        XNextEvent(display, &xevent);

        switch (xevent.type) {
            case MotionNotify:
                printf("Mouse move      : [%d, %d]\n", xevent.xmotion.x_root, xevent.xmotion.y_root);
                break;
            case ButtonPress:
                std::cout << "Button pressed  : " <<
                    key_name[xevent.xbutton.button - 1] << std::endl;
                break;
            case ButtonRelease:
                std::cout << "Button released  : " <<
                    key_name[xevent.xbutton.button - 1] << std::endl;
                break;
        }
    }

    return 0;
}
