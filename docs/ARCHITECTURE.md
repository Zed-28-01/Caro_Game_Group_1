# Architecture — Caro Game

> 📌 File này mô tả **cấu trúc code + data flow** chi tiết. Đọc khi cần hiểu module trước khi modify.

---

## 1. High-level overview

```
┌──────────────────────────────────────────────────────────────┐
│                         main.cpp                              │
│   - Tạo sf::RenderWindow                                      │
│   - Khởi tạo GameResources (load font/texture/sound)          │
│   - Gọi gameRun(window, res) → main game loop                 │
└──────────────────────┬───────────────────────────────────────┘
                       ↓
┌──────────────────────────────────────────────────────────────┐
│                   gameRun() (menu.cpp)                        │
│   while (window.isOpen()):                                    │
│       switch (currentScreen):                                 │
│           SCREEN_MAIN_MENU → handleMainMenu()                 │
│           SCREEN_MODE_SELECT → handleModeSelect()             │
│           SCREEN_DIFFICULTY → handleDifficulty()              │
│           SCREEN_STYLE_SELECT → handleStyleSelect()           │
│           SCREEN_INPUT_NAMES → handleInputNames()             │
│           SCREEN_PLAYING → handleGameplay()                   │
│           SCREEN_PAUSE_MENU → handlePauseMenu()               │
│           SCREEN_GAME_OVER → handleGameOver()                 │
│           SCREEN_SAVE → handleSaveScreen()                    │
│           SCREEN_LOAD → handleLoadScreen()                    │
│           SCREEN_SETTINGS → handleSettings()                  │
│           SCREEN_HELP → handleHelp()                          │
│           SCREEN_ABOUT → handleAbout()                        │
└──────────────────────────────────────────────────────────────┘
```

Mỗi `handle*()` function:
1. Có event loop riêng (`while window.pollEvent`)
2. Gọi `handleCommonEvent()` đầu loop (Closed, Resized, mouse map)
3. Xử lý input chuyên biệt cho screen đó
4. Gọi `render*()` để vẽ
5. `window.display()`
6. Return `GameScreen` next state khi user navigate

---

## 2. Module breakdown

### 2.1. `game_types.h` — Core types
```cpp
// Constants
#define BOARD_SIZE      15
#define CELL_SIZE       40
#define WINDOW_WIDTH    1280
#define WINDOW_HEIGHT   720
#define WIN_COUNT       5
#define MAX_SAVE_FILES  15
// + UI layout constants (UI_MENU_*, UI_PANEL_*, UI_GAMEOVER_*, ...)

// Enums
enum GameScreen { SCREEN_MAIN_MENU, SCREEN_PLAYING, ... };
enum GameMode { MODE_PVP, MODE_PVC };
enum GameStyle { STYLE_BASIC, STYLE_SPEED };
enum BotDifficulty { BOT_EASY, BOT_MEDIUM, BOT_HARD, BOT_EXPERT };
enum GameResult { RESULT_NONE, RESULT_PLAYER1_WIN, RESULT_PLAYER2_WIN, RESULT_DRAW };
enum Language { LANG_VIETNAMESE, LANG_ENGLISH };

// Structs
struct Cell { int value; };          // 0=empty, -1=X, 1=O
struct Move { int row, col; int player; };
struct Player { std::string name; int moves, totalWins; };
struct TimerState { float gameTimeLeft, turnTimeLeft; bool isRunning; };
struct WinLine { int positions[WIN_COUNT][2]; int count; };

// GameState (POD-ish, dùng cho save/load)
struct GameState {
    Cell board[BOARD_SIZE][BOARD_SIZE];
    Player player1, player2;
    bool isPlayer1Turn;
    int cursorRow, cursorCol;
    GameMode mode;
    GameStyle style;
    BotDifficulty difficulty;
    TimerState timer;
    Move moveHistory[BOARD_SIZE * BOARD_SIZE];
    int moveCount;
};

// GameResources (load 1 lần trong main.cpp)
struct GameResources {
    sf::Font mainFont, titleFont;
    sf::Texture backgroundTex, boardTex, xPieceTex, oPieceTex;
    sf::Texture mascotP1Idle, mascotP1Win, mascotP1Over;
    sf::Texture mascotP2Idle, mascotP2Win, mascotP2Over;
    sf::Texture logoCaroTex, bannerWinTex, bannerDefeatTex, buttonFrameTex;
    sf::SoundBuffer moveSfx, placeSfx, winSfx, drawSfx, menuSfx;
    sf::Music bgMusic;
};
```

### 2.2. `render.cpp` — Rendering (file lớn nhất ~900 dòng)

