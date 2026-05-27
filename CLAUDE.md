# Caro Game — Claude Context

> 📌 **Đọc file này TRƯỚC khi làm bất kỳ thay đổi nào.**
> File này là entry point chính. Các file chi tiết khác nằm trong `docs/`.

---

## 1. Tóm tắt nhanh dự án

| Field | Giá trị |
|-------|---------|
| **Tên** | Caro Game - Group 1 |
| **Loại** | Đồ án cuối kỳ môn Lập trình C++ — HCMUS |
| **Thể loại** | Caro / Gomoku 15×15 |
| **Tech stack** | C++17 + SFML 2.6.2 |
| **IDE** | Visual Studio 2022 (Windows) |
| **Deadline thuyết trình** | 1/6/2026 |
| **Số người trong nhóm** | 5 (chia 3 nhóm con — xem `docs/PRESENTATION_PLAN.md`) |
| **Người present chính** | Nhật (1 người duy nhất thuyết trình + demo) |

---

## 2. Cấu trúc thư mục

```
Caro_Game_Group_1/
├── CLAUDE.md                    ← Bạn đang đọc
├── README.md                    ← Mô tả ngắn dự án
├── LICENSE                      ← MIT
├── .gitignore
│
├── CaroGame/                    ← Visual Studio solution
│   ├── CaroGame.sln             ← Mở file này trong VS
│   ├── CaroGame.vcxproj
│   ├── sfml-*.dll               ← Pre-built SFML libs
│   └── x64/Debug/CaroGame.exe   ← Output executable
│
├── include/                     ← Header files (.h)
│   ├── game_types.h             ← Core: structs, constants, enums, GameResources
│   ├── render.h                 ← Hàm vẽ + helper (letterbox, mouse map)
│   ├── menu.h                   ← Game loop dispatcher + handle*() functions
│   ├── bot.h                    ← AI: 4 mức độ (Easy → Expert)
│   ├── board.h                  ← Logic bàn cờ, check thắng, undo
│   ├── timer.h                  ← Speed mode timer
│   ├── save_load.h              ← Lưu/tải game (text format)
│   ├── sound.h                  ← BGM + SFX
│   └── language.h               ← TextStrings struct, VN/EN i18n
│
├── src/                         ← Implementation files (.cpp)
│   ├── main.cpp                 ← Entry point (chỉ ~30 dòng)
│   ├── render.cpp               ← ~900 dòng - file lớn nhất
│   ├── menu.cpp                 ← ~1300 dòng - game loop chính
│   ├── bot.cpp, board.cpp, timer.cpp, save_load.cpp, sound.cpp, language.cpp
│
├── assets/                      ← Tài nguyên đã wire
│   ├── fonts/
│   │   ├── title_font.ttf       ← Coiny Regular (bubble cartoon)
│   │   └── main_font.ttf        ← Be Vietnam Pro Regular (UI body)
│   ├── sounds/                  ← 6 file (bgm.ogg + 5 .wav)
│   └── textures/                ← 12 file PNG (xem mục 5)
│
├── docs/                        ← Documentation
│   ├── HANDOVER.md              ← Chi tiết session vừa rồi (đọc thứ 2)
│   ├── ARCHITECTURE.md          ← Chi tiết module + flow (đọc khi cần)
│   ├── ASSETS_PLAN.md           ← Kế hoạch asset (đã hoàn thành)
│   ├── PRESENTATION_PLAN.md     ← 15 slide outline + pseudocode
│   ├── SETUP.md                 ← Hướng dẫn setup VS + SFML
│   ├── build_guide.md           ← Build instructions chi tiết
│   └── GitCommand.md            ← Git workflow nhóm
│
├── libs/                        ← SFML headers (include)
└── saves/                       ← Save game files (.txt)
```

---

## 3. Build & Run

```
1. Mở Visual Studio 2022
2. File → Open → CaroGame/CaroGame.sln
3. Configuration: Debug | x64
4. Bấm F5 (Build + Run)
5. Output: CaroGame/x64/Debug/CaroGame.exe
```

