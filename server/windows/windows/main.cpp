#include <iostream>
#include <Windows.h>

int main() {

    POINT p;
    if (GetCursorPos(&p)) {
        std::cout << p.x << ":" << p.y << std::endl;
    }

    return 0;
}