**Hàm public:**
- `renderLoadResources(res)` — load tất cả assets từ `../assets/`
- `applyLetterbox(window, w, h)` — set view với letterbox 16:9
- `handleCommonEvent(window, event)` — map mouse + handle Closed/Resized
- `renderTextCentered(window, font, text, size, x, y, color, outlineColor?, outlineThickness?)` — text center + UTF-8 + outline

**Hàm vẽ screen:**
- `renderMainMenu`, `renderModeSelect`, `renderDifficultySelect`, `renderStyleSelect` → đều gọi `renderMenuGeneric()`
- `renderPauseMenu`, `renderGameOver`, `renderSettings`, `renderHelp`, `renderAbout` → tự vẽ
- `renderInputNames(window, res, name1, name2, isEditingP1, showError, isPvC)` — PvC vs PvP UI khác
- `renderSaveScreen`, `renderLoadScreen` — file list + hint
- `renderGameplay(window, state, res, winLine, hintRow, hintCol, showHint, result?)` — board + pieces + panel + timer

**Hàm helper internal:**
- `renderBackdrop(window, res, dimForMenu)` — background ảnh + (optional) overlay tối
- `renderBoard(window, res)` — procedural grid 15×15 + semi-transparent panel
- `renderPieces(window, state, res)` — vẽ X/O sprites
- `renderCursor(window, row, col)` — highlight cursor đang chọn
- `renderHint(window, row, col)` — highlight hint suggest
- `renderWinLine(window, winLine)` — highlight 5 quân thắng màu vàng
- `renderPlayerPanel(window, state, res, result?)` — 2 player boxes + mascots + VICTORY/DEFEAT
- `renderTurnTimer(window, res, percentage)` — Speed mode timer bar
- `renderGameTimer(window, res, secondsLeft)` — Speed mode countdown
- `drawCaroLogo(window, res)` — vẽ logo CARO ở Main Menu
- `drawMenuButton(window, res, centerY, selected)` — vẽ button frame sau menu item
- `renderMenuGeneric(window, res, title, items, count, menuIndex, useLogo?)` — vẽ menu chung

**Coords system:**
- `renderBoardToPixel(row, col)` → `sf::Vector2f` (pixel coord trong view)
- `renderPixelToBoard(x, y, &row, &col)` → bool (true nếu trong board)

### 2.3. `menu.cpp` — Game flow (file lớn thứ 2 ~1300 dòng)

**Hàm public chính:**
- `gameRun(window, res)` — Main dispatcher loop

**Mỗi screen có 1 hàm `handle*()`:**

```cpp
GameScreen handleMainMenu(window, res, state) {
    int menuIndex = 0;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;   // ← LUÔN gọi đầu loop
            // ... handle specific events for this screen
        }
        renderMainMenu(window, res, menuIndex);
        window.display();
    }
    return SCREEN_X;  // next screen to navigate to
}
```

**Helper:**
- `menuHitTest(mx, my, startY, step, count, halfW?, halfH?)` — check mouse có hit menu item nào

**Lambda bên trong:**
- `handleGameplay` có `doBotMove()` lambda
- `handleInputNames` có `utf32ToUtf8()` + `utf8PopBack()` lambdas cho UTF-8 input
- `handleSettings` có `setVolumeFromX()`, `isOnSlider()` lambdas cho slider

### 2.4. `bot.cpp` — AI logic
```cpp
Move botEasyMove(state)        // Random valid move + chặn cơ bản
Move botMediumMove(state)      // Heuristic scoring (count chains)
Move botHardMove(state)        // Minimax depth 3
Move botExpertMove(state)      // Minimax depth 4 + Alpha-Beta + move ordering
Move botGetMove(state)         // Dispatcher theo state.difficulty
```

### 2.5. `board.cpp` — Board logic
```cpp
void boardResetAll(state)              // Reset board + player stats về 0
void boardPlaceMove(state, r, c, p)    // Đặt quân + record move history
void boardUndo(state)                  // Pop từ moveHistory
GameResult boardCheckWin(state, &winLine)  // Check 4 directions, return result
bool boardIsFull(state)                // Check hòa
```

### 2.6. `timer.cpp` — Speed mode
```cpp
void timerStart(timer, gameSeconds, turnSeconds)
void timerUpdate(timer, deltaTime)     // Trừ thời gian theo dt
void timerResetTurn(timer, turnSeconds) // Reset khi sang lượt mới
float timerGetTurnPercent(timer)       // 0.0 → 1.0 cho progress bar
float timerGetGameSecondsLeft(timer)
bool timerIsTimeUp(timer)              // Game time hoặc turn time hết
```

