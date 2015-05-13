#include <iostream>
#include <algorithm>
#include "InputSimulator.h"


bool QUIT = false;

int main() {

    InputSimulator *inputSimulator = InputSimulator::getInstance();

    while (!QUIT) {
        std::string input;
        std::getline(std::cin, input);
        input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());
        if (input == "Quit" ) {
            QUIT = true;
        }
        else {
            inputSimulator->processInput(input);
        }

    }

    delete inputSimulator;
    return 0;
}
