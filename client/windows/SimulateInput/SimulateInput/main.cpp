#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>


bool QUIT = false;

std::vector<std::string> splitString(std::string &str_input);

int main() {

    std::string str_input;
    std::vector<std::string> tokens;
    while (!QUIT) {
        std::getline(std::cin, str_input);
        tokens = splitString(str_input);
        if (tokens.size() == 0) {}
        else if (tokens[0] == "Quit") {
            QUIT = true;
        }
        else if (tokens[0] == "MoveMouse" && tokens.size() == 3) {
            try {
                int x = std::stoi(tokens[1]);
                int y = std::stoi(tokens[2]);
                SetCursorPos(x, y);
            }
            catch (std::invalid_argument) {}
        }
        else if (tokens[0] == "MouseClick" && tokens.size() == 3) {
            INPUT input;
            input.type = INPUT_MOUSE;
            if (tokens[1] == "Left") {
                if (tokens[2] == "Down") {
                    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                }
                else {
                    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                }
            }
            else {
                if (tokens[2] == "Down") {
                    input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                }
                else {
                    input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                }
            }
            SendInput(1, &input, sizeof(input));
        }
        else if (tokens[0] == "KeyPress" && tokens.size() == 3) {
            try {
                unsigned long keycode = std::stoul(tokens[2]);
                INPUT input;
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = keycode;
                if (tokens[1] == "Down") {
                    input.ki.dwFlags = 0;
                }
                else {
                    input.ki.dwFlags = KEYEVENTF_KEYUP;
                }
                SendInput(1, &input, sizeof(input));
            }
            catch (std::invalid_argument) {}
        }
    }

    return 0;
}


std::vector<std::string> splitString(std::string &str_input) {
    std::vector<std::string> tokens;
    std::istringstream stream(str_input);
    std::copy(std::istream_iterator<std::string>(stream),
        std::istream_iterator<std::string>(),
        std::back_inserter(tokens));
    return tokens;
}