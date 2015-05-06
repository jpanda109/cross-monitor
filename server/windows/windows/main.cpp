#include <iostream>
#include <Windows.h>

int main() {

    POINT p;
    if (GetCursorPos(&p)) {
        std::cout << p.x << ":" << p.y << std::endl;
    }

    // The breakpoint is to persistently display output when program is finished
    return 0;
}