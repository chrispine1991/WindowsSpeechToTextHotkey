#include <windows.h>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>

HINSTANCE hInst;
NOTIFYICONDATA nid = {};
bool running = true;

std::string modifier = "Shift";
std::string button = "MMB";

void LoadConfig() {
    std::ifstream file("SpeechHotkey.ini");
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.find("Modifier=") == 0) modifier = line.substr(9);
        if (line.find("Button=") == 0) button = line.substr(7);
    }
}

UINT GetButtonVK() {
    if (button == "MMB") return VK_MBUTTON;
    if (button == "RMB") return VK_RBUTTON;
    if (button == "LMB") return VK_LBUTTON;
    if (button == "F13") return VK_F13;
    if (button == "F14") return VK_F14;
    return VK_MBUTTON; // Default
}

bool IsModifierPressed() {
    if (modifier == "None") return true;
    if (modifier == "Shift") return (GetAsyncKeyState(VK_SHIFT) & 0x8000);
    if (modifier == "Ctrl")  return (GetAsyncKeyState(VK_CONTROL) & 0x8000);
    if (modifier == "Alt")   return (GetAsyncKeyState(VK_MENU) & 0x8000);
    return true;
}

void PressKey(WORD vk, bool down) {
    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    input.ki.dwFlags = down ? 0 : KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}

void SendWinHWithShiftFix() {
    bool shiftHeld = (GetAsyncKeyState(VK_SHIFT) & 0x8000);

    // Release Shift if held
    if (shiftHeld) PressKey(VK_SHIFT, false);

    INPUT inputs[4] = {};
    inputs[0].type = INPUT_KEYBOARD; inputs[0].ki.wVk = VK_LWIN;
    inputs[1].type = INPUT_KEYBOARD; inputs[1].ki.wVk = 'H';
    inputs[2].type = INPUT_KEYBOARD; inputs[2].ki.wVk = 'H'; inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[3].type = INPUT_KEYBOARD; inputs[3].ki.wVk = VK_LWIN; inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(4, inputs, sizeof(INPUT));

    // Restore Shift if it was held
    if (shiftHeld) PressKey(VK_SHIFT, true);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_COMMAND && LOWORD(wParam) == 1) {
        running = false;
        PostQuitMessage(0);
    }
    if (msg == WM_USER + 1 && lParam == WM_RBUTTONUP) {
        HMENU menu = CreatePopupMenu();
        AppendMenu(menu, MF_STRING, 1, TEXT("Exit"));
        POINT pt;
        GetCursorPos(&pt);
        SetForegroundWindow(hwnd);
        TrackPopupMenu(menu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
        DestroyMenu(menu);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    LoadConfig();

    // Register hidden window
    const char CLASS_NAME[] = "SpeechHotkeyClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, "SpeechHotkey", 0,
        0, 0, 0, 0, HWND_MESSAGE, NULL, hInstance, NULL);

    // Tray icon
    nid.cbSize = sizeof(nid);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    nid.uCallbackMessage = WM_USER + 1;
    nid.hIcon = LoadIcon(NULL, IDI_INFORMATION);
    lstrcpy(nid.szTip, TEXT("SpeechHotkey Running"));
    Shell_NotifyIcon(NIM_ADD, &nid);

    UINT vkButton = GetButtonVK();

    std::thread hotkeyThread([&]() {
        while (running) {
            if (IsModifierPressed() && (GetAsyncKeyState(vkButton) & 0x8000)) {
                SendWinHWithShiftFix();
                while (GetAsyncKeyState(vkButton) & 0x8000)
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        });

    MSG msg;
    while (running && GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Shell_NotifyIcon(NIM_DELETE, &nid);
    hotkeyThread.join();

    return 0;
}
