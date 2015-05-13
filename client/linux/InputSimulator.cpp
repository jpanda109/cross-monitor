//
// Created by jason on 5/13/15.
//

#include "InputSimulator.h"


InputSimulator::InputSimulator()
    : mDisplay(XOpenDisplay(nullptr)) {}

InputSimulator *InputSimulator::getInstance() {
    if (instance == nullptr) {
        instance = new InputSimulator();
    }
    return instance;
}

InputSimulator::~InputSimulator() {
    XCloseDisplay(mDisplay);
    instance = nullptr;
}

void InputSimulator::processInput(const std::string &input) {
    std::vector<std::string> tokens;
    std::stringstream ssin(input);
    while (ssin.good()) {
        std::string value;
        ssin >> value;
        tokens.push_back(value);
    }
    if (tokens.size() >= 3) {
        if (tokens[0] == "Mouse" && tokens.size() == 4) {
            if (tokens[1] == "Move" ) {
                try {
                    int x = std::stoi(tokens[2]);
                    int y = std::stoi(tokens[3]);
                    moveMouse(x, y);
                } catch (std::invalid_argument) {}
            }
            else if (tokens[1] == "Button") {
                bool down = tokens[3] == "Down";
                if (tokens[2] == "Left") {
                    mouseButton(0, down);
                }
                else if (tokens[2] == "Right") {
                    mouseButton(2, down);
                }
            }
        }
        else if (tokens[0] == "KeyPress" && tokens.size() == 3) {
            try {
                bool down = tokens[1] == "Down";
                unsigned int keycode = (unsigned int) std::stoi(tokens[2]);
                key(keycode, down);
            } catch (std::invalid_argument) {}
        }
    }
}

void InputSimulator::moveMouse(const int &x, const int &y) {
    XTestFakeMotionEvent(mDisplay, 0, x, y, CurrentTime);
    XSync(mDisplay, 0);
}

void InputSimulator::mouseButton(const unsigned int &button, bool down) {
    XTestFakeButtonEvent(mDisplay, button, down, CurrentTime);
    XSync(mDisplay, 0);
}

void InputSimulator::key(const unsigned int &keycode, bool down) {
    XTestFakeKeyEvent(mDisplay, keycode, down, 0);
    XSync(mDisplay, 0);
}

InputSimulator *InputSimulator::instance = nullptr;
