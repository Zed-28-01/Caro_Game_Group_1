# HUONG DAN PHAT TRIEN GAME CARO - CARO_GAME_GROUP_1

> Tai lieu nay mo ta chi tiet tung buoc de xay dung game Caro hoan chinh
> su dung SFML, dua tren logic tu du an CSLT---N---CARO (da duoc cai tien).

---

## MUC LUC

1. [Cau truc thu muc](#1-cau-truc-thu-muc)
2. [So do module va dependency](#2-so-do-module-va-dependency)
3. [Cac buoc phat trien (Theo thu tu)](#3-cac-buoc-phat-trien)
   - Phase 1: Nen tang (board + render co ban)
   - Phase 2: Gameplay PvP
   - Phase 3: AI Bot (4 muc do kho)
   - Phase 4: Timer, Save/Load
   - Phase 5: UI nang cao, hieu ung, polish
4. [Chi tiet tung file .cpp can viet](#4-chi-tiet-tung-file-cpp)
5. [Bang diem AI (Pattern Scoring Table)](#5-bang-diem-ai)
6. [Assets can chuan bi](#6-assets-can-chuan-bi)
7. [Luu y quan trong](#7-luu-y-quan-trong)

---

## 1. CAU TRUC THU MUC

```
Caro_Game_Group_1/
├── CaroGame/                       # Visual Studio project
│   ├── CaroGame.sln
│   └── CaroGame.vcxproj
├── src/                            # SOURCE CODE (.cpp)
│   ├── main.cpp                    # Entry point
│   ├── board.cpp                   # Logic ban co
│   ├── bot.cpp                     # AI 4 muc do
│   ├── render.cpp                  # Ve SFML
│   ├── menu.cpp                    # Dieu huong man hinh
│   ├── save_load.cpp               # Luu/tai game
│   ├── sound.cpp                   # Am thanh
│   ├── timer.cpp                   # Timer & progress bar
│   └── language.cpp                # Da ngon ngu
├── include/                        # HEADER FILES (.h)
│   ├── game_types.h                # Structs, enums, constants
│   ├── board.h
│   ├── bot.h
│   ├── render.h
│   ├── menu.h
│   ├── save_load.h
│   ├── sound.h
│   ├── timer.h
│   └── language.h
├── assets/                         # TAI NGUYEN
│   ├── fonts/
│   │   ├── main_font.ttf           # Font chinh (VD: Roboto, Montserrat)
│   │   └── title_font.ttf          # Font tieu de (VD: Bungee, PressStart2P)
│   ├── sounds/
│   │   ├── bgm.ogg                 # Nhac nen (SFML ho tro .ogg tot nhat)
│   │   ├── move.wav                # Am thanh di chuyen
│   │   ├── place.wav               # Am thanh dat quan
│   │   ├── select.wav              # Am thanh chon menu
│   │   ├── win.wav                 # Am thanh thang
│   │   └── draw.wav                # Am thanh hoa
│   └── textures/                   # (Tuy chon) Hinh anh
│       ├── background.png          # Hinh nen
│       └── ...
├── docs/
│   ├── BUILD_GUIDE.md              # File nay
│   ├── SETUP.md
│   └── GitCommand.md
├── libs/
│   └── SFML/                       # Thu vien SFML 2.6.2
└── README.md
```

---

## 2. SO DO MODULE VA DEPENDENCY

```
                    main.cpp
                       |
                    menu.h/.cpp
                   /    |    \
                  /     |     \
           board.h   render.h  save_load.h
           board.cpp render.cpp save_load.cpp
              |         |
           bot.h     timer.h    sound.h    language.h
           bot.cpp   timer.cpp  sound.cpp  language.cpp
              \         |          |           /
               \        |          |          /
                 game_types.h  (STRUCT & CONSTANTS)
```

**Quy tac dependency:**
- `game_types.h` → KHONG include file nao khac (tru SFML va STL)
- `board.h` → include `game_types.h`
- `bot.h` → include `game_types.h` (chi can board data, khong can SFML)
- `render.h` → include `game_types.h` (can SFML)
- `menu.h` → include `game_types.h` (dieu phoi tat ca)
- `timer.h`, `sound.h`, `save_load.h`, `language.h` → include `game_types.h`

**Trong cac file .cpp:**
- `menu.cpp` se `#include` tat ca cac .h khac (vi no dieu phoi)
- Cac .cpp khac chi include .h tuong ung cua minh

---

## 3. CAC BUOC PHAT TRIEN

### =============================================
### PHASE 1: NEN TANG (Ket qua: hien thi ban co trong)
### =============================================

#### Buoc 1.1: Chuan bi assets
- [ ] Tai 2 file font .ttf tu Google Fonts:
  - Font chinh: Roboto, Open Sans, hoac Montserrat (de doc)
  - Font tieu de: PressStart2P, Bungee, hoac Silkscreen (co phong cach game)
  - Dat vao `assets/fonts/`
- [ ] Tai am thanh .wav va nhac .ogg mien phi tu:
  - https://freesound.org hoac https://opengameart.org
  - Dat vao `assets/sounds/`

#### Buoc 1.2: Viet `src/language.cpp`
- [ ] Implement `langGetText()`: tao 2 bo TextStrings (Viet/Anh)
- [ ] Implement `langGetCurrent()`, `langSetCurrent()`, `langToggle()`
- [ ] Su dung 1 bien global `static Language currentLang = LANG_VIETNAMESE;`
- **Lam truoc de cac module khac co the lay text ngay**

#### Buoc 1.3: Viet `src/board.cpp` (LOGIC CO BAN)
- [ ] `boardInit()`: khoi tao mang board[15][15] = 0, cursor = (7,7) giua ban co
- [ ] `boardResetRound()`: xoa ban co, doi luot di truoc
- [ ] `boardResetAll()`: xoa ca thong ke
- [ ] `boardMoveCursor()`: di chuyen cursor, kiem tra khong vuot bien
- [ ] `boardPlacePiece()`: dat quan, push vao moveHistory, doi luot
- [ ] `boardIsEmpty()`, `boardIsValid()`, `boardCountEmpty()`

**Tham khao tu CSLT---N---CARO:**
- `ResetGame()` (Game.cpp dong 139-151) → `boardInit()`
- `CheckBoard()` (Game.cpp dong 184-195) → `boardPlacePiece()`
- `MoveUp/Down/Left/Right()` → `boardMoveCursor()`

#### Buoc 1.4: Viet `src/render.cpp` (VE CO BAN)
- [ ] `renderLoadResources()`: load font, texture, sound tu assets/
- [ ] `renderBoardToPixel()`: chuyen (row, col) → pixel. Cong thuc:
  ```
  float boardOffsetX = 40.0f;   // Khoang cach tu trai
  float boardOffsetY = 60.0f;   // Khoang cach tu tren
  x = boardOffsetX + col * CELL_SIZE + CELL_SIZE / 2;
  y = boardOffsetY + row * CELL_SIZE + CELL_SIZE / 2;
  ```
- [ ] `renderPixelToBoard()`: nguoc lai (dung cho mouse click sau nay)
- [ ] `renderBoard()`: ve 16 duong ngang + 16 duong doc bang `sf::RectangleShape`
- [ ] `renderPieces()`: duyet board[][], ve X (2 duong cheo) va O (hinh tron)
- [ ] `renderCursor()`: ve hinh vuong highlight vang tai vi tri cursor

#### Buoc 1.5: Viet `src/main.cpp` (KHUNG CHUONG TRINH)
- [ ] Tao `sf::RenderWindow` 1280x720
- [ ] Goi `renderLoadResources()`
- [ ] Goi `boardInit()`
- [ ] Vong lap: clear → ve board → ve pieces → ve cursor → display
- [ ] Xu ly phim WASD di chuyen cursor
- [ ] **TEST**: Chay len thay ban co 15x15, cursor di chuyen duoc

### =============================================
### PHASE 2: GAMEPLAY PVP (Ket qua: 2 nguoi choi duoc)
### =============================================

#### Buoc 2.1: Hoan thien board.cpp - Win/Draw
- [ ] `boardCheckWin()`: kiem tra 4 huong tu vi tri vua danh
  ```
  4 huong: ngang (0,1), doc (1,0), cheo chinh (1,1), cheo phu (1,-1)
  Voi moi huong: dem lien tiep ve 2 phia
  Neu tong >= 5: thang, luu vi tri vao WinLine
  ```
- [ ] `boardCheckDraw()`: kiem tra board day (dem o trong = 0)
- [ ] `boardEvaluateResult()`: goi checkWin + checkDraw, tra ve GameResult
- [ ] `boardUndo()`: pop moveHistory, xoa quan, doi luot

**Tham khao tu CSLT---N---CARO:**
- `checkWin()` (Game.cpp dong 221-293) → `boardCheckWin()` (fix: chi check 4 huong tu nuoc vua danh, khong duyet toan bo board)
- `isDraw()` (Game.cpp dong 295-304) → `boardCheckDraw()`

#### Buoc 2.2: Viet `src/menu.cpp` (DIEU HUONG)
- [ ] `gameRun()`: vong lap chinh, dung switch(currentScreen) de goi handle tuong ung
  ```
  GameScreen currentScreen = SCREEN_MAIN_MENU;
  while (window.isOpen()) {
      switch (currentScreen) {
          case SCREEN_MAIN_MENU:
              currentScreen = handleMainMenu(window, res, state);
              break;
          case SCREEN_PLAYING:
              currentScreen = handleGameplay(window, res, state);
              break;
          // ... tuong tu cho cac man hinh khac
      }
  }
  ```
- [ ] `handleMainMenu()`: ve menu, xu ly W/S di chuyen, Enter chon
- [ ] `handleModeSelect()`: chon PvP/PvC
- [ ] `handleStyleSelect()`: chon Basic/Speed
- [ ] `handleInputNames()`: nhap ten bang ban phim (xu ly sf::Event::TextEntered)

#### Buoc 2.3: Viet `handleGameplay()` cho PvP
- [ ] Vong lap game:
  ```
  1. Xu ly event (WASD, Enter, Z, ESC, L)
  2. Khi Enter: goi boardPlacePiece()
     - Neu thanh cong: ve quan moi, check win/draw
     - Neu thang: chuyen sang SCREEN_GAME_OVER
     - Neu hoa: chuyen sang SCREEN_GAME_OVER
  3. Khi Z: goi boardUndo()
  4. Khi ESC: chuyen sang SCREEN_PAUSE_MENU
  5. Ve: board + pieces + cursor + player panel
  ```
- [ ] Implement `renderPlayerPanel()`: hien thi ten, so nuoc, so thang
- [ ] Implement `renderGameOver()`: hien ket qua, hoi choi tiep

**Tham khao tu CSLT---N---CARO:**
- `GamePlayPvP()` (Game.cpp dong 779-905) → `handleGameplay()` (gop chung 1 ham, dung state.mode de phan biet PvP/PvC)

#### Buoc 2.4: Ve cac man hinh render con lai
- [ ] `renderMainMenu()`: tieu de "CARO GAME" lon + 6 nut menu
- [ ] `renderModeSelect()`: 2 nut PvP, PvC
- [ ] `renderInputNames()`: 2 text box nhap ten
- [ ] `renderPauseMenu()`: Resume, Save, Settings, Menu
- [ ] `renderGameOver()`: ket qua + nut Continue/Menu

#### Buoc 2.5: TEST PHASE 2
- [ ] Choi PvP day du: 2 nguoi thay phien, nhan dien thang/thua/hoa
- [ ] Undo hoat dong dung
- [ ] Menu dieu huong muot

### =============================================
### PHASE 3: AI BOT - 4 MUC DO KHO (TRONG TAM)
### =============================================

> Day la phan quan trong nhat. Viet `src/bot.cpp` theo thu tu tu de den kho.

#### Buoc 3.1: Ham tien ich chung
- [ ] `botGetCandidates()`: tim tat ca o trong lan can quan da danh
  ```
  Duyet toan bo board:
    Voi moi o trong (value == 0):
      Kiem tra xung quanh (ban kinh radius) co quan nao khong
      Neu co: them vao candidates[]
  Tra ve so luong candidates
  ```
- [ ] `botCountConsecutive()`: dem quan lien tiep theo 1 huong
- [ ] `botCheckImmediateWin()`: duyet candidates, thu dat quan, check thang
- [ ] `botCheckImmediateBlock()`: tuong tu nhung cho doi thu

#### Buoc 3.2: `botScoreLine()` - HAM CHAM DIEM CHUOI (QUAN TRONG NHAT)

Day la ham quyet dinh suc manh cua AI. Can danh gia CHINH XAC.

```
Voi 1 huong (vd: ngang) tu vi tri (row, col) cho player:

1. Dem lien tiep ve phia truoc (dx, dy): countForward
2. Dem lien tiep ve phia sau (-dx, -dy): countBackward
3. Tong = countForward + countBackward + 1

4. Kiem tra 2 dau cua chuoi:
   - openEnds = 0
   - O dau truoc (sau quan cuoi chuoi truoc) co trong? → openEnds++
   - O dau sau (sau quan cuoi chuoi sau) co trong?   → openEnds++

5. Tra diem theo bang:
   Xem BANG DIEM AI (Muc 5)
```

#### Buoc 3.3: `botEvaluatePosition()` - Danh gia 1 o trong
```
Voi o (row, col) dang trong:
  score = 0
  Voi moi huong trong 4 huong:
    Gia lap dat quan cua bot tai (row, col)
    score += botScoreLine(..., botPlayer)     // Diem tan cong
    Gia lap dat quan cua doi thu tai (row, col)
    score += botScoreLine(..., opponent) * 0.9 // Diem phong thu (nhan 0.9 de uu tien tan cong)
    Xoa gia lap
  return score
```

#### Buoc 3.4: EASY BOT
```
botEasyMove():
  1. Neu doi thu co chuoi 4: chan ngay (botCheckImmediateBlock)
  2. Neu khong: chon NGAU NHIEN 1 trong cac candidates
     (co the them trong so nhe: uu tien gan trung tam)
```

#### Buoc 3.5: MEDIUM BOT
```
botMediumMove():
  1. Kiem tra thang ngay (botCheckImmediateWin)
  2. Kiem tra chan ngay (botCheckImmediateBlock)
  3. Danh gia diem cho tat ca candidates bang botEvaluatePosition()
  4. Chon o co diem cao nhat
```

#### Buoc 3.6: HARD BOT (Minimax depth 3)
```
botHardMove():
  1. Kiem tra thang ngay
  2. Kiem tra chan ngay
  3. Lay candidates (ban kinh 2)
  4. Voi moi candidate:
     - Dat quan bot
     - Goi minimax(depth=0, maxDepth=3, alpha=-INF, beta=+INF, false)
     - Xoa quan
     - Luu score
  5. Chon candidate co score cao nhat

botMinimax():
  Neu depth == maxDepth: return botEvaluateBoard()
  Neu isMaximizing (luot bot):
    Lay candidates
    Voi moi candidate:
      Dat quan bot
      score = minimax(depth+1, maxDepth, alpha, beta, false)
      Xoa quan
      maxEval = max(maxEval, score)
      alpha = max(alpha, score)
      Neu beta <= alpha: break (cat tia)
    return maxEval
  Neu !isMaximizing (luot doi thu):
    Tuong tu nhung tim min
```

#### Buoc 3.7: EXPERT BOT (Minimax depth 4 + Move Ordering)
```
botExpertMove():
  Giong Hard nhung:
  - maxDepth = 4
  - Truoc khi minimax: goi botSortCandidates() de sap xep
    candidates theo diem giam dan (danh gia nhanh)
  - Move ordering giup alpha-beta cat tia hieu qua hon
    → co the tim kiem sau hon ma khong cham

botSortCandidates():
  Voi moi candidate: tinh score = botEvaluatePosition()
  Sap xep candidates theo score giam dan (selection sort hoac bubble sort)
```

#### Buoc 3.8: `botGetHint()` - Goi y cho nguoi choi
```
botGetHint():
  Goi botMediumMove() nhung cho nguoi choi (player = -1 thay vi 1)
  Tra ve vi tri goi y
```

#### Buoc 3.9: Tich hop bot vao gameplay
- [ ] Trong `handleGameplay()`:
  ```
  Sau khi nguoi choi danh xong:
    Neu mode == MODE_PVC:
      Goi botGetMove(state, state.difficulty, botRow, botCol)
      Goi boardPlacePiece(state, botRow, botCol)
      Ve quan bot
      Check win/draw
  ```
- [ ] Xu ly phim H (hint):
  ```
  Neu mode == MODE_PVC va dang la luot nguoi choi:
    Goi botGetHint(state, hintRow, hintCol)
    Ve highlight goi y tai (hintRow, hintCol)
  ```

#### Buoc 3.10: TEST PHASE 3
- [ ] Easy: bot phai de thua, nhung van chan khi nguoi sap thang
- [ ] Medium: bot choi kha, co chien luoc tan cong
- [ ] Hard: bot manh, kho thang
- [ ] Expert: bot rat manh, biet tao fork (2 chuoi 3 mo cung luc)
- [ ] Hint hien thi dung, chi trong PvC

### =============================================
### PHASE 4: TIMER, SAVE/LOAD
### =============================================

#### Buoc 4.1: Viet `src/timer.cpp`
- [ ] Su dung bien float de luu thoi gian con lai
- [ ] `timerUpdate()`: tru deltaTime moi frame
  ```
  timer.turnTimeLeft -= deltaTime;
  timer.gameTimeLeft -= deltaTime;
  if (timer.turnTimeLeft < 0) timer.turnTimeLeft = 0;
  if (timer.gameTimeLeft < 0) timer.gameTimeLeft = 0;
  ```
- [ ] `timerGetTurnPercent()`: `return turnTimeLeft / MAX_TURN_TIME;`
- [ ] `timerResetTurn()`: `turnTimeLeft = MAX_TURN_TIME;`

**Khac biet so voi ban goc:** Ban goc dung `std::thread` phuc tap, de race condition.
Ta dung `sf::Clock` + `deltaTime` trong game loop → don gian, an toan, khong can mutex.

#### Buoc 4.2: Tich hop timer vao gameplay
- [ ] Trong `handleGameplay()`:
  ```
  sf::Clock clock;
  while (...) {
      float dt = clock.restart().asSeconds();
      if (state.style == STYLE_SPEED) {
          timerUpdate(state.timer, dt);
          if (timerIsTurnExpired(state.timer)):
              boardSwitchTurn(state);
              timerResetTurn(state.timer);
          if (timerIsGameExpired(state.timer)):
              → SCREEN_GAME_OVER (hoa hoac tinh diem)
      }
  }
  ```
- [ ] Ve progress bar: `renderTurnTimer()` dung `timerGetTurnPercent()`
- [ ] Doi mau khi sap het gio: < 25% → do

#### Buoc 4.3: Viet `src/save_load.cpp`
- [ ] `saveGame()`: ghi ra file text
  ```
  Format file:
  [Player1 Name]
  [Player1 Moves] [Player1 Wins]
  [Player2 Name]
  [Player2 Moves] [Player2 Wins]
  [isPlayer1Turn: 0 hoac 1]
  [cursorRow] [cursorCol]
  [mode: 0=PvP, 1=PvC]
  [style: 0=Basic, 1=Speed]
  [difficulty: 0-3]
  [gameTimeLeft] [turnTimeLeft]
  [moveCount]
  [board: 15 dong, moi dong 15 so]
  [moveHistory: moveCount dong, moi dong: row col player]
  ```
- [ ] `loadGame()`: doc va parse nguoc lai
- [ ] `saveGetList()`: doc Gamelist.txt
- [ ] `saveAddToList()`, `saveDeleteFile()`, `saveRenameFile()`

**Tham khao tu CSLT---N---CARO:**
- `SaveData()` (SaveLoad.cpp dong 10-36) → `saveGame()`
- `LoadData()` (SaveLoad.cpp dong 430-493) → `loadGame()`
- Cai tien: them moveHistory, difficulty, them kiem tra loi

#### Buoc 4.4: Ve man hinh Save/Load
- [ ] `renderSaveScreen()`: danh sach file + input box nhap ten
- [ ] `renderLoadScreen()`: danh sach file + chon bang W/S/Enter
- [ ] Xu ly trong `handleSaveScreen()`, `handleLoadScreen()`

#### Buoc 4.5: TEST PHASE 4
- [ ] Timer dem nguoc chinh xac, progress bar hoat dong
- [ ] Het gio luot → tu dong chuyen luot
- [ ] Het gio van → ket thuc game
- [ ] Save → Load → trang thai khoi phuc chinh xac
- [ ] Xoa, doi ten file save hoat dong

### =============================================
### PHASE 5: UI NANG CAO, HIEU UNG, POLISH
### =============================================

#### Buoc 5.1: Viet `src/sound.cpp`
- [ ] `soundLoadAll()`: load tat ca file am thanh
- [ ] Cac ham soundPlay*(): tao `sf::Sound`, set buffer, play
- [ ] Luu y: `sf::Sound` can ton tai trong khi phat → dung mang tinh
  ```
  static sf::Sound sounds[8];  // Pool am thanh
  static int soundIndex = 0;
  // Moi lan phat: sounds[soundIndex].setBuffer(buf); sounds[soundIndex].play();
  // soundIndex = (soundIndex + 1) % 8;
  ```
- [ ] Nhac nen: dung `sf::Music` (stream, khong load het vao RAM)

#### Buoc 5.2: Nang cap render - Background & Theme
- [ ] Background khong den: dung COLOR_BACKGROUND (wheat/be)
- [ ] Hoac load hinh nen tu texture: `backgroundTex.loadFromFile("assets/textures/background.png")`
- [ ] Ban co co mau go: COLOR_BOARD_BG
- [ ] Duong ke dam: COLOR_GRID_LINE
- [ ] Them bong (shadow) cho cac thanh phan UI

#### Buoc 5.3: Nang cap render - Animation
- [ ] Hieu ung dat quan: quan hien ra tu nho lon (scale 0 → 1, trong 0.2s)
  ```
  float progress = animTimer / 0.2f;  // 0.0 → 1.0
  float scale = progress;
  piece.setScale(scale, scale);
  ```
- [ ] Hieu ung cursor: nhap nhay hoac pulsing
  ```
  float alpha = 128 + 127 * sin(clock.getElapsedTime().asSeconds() * 4);
  cursorRect.setFillColor(sf::Color(255, 255, 0, (int)alpha));
  ```
- [ ] Hieu ung thang: 5 quan thang nhap nhay + ve duong noi
- [ ] Hieu ung hoa: toan bo ban co mo dan

#### Buoc 5.4: Nang cap render - Menu dep
- [ ] Tieu de "CARO GAME" to, font dac biet, co bong
- [ ] Cac nut menu co hover effect (doi mau khi chon)
- [ ] Transition giua cac man hinh (fade in/out don gian)
  ```
  // Fade: ve hinh chu nhat den toan man hinh voi alpha giam dan
  sf::RectangleShape fade(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
  fade.setFillColor(sf::Color(0, 0, 0, fadeAlpha));
  window.draw(fade);
  ```

#### Buoc 5.5: Viet Settings, Help, About
- [ ] `handleSettings()`: doi ngon ngu, am luong, bat/tat SFX
- [ ] `handleHelp()`: hien phim tat va cach choi
- [ ] `handleAbout()`: ten nhom, mon hoc, nam hoc

#### Buoc 5.6: MOUSE SUPPORT (Bonus)
- [ ] Trong game loop, xu ly them `sf::Event::MouseButtonPressed`:
  ```
  if (event.mouseButton.button == sf::Mouse::Left) {
      int row, col;
      if (renderPixelToBoard(event.mouseButton.x, event.mouseButton.y, row, col)) {
          // Xu ly nhu Enter tai vi tri (row, col)
      }
  }
  ```
- [ ] Hover effect: khi di chuot qua o → highlight nhe

#### Buoc 5.7: FINAL TEST & POLISH
- [ ] Choi thu tat ca che do: PvP Basic, PvP Speed, PvC Easy/Med/Hard/Expert
- [ ] Test save/load voi moi che do
- [ ] Test undo trong PvP va PvC
- [ ] Test hint trong PvC
- [ ] Kiem tra khong crash, khong leak memory
- [ ] Dieu chinh mau sac, font size cho dep
- [ ] Dieu chinh do kho AI cho can bang

---

## 4. CHI TIET TUNG FILE .CPP CAN VIET

### `src/main.cpp` (~30-50 dong)
```
Vai tro: Entry point duy nhat
1. Tao sf::RenderWindow(sf::VideoMode(1280, 720), "Caro Game")
2. Tao GameResources, goi renderLoadResources()
3. Goi gameRun(window, res)
4. return 0
```

### `src/board.cpp` (~200-250 dong)
```
Vai tro: Toan bo logic ban co, khong lien quan den SFML
- boardInit, boardResetRound, boardResetAll
- boardPlacePiece, boardSwitchTurn, boardMoveCursor
- boardCheckWin (kiem tra 4 huong, luu WinLine)
- boardCheckDraw (dem o trong)
- boardEvaluateResult
- boardUndo (PvP: 1 nuoc, PvC: 2 nuoc)
- boardIsEmpty, boardIsValid, boardCountEmpty
```

### `src/bot.cpp` (~400-500 dong) *** TRONG TAM ***
```
Vai tro: AI voi 4 muc do kho
- botGetMove: dispatcher chon ham theo difficulty
- botEasyMove: random + chan co ban
- botMediumMove: heuristic scoring
- botHardMove: minimax depth 3
- botExpertMove: minimax depth 4 + move ordering
- botGetHint: goi y cho nguoi choi
- botScoreLine: cham diem 1 chuoi (xem bang diem)
- botEvaluatePosition: danh gia 1 o trong
- botEvaluateBoard: danh gia toan bo ban co
- botMinimax: thuat toan minimax + alpha-beta
- botGetCandidates: tim o lan can
- botSortCandidates: sap xep theo diem
- botCheckImmediateWin, botCheckImmediateBlock
```

### `src/render.cpp` (~500-700 dong)
```
Vai tro: Tat ca code ve SFML
- renderLoadResources: load font/texture/sound
- renderBoard: ve grid
- renderPieces: ve X/O
- renderCursor: highlight o dang chon
- renderHint: highlight goi y
- renderWinLine: ve duong thang
- renderPlayerPanel: ten, moves, wins
- renderTurnTimer: progress bar
- renderGameTimer: thoi gian van
- renderMainMenu, renderModeSelect, renderDifficultySelect, ...
- renderGameOver, renderSaveScreen, renderLoadScreen
- renderPlaceEffect, renderWinEffect, renderDrawEffect
- renderBoardToPixel, renderPixelToBoard, renderTextCentered
```

### `src/menu.cpp` (~400-500 dong)
```
Vai tro: Game loop + dieu huong tat ca man hinh
- gameRun: vong lap chinh voi switch(screen)
- handleMainMenu: W/S/Enter dieu huong
- handleModeSelect, handleDifficultySelect, handleStyleSelect
- handleInputNames: xu ly sf::Event::TextEntered
- handleGameplay: *** VONG LAP GAME CHINH ***
  - Input WASD/Enter/Z/H/ESC/L
  - Bot turn (neu PvC)
  - Timer update (neu Speed)
  - Win/Draw check
  - Render
- handlePauseMenu, handleGameOver
- handleSaveScreen, handleLoadScreen
- handleSettings, handleHelp, handleAbout
```

### `src/save_load.cpp` (~200-250 dong)
```
Vai tro: Doc/ghi file save
- saveGame, loadGame
- saveGetList, saveAddToList
- saveDeleteFile, saveRenameFile
- saveFileExists, saveCountFiles
```

### `src/sound.cpp` (~80-100 dong)
```
Vai tro: Quan ly am thanh
- soundLoadAll: load tat ca .wav/.ogg
- soundPlayBGM, soundSetBGMVolume
- soundPlayMove, soundPlayPlace, soundPlaySelect
- soundPlayWin, soundPlayDraw
- soundSetSFXEnabled, soundIsSFXEnabled
```

### `src/timer.cpp` (~60-80 dong)
```
Vai tro: Timer don gian (khong thread)
- timerStart, timerUpdate (tru deltaTime)
- timerResetTurn, timerPause, timerResume
- timerGetTurnPercent, timerGetGamePercent
- timerIsTurnExpired, timerIsGameExpired
- timerGetTurnSecondsLeft, timerGetGameSecondsLeft
```

### `src/language.cpp` (~150-200 dong)
```
Vai tro: 2 bo van ban Viet/Anh
- langGetText: tra ve TextStrings day du
- langGetCurrent, langSetCurrent, langToggle
```

---

## 5. BANG DIEM AI (PATTERN SCORING TABLE)

Day la bang diem cho ham `botScoreLine()`. Quyet dinh suc manh AI.

| Pattern               | openEnds | Score     | Giai thich                       |
|-----------------------|----------|-----------|----------------------------------|
| 5+ lien tiep          | bat ky   | 1,000,000 | THANG - uu tien tuyet doi        |
| 4 lien tiep           | 2        | 100,000   | Thang chac chan (doi thu ko chan duoc) |
| 4 lien tiep           | 1        | 5,000     | Doi thu phai chan ngay            |
| 3 lien tiep           | 2        | 5,000     | Nguy hiem - buoc doi thu chan     |
| 3 lien tiep           | 1        | 500       | De doa trung binh                |
| 2 lien tiep           | 2        | 500       | Tiem nang phat trien             |
| 2 lien tiep           | 1        | 50        | It nguy hiem                     |
| 1 (don le)            | 2        | 10        | Moi bat dau                      |
| 1 (don le)            | 1        | 5         | Yeu                              |
| Bi chan 2 dau         | 0        | 0         | Vo gia tri                       |

**Diem PHONG THU = Diem TAN CONG * 0.9**
(Uu tien tan cong hon phong thu mot chut)

**Diem vi tri trung tam (bonus):**
```
Khoang cach tu trung tam = abs(row - 7) + abs(col - 7)
centerBonus = max(0, (14 - khoangCach) * 2)
```
→ O giua ban co duoc cong them diem

---

## 6. ASSETS CAN CHUAN BI

### Fonts (BAT BUOC)
| File | Goi y | Link |
|------|-------|------|
| `main_font.ttf` | Roboto, Open Sans | fonts.google.com |
| `title_font.ttf` | PressStart2P, Bungee | fonts.google.com |

### Sounds (BAT BUOC)
| File | Mo ta | Luu y |
|------|-------|-------|
| `bgm.ogg` | Nhac nen game | Dung .ogg (SFML ho tro tot) |
| `move.wav` | Di chuyen cursor | Ngan, nhe |
| `place.wav` | Dat quan co | "Click" hoac "Pop" |
| `select.wav` | Chon menu | Ngan |
| `win.wav` | Thang van | Vui, phan khich |
| `draw.wav` | Hoa | Trung tinh |

### Textures (TUY CHON)
| File | Mo ta |
|------|-------|
| `background.png` | Hinh nen (1280x720) |

---

## 7. LUU Y QUAN TRONG

### Khong OOP
- TUYET DOI khong dung `class`. Chi dung `struct` + ham toan cuc
- Bien global chi dat trong .cpp, khai bao `extern` trong .h neu can chia se
- Uu tien truyen `GameState&` qua tham so thay vi dung bien global

### SFML Tips
- `sf::Font`, `sf::Texture`, `sf::SoundBuffer` phai ton tai SUOT doi cua `sf::Text`/`sf::Sprite`/`sf::Sound` su dung chung → giu trong `GameResources`
- Ve theo thu tu: background → board → pieces → cursor → UI → effects (truoc bi de sau)
- Moi frame: `window.clear()` → ve tat ca → `window.display()`

### Performance Bot
- **KHONG** duyet toan bo 225 o trong minimax. Chi duyet candidates (o lan can)
- Voi depth 3: khoang 20-30 candidates moi buoc → 20^3 = 8000 node → nhanh
- Voi depth 4 + move ordering: alpha-beta cat bo ~60-70% → van nhanh

### So sanh voi du an ban (CSLT---N---CARO)
| Van de ban goc | Cach khac phuc |
|---|---|
| Code lap (6 ham GamePlay gan giong nhau) | 1 ham `handleGameplay()` duy nhat, dung state.mode/style |
| Bien global tran lan | Gom vao `GameState` struct, truyen qua tham so |
| Bot evaluate don gian | Bang diem chi tiet, xet openEnds |
| Minimax depth 2 | Depth 3-4 + move ordering + chi duyet candidates |
| Timer dung thread (race condition) | Dung sf::Clock + deltaTime (an toan) |
| Switch-case fall-through bug | Xu ly tung case rieng biet |
| Tim o bang toa do pixel (O(n^2)) | Truy cap truc tiep board[row][col] |
| Menu bug (choicestyle thay vi choicegame) | Dung enum ro rang |

---

## TONG KET THU TU LAM VIEC

```
Phase 1 (2-3 ngay):  assets + language + board + render co ban + main
Phase 2 (3-4 ngay):  menu + gameplay PvP + win/draw + undo + cac man hinh
Phase 3 (4-5 ngay):  bot Easy → Medium → Hard → Expert + hint
Phase 4 (2-3 ngay):  timer + save/load
Phase 5 (3-4 ngay):  sound + animation + UI nang cap + polish + test
```

**Tong: ~14-19 ngay lam viec**

Chuc cac ban lam viec hieu qua! 💪
