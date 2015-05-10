#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>


bool QUIT = false;

std::vector<std::string> splitString(std::string &str_input);

int main() {

    std::ios::sync_with_stdio(false);
    
    std::string str_input;
    while (!QUIT) {
        std::getline(std::cin, str_input);
        std::vector<std::string> tokens = splitString(str_input);
        if (tokens[0] == "Quit") {
            QUIT = true;
        }
        else if (tokens[0] == "MoveMouse" && tokens.size() == 3) {
            try {
                int x = std::stoi(tokens[1]);
                int y = std::stoi(tokens[2]);
                SetCursorPos(x, y);
            }
            catch (std::invalid_argument) {
            }
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