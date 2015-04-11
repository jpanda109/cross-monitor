#include <iostream>
#include <stdio.h>
#include <X11/Xlib.h>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>

/* compile with g++ mouse.cpp -o echomouseevents -lX11 -lpthread -std=c++11 */

std::string key_name[] = {
    "first",
    "second (or middle)",
    "third",
    "fourth",  // :D
    "fifth"    // :|
};

bool getEvents = true;
std::condition_variable cv;
std::mutex mtx;

void process_input(Display *display, Window window) {
    std::string str_input;
    while (true) {
        getline(std::cin, str_input);
        if (str_input == "getEvents") {
            XGrabPointer(display,
                         window,
                         1,
                         PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
                         GrabModeAsync,
                         GrabModeAsync,
                         None,
                         None,
                         CurrentTime);
            getEvents = true;
            cv.notify_all();
        }
        else if (str_input == "cancel") {
            XUngrabPointer(display, CurrentTime);
            getEvents = false;
        }
        else {
            std::cout << str_input << std::endl;
        }
    }
}

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

    std::thread input_thread(process_input, display, window);
    while (true) {
        while(getEvents) {
            while (XPending(display)) {
                XNextEvent(display, &xevent);

                switch (xevent.type) {
                    case MotionNotify:
                        std::cout << "Mouse move: " << xevent.xmotion.x_root << " "
                            << xevent.xmotion.y_root << std::endl;
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
                if (!getEvents) {
                    std::cout << "events cancelled" << std::endl;
                }
            }
        }
        XFlush(display);
        std::cout << "while loop broken" << std::endl;
        std::unique_lock<std::mutex> lck(mtx);
        // XUngrabPointer(display,
                       // CurrentTime);
        while (!getEvents) {
            std::cout << "waiting" << std::endl;
            cv.wait(lck);
        }
    }

    return 0;
}
