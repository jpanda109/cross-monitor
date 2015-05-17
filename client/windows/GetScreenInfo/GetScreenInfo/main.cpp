#include <Windows.h>
#include <iostream>


int main() {
    std::ios::sync_with_stdio(false);
    std::cout << GetSystemMetrics(SM_CXSCREEN) << std::endl;
    std::cout << GetSystemMetrics(SM_CYSCREEN) << std::endl;
    return 0;
}