**Build từ command line:**
```powershell
$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
$proj = "D:\HCMUS\Programming C++\TTT\Caro_Project\Caro_Game_Group_1\CaroGame\CaroGame.vcxproj"
& $msbuild $proj /p:Configuration=Debug /p:Platform=x64 /nologo /verbosity:minimal
```

---

## 4. Tính năng đã có (CURRENT STATE — 24/5/2026)

### Gameplay
- ✅ Bàn cờ 15×15, di chuyển W/A/S/D + Enter, đặt quân
- ✅ 2 chế độ: **PvP** (2 người) + **PvC** (vs máy)
- ✅ 4 mức bot: **Easy** (random) / **Medium** (heuristic) / **Hard** (Minimax d3) / **Expert** (Alpha-Beta d4)
- ✅ 2 kiểu chơi: **Basic** (vô hạn) + **Speed** (10 phút/ván, 20s/lượt)
- ✅ Undo (phím Z), Hint (phím H trong PvC)
- ✅ Check thắng theo 4 hướng, highlight 5 quân thắng màu vàng

### UI/UX
- ✅ Main Menu + Submenu (Mode/Difficulty/Style)
- ✅ Background ảnh Dragon Ball xuyên suốt mọi màn hình
- ✅ Logo CARO sticker (texture) trên Main Menu
- ✅ Menu items có button frame bo tròn vàng (texture)
- ✅ Player panel: tên + số bước + thắng + mascot Dragon Ball
- ✅ Mascot đổi state: idle / win / over (theo kết quả)
- ✅ Game Over: VICTORY texture (winner) + DEFEAT texture (loser) trong panel
- ✅ Timer Speed mode: dark plate + outline + đổi đỏ khi ≤ 30s
- ✅ Pause menu (ESC)
- ✅ Save / Load game (text format, max 15 file)
- ✅ Settings: Ngôn ngữ VN/EN + Volume slider kéo được + SFX toggle
- ✅ Help / About screens
- ✅ Window có nút **Maximize + Resize** với **letterbox** (giữ tỷ lệ 16:9)

### i18n
- ✅ Full Vietnamese + English support
- ✅ Switch trong Settings (persist trong `saves/settings.txt`)
- ✅ Input tên Vietnamese (qua Unikey/Telex) — UTF-8 encoded
- ✅ Display dùng `sf::String::fromUtf8`

### Audio
- ✅ BGM nhạc nền (loop)
- ✅ SFX: place quân, move cursor, menu click, win, draw

---

## 5. Asset inventory (đã wire 100%)

### `assets/fonts/` (2 file)
| File | Font gốc | Dùng cho |
|------|---------|---------|
| `title_font.ttf` | **Coiny Regular** | Tiêu đề lớn, "PAUSE", "X chiến thắng!" |
| `main_font.ttf` | **Be Vietnam Pro Regular** | Body, menu items, panel, timer |

### `assets/sounds/` (6 file)
| File | Trigger |
|------|---------|
| `bgm.ogg` | Nhạc nền (loop) |
| `place.wav` | Đặt quân X/O |
| `move.wav` | Di chuyển cursor WASD |
| `menu.wav` | Click menu / Select |
| `win.wav` | Có người thắng |
| `draw.wav` | Hòa cờ |

### `assets/textures/` (12 file PNG)
| File | Mô tả |
|------|-------|
| `background.png` | 1285×720 — Dragon Ball anime scenery (xanh trời + cỏ) |
| `logo_caro.png` | CARO sticker bubble cream (transparent) — Main Menu title |
| `button_frame.png` | Khung gold rounded (transparent) — Background mọi menu item |
| `banner_winner.png` | VICTORY vàng + confetti (transparent) — Trong winner's panel |
| `banner_defeat.png` | DEFEAT đỏ + nứt nẻ (transparent) — Trong loser's panel |
| `x_piece.png` | 36×36 — Quân X (cờ đỏ) |
| `o_piece.png` | 36×36 — Quân O (cờ xanh) |
| `mascot_p1.png` | 200×405 — Goku idle |
| `mascot_p1_Win.png` | 200×405 — Goku UI win pose |
| `mascot_p1_Over.png` | 200×405 — Goku sad/over |
| `mascot_p2.png` | 200×405 — Vegeta idle |
| `mascot_p2_Win.png` | 200×405 — Vegeta SSGSS win |
| `mascot_p2_Over.png` | 200×405 — Vegeta defeated |

