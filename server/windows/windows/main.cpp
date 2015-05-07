#include <iostream>
#include <thread>
#include <string>
#include <unordered_map>
#include <Windows.h>

bool QUIT = false;
typedef struct Keymap : std::unordered_map<unsigned long, bool> {
    Keymap() {
        for (int i = 0; i < 256; ++i) {
            (*this)[i] = false;
        }
    }
} Keymap;  // keycode : isPressed
Keymap keymap;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

void processInput();

void exitProgram();


int main() {

    std::thread input_thread(processInput);

    HHOOK llkbHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    HHOOK llmHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, 0, 0);
    MSG msg;
    GetMessage(&msg, NULL, NULL, NULL);
    UnhookWindowsHookEx(llkbHook);
    UnhookWindowsHookEx(llmHook);
    //POINT newPoint;
    //POINT oldPoint = { 0, 0 };
    //while (!QUIT) {
        //if (GetCursorPos(&newPoint) && newPoint.x != oldPoint.x && newPoint.y != oldPoint.y) {
            //std::cout << newPoint.x << ":" << newPoint.y << std::endl;
            //oldPoint = newPoint;
        //}
    //}

    input_thread.join();

    return 0;
}


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    unsigned long keycode;
    if (nCode == HC_ACTION) {
        PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
        keycode = p->vkCode;
        switch (wParam) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            std::cout << "KeyDown" << keycode << std::endl;
            keymap[keycode] = true;
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            std::cout << "KeyUp" << keycode << std::endl;
            keymap[keycode] = false;
            break;
        }
    }
    if (keymap[162] && keymap[160] && keymap[115]) {  // ctrl+shift+f4
        exitProgram();
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
        switch (wParam) {
        case WM_MOUSEWHEEL:
            std::cout << "MouseScroll" << p->mouseData << std::endl;
            break;
        case WM_XBUTTONDOWN:
        case WM_NCXBUTTONDOWN:
            std::cout << "MouseButtonDown" << p->mouseData << std::endl;
            break;
        case WM_XBUTTONUP:
        case WM_NCXBUTTONUP:
            std::cout << "MouseButtonUp" << p->mouseData << std::endl;
            break;
        case WM_XBUTTONDBLCLK:
        case WM_NCXBUTTONDBLCLK:
            std::cout << "MouseButtonDoubleClick" << p->mouseData << std::endl;
            break;
        default:
            std::cout << "MouseMove " << p->pt.x << " " << p->pt.y << std::endl;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void processInput() {
    std::string str_input;
    while (!QUIT) {
        std::getline(std::cin, str_input);
        if (str_input == "quit") {
            exitProgram();
        }
    }
}

void exitProgram() {
    PostQuitMessage(0);
    QUIT = true;
}