# Architecture — Caro Game (V2)

> 📌 File này mô tả **cấu trúc code + data flow** chi tiết theo trạng thái **hiện tại (V2)**.
> Đọc khi cần hiểu module trước khi modify. Các chữ ký hàm dưới đây lấy trực tiếp từ header.

---

## 1. High-level overview

```
┌──────────────────────────────────────────────────────────────┐
│                         main.cpp (~37 dòng)                    │
│   - srand(time)                                                │
│   - Tạo sf::RenderWindow (Close | Resize), framerate 60        │
│   - applyLetterbox(window, 1280, 720)                          │
│   - GameResources res                                          │
│   - renderLoadResources(res)   → font + texture + shader + bg  │
│   - soundLoadResources(res)    → load SFX (module sound)       │
│   - settingsLoad(res)          → đọc saves/settings.txt        │
│   - gameRun(window, res)       → main game loop                │
│   - soundShutdown()            → dừng + gỡ buffer pool          │
└──────────────────────┬─────────────────────────────────────────┘
                       ↓
┌──────────────────────────────────────────────────────────────┐
│                   gameRun() (menu.cpp)                         │
│   while (window.isOpen()):                                    │
│       soundPlayBGMTrack(PLAYING ? BGM_GAME : BGM_MENU)        │
│       switch (currentScreen):                                 │
│           SCREEN_MAIN_MENU   → handleMainMenu()               │
│           SCREEN_MODE_SELECT → handleModeSelect()             │
│           SCREEN_DIFFICULTY  → handleDifficultySelect()       │
│           SCREEN_STYLE_SELECT→ handleStyleSelect()            │
│           SCREEN_CHAR_SELECT → handleCharSelect()   (V2 #34)  │
│           SCREEN_INPUT_NAMES → handleInputNames()             │
│           SCREEN_PLAYING     → handleGameplay()               │
│           SCREEN_SETTINGS    → handleSettings()               │
│           SCREEN_HELP        → handleHelp()                   │
│           SCREEN_ABOUT       → handleAbout()                  │
│           SCREEN_LOAD        → handleLoadScreen()             │
│           default            → SCREEN_MAIN_MENU   (R7 guard)  │
└──────────────────────────────────────────────────────────────┘
```

> ⚠️ **3 màn KHÔNG được dispatch bởi `gameRun`:** `SCREEN_PAUSE_MENU`, `SCREEN_GAME_OVER`, `SCREEN_SAVE`. Chúng là **màn lồng (nested)** — gọi trực tiếp dạng hàm từ trong `handleGameplay` (Pause/Save) hoặc `handleGameOver` (Save), rồi `return` giá trị navigation lên `gameRun`. Vì vậy 3 enum này tồn tại trong `enum GameScreen` nhưng không bao giờ rơi vào `switch`; `default` đưa mọi giá trị lạ về `SCREEN_MAIN_MENU` (audit R7).

Mỗi `handle*()` function:
1. Có event loop riêng (`while window.pollEvent`)
2. Gọi `handleCommonEvent()` **đầu loop** (Closed, Resized, map mouse pixel→view IN-PLACE)
3. Xử lý input chuyên biệt cho screen đó
4. Gọi `render*()` để vẽ → `window.display()`
5. Return `GameScreen` next state khi user navigate

---

## 2. Module breakdown

```
main.cpp → menu.cpp (orchestrator)
   ├─ board.cpp / bot.cpp / timer.cpp / save_load.cpp ──┐
   │                                                    ├─→ utils.h/.cpp (DIRS, inBounds, countConsecutive)
   ├─ sound.cpp / language.cpp / render.cpp ─────────────┘
   │                                   └─→ rounded_rect.h (RoundedRectangleShape)
   └─ game_types.h = từ điển chung (hằng số / enum / struct), MỌI module include
```
Phụ thuộc **một chiều, không vòng**. Logic cờ (board/bot/timer) tách hẳn khỏi render.

