#include <iostream>
#include <thread>
#include <string>
#include <Windows.h>

bool QUIT = false;

void process_input() {
    std::string str_input;
    while (!QUIT) {
        std::getline(std::cin, str_input);
        if (str_input == "quit") {
            QUIT = true;
        }
    }
}

int main() {

    std::thread input_thread(process_input);

    POINT newPoint;
    POINT oldPoint = { 0, 0 };
    while (!QUIT) {
        if (GetCursorPos(&newPoint) && newPoint.x != oldPoint.x && newPoint.y != oldPoint.y) {
            std::cout << newPoint.x << ":" << newPoint.y << std::endl;
            oldPoint = newPoint;
        }
    }

    return 0;
}