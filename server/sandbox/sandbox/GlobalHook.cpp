#include <Windows.h>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <typeinfo>


// std::unordered_map<unsigned long, bool> keymap;  // keyCode : isPressed
typedef struct Keymap : std::unordered_map < unsigned long, bool > {
    Keymap() {
        for (int i = 0; i < 256; ++i) {
            (*this)[i] = false;
        }
    }
} Keymap;
Keymap keymap;


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

void keymapInit();

int main() {
    // keymapInit();

    // set global hook until program termination
    HHOOK llkbHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
    MSG msg;
    GetMessage(&msg, NULL, NULL, NULL);
    UnhookWindowsHookEx(llkbHook);

    return(0);
}


LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    unsigned long keycode;
    if (nCode == HC_ACTION) {
        PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
        keycode = p->vkCode;
        switch (wParam) {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            std::cout << "KeyDown " << keycode << std::endl;
            keymap[keycode] = true;
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            std::cout << "KeyUp " << keycode << std::endl;
            keymap[keycode] = false;
            break;
        }
    }
    if (keymap[162] && keymap[160] && keymap[115]) {  // ctrl+shift+f4
        PostQuitMessage(0);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void keymapInit() {
    for (int i = 0; i < 256; ++i) {
        keymap[i] = false;
    }
}