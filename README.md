# 💀 Activate Windows Watermark

> *no cap this is the most unhinged thing i've ever coded and i'm lowkey proud of it*

A tiny Win32 program that slaps the classic **"Activate Windows"** watermark on your desktop. Always on top. Click-through. Just like the real thing — except you're doing it *on purpose*. absolute cinema.

## ✨ Features

- 🎯 **Pixel-perfect replica** — it's giving authentic Windows 10 watermark energy fr fr
- 👻 **Click-through & transparent** — this thing is more ghosted than your last situationship
- 📌 **Always on top** — no window can ratio this watermark, it stays winning
- 🔧 **System tray controls** — right-click the tray icon to customize everything, it's bussin
- 🚫 **No taskbar icon** — stealth mode activated, this thing is the main character with zero screen time

## 🎮 Tray Menu

Right-click the ℹ️ icon in your system tray, it's giving full customization:

| Option | What it does |
|---|---|
| **Position** | Move it to any corner — bottom right is the OG but you do you bestie |
| **Opacity** | 25% → 100%, from "is that even there?" to "bro that's literally unhinged" |
| **Font Size** | Small → Extra Large, for when you want everyone on the Discord call to witness your aura |
| **Edit Line 1** | Change "Activate Windows" to whatever brainrot you want |
| **Edit Line 2** | Same for the subtitle. go absolutely feral. no one's stopping you. |
| **Exit** | ok bye ig 💔 |

## 🔨 Building

Requires **Visual Studio** (MSVC). if you don't have it just download it it's free smh

```powershell
cmd /c "call `"D:\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat`" x64 >nul 2>&1 && cl /O2 /W3 /EHsc /D UNICODE /D _UNICODE activate_watermark.cpp /Fe:activate_watermark.exe /link user32.lib gdi32.lib msimg32.lib"
```

## 🚀 Usage

```
activate_watermark.exe
```

That's literally it. No flags. No config files. No tutorial needed. Just run it and let it cook. 🧑‍🍳

To exit: **right-click the tray icon → Exit**.

Or just yeet it from Task Manager. We don't judge bestie.

## 🤔 Why tho?

- Gaslight your friends who flex their "genuine Windows" license 💅
- Make your $2000 gaming rig look like it came from a skibidi toilet
- Assert maximum aura during screen shares
- It's just a silly little prank. a whimsical jape. a goofy endeavor.
- Remind yourself that deep down, we're all just unactivated Windows installations trying our best

## ⚠️ Disclaimer

This does **absolutely nothing** to your Windows activation status. It's literally just text on screen. Touch grass if you're worried about it. It's not that deep fam. 😭

## 📜 License

Do whatever you want with it. It's public domain or whatever. I am not your lawyer. Slay responsibly. 💅