### 2.7. `save_load.cpp` — Persistence
- **Format:** Text-based (không phải binary fwrite)
- **File:** `saves/<name>.txt`
- **List file:** `saves/Gamelist.txt` (danh sách save files)

```cpp
bool saveGame(state, filename)    // Write GameState to text file
bool loadGame(state, filename)    // Read GameState from text file
int saveGetList(saveList[], maxCount)
bool saveAddToList(filename)
bool saveDeleteFile(filename)
bool saveRenameFile(oldName, newName)
bool saveFileExists(filename)
int saveCountFiles()
```

### 2.8. `sound.cpp` — Audio
- **Pool 8 `sf::Sound`** để chống chồng nhau
- BGM dùng `sf::Music` (streaming, không load vào RAM)

```cpp
void soundLoadAll(res)             // Setup BGM loop + volume
void soundPlayBGM(res, play)
void soundSetBGMVolume(res, volume) // 0-100
void soundPlayMove/Place/Select/Win/Draw(res)
void soundSetSFXEnabled(bool)
void settingsLoad(res)             // Đọc saves/settings.txt
void settingsSave()                // Ghi saves/settings.txt
```

### 2.9. `language.cpp` — i18n
- 2 ngôn ngữ: VN + EN
- `TextStrings` struct với ~60 string fields
- VN dùng `u8"..."` literal cho UTF-8
- `langGetText(lang)` → returns TextStrings
- `langToggle()` → switch giữa VN/EN

---

## 3. Data flow

### 3.1. Khởi tạo
```
main.cpp
   ↓
new sf::RenderWindow (Close|Resize)
   ↓
applyLetterbox(window, 1280, 720)
   ↓
GameResources res
   ↓
renderLoadResources(res)  → Load font + 12 texture + 6 sound + 1 music
   ↓
settingsLoad(res)          → Đọc saves/settings.txt (lang, volume, sfx)
   ↓
gameRun(window, res)       → Main loop
```

### 3.2. Game flow điển hình
```
Main Menu
   → Chơi mới
   → Mode Select (PvP / PvC)
   → [if PvC] Difficulty Select (Easy/Medium/Hard/Expert)
   → Style Select (Basic / Speed)
   → Input Names
       → Player 1 name (UTF-8 from Telex)
       → Player 2 name (skip if PvC, auto = "Máy")
   → SCREEN_PLAYING (handleGameplay)
       → Loop: input → place → check win → switch player
       → ESC → SCREEN_PAUSE_MENU
       → L → SCREEN_SAVE
       → Game end → SCREEN_GAME_OVER
   → Game Over: hiện VICTORY/DEFEAT trong panel
       → Yes → reset board, play again
       → No → SCREEN_MAIN_MENU
```

### 3.3. Save/Load flow
```
Save (in-game ESC → Lưu):
   handleSaveScreen
       → User nhập tên file
       → saveGame(state, name)  → write text file
       → saveAddToList(name)    → add to Gamelist.txt
       → Notification "Lưu thành công!"

Load (Main Menu → Tải Game):
   handleLoadScreen
       → saveGetList()          → read Gamelist.txt
       → User chọn file
       → loadGame(state, name)  → parse text → fill GameState
       → SCREEN_PLAYING với state đã load
```

---

## 4. Coordinate system

### Window vs View
- **Window pixel space:** thật của OS window (có thể 1280x720 hoặc 1920x1080 khi maximize)
- **View space (game space):** luôn là **1280×720** (logical coords)
- `applyLetterbox()` map view → viewport (với letterbox)
- Mouse events nhận PIXEL coords → `handleCommonEvent()` map về VIEW coords IN-PLACE

### Board coords
- `BOARD_OFFSET_X = 40`, `BOARD_OFFSET_Y = 60` (view space)
- `CELL_SIZE = 40` pixels
- Board area: x=40..640, y=60..660 (15×40 = 600px each side)
- `renderBoardToPixel(row, col)` → center của ô (view coords)

### Panel coords
- `panelX = BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + UI_PANEL_GAP_LEFT = 40+600+40 = 680`
- `panelW = WINDOW_WIDTH - panelX - UI_PANEL_GAP_RIGHT = 1280-680-20 = 580`
- 2 player boxes:
  - Box P1: y=60..230 (height 170)
  - Box P2: y=250..420 (step 190)
- Timer area: y=432..525 (Speed mode only)
- Game Over plate: y=550..730 (panel bottom)

---

## 5. Event handling pattern

