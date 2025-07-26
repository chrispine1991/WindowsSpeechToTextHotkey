# 🖱️ PAUSE Button-to-WinH 🔊

Trigger **Windows 11 Voice Typing (Win + H)** with a press of WIN + **Mouse Button 3 (middle click)** using this tiny, silent C++ app.  
Perfect for accessibility, dictation, or productivity without needing to press key combos!

---

## ✨ Features

- ✅ Runs silently in the background (no console window)
- 🖱️ Uses **middle mouse button** as a hotkey (VK_MBUTTON)
- 🔊 Triggers Windows’ native **Voice Typing** (`Win + H`)
- 💻 Ultra-lightweight — no install, no dependencies

---

## 🚀 How to Use

1. **Download or compile** the app (see below)
2. **Run `SpeechHotkey.exe`** — it runs quietly in the background
3. **Press your middle mouse button (scroll wheel click)** to trigger `Win + H`

That's it! Dictation pops up instantly. 🎙️

---

## 🛠️ How to Compile (Optional)

### ✅ Prerequisites:
- Windows 10/11 x64
- Visual Studio or [Build Tools for Visual Studio](https://visualstudio.microsoft.com/visual-cpp-build-tools/)

### 🔧 Build Instructions:
```bash
cl SpeechHotkey.cpp user32.lib /link /SUBSYSTEM:WINDOWS
