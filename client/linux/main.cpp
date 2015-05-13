#include <iostream>
#include <X11/extensions/XTest.h>
#include <sstream>
#include <vector>
#include <algorithm>


bool QUIT = false;

std::vector<std::string> splitString(std::string &input) {
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

    while (!QUIT) {
        display = XOpenDisplay(nullptr);
        std::string input;
        std::getline(std::cin, input);
        input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
        std::vector<std::string> tokens = splitString(input);
        if (tokens[0] == "quit" || tokens[0] == "Quit") {
            QUIT = true;
        }
        else if (tokens[0] == "Mouse" && tokens.size() == 4) {
            if (tokens[1] == "Move") {
                try {
                    int x = std::stoi(tokens[2]);
                    int y = std::stoi(tokens[3]);
                    XTestFakeMotionEvent(display, 0, x, y, CurrentTime);
                }
                catch (std::invalid_argument) {}
            }
            else if (tokens[1] == "Button" && tokens.size() == 3) {
                if (tokens[2] == "Left" && tokens[3] == "Down") {
                    XTestFakeButtonEvent(display, 0, true, CurrentTime);
                }
                else if (tokens[2] == "Left" && tokens[3] == "Up") {
                    XTestFakeButtonEvent(display, 0, false, CurrentTime);
                }
                else if (tokens[2] == "Right" && tokens[3] == "Down") {
                    XTestFakeButtonEvent(display, 2, true, CurrentTime);
                }
                else if (tokens[2] == "Right" && tokens[3] == "Up") {
                    XTestFakeButtonEvent(display, 0, false, CurrentTime);
                }
            }
        }
        else if (tokens[0] == "KeyPress" && tokens.size() == 3) {
            try {
                unsigned int keycode = (unsigned int) std::stoi(tokens[2]);
                if (tokens[1] == "Down") {
                    XTestFakeKeyEvent(display, keycode, true, 0);
                }
                else if (tokens[1] == "Up") {
                    XTestFakeKeyEvent(display, keycode, false, 0);
                }
            }
            catch (std::invalid_argument) {}
        }
#ifdef _DEBUG
        else {
            std::cout << input << std::endl;
        }
#endif
        XSync(display, 0);

    }

    XCloseDisplay(display);
    return 0;
}
