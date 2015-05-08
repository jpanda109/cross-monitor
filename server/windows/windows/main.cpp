#include <iostream>
#include <fstream>
#include <cstdio>
#include <thread>
#include <string>
#include <unordered_map>
#include <Windows.h>

typedef struct Keymap : std::unordered_map<unsigned long, bool> {
    Keymap() {
        for (int i = 0; i < 256; ++i) {
            (*this)[i] = false;
        }
    }
} Keymap;  // keycode : isPressed
Keymap keymap;
bool QUIT = false;
DWORD mainThreadId;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

void processInput();

void exitProgram();


int main() {

    std::ios::sync_with_stdio(false);  // not using stdio, boost performance

    mainThreadId = GetCurrentThreadId();
    std::thread input_thread(processInput);

    HHOOK llkbHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    HHOOK llmHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, 0, 0);
    MSG msg;
    GetMessage(&msg, NULL, NULL, NULL);
    UnhookWindowsHookEx(llkbHook);
    UnhookWindowsHookEx(llmHook);

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
#ifdef _DEBUG
    if (keymap[162] && keymap[160] && keymap[115]) {  // ctrl+shift+f4
        exitProgram();
    }
#endif
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
        switch (wParam) {
        case WM_MOUSEWHEEL:
            std::cout << "MouseScroll" << std::endl;
            break;
        case WM_MOUSEHWHEEL:
            std::cout << "HMouseScroll" << std::endl;
            break;
        case WM_LBUTTONDOWN:
            std::cout << "MouseButtonDown LEFT" << std::endl;
            break;
        case WM_RBUTTONDOWN:
            std::cout << "MouseButtonDown RIGHT" << std::endl;
            break;
        case WM_LBUTTONUP:
            std::cout << "MouseButtonUp LEFT" << std::endl;
            break;
        case WM_RBUTTONUP:
            std::cout << "MouseButtonUp RIGHT" << std::endl;
            break;
        case WM_MOUSEMOVE:
            std::cout << "MouseMove " << p->pt.x << " " << p->pt.y << std::endl;
            break;
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
        else if (str_input == "MovementOnly") {

        }
        else if (str_input == "DoNothing") {

        }
        else if (str_input == "CatchAll") {

        }
    }
}

void exitProgram() {
    PostThreadMessage(mainThreadId, WM_QUIT, 0, 0);
    QUIT = true;
}