### 2.1. `game_types.h` — Core types (hằng số, enum, struct, GameResources)
```cpp
// --- Hằng số game ---
#define BOARD_SIZE 15        #define CELL_SIZE 40
#define WINDOW_WIDTH 1280     #define WINDOW_HEIGHT 720
#define WIN_COUNT 5
#define CELL_P1 (-1)          #define CELL_P2 1     // 1 nguồn mã hóa người chơi (audit #4)
#define MAX_GAME_TIME 600     #define MAX_TURN_TIME 20
#define TURN_ALARM_SECONDS 5.f                       // V2 #27: reo chuông khi lượt còn <=5s
// (KHÔNG còn MAX_SAVE_FILES — V2 #29 bỏ giới hạn, dùng directory scan)
// + UI layout constants: UI_MENU_*, UI_SETTINGS_*, UI_SAVE_*, UI_LOAD_*, UI_LIST_*,
//   UI_PAUSE_TITLE_Y/START_Y/STEP (audit D10 — 1 nguồn), UI_BOARD_OFFSET_X/Y,
//   UI_PANEL_*, UI_TIMER_*, UI_BOT_THINKING_Y, UI_GAMEOVER_*, UI_LIST_VISIBLE=10

// --- Enums ---
enum GameScreen { SCREEN_MAIN_MENU, SCREEN_MODE_SELECT, SCREEN_DIFFICULTY,
                  SCREEN_STYLE_SELECT, SCREEN_CHAR_SELECT, SCREEN_INPUT_NAMES,
                  SCREEN_PLAYING, SCREEN_PAUSE_MENU, SCREEN_GAME_OVER,
                  SCREEN_SAVE, SCREEN_LOAD, SCREEN_SETTINGS, SCREEN_HELP, SCREEN_ABOUT };
enum GameMode { MODE_PVP, MODE_PVC };
enum GameStyle { STYLE_BASIC, STYLE_SPEED };
enum BotDifficulty { BOT_EASY, BOT_MEDIUM, BOT_HARD, BOT_EXPERT };
enum GameResult { RESULT_NONE, RESULT_PLAYER1_WIN, RESULT_PLAYER2_WIN, RESULT_DRAW };
enum Language { LANG_VIETNAMESE, LANG_ENGLISH };

// --- Structs (đều có default member initializer — tránh C26495) ---
struct Cell { int value = 0; };   // 0=trống, CELL_P1=P1, CELL_P2=P2 (X/O hiển thị động theo firstPlayerOfRound)
struct Move { int row = 0, col = 0; int player = 0; };  // player = CELL_P1/CELL_P2
struct Player { std::string name; int moves = 0; int totalWins = 0; };
struct TimerState {                      // chess-clock: mỗi người 1 đồng hồ riêng
    float gameTimeLeftP1, gameTimeLeftP2, turnTimeLeft;
    bool isRunning;
    bool turnAlarmFired;                 // V2 #27: đã reo chuông cho lượt này chưa
};
struct WinLine { int positions[WIN_COUNT][2]; int count; };  // count = số quân thắng thực tế, CÓ THỂ > 5

struct GameState {                       // toàn bộ trạng thái 1 ván (dùng cho save/load)
    Cell board[BOARD_SIZE][BOARD_SIZE];
    Player player1, player2;
    bool isPlayer1Turn;
    int cursorRow, cursorCol;
    GameMode mode; GameStyle style; BotDifficulty difficulty;
    TimerState timer;
    Move moveHistory[BOARD_SIZE * BOARD_SIZE]; int moveCount;
    int firstPlayerOfRound;              // V2 #23: ai đi trước ván này (1=P1, 2=P2). Người thua ván trước đi trước + đánh X
    int heroP1, heroP2;                  // V2 #34: hero chọn ở Character Select (index roster 0..5)
};

struct MascotSet { sf::Texture idle, win, lose; };  // V2: 3 tư thế / nhân vật (win/lose thiếu → fallback idle + tint)

struct GameResources {                   // load 1 lần trong main, truyền tham chiếu
    sf::Font mainFont, titleFont;
    sf::Texture backgroundTex;
    static const int BG_FRAME_COUNT = 120;
    sf::Texture bgFrames[BG_FRAME_COUNT]; int bgFrameCount;   // V2 #35: nền động (~106 frame .jpg, ping-pong 18fps)
    sf::Texture xPieceTex, oPieceTex;
    MascotSet heroGoku, heroVegeta, heroGohan, heroTrunks, heroKrillin, heroPiccolo; // 6 hero (gameplay full-body)
    MascotSet villainFrieza, villainCell, villainBuu, villainBroly;                  // 4 villain theo độ khó (PvC)
    sf::Texture modePvpTex, modePvcTex;                          // 2 tile màn chọn chế độ
    sf::Texture heroAvatar[6], villainAvatar[4];                 // V2 #34/#36: ảnh chân dung màn chọn
    sf::Texture logoCaroTex, bannerWinTex, bannerDefeatTex, buttonFrameTex;
    sf::Shader shockwaveShader; bool shockwaveOk;                // V2 #33: guard isAvailable()
    sf::SoundBuffer placeSfx, winSfx, drawSfx, menuSfx, undoSfx, hintSfx, alarmSfx;
    sf::Music bgMusic;
};
```

