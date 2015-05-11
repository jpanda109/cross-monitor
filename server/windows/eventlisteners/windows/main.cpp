#include <iostream>
#include <fstream>
#include <cstdio>
#include <thread>
#include <string>
#include <unordered_map>
#include <Windows.h>


#define QUIT 0
#define MOVEMENT_ONLY 1
#define DO_NOTHING 2
#define CATCH_ALL 3

int currentMode = DO_NOTHING;
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
    if (currentMode != CATCH_ALL) {  // only use this hook if block/catching all input
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    unsigned long keycode;
    if (nCode == HC_ACTION) {
        PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
        keycode = p->vkCode;
        switch (wParam) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            std::cout << "KeyPress Down " << keycode << std::endl;
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            std::cout << "KeyPress Up " << keycode << std::endl;
            break;
        }
    }
#ifdef _DEBUG  // Just in case keyboard locks without escape
    if (keycode == 115) {  // f4
        exitProgram();
    }
#endif
    return 1;
}

LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (currentMode == DO_NOTHING) {
        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }
    if (nCode == HC_ACTION) {
        PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
        switch (wParam) {
        case WM_MOUSEWHEEL:
            std::cout << "Mouse Scroll" << std::endl;
            break;
        case WM_MOUSEHWHEEL:
            std::cout << "Mouse HScroll" << std::endl;
            break;
        case WM_LBUTTONDOWN:
            std::cout << "Mouse Button Left Down" << std::endl;
            break;
        case WM_RBUTTONDOWN:
            std::cout << "Mouse Button Right Down" << std::endl;
            break;
        case WM_LBUTTONUP:
            std::cout << "Mouse Button Left Up" << std::endl;
            break;
        case WM_RBUTTONUP:
            std::cout << "Mouse Button Right Up" << std::endl;
            break;
        case WM_MOUSEMOVE:
            std::cout << "Mouse Move " << p->pt.x << " " << p->pt.y << std::endl;
            break;
        }
    }
    return currentMode == CATCH_ALL && wParam != WM_MOUSEMOVE ? 1 : CallNextHookEx(NULL, nCode, wParam, lParam);
}

void processInput() {
    std::string str_input;
    while (currentMode != QUIT) {
        std::getline(std::cin, str_input);
        if (str_input == "Quit") {
            exitProgram();
        }
        else if (str_input == "MovementOnly") {
            currentMode = MOVEMENT_ONLY;
        }
        else if (str_input == "DoNothing") {
            currentMode = DO_NOTHING;
        }
        else if (str_input == "CatchAll") {
            currentMode = CATCH_ALL;
        }
    }
}

void exitProgram() {
    currentMode = QUIT;
    PostThreadMessage(mainThreadId, WM_QUIT, 0, 0);
}