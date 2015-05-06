#include <iostream>
#include <Windows.h>

int main() {

    POINT newPoint;
    POINT oldPoint = { 0, 0 };
    for (size_t i = 0; i < 10; i++) {
        if (GetCursorPos(&newPoint) && newPoint.x != oldPoint.x && newPoint.y != oldPoint.y) {
            std::cout << newPoint.x << ":" << newPoint.y << std::endl;
            oldPoint = newPoint;
        }
        Sleep(300);
    }

    return 0;
}