### 2.2. `render.cpp` — Rendering (file lớn nhất ~1950 dòng)

**Khởi tạo / sự kiện chung:**
- `renderLoadResources(res)` — load **font + texture + shader + bgFrames** (KHÔNG load sound; SFX do `soundLoadResources` lo). Trả `false` nếu thiếu font (main thoát −1).
- `applyLetterbox(window, w, h)` — set view letterbox 16:9.
- `handleCommonEvent(window, event)` — map mouse pixel→view IN-PLACE + xử Closed/Resized + phím **F2** toggle nền động/tĩnh. Trả `true` nếu là Closed/Resized.
- `renderTextCentered(window, font, text, size, x, y, color, [outlineColor], [thickness])` — text center, UTF-8, outline tùy chọn.

**Vẽ từng màn (public):** `renderMainMenu`, `renderModeSelect`, `renderDifficultySelect`, `renderCharSelect`, `renderStyleSelect`, `renderPauseMenu(.., volume, sfxOn)`, `renderInputNames`, `renderGameplay`, `renderGameOver(.., menuIndex, askingSave)`, `renderSaveScreen(.., const vector<string>&, inputName, sel, scrollTop)`, `renderLoadScreen(.., const vector<string>&, sel, scrollTop)`, `renderSettings`, `renderHelp`, `renderAbout`.

**Hit-test các màn tile (V2):** `modeSelectHitTest(mx,my)` (0=PvP/1=PvC), `diffSelectHitTest` (0..3), `charSelectHitTest` (0..5).

**Nút dùng chung:** `renderBackButton(window, res, [mx=-1, my=-1])` (tự đọc chuột khi mx<0 — audit D8) + `backButtonContains`; `renderGameplayActions` + `gameplaySaveBtnContains` / `gameplayExitBtnContains`.

**Gameplay con:** `renderBoard`, `renderPieces` (swap X/O theo `firstPlayerOfRound`), `renderCursor`, `renderHint`, `renderWinLine`, `renderPlayerPanel(.., result)` (panel 2 người + mascot 3-state + thời gian/người), `renderTurnTimer(percentage)`, `renderBotThinking` (hiện ở **mọi mode** — audit 3.3), `renderPlaceEffect`.

**Hiệu ứng thắng (V2):** Confetti — `confettiSpawnBurst / confettiUpdate / confettiDraw` trên `ConfettiSystem { std::vector<ConfettiPiece> }` (1 VertexArray batched). Shockwave — `renderShockwave(window, res, cx, cy, elapsed)` (fragment shader, no-op an toàn nếu `shockwaveOk=false`).

**Toạ độ:** `renderBoardToPixel(row, col) → sf::Vector2f`; `renderPixelToBoard(x, y, &row, &col) → bool` (audit R2: chặn sớm x/y < offset trước khi `(int)` cắt, tránh nhận nhầm hàng/cột 0).

