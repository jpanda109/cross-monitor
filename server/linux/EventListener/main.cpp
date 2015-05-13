#include <array>
#include <sstream>
#include <iostream>
#include <X11/Xlib.h>
#include <thread>
#include <condition_variable>

/* compile with g++ mouse.cpp -o echomouseevents -lX11 -lpthread -std=c++11 */

std::string key_name[] = {
    "first",
    "second (or middle)",
    "third",
    "fourth",  // :D
    "fifth"    // :|
};

#define QUIT 0
#define DO_NOTHING 1
#define MOVEMENT_ONLY 2
#define CATCH_ALL 3
int mode = DO_NOTHING;
std::condition_variable cv;
std::mutex mtx;


/* process input stream commands to stop or resume pointer monitoring */
void process_input(Display *display, Window window);

std::array<int, 2> getMousePos(Display *display);

int main(int argc, char **argv)
{
    XInitThreads();
    Display *display;
    XEvent xevent;
    Window window;

    if( (display = XOpenDisplay(NULL)) == NULL )
        return -1;


    window = DefaultRootWindow(display);
    XAllowEvents(display, AsyncBoth, CurrentTime);


    std::thread input_thread(process_input, display, window);
    while (mode != QUIT) {
        while(mode == CATCH_ALL) {
            while (XPending(display)) {
                XNextEvent(display, &xevent);

                switch (xevent.type) {
                    case MotionNotify:
                        std::cout << "Mouse Move " << xevent.xmotion.x_root << " "
                                                                               << xevent.xmotion.y_root << std::endl;
                        break;
                    case ButtonPress:
                        std::cout << "Button Pressed " <<
                                     key_name[xevent.xbutton.button - 1] << std::endl;
                        break;
                    case ButtonRelease:
                        std::cout << "Button Released " <<
                                     key_name[xevent.xbutton.button - 1] << std::endl;
                        break;
                    case KeyPress:
                        std::cout << "Key Pressed " <<
                                     xevent.xkey.keycode << std::endl;
#ifdef _DEBUG
                        if (xevent.xkey.keycode == 70) {
                            XUngrabPointer(display, CurrentTime);
                            XUngrabKeyboard(display, CurrentTime);
                            mode = DO_NOTHING;
                        } else {
                            std::cout << xevent.xkey.keycode << std::endl;
                        }
#endif
                        break;
                    case KeyRelease:
                        std::cout << "Key Released " <<
                            xevent.xkey.keycode << std::endl;
                        break;
                    default:
                        break;
                }
            }
        }
        XFlush(display);

        std::array<int, 2> coords = {0, 0};
        while (mode == MOVEMENT_ONLY) {
            std::array<int, 2> newCoords = getMousePos(display);
            if (coords != newCoords) {
                coords = newCoords;
                std::cout << "Mouse Move " << coords[0] << " " << coords[1] << std::endl;
            }
        }

        std::unique_lock<std::mutex> lck(mtx);
        while (mode == DO_NOTHING) {
            cv.wait(lck);
        }
    }
    input_thread.join();
    XCloseDisplay(display);
    return 0;
}


void process_input(Display *display, Window window) {
    std::string str_input;
    while (mode != QUIT) {
        getline(std::cin, str_input);
        if (str_input == "CatchAll") {
            XGrabPointer(display,
                         window,
                         1,
                         PointerMotionMask | ButtonPressMask | ButtonReleaseMask,
                         GrabModeAsync,
                         GrabModeAsync,
                         None,
                         None,
                         CurrentTime);
            XGrabKeyboard(display,
                          window,
                          1,
                          GrabModeAsync,
                          GrabModeAsync,
                          CurrentTime);
            mode = CATCH_ALL;
            cv.notify_all();
        }
        else if (str_input == "DoNothing") {
            XUngrabPointer(display, CurrentTime);
            XUngrabKeyboard(display, CurrentTime);
            mode = DO_NOTHING;
        }
        else if (str_input == "MovementOnly") {
            XUngrabPointer(display, CurrentTime);
            XUngrabKeyboard(display, CurrentTime);
            mode = MOVEMENT_ONLY;
            cv.notify_all();
        }
        else if (str_input == "Quit") {
            XUngrabPointer(display, CurrentTime);
            XUngrabKeyboard(display, CurrentTime);
            mode = QUIT;
            cv.notify_all();
        }
    }
}

std::array<int, 2> getMousePos(Display *display) {
    XEvent event;
    XQueryPointer(display, RootWindow(display, 0), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root,
    &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);
    std::array<int, 2> coords = {event.xbutton.x, event.xbutton.y};
    return coords;
}
