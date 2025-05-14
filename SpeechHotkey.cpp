#include <windows.h>
#include <thread>
#include <chrono>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    while (true) {
        if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
            // Simulate Win + H
            INPUT input[4] = {};

            input[0].type = INPUT_KEYBOARD;
            input[0].ki.wVk = VK_LWIN;

            input[1].type = INPUT_KEYBOARD;
            input[1].ki.wVk = 'H';

            input[2].type = INPUT_KEYBOARD;
            input[2].ki.wVk = 'H';
            input[2].ki.dwFlags = KEYEVENTF_KEYUP;

            input[3].type = INPUT_KEYBOARD;
            input[3].ki.wVk = VK_LWIN;
            input[3].ki.dwFlags = KEYEVENTF_KEYUP;

            SendInput(4, input, sizeof(INPUT));

            // Wait until button is released
            while (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
