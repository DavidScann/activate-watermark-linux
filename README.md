# 💀 Activate Watermark

> **Linux port** of [activate-watermark](https://github.com/DavidScann/activate-watermark) by [DavidScann](https://github.com/DavidScann). Original concept and Windows implementation by the same legend. This version brings the same unhinged energy to Linux because Wayland girlies deserve aura too.

> *no cap this is the most unhinged thing i've ever ported and i'm lowkey proud of it*

A tiny program that slaps the classic **"Activate Windows"** watermark on your desktop — now also for **Linux** because why should Windows have all the aura. Always on top. Click-through. Just like the real thing — except you're doing it *on purpose*. absolute cinema.

## ✨ Features

- 🎯 **Pixel-perfect replica** — it's giving authentic Windows 10 watermark energy fr fr
- 👻 **Click-through & transparent** — this thing is more ghosted than your last situationship
- 📌 **Always on top** — no window can ratio this watermark, it stays winning
- 🔧 **System tray controls** — right-click the tray icon to customize everything, it's bussin
- 🚫 **No taskbar icon** — stealth mode activated, this thing is the main character with zero screen time
- 🐧 **Linux native** — Qt6 + Wayland layer-shell protocol. KDE Plasma users stay winning
- 🪟 **Windows original** — still in the git history for the nostalgic besties

## 🎮 Tray Menu

Right-click the ℹ️ icon in your system tray, it's giving full customization:

| Option | What it does |
|---|---|
| **Position** | Move it to any corner — bottom right is the OG but you do you bestie |
| **Opacity** | 25% → 100%, from "is that even there?" to "bro that's literally unhinged" |
| **Font Size** | Small → Extra Large, for when you want everyone on the Discord call to witness your aura |
| **Edit Line 1** | Change "Activate Linux" to whatever brainrot you want |
| **Edit Line 2** | Same for the subtitle. go absolutely feral. no one's stopping you. |
| **Exit** | ok bye ig 💔 |

## 🐧 Linux Build (Qt6)

### Dependencies

- Qt6 Widgets >= 6.2 — the backbone, the moment, the legend
- CMake >= 3.16 — it's free smh
- C++17 compiler (g++ or clang++) — you literally already have this bestie

**Wayland** (recommended for full unhinged overlay support):
- `LayerShellQt` — KDE's gift to the Wayland girlies
- `wayland-client` — for input region clearing (click-through, bestie)

On Fedora/KDE:
```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel \
  qt6-qtwayland-devel layershell-qt-devel wayland-devel
```

On Debian/Ubuntu/KDE:
```bash
sudo apt install build-essential cmake qt6-base-dev \
  qt6-wayland liblayershellqt-dev libwayland-dev
```

On X11 or other WMs it falls back gracefully. We don't gatekeep.

### Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

Binary: `build/activate-watermark`

> **Tip:** On Wayland, run with `QT_QPA_PLATFORM=wayland` if your compositor has `zwlr_layer_shell_v1` (KWin, Sway, Hyprland, etc.). Otherwise let it cook on XWayland.

## 🚀 Usage

```bash
./build/activate-watermark
```

That's literally it. No flags. No config files. No tutorial needed. Just run it and let it cook. 🧑‍🍳

To exit: **right-click the tray icon → Exit**.

## 🤔 Why tho?

- Gaslight your friends who flex their "genuine Windows" license 💅
- Make your $2000 Linux rig look like it came from a skibidi toilet
- Assert maximum aura during screen shares
- It's just a silly little prank. a whimsical jape. a goofy endeavor.
- Remind yourself that deep down, we're all just unactivated operating systems trying our best

## ⚠️ Disclaimer

This does **absolutely nothing** to your Windows or Linux activation status. It's literally just text on screen. Touch grass if you're worried about it. It's not that deep fam. 😭

## 📜 License

Do whatever you want with it. It's public domain or whatever. I am not your lawyer. Slay responsibly. 💅

---

## 🪟 Original Windows Version

For the besties still on Windows, the original source lives in the git history:

```powershell
git show 1df9be6:activate_watermark.cpp
```

Built with MSVC + GDI+ because sometimes you just gotta touch Win32 API:

```powershell
cmd /c "call `"D:\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat`" x64 >nul 2>&1 && cl /O2 /W3 /EHsc /D UNICODE /D _UNICODE activate_watermark.cpp /Fe:activate_watermark.exe /link user32.lib gdi32.lib msimg32.lib"
```