---

## 6. Key design decisions (QUAN TRỌNG)

### A. Texture vs Font strategy
- **Texture**: chỉ dùng cho elements **TĨNH + DECORATIVE**:
  - `logo_caro.png` (Main Menu title)
  - `button_frame.png` (menu button bg)
  - `banner_winner.png` / `banner_defeat.png` (game over result)
- **Font**: tất cả còn lại (menu items, names, scores, time, hints, ...)
- **Lý do**: Texture không scale dynamic, không switch ngôn ngữ. Font flexible cho UI dynamic.

### B. Vietnamese (UTF-8) handling
- Tất cả string VN dùng `u8"..."` literal trong `language.cpp`
- Display PHẢI dùng `sf::String::fromUtf8(s.begin(), s.end())`
- `renderTextCentered()` helper đã handle sẵn UTF-8
- Trong renderPlayerPanel, các text dùng `setString` trực tiếp PHẢI map qua `fromUtf8` (đã fix)
- Player name input: chấp nhận mọi Unicode codepoint, encode UTF-8 trước khi append vào std::string

### C. Window letterbox (Resize support)
- Window có `sf::Style::Close | sf::Style::Resize`
- `applyLetterbox()` giữ tỷ lệ 16:9, thêm viền đen khi cần
- `handleCommonEvent()` map mouse pixel → view coords IN-PLACE → mọi mouse hit-test code hiện tại KHÔNG cần đổi
- **GỌI `handleCommonEvent()` ở đầu mọi event loop** (xem pattern trong menu.cpp)

### D. Save/Load architecture
- Format **text-based** (không phải binary)
- `std::ofstream` + `<<`, `std::getline()` cho name
- ✅ UTF-8 safe (multi-byte UTF-8 không chứa `\n`)
- Save file names = ASCII only (filter input)
- Player names = UTF-8 OK (lưu trong file)

### E. Architecture pattern
- **Procedural** (không OOP nặng) — phù hợp class project
- `gameRun()` trong menu.cpp = main game loop dispatcher
- Mỗi screen = 1 hàm `handle*()` với event loop riêng
- `GameState` struct (POD) chứa toàn bộ state
- `GameResources` struct chứa tất cả font/texture/sound (load 1 lần ở main)

---

## 7. Files đã thay đổi trong session vừa rồi

Xem chi tiết trong `docs/HANDOVER.md`. Tóm tắt:

- ✏️ `include/game_types.h` — Thêm 6 mascot tex + 3 UI tex + 3 banner tex
- ✏️ `include/render.h` — Thêm `applyLetterbox`, `handleCommonEvent`, mở rộng `renderTextCentered` với outline
- ✏️ `include/language.h` — Thêm nhiều TextStrings cho i18n
- ✏️ `src/main.cpp` — Resize style + apply letterbox initial
- ✏️ `src/render.cpp` — Phần lớn UI polish ở đây
- ✏️ `src/menu.cpp` — Replace 13 chỗ `Closed` event → `handleCommonEvent`
- ✏️ `src/language.cpp` — Full Vietnamese strings
- ✨ `assets/fonts/title_font.ttf` — Coiny (thay Bagel Fat One vì thiếu dấu Việt)
- ✨ `assets/fonts/main_font.ttf` — Be Vietnam Pro
- ✨ 3 textures mới: logo_caro, button_frame, banner_winner, banner_defeat

---

## 8. Pending tasks (cho deadline 1/6/2026)

