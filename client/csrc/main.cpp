#include <iostream>
#include <X11/extensions/XTest.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <algorithm>


std::vector<std::string> splitInfo(std::string input) {
    std::vector<std::string> info;
    std::stringstream ssin(input);
    while (ssin.good()) {
        std::string value;
        ssin >> value;
        info.push_back(value);
    }
    return info;
}

int main() {
    Display *display = nullptr;

    while (true) {
        display = XOpenDisplay(nullptr);
        std::string input;
        std::getline(std::cin, input);
        input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
        std::vector<std::string> info = splitInfo(input);
        if (info[0] == "quit" || info[0] == "Quit") {
            XCloseDisplay(display);
            return 0;
        }
        else if (info[0] == "Mouse") {
            std::cout << info[2] << " " << info[3] << std::endl;
            XTestFakeMotionEvent(display, 0, std::stoi(info[2]), std::stoi(info[3]), CurrentTime);
            XCloseDisplay(display);
        }
        else if (info[0] == "Button") {
            if (info[1] == "Pressed") {
                XTestFakeButtonEvent(display, (unsigned int) std::stoi(info[2]), true, CurrentTime);
                XCloseDisplay(display);
            }
            else if (info[1] == "Released") {
                XTestFakeButtonEvent(display, (unsigned int) std::stoi(info[2]), false, CurrentTime);
                XCloseDisplay(display);
            }
        }
        else if (info[0] == "Key") {
            if (info[1] == "Pressed") {
                XTestFakeKeyEvent(display, (unsigned int) std::stoi(info[2]), true, 0);
                XCloseDisplay(display);
            }
            if (info[1] == "Released") {
                XTestFakeKeyEvent(display, (unsigned int) std::stoi(info[2]), false, 0);
                XCloseDisplay(display);
            }
        }
        else {
            XCloseDisplay(display);
        }
    }
}