**Helper internal (static trong render.cpp, không export):** `currentBackdropTex` (ping-pong frame nền) + `renderBackdrop`; bộ glow `menuGlowPulse / drawGlowHalo / drawGlowPill / drawContentPanel` (audit #31); bộ tile `tileGridCenter / tileGridHitTest / drawTileFrame / drawTileSprite` (audit D7 — Mode/Diff/Char dùng chung); `sliderDraw` (audit D1); `renderScrollableList` (dùng `rounded_rect.h`); `drawInputField` (ô nhập bo góc + caret + glow, audit 3.7); `loadMascotSet`, `heroSetByIndex`.

### 2.3. `menu.cpp` — Game flow (~1790 dòng)

**Public:** `gameRun(window, res)` (dispatcher) + 14 hàm `handle*()` (xem §1). `handleSaveScreen` có tham số ngữ cảnh:
```cpp
GameScreen handleSaveScreen(window, res, state, GameScreen exitTarget = SCREEN_PLAYING);
//  exitTarget = SCREEN_PLAYING  → mở trong ván → Back/ESC về bàn cờ
//  exitTarget = SCREEN_MAIN_MENU → mở từ Game Over → Back/ESC ra thẳng Menu (§5.17)
```

**Static helper:** `menuHitTest(mx,my,startY,step,count,[halfW,halfH])`; `menuNavStep(index,count,delta)` = `((i+delta)%n+n)%n` (audit D9, gom ~7 site điều hướng); slider `sliderSetFromX / sliderContains / sliderNudge` (audit D1, dùng chung Settings + Pause); list `ensureVisibleIn / hitTestListAt / scrollListBy` (audit D2/D4, dùng chung Save + Load).

**Lambda trong `handleGameplay`:**
- `openNestedScreen(fn)` — gói **pause timer (Speed) → gọi màn con → resume → clock.restart**, trả `GameScreen` để call site tự forward (audit D5). Dùng cho Save-button / Exit-button / phím L / ESC(Pause).
- `applyResult(GameResult)` — cộng `totalWins` + phát win/draw sound, gọi đúng 1 lần/ván (audit D6).
- `flushPendingInput()` — xả event tồn đọng sau khi bot nghĩ (tránh click lọt khe, audit 3.4).
- `doBotMove()` — bot đi 1 nước (đồng bộ, block); trừ chess-clock P2; `renderBotThinking` mọi mode.
- `doPlayerPlace(r,c)` — người đặt quân (Enter/click); nếu PvC + tới lượt bot → gọi `doBotMove`.

> **Lưu ý event Game Over:** cú đặt quân thắng đi qua guard `result==NONE` (đặt) **rồi ngay sau** guard `result!=NONE` ([menu.cpp] click/keypress) → `return handleGameOver` trong **cùng 1 sự kiện** (không cần click thừa). Thua do hết giờ thì `return handleGameOver` trực tiếp.

**`handleSaveScreen`:** entry-drain event tồn đọng (chống input-bleed phím L, §5.17); `pendingOverwrite` (Enter lần 1 tên trùng = hỏi xác nhận, Enter lần 2 = ghi đè; mọi thao tác sửa `inputName` đều reset — §5.18).

### 2.4. `bot.cpp` — AI logic
```cpp
void botGetMove(state, difficulty, &outRow, &outCol);   // dispatcher theo độ khó
void botEasyMove   (state, &r, &c);   // random + chặn cơ bản
void botMediumMove (state, &r, &c);   // heuristic scoring (pattern)
void botHardMove   (state, &r, &c);   // Minimax depth 3 + Alpha-Beta (radius 2 candidates)
void botExpertMove (state, &r, &c);   // Minimax depth 4 + move ordering + threat
void botGetHint    (state, &r, &c);   // gợi ý cho người (dùng mức Medium)

int  botEvaluatePosition(board, r, c, botPlayer);
int  botEvaluateBoard(board, botPlayer);
int  botScoreLine(board, r, c, dRow, dCol, player);     // bảng điểm pattern (5+/4 mở.../3 mở...)
int  botMinimax(board, depth, maxDepth, alpha, beta, isMaximizing, botPlayer);
int  botGetCandidates(board, candidates[][2], radius);
void botSortCandidates(board, candidates[][2], count, botPlayer);
bool botCheckImmediateWin(board, player, &r, &c);
bool botCheckImmediateBlock(board, botPlayer, &r, &c);
// static: botIsWinningMove(board,r,c) — audit 3.2: cắt node terminal trong minimax,
//         trả ±(SCORE_WIN−depth) để ưu tiên thắng nhanh / hoãn thua.
// BOT_PLAYER = CELL_P2 (+1), HUMAN_PLAYER = CELL_P1 (−1).
```
`DIRS`, `inBounds`, `countConsecutive` lấy từ `utils.h` (DRY, dùng chung board.cpp).

### 2.5. `board.cpp` — Board logic
```cpp
void boardInit(state);                              // khởi tạo bàn cờ, firstPlayerOfRound = 1
void boardResetRound(state, int loserPlayerId = 0); // ván mới cùng trận: người thua đi trước (đánh X)
void boardResetAll(state);                          // reset hoàn toàn (xoá cả thống kê)
bool boardPlacePiece(state, r, c);                  // đặt quân người hiện tại; false nếu ô đã có
void boardSwitchTurn(state);
void boardMoveCursor(state, dRow, dCol);            // di chuyển cursor (clamp biên)
bool boardCheckWin(const state, r, c, &winLine);    // kiểm 4 hướng từ nước vừa đánh
bool boardCheckDraw(const state);                   // bàn đầy (O(1) qua moveCount)
GameResult boardEvaluateResult(const state, lastR, lastC, &winLine);  // gọi sau mỗi nước
int  boardUndo(state);                              // PvP huỷ 1, PvC huỷ 2; trả số nước đã huỷ
bool boardIsEmpty(const state, r, c);
// boardIsValid cũ → utils.h::inBounds (DRY).
```
`isPlayer1Turn` ↔ ký hiệu X/O suy từ `firstPlayerOfRound` (render quyết hiển thị).

### 2.6. `timer.cpp` — Speed mode (chess-clock per-player)
```cpp
void timerStart(timer, gameTime, turnTime);          // cả P1 & P2 = gameTime
void timerUpdate(timer, dt, isPlayer1Turn);          // trừ turn + game time CỦA NGƯỜI ĐANG ĐI
void timerResetTurn(timer);                          // reset 20s + re-arm chuông (turnAlarmFired=false)
void timerPause(timer);  void timerResume(timer);
void timerConsumeP1(timer, sec);  void timerConsumeP2(timer, sec);   // trừ thủ công (bot nghĩ)
float timerGetTurnPercent(timer);                    // 0..1 cho progress bar
bool  timerIsTurnExpired(timer);
bool  timerIsGameExpiredP1(timer);  bool timerIsGameExpiredP2(timer); // ai hết → người đó thua
float timerGetTurnSecondsLeft(timer);
float timerGetGameSecondsLeftP1(timer);  float timerGetGameSecondsLeftP2(timer);
// (timerGetGamePercentP1/P2 ĐÃ XOÁ — panel hiển thị giây, không dùng %)
```

### 2.7. `save_load.cpp` — Persistence
- **Format text** (`std::ofstream <<` / `std::getline` + `>>`), file `saves/<name>.txt`. UTF-8 safe cho tên người chơi; tên file lọc ký tự cấm filesystem.
- **V2 #29:** **bỏ `Gamelist.txt`** — liệt kê bằng `std::filesystem::directory_iterator` (bỏ qua `settings.txt`/`Gamelist.txt`), **sort theo thời gian sửa (mới nhất lên đầu)**. Không giới hạn số file.
- **Cache (V2 #28 + §5.18):** `static std::unordered_set<std::string> g_saveNames` — chỉ tên file, cho `saveFileExists` query O(1). (Trước là `unordered_map<string,SaveMetadata>` nhưng các field metadata không UI nào đọc → đã rút gọn thành set, gỡ `struct SaveMetadata` + `parseMetadata`.)
```cpp
bool saveGame(const state, filename);            // ghi text + cập nhật cache; append heroP1/heroP2 cuối file
bool loadGame(state, filename);                  // parse text → GameState; VALIDATE (audit R1/R3)
std::vector<std::string> saveScanFiles();        // directory scan, rebuild cache, sort theo mtime
bool saveDeleteFile(filename);                   // xoá disk + cache
bool saveFileExists(filename);                   // O(1) qua set (lazy-scan nếu chưa scan)
```
- **loadGame validate (audit):** R1 — nếu `STYLE_SPEED` ép `isRunning=true` + `turnAlarmFired=false` (đồng hồ chạy lại sau load). R3 — `moveCount ∈ [0,225]`, mỗi ô ∈ {CELL_P1,0,CELL_P2}, history `inBounds` + player ∈ {CELL_P1,CELL_P2}; sai → `return false` → màn Load hiện lỗi. File V1 cũ thiếu `heroP1/heroP2` → fallback Goku/Vegeta.

### 2.8. `sound.cpp` — Audio
- Pool `SOUND_POOL_SIZE = 8` `sf::Sound` chống chồng. BGM dùng `sf::Music` (streaming, **bắt buộc .ogg**).
```cpp
enum BGMTrack { BGM_MENU = 0, BGM_GAME = 1 };
void soundLoadResources(res);                    // V2 #6: load 7 SFX (place/win/draw/menu/undo/hint/alarm)
void soundShutdown();                            // V2 R8: stop + resetBuffer pool (gọi cuối main)
void soundPlayBGMTrack(res, int track);          // V2 #27: BGM theo màn (no-op nếu cùng track)
void soundSetBGMVolume(res, 0..100);
void soundPlayPlace/Select/Win/Draw/Undo/Hint/Alarm(res);
void soundSetSFXEnabled(bool);  bool soundIsSFXEnabled();  int soundGetBGMVolume();
void settingsLoad(res);                          // đọc saves/settings.txt (lang/volume/sfx)
void settingsSave();                             // ghi settings.txt
// (soundLoadAll / soundPlayBGM / soundPlayMove ĐÃ XOÁ — move.wav đã gỡ)
```

### 2.9. `language.cpp` — i18n
- 2 ngôn ngữ VN + EN. `TextStrings` ~70 field; VN dùng `u8"..."`. Hiển thị qua `sf::String::fromUtf8`.
- `const TextStrings& langGetText(Language)` — **trả tham chiếu** (audit 3.1: cache 2 bản static EN/VI build 1 lần, hết copy ~75 string/lần gọi).
- `langGetCurrent() / langSetCurrent(lang) / langToggle()`.

### 2.10. `utils.h` / `utils.cpp` — Shared (DRY, audit #22)
- `extern const int DIRS[4][2]` — 4 hướng (ngang/dọc/2 chéo).
- `inline bool inBounds(int r, int c)` — toạ độ trong bàn 15×15.
- `int countConsecutive(board, r, c, dRow, dCol, player)` — đếm quân liên tiếp 1 hướng (dùng bởi board.cpp + bot.cpp).

### 2.11. `rounded_rect.h` — UI shape (header-only)
- `class RoundedRectangleShape : public sf::Shape` (override `getPoint/getPointCount`). **Ngoại lệ OOP duy nhất** của dự án — idiom mở rộng SFML (theo SFML wiki), chỉ dùng để vẽ panel/list/scrollbar bo góc. Mọi nơi khác là procedural (free function + POD struct).

---

## 3. Data flow

### 3.1. Khởi tạo (main.cpp)
```
sf::RenderWindow (Close|Resize, 60fps)
   → applyLetterbox(1280, 720)
   → GameResources res
   → renderLoadResources(res)   // font + texture (logo/banner/mascot/avatar/mode...) + shockwave shader + ~106 bgFrames
   → soundLoadResources(res)    // 7 SFX buffer (module sound, audit #6)
   → settingsLoad(res)          // language / bgm volume / sfx on-off
   → gameRun(window, res)       // main loop (BGM menu phát ngay)
   → soundShutdown()            // cuối, trước teardown audio device (audit R8)
```

### 3.2. Game flow điển hình
```
Main Menu → New Game
   → Mode Select (PvP / PvC)                 [2 tile + Back]
   → [nếu PvC] Difficulty Select             [4 tile villain: Frieza/Cell/Buu/Broly]
   → Style Select (Basic / Speed)
   → Character Select (V2 #34)               [lưới 6 hero; PvP chọn 2 phase, PvC chỉ P1]
   → Input Names                             [P1 UTF-8; P2 = "Máy" nếu PvC]
   → boardResetAll + (Speed) timerStart → SCREEN_PLAYING
       → loop: input → boardPlacePiece → boardEvaluateResult → boardSwitchTurn → (PvC) doBotMove
       → ESC / nút Exit → Pause (nested, openNestedScreen)
       → nút Save / phím L → Save screen (nested)
       → kết thúc → handleGameOver (nested): confetti + shockwave + mascot win/lose
   → Game Over: "Chơi tiếp?"
       → Có → boardResetRound(loserId): người thua đi trước + đánh X → SCREEN_PLAYING
       → Không → "Lưu game?" → [Có → Save screen (exitTarget=MAIN_MENU)] / [Không → Main Menu]
```

### 3.3. Save / Load flow
```
Save (nút Save / L trong ván, hoặc Pause→Save, hoặc Game Over→Lưu):
   handleSaveScreen(.., exitTarget)
       → drain event tồn đọng (chống input-bleed)
       → saveScanFiles() (directory scan, sort mtime)
       → nhập tên → Enter:
            tên trùng lần 1 → pendingOverwrite = tên, hỏi "Enter lần nữa để ghi đè"
            Enter lần 2 (tên không đổi) → saveGame() ghi đè
            (mọi sửa tên/chọn file khác → reset pendingOverwrite, hỏi lại)
       → Back/ESC → return exitTarget (SCREEN_PLAYING hoặc SCREEN_MAIN_MENU)

Load (Main Menu → Tải Game):
   handleLoadScreen
       → saveScanFiles() (list cuộn được, wheel + W/S + Up/Down)
       → chọn file → loadGame() (validate R1/R3) → SCREEN_PLAYING với state đã load
       → file lỗi/V1 rác → "Lỗi tải file!"
```

---

## 4. Coordinate system

### Window vs View
- **Window pixel space:** thật của OS (có thể 1920×1080 khi maximize).
- **View space (game space):** luôn **1280×720** (logical).
- `applyLetterbox()` map view → viewport (viền đen giữ 16:9). `handleCommonEvent()` map mouse pixel → view coords **IN-PLACE** → mọi hit-test dùng view coords mà không cần đổi.

### Board coords (view space)
- `UI_BOARD_OFFSET_X = 40`, `UI_BOARD_OFFSET_Y = 60`, `CELL_SIZE = 40`.
- Vùng bàn: x = 40..640, y = 60..660 (15 × 40 = 600px mỗi cạnh).
- `renderBoardToPixel(row, col)` → tâm ô; `renderPixelToBoard` → ngược lại (chặn mép trái/trên trước khi chia).

### Panel coords
- `panelX = UI_BOARD_OFFSET_X + BOARD_SIZE*CELL_SIZE + UI_PANEL_GAP_LEFT = 40+600+40 = 680`.
- `panelW = WINDOW_WIDTH − panelX − UI_PANEL_GAP_RIGHT = 1280−680−20 = 580`.
- 2 player box: P1 y = 60.., P2 cách `UI_PANEL_BOX_STEP = 190`; mỗi box cao `UI_PANEL_BOX_HEIGHT = 170`.
- Turn timer (Speed): `UI_TIMER_BAR_Y = 450`; "Bot đang suy nghĩ" `UI_BOT_THINKING_Y = 505`.
- Game Over: text/nút theo `UI_GAMEOVER_*` (nút Có/Không: `_BTN_GAP_X = 95`, `_BTN_HALF_W = 55`).
- Pause overlay: `UI_PAUSE_TITLE_Y = 150`, `UI_PAUSE_START_Y = 230`, `UI_PAUSE_STEP = 60` (audit D10, 1 nguồn).

---

## 5. Event handling pattern

```cpp
while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (handleCommonEvent(window, event)) continue;   // ← LUÔN gọi đầu (Closed/Resized + map mouse + F2)

        if (event.type == sf::Event::MouseMoved) { /* x/y đã ở view coords */ }
        if (event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left) { /* hit-test */ }
        if (event.type == sf::Event::KeyPressed) { switch (event.key.code) { ... } }
        if (event.type == sf::Event::TextEntered) { /* nhập tên UTF-8 */ }
    }
    renderXxx(window, ...);
    window.display();
}
```

### Màn lồng (nested screen) — Pause / Save / Game Over
Không qua `gameRun`. Trong `handleGameplay` dùng `openNestedScreen(fn)` để **pause đồng hồ Speed → gọi handler → resume → restart clock**, rồi forward giá trị trả về:
```cpp
GameScreen r = openNestedScreen(handlePauseMenu);   // hoặc lambda gọi handleSaveScreen
if (r != SCREEN_PLAYING) return r;                  // != PLAYING (vd MAIN_MENU) → thoát ván
```
`handleGameOver` được `return` trực tiếp từ `handleGameplay`; nó tự gọi `handleSaveScreen(.., SCREEN_MAIN_MENU)` khi người chơi chọn "Lưu" (không qua `openNestedScreen` vì ván đã kết thúc, không có đồng hồ đang chạy để pause).

---

## 6. Adding a new screen (vd "Statistics")

1. Thêm `SCREEN_STATISTICS` vào `enum GameScreen` (`game_types.h`).
2. `renderStatistics()` trong `render.cpp` + declare `render.h`.
3. `handleStatistics()` trong `menu.cpp` + declare `menu.h`.
4. Thêm `case SCREEN_STATISTICS` vào `gameRun()` dispatcher.
5. Thêm menu item dẫn vào (Main Menu) + nhớ tăng `MENU_COUNT` + cập nhật `confirm()`.
6. Thêm `TextStrings` field + gán EN/VI trong `language.cpp` (đừng hardcode chuỗi).

---

## 7. Adding a new asset

### Texture
1. Bỏ PNG vào `assets/textures/`.
2. Thêm `sf::Texture xxxTex` (hoặc `MascotSet`) vào `GameResources` (`game_types.h`).
3. Load trong `renderLoadResources()` — nên `std::filesystem::exists()` trước khi `loadFromFile` để tránh SFML in lỗi console; fallback hợp lý nếu thiếu.
4. Vẽ qua `sf::Sprite` (setOrigin/scale fit) ở render function tương ứng.

### Sound
1. Bỏ `.wav`/`.ogg` vào `assets/sounds/` (BGM **bắt buộc .ogg**).
2. Thêm `sf::SoundBuffer xxxSfx` vào `GameResources`.
3. Load trong **`soundLoadResources()` (sound.cpp)** — KHÔNG phải renderLoadResources.
4. Thêm `void soundPlayXxx(res)` trong `sound.h/.cpp` (gọi `playSfx`).

### Font: chỉ 2 slot (title + main) — đổi file trong `assets/fonts/`, không cần sửa code.

---

## 8. Common pitfalls

**P1 — UTF-8 mojibake khi setString:** luôn `text.setString(sf::String::fromUtf8(s.begin(), s.end()))`, không `setString(s)` trực tiếp với std::string UTF-8.

**P2 — PNG mất alpha khi xử lý:** convert sang 32-bit Argb trước khi sửa byte (xem ASSETS_PLAN).

**P3 — Mouse sai sau resize:** `handleCommonEvent()` đã map pixel→view IN-PLACE → **PHẢI gọi đầu mọi event loop**.

**P4 — Font thiếu dấu tiếng Việt:** test trên Google Fonts preview với "Ơ Ư Ậ Ọ Ữ Ằ Ề" trước. Font an toàn: Be Vietnam Pro, Coiny.

**P5 — Default arg không sống qua function-pointer decay:** `handleSaveScreen` truyền vào template `openNestedScreen(auto&& fn)` → phải bọc lambda 3-arg, không dựa vào default `exitTarget` (§5.17).

**P6 — Phải kill `CaroGame.exe` trước khi build** (LNK1168 nếu exe đang chạy).

---

## 9. Critical files quick reference

| File | Khi nào sửa? |
|------|-------------|
| `game_types.h` | Thêm const, struct, enum, field GameResources |
| `utils.h/cpp` | Helper dùng chung (DIRS, inBounds, countConsecutive) |
| `render.h/cpp` | UI/rendering, hiệu ứng, layout |
| `menu.h/cpp` | Game flow, event handling, điều hướng màn |
| `language.h/cpp` | Thêm/sửa text VN/EN |
| `bot.cpp` | AI logic |
| `board.cpp` | Luật cờ (win/draw, đặt quân, undo, reset) |
| `timer.cpp` | Chess-clock Speed mode |
| `save_load.cpp` | Lưu/tải (text + directory scan) |
| `sound.cpp` | Audio (SFX + BGM theo màn + settings) |
| `rounded_rect.h` | Shape bo góc (UI) |
| `main.cpp` | Hiếm khi (chỉ window + thứ tự init/teardown) |

---

**Last updated:** 13/06/2026 (đồng bộ với code V2 sau Sprint 1–4 + audit kỹ thuật mục 5 + §5.17/§5.18).

**Tóm tắt thay đổi V1 → V2 phản ánh trong file này:**
- Thêm màn `SCREEN_CHAR_SELECT`; Mode/Difficulty thành lưới tile (hit-test riêng); làm rõ Pause/Game Over/Save là **màn lồng** (không dispatch).
- `GameResources` mở rộng: `bgFrames[]` (nền động), `MascotSet` 10 nhân vật (hero+villain), `heroAvatar/villainAvatar`, `modePvp/PvcTex`, `shockwaveShader`; bỏ `boardTex`, `moveSfx`, mascot 3-field cũ; thêm SFX undo/hint/alarm.
- `save_load`: bỏ `Gamelist.txt`/`saveGetList`/`saveAddToList`/`saveRenameFile`/`saveCountFiles`; directory scan + cache `unordered_set` (bỏ `SaveMetadata`/`parseMetadata` — §5.18); validate khi load (R1/R3); ghi đè có xác nhận.
- `sound`: `soundLoadResources` + `soundShutdown` + `soundPlayBGMTrack` (BGM theo màn); bỏ `soundLoadAll`/`soundPlayBGM`/`soundPlayMove`.
- `timer`: bỏ `timerGetGamePercentP1/P2`; thêm `turnAlarmFired` + chuông cảnh báo.
- `bot`: `botGetMove` dùng out-param; thêm cắt node terminal minimax (3.2); `BOT_PLAYER=CELL_P2`/`HUMAN=CELL_P1`.
- `board`: `boardPlacePiece`/`boardEvaluateResult`/`boardResetRound(loserId)`; `boardUndo` trả int; `boardIsValid`→`utils::inBounds`.
- `language`: `langGetText` trả `const&` (cache static).
- DRY: `utils.h` (DIRS/inBounds/countConsecutive); helper menu/render (`menuNavStep`, slider, list, tile-grid).
