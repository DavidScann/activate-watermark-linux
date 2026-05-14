# Activate Watermark

A humorous desktop overlay that mimics the "Activate Windows" watermark, ported to Linux.

## Linux Build (Qt6)

### Dependencies

- Qt6 (Widgets module) >= 6.2
- CMake >= 3.16
- C++17 compiler (g++ or clang++)

**Wayland** (recommended for full overlay support):
- LayerShellQt (KDE library for `zwlr-layer-shell` protocol)
- Qt6 WaylandClient module

On Fedora/KDE:
```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel \
  qt6-qtwayland-devel layershell-qt-devel
```

On Debian/Ubuntu/KDE:
```bash
sudo apt install build-essential cmake qt6-base-dev \
  qt6-wayland liblayershellqt-dev
```

On other distros/WMs, the app falls back to X11 (XWayland) mode automatically.

### Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

Binary: `build/activate-watermark`

> **Tip:** On Wayland, run with `QT_QPA_PLATFORM=wayland` if your compositor supports the `zwlr_layer_shell_v1` protocol (KWin, Sway, Hyprland, etc.).

### Features

- Semi-transparent text overlay on desktop
- Click-through (does not intercept mouse events)
- Always on top of other windows
- System tray icon with right-click menu:
  - **Position:** Bottom Right, Bottom Left, Top Right, Top Left
  - **Opacity:** 25%, 50%, 75%, 100%
  - **Font Size:** Small, Medium, Large, Extra Large
  - **Edit Line 1... / Edit Line 2...:** Customize the displayed text
  - **Exit:** Quit the application

### Windows Build (Original)

See git history for the original MSVC build using `cl.exe` and GDI+.
