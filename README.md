# Caro Game — Group 1

A polished Caro/Gomoku 15×15 game built with **C++17 + SFML 2.6.2**.
University project — HCMUS Programming C++ course.

![Tech](https://img.shields.io/badge/C%2B%2B-17-blue)
![SFML](https://img.shields.io/badge/SFML-2.6.2-green)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey)

---

## ✨ Features

- 🎮 **2 modes**: Player vs Player + Player vs Computer
- 🤖 **4 AI levels**: Easy / Medium / Hard / Expert (Minimax + Alpha-Beta)
- ⏱️ **2 play styles**: Basic (unlimited) + Speed Mode (10min/match, 20s/turn)
- 💾 **Save / Load** game (text format, max 15 saves)
- 🌐 **i18n**: Vietnamese + English (toggle in Settings)
- 🎨 **UI polish**: Dragon Ball mascots, sticker logos, button frames
- 🔊 **Audio**: BGM + 5 SFX
- 🖼️ **Window**: Maximize + Resize with letterbox (preserves 16:9)
- ↩️ **Undo, Hint** (PvC only), Pause Menu

---

## 🚀 Quick Start

### Requirements
- Windows 10/11
- Visual Studio 2022 with C++ Desktop Development workload

### Build & Run
1. Clone repo
2. Open `CaroGame/CaroGame.sln` in Visual Studio
3. Configuration: **Debug | x64**
4. Press **F5**

Output: `CaroGame/x64/Debug/CaroGame.exe`

### Vietnamese input
Install [Unikey](https://www.unikey.org/) or [EVKey](https://github.com/lamquangminh/EVKey)
to type Vietnamese names in the game.

---

## 📁 Project Structure

```
Caro_Game_Group_1/
├── CLAUDE.md           ← Project context (read first if you're Claude)
├── README.md           ← You are here
├── CaroGame/           ← Visual Studio solution
├── include/            ← Headers (.h)
├── src/                ← Sources (.cpp)
├── assets/             ← Fonts, sounds, textures
├── docs/               ← Documentation
├── libs/               ← SFML headers
└── saves/              ← Save files (.txt)
```

See `CLAUDE.md` and `docs/ARCHITECTURE.md` for details.

---

## 📚 Documentation

| File | Purpose |
|------|---------|
| [`CLAUDE.md`](CLAUDE.md) | Quick project context for new contributors / AI assistants |
| [`docs/HANDOVER.md`](docs/HANDOVER.md) | Detailed change log of recent work |
| [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) | Code structure, modules, data flow |
| [`docs/PRESENTATION_PLAN.md`](docs/PRESENTATION_PLAN.md) | 15-slide presentation outline + pseudocode |
| [`docs/ASSETS_PLAN.md`](docs/ASSETS_PLAN.md) | Asset spec + AI generation prompts |
| [`docs/SETUP.md`](docs/SETUP.md) | First-time setup (VS + SFML) |
| [`docs/build_guide.md`](docs/build_guide.md) | Detailed build instructions |
| [`docs/GitCommand.md`](docs/GitCommand.md) | Git workflow for team |

---

## 🎮 Controls

| Key | Action |
|-----|--------|
| **W/A/S/D** or **Arrows** | Move cursor |
| **Enter** or **Mouse Click** | Place piece |
| **Z** | Undo last move |
| **H** | Show hint (PvC only) |
| **ESC** | Pause / Back |
| **L** | Save game (in-game) |
| **F11** | (planned) Fullscreen toggle |
| **Maximize button** | Resize window |

---

## 👥 Team

- **Project Lead + Presenter**: Nhật
- **Slides + Pseudocode (Group 1)**: Đại, Lực
- **Core Code (Group 2)**: Vũ Quang
- **UI Polish + Demo (Group 3)**: Nhật, Toàn

---

## 📜 License

MIT — see [LICENSE](LICENSE)

---

**Built with ❤️ for HCMUS Programming C++ course, 2026.**