### 🔥 Còn lại
1. **Chụp 13 screenshots** từ game (xem danh sách trong `docs/PRESENTATION_PLAN.md` section 3.5)
2. **Nhóm 1 (Đại & Lực)** chèn pseudocode + sơ đồ + screenshot vào PowerPoint
3. **Tập demo 6 phút** (xem demo script trong PRESENTATION_PLAN section 5)
4. **Generate `code_snippets.md`** (optional) — text pseudocode để Nhóm 1 paste vào PowerPoint Text Box

### ⚙️ Optional polish (nếu còn thời gian)
- F11 fullscreen toggle (đã có resize, fullscreen là plus)
- Animation cho VICTORY/DEFEAT banner (scale-in + fade)
- Music volume per-screen (BGM khác cho menu vs gameplay)

---

## 9. Hướng dẫn cho Claude session mới

Khi user gửi tin nhắn đầu tiên, làm theo thứ tự:

1. **Đọc file này** (`CLAUDE.md`) — bạn đang ở đây
2. **Đọc `docs/HANDOVER.md`** — chi tiết session trước
3. **Đọc `docs/PRESENTATION_PLAN.md`** — context về thuyết trình
4. **Skim `docs/ARCHITECTURE.md`** — nếu cần hiểu module
5. **Hỏi user**: "Bạn muốn tiếp tục với phần nào?" (slide, demo, polish code, ...)

### ⚠️ Đừng làm gì TRƯỚC khi user yêu cầu:
- Đừng refactor code (đã ổn định)
- Đừng đổi font/texture mặc định
- Đừng đổi UI layout đã polish
- Đừng generate ASSETS mới

### ✅ Việc THƯỜNG được nhờ:
- Chụp screenshot, mô tả pixel-by-pixel
- Wire asset mới (nếu user generate thêm)
- Fix bug nếu phát hiện
- Tinh chỉnh layout (vị trí/kích thước UI)
- Viết documentation
- Hỗ trợ tạo slide PowerPoint (paste pseudocode, etc.)

---

## 10. Convention quan trọng

### Code comments
- Tiếng Việt **không dấu** trong code (vd `// Khoi tao font`) — tránh encoding issue trong VS
- Tiếng Việt **có dấu** trong UI strings (vd `u8"Lưu thành công!"`)

### Naming
- `snake_case_lowercase` cho file (vd `game_types.h`, `save_load.cpp`)
- `camelCase` cho function (vd `renderMainMenu`, `applyLetterbox`)
- `PascalCase` cho struct (vd `GameState`, `TextStrings`)
- `SCREAMING_CASE` cho constant/macro (vd `BOARD_SIZE`, `UI_MENU_STEP`)
- `ENUM_VALUE_CASE` cho enum (vd `SCREEN_PLAYING`, `MODE_PVC`)

### File path trong code
- Dùng relative path từ exe location: `"../assets/textures/..."`
- exe location = `CaroGame/x64/Debug/` nên `..` về `CaroGame/`, rồi `..` về root

### Pseudocode convention (cho slides)
- UPPERCASE keywords (FUNCTION, IF, FOR, RETURN)
- `←` cho assignment (không phải `=`)
- `≤`, `≥`, `≠` cho comparison
- Indent 4 spaces

---

## 11. Liên hệ với existing docs

| File | Nội dung |
|------|---------|
| `docs/HANDOVER.md` | 📝 Chi tiết session trước (đọc thứ 2 sau CLAUDE.md) |
| `docs/ARCHITECTURE.md` | 🏗️ Module flow + data flow |
| `docs/PRESENTATION_PLAN.md` | 🎤 15-slide outline, pseudocode, demo script |
| `docs/ASSETS_PLAN.md` | 🎨 Asset spec (đã complete) |
| `docs/SETUP.md` | ⚙️ Setup VS + SFML từ đầu |
| `docs/build_guide.md` | 🔨 Build instructions chi tiết |
| `docs/GitCommand.md` | 🌿 Git workflow cho team |

---

**Last updated:** 24/05/2026
**By:** Claude (anthropic) — session với @vanhoangnhatbp1
**Status:** Game ~95% complete, sẵn sàng demo sau khi chụp screenshot + làm slide
