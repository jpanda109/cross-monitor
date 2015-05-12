#include <array>
#include <sstream>
#include <iostream>
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

#define QUIT 0
#define DO_NOTHING 1
#define MOVEMENT_ONLY 2
#define CATCH_ALL 3
int mode = DO_NOTHING;
std::condition_variable cv;
std::mutex mtx;


/* process input stream commands to stop or resume pointer monitoring */
void process_input(Display *display, Window window) {
    std::string str_input;
    while (mode != QUIT) {
        getline(std::cin, str_input);
        if (str_input == "resume") {
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
        else if (str_input == "cancel") {
            XUngrabPointer(display, CurrentTime);
            XUngrabKeyboard(display, CurrentTime);
            mode = DO_NOTHING;
        }
        else if (str_input == "movement only") {
            XUngrabPointer(display, CurrentTime);
            XUngrabKeyboard(display, CurrentTime);
            mode = MOVEMENT_ONLY;
            cv.notify_all();
        }
        else if (str_input == "Quit") {
            XUngrabPointer(display, CurrentTime);
            XUngrabKeyboard(display, CurrentTime);
            mode = QUIT;
        }
    }
}

std::string exec(char *cmd) {
    FILE *pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
}

std::array<std::string, 2> parse_xdotool(std::string location) {
    std::string info[4];
    std::stringstream ssin(location);
    int i = 0;
    while (ssin.good() && i < 4) {
        ssin >> info[i];
        ++i;
    }
    std::array<std::string, 2> coords = {info[0].substr(2), info[1].substr(2)};
    return coords;
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


    std::thread input_thread(process_input, display, window);
    while (true) {
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
                        if (key_name[xevent.xbutton.button - 1] == "third") {
                            // safeguard against always locked
                            return 0;
                        }
                        break;
                    case ButtonRelease:
                        std::cout << "Button Released " <<
                            key_name[xevent.xbutton.button - 1] << std::endl;
                        break;
                    case KeyPress:
                        std::cout << "Key Pressed " <<
                            xevent.xkey.keycode << std::endl;
                        break;
                    case KeyRelease:
                        std::cout << "Key Released " <<
                            xevent.xkey.keycode << std::endl;
                        break;
                }
            }
        }
        XFlush(display);

        std::array<std::string, 2> coords = {"", ""};
        while (mode == MOVEMENT_ONLY) {
            std::array<std::string, 2> newCoords = parse_xdotool(exec("xdotool getmouselocation"));
            if (coords != newCoords) {
                coords = newCoords;
                std::cout << "Mouse move " << coords[0] << " " << coords[1] << std::endl;
            }
        }

        std::unique_lock<std::mutex> lck(mtx);
        while (mode == DO_NOTHING) {
            cv.wait(lck);
        }
    }

    return 0;
}
