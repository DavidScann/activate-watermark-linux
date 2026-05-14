# Activate Watermark

A humorous desktop overlay that mimics the "Activate Windows" watermark, ported to Linux.

## Linux Build (Qt6)

### Dependencies

- Qt6 (Widgets module) >= 6.2
- CMake >= 3.16
- C++17 compiler (g++ or clang++)

On Debian/Ubuntu:
```bash
sudo apt install build-essential cmake qt6-base-dev
```

On Fedora:
```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel
```

### Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

Binary: `build/activate-watermark`

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