**Mọi event loop có pattern:**
```cpp
while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        // 1. ALWAYS gọi đầu - handle Closed + Resized + map mouse
        if (handleCommonEvent(window, event)) continue;

        // 2. Mouse hover update
        if (event.type == sf::Event::MouseMoved) {
            // event.mouseMove.x/y ĐÃ được map sang view coords
            int hit = menuHitTest(event.mouseMove.x, event.mouseMove.y, ...);
        }

        // 3. Mouse click
        if (event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left) {
            // event.mouseButton.x/y ĐÃ được map sang view coords
        }

        // 4. Keyboard
        if (event.type == sf::Event::KeyPressed) {
            switch (event.key.code) {
                case sf::Keyboard::Enter: ...
                case sf::Keyboard::Escape: ...
            }
        }
    }

    // 5. Render
    renderXxx(window, ...);
    window.display();
}
```

---

## 6. Adding a new screen (vd nếu cần thêm "Statistics")

### Steps:
1. Add `SCREEN_STATISTICS` vào `enum GameScreen` trong `game_types.h`
2. Tạo `renderStatistics()` trong `render.cpp` + declare trong `render.h`
3. Tạo `handleStatistics()` trong `menu.cpp` + declare trong `menu.h`
4. Add case `SCREEN_STATISTICS` vào `gameRun()` dispatcher
5. Add menu item "Thống kê" vào Main Menu (nếu cần navigate vào)
6. Add language strings nếu cần

---

## 7. Adding a new asset

### Texture:
1. Generate/download PNG → put in `assets/textures/`
2. Add `sf::Texture xxxTex` field vào `GameResources` (game_types.h)
3. Load trong `renderLoadResources()`:
   ```cpp
   res.xxxTex.loadFromFile("../assets/textures/xxx.png");
   res.xxxTex.setSmooth(true);
   ```
4. Vẽ trong render function nào cần:
   ```cpp
   sf::Sprite s(res.xxxTex);
   s.setOrigin(s.getTexture()->getSize().x / 2.f, ...);
   s.setPosition(...);
   window.draw(s);
   ```

### Sound:
1. Put `.wav` hoặc `.ogg` in `assets/sounds/`
2. Add `sf::SoundBuffer xxxSfx` vào `GameResources`
3. Load: `res.xxxSfx.loadFromFile("../assets/sounds/xxx.wav");`
4. Add `void soundPlayXxx(res)` trong `sound.h/cpp`
5. Call where needed

### Font:
- Chỉ có 2 slot (title + main). Đổi font = thay file trong `assets/fonts/` (no code change).

---

## 8. Common pitfalls

### Pitfall 1: UTF-8 mojibake khi setString
```cpp
// ❌ SAI:
text.setString(p.name);   // UTF-8 bytes interpreted as Latin-1 → "MÃiy"

// ✅ ĐÚNG:
text.setString(sf::String::fromUtf8(p.name.begin(), p.name.end()));
```

### Pitfall 2: PNG transparency mất khi process
```powershell
# ❌ SAI: Load PNG 24-bit, modify alpha, save → mất alpha
$img = [Bitmap]::new($path)
# ... process bytes
$img.Save($path)  # Saves as 24-bit, alpha discarded

# ✅ ĐÚNG: Convert sang 32-bit Argb trước
$newImg = [Bitmap]::new($w, $h, [PixelFormat]::Format32bppArgb)
$gfx = [Graphics]::FromImage($newImg)
$gfx.DrawImage($srcImg, 0, 0, $w, $h)
# Now process $newImg
```

### Pitfall 3: Mouse coords sai sau resize
- Window resize → `event.mouseButton.x` ở pixel coords (vd 1920 khi window full)
- Code hit-test compare với view coords (vd 640 cho centerX)
- **Fix:** `handleCommonEvent()` đã map sẵn IN-PLACE. **PHẢI gọi nó đầu event loop.**

### Pitfall 4: Font thiếu Vietnamese diacritics
- Google Fonts ghi "Vietnamese" nhưng không phải lúc nào cũng có ĐẦY ĐỦ
- ✅ Test trước trên Google Fonts preview với "Ơ Ư Ậ Ọ Ữ Ằ Ề" trước khi tải
- **Safe fonts:** Be Vietnam Pro, Coiny, Lobster, Pacifico

---

## 9. Critical files quick reference

| File | Khi nào sửa? |
|------|-------------|
| `game_types.h` | Thêm const, struct, enum mới |
| `render.h/cpp` | UI/rendering changes |
| `menu.h/cpp` | Game flow, event handling |
| `language.h/cpp` | Thêm/sửa text strings VN/EN |
| `bot.cpp` | AI logic tweak |
| `board.cpp` | Game rules (win check, move logic) |
| `main.cpp` | Hiếm khi sửa (chỉ window setup) |

---

**Last updated:** 24/05/2026
