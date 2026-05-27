# Session Handover — 24/05/2026

> 📌 **File này ghi lại CHI TIẾT** mọi thay đổi đã làm trong session vừa rồi.
> Đọc sau khi đọc `CLAUDE.md` (root). Mục đích: giúp Claude mới hiểu context + tránh redo việc đã làm.

---

## A. Trạng thái bắt đầu session (đầu ngày)

**Game trước đó (kế thừa):**
- Code C++/SFML đã chạy được (gameplay, menu, save/load)
- Font cũ: `main_font.ttf` (Fredoka), `title_font.ttf` (Luckiest Guy)
- Assets thiếu hoàn toàn (sounds + textures = 0 file)
- UI: chỉ vẽ text + rectangle procedural
- Background: solid color
- Không có mascot, không có texture quân cờ

**Docs đã có:**
- `docs/PRESENTATION_PLAN.md` (15 slides outline cho thuyết trình 20 phút)
- `docs/ASSETS_PLAN.md` (kế hoạch asset chi tiết)

---

## B. Tổng quan thay đổi session vừa rồi

User chia làm nhiều phases:
1. **Wire assets** — tải + integrate fonts/sounds/textures
2. **UI Polish** — background xuyên suốt, mascot animation, panel design
3. **Bug fixes** — UTF-8, font Vietnamese, alignment, slider volume, etc.
4. **Texture upgrades** — AI-generated logo + button frame + VICTORY/DEFEAT banners
5. **Window resize** — Maximize button + letterbox

---

## C. Chronological log (theo thứ tự thực tế)

### Phase 1: Wire assets ban đầu

**User actions:**
- Tải 2 font Bagel Fat One + Be Vietnam Pro từ Google Fonts (vào folder `Bagel_Fat_One,Be_Vietnam_Pro/`)
- Tải 6 sounds từ Kenney.nl, Zapsplat (vào folder `AmThanh/`)
- Tạo/tải textures mascot Goku + Vegeta + background + quân X/O (vào folder `Nhanvat/`)

**Claude actions:**
- Verify dimensions (background 1285×720, mascot 200×405, pieces 36×36)
- Verify alpha channels
- Copy + rename file vào `assets/fonts/`, `assets/sounds/`, `assets/textures/`
- Rename `bmg.ogg` → `bgm.ogg` (typo fix)
- Update `game_types.h` thêm 6 mascot texture fields
- Update `renderLoadResources()` load tất cả
- Update `renderPlayerPanel()` vẽ mascot với state logic (Win/Over/Idle)
- Wire texture X/O vào `renderPieces()`

### Phase 2: Board.png alignment issue

**Problem:** board.png AI tạo có ~12 cột không khớp với `BOARD_SIZE = 15`. Quân cờ không nằm giữa ô.

**Fix:**
- Drop board.png (vẫn lưu file để backup)
- Vẽ procedural grid 15×15 trong `renderBoard()`
- Semi-transparent wheat panel + brown grid lines + dark wood outline

### Phase 3: Background xuyên suốt

**Problem:** Background ảnh chỉ hiện ở gameplay, các menu khác nền xám đặc.

**Fix:**
- Tạo `renderBackdrop(window, res, dimForMenu)` helper:
  - Vẽ background image full screen
  - Nếu `dimForMenu=true` → thêm overlay dark blue 67% alpha (cho text menu đọc rõ)
- Replace tất cả `window.clear(COLOR_MENU_BG)` → `renderBackdrop(window, res, true)`
- 10 chỗ cần update (Main, Mode, Difficulty, Style, InputNames, Save, Load, Settings, Help, About)

### Phase 4: i18n bugs (UTF-8)

**Problems phát hiện:**
1. "Tieng Viet" hardcoded thay vì `u8"Tiếng Việt"` → fix qua language module
2. Double colons `::` (do `txt.language` đã có `:` + code thêm `": "`)
3. Volume control không có visual indicator
4. PvC mode hỏi tên 2 người chơi (chỉ cần 1)
5. Save/Load hint text không dấu
6. Bot name display "MÃiy" (mojibake) thay vì "Máy"
7. Timer text + panel text Vietnamese không dấu
8. Save messages "Luu thanh cong" không dấu

**Fixes:**
- Thêm strings mới vào `language.h` + `language.cpp` (VN + EN):
  - `langVietnamese`, `langEnglish`
  - `saveHintBottom`, `saveHintMouse`, `loadHintBottom`, `loadHintMouse`, `settingsHint`
  - `inputNameHintPvP`, `inputNameHintPvC`, `botName`
  - `yourTurn`, `turnTimeBar`, `gameTimeLabel`, `defeatText`
  - `msgSaveOK`, `msgSaveError`, `msgFileExists`, `msgMaxFiles`, `msgFileDeleted`, `msgLoadError`
- Fix `renderPlayerPanel`: dùng `sf::String::fromUtf8` cho ALL `setString` calls
- Fix `renderInputNames`: thêm parameter `bool isPvC` → chỉ hiện 1 ô khi PvC
- Fix `handleInputNames`: skip Tab khi PvC, auto-set name2 = `txt.botName`

### Phase 5: Slider volume + button frame

**Problem:** Volume control là text "Volume: 30%" — không trực quan. Mũi tên ◀ ▶ bị box (font không support).

**Fix slider:**
- Vẽ track 360×8 + fill xanh + handle tròn trong `renderSettings`
- Update `handleSettings`: click + drag để chỉnh
- Snap mỗi 5%

### Phase 6: Vietnamese input

**Problem:** Input tên người chơi không gõ được tiếng Việt (filter `< 128` chặn UTF-8).

**Fix `handleInputNames`:**
- Bỏ filter ASCII
- Encode Unicode codepoint → UTF-8 bytes inline
- Backspace: xóa 1 ký tự UTF-8 (1-4 bytes) thay vì 1 byte cuối
- Max byte limit 30 thay vì 15 char

### Phase 7: Text sharpness (sub-pixel + outline)

**Problem:** Chữ "THỜI GIAN LƯỢT" mờ. User nhận xét chữ trên background ảnh khó đọc.

**Fixes:**
- `renderTextCentered`: snap origin + position vào integer pixel (dùng `std::round`)
- Tăng size timer text: 13→16, 16→18
- Add parameter `outlineColor` + `outlineThickness` cho `renderTextCentered`
- Timer: dark plate xanh navy + outline đen 2px + chữ trắng pure
- Timer "Thời gian ván" đổi đỏ khi ≤ 30s
- GameOver text: outline đen 3px + dark plate phía sau

### Phase 8: Font upgrade (Bagel → Coiny)

**Problem:** Bagel Fat One thiếu dấu Việt phức tạp (Ơ, Ư, Ậ, Ọ, Ữ, Ằ → hiện thành □ box).

**User tried:**
- Bagel Fat One (failed - thiếu dấu)
- Coiny (success - full Vietnamese)

**Final fonts:**
- `title_font.ttf` = **Coiny Regular** (bubble cartoon)
- `main_font.ttf` = **Be Vietnam Pro Regular** (clean UI body)

### Phase 9: AI textures + background removal

**User generated 4 textures bằng AI (Leonardo.ai):**
1. `logo_caro.png` — CARO sticker bubble cream
2. `banner_winner.png` — VICTORY yellow + confetti
3. `button_frame.png` — Rounded gold/dark-blue button
4. `banner_defeat.png` — DEFEAT red + cracks

**Problem:** AI output là PNG 24-bit (no alpha), checkerboard pattern bị "baked into image".

**Fix:**
- Viết PowerShell script remove background:
  - Detect pure grayscale pixels (R==G==B trong tolerance 4)
  - Trong range gray cụ thể per image
  - Set alpha = 0
- **Bug ban đầu:** Save lại PNG mất alpha vì source là 24-bit. Fix bằng convert sang 32-bit Argb bitmap trước khi xử lý.
- Apply per image với range khác nhau:
  - logo_caro: gray 50-200
  - button_frame: gray 180-230
  - banner_winner: gray 30-254

`banner_defeat.png` user generate sau (round 2) đã có alpha sẵn (Format32bppArgb) — không cần process.

### Phase 10: Wire textures vào game

**Add to `GameResources`:**
- `logoCaroTex`, `bannerWinTex`, `bannerDefeatTex`, `buttonFrameTex`

**Wire vào code:**
- `drawCaroLogo()` helper — vẽ logo thay text "TRÒ CHƠI CARO" trên Main Menu
- `drawMenuButton()` helper — vẽ button frame sau mỗi menu item (selected: vàng kim 420×64, normal: trắng 380×56)
- VICTORY/DEFEAT trong `renderPlayerPanel`:
  - Vị trí: giữa text info và mascot
  - Width 200px (preserve aspect)
  - Winner → VICTORY texture, Loser → DEFEAT texture
  - Hòa → không hiện gì

### Phase 11: Window resize support

**User request:** Cho phép phóng to màn hình game.

**Implementation (Phương án A):**
- `main.cpp`: `sf::Style::Close | sf::Style::Resize`
- `applyLetterbox()`: Tính viewport giữ tỷ lệ 16:9 + viền đen nếu lệch
- `handleCommonEvent(window, event)`:
  - Map mouse coords pixel→view IN-PLACE (event được pass non-const)
  - Handle Closed + Resized
  - Return true nếu event xử lý xong → caller `continue`
- Replace 13 chỗ `if (event.type == sf::Event::Closed) window.close();` → `if (handleCommonEvent(window, event)) continue;`
- **Mouse hit-test code 21 chỗ KHÔNG cần đổi** (vì mouse coords đã được map trước khi đến code đó)

---

## D. Files đã modify

### Headers
- `include/game_types.h`:
  - Thêm 6 mascot texture fields
  - Thêm 4 UI texture fields (logoCaro, bannerWin, bannerDefeat, buttonFrame)

- `include/render.h`:
  - Thêm `applyLetterbox()`, `handleCommonEvent()`
  - Mở rộng `renderTextCentered()` với 2 optional params: `outlineColor`, `outlineThickness`
  - Thêm `bool isPvC` param cho `renderInputNames()`
  - Thêm `GameResult result = RESULT_NONE` cho `renderGameplay()` + `renderPlayerPanel()`

- `include/language.h`:
  - Thêm ~15 string mới vào `TextStrings` struct

### Sources
- `src/main.cpp`:
  - Window style → Resize
  - Call `applyLetterbox()` initial

- `src/render.cpp` (file lớn nhất, nhiều thay đổi):
  - Thêm `applyLetterbox`, `handleCommonEvent`
  - `renderTextCentered()` outline + std::round snap
  - `renderBackdrop()` mới (replace inline `renderBackground`)
  - `renderBoard()` procedural grid (drop board.png)
  - `renderPieces()` dùng sprite X/O
  - `renderPlayerPanel()` + mascot + VICTORY/DEFEAT
  - `renderTurnTimer()` + `renderGameTimer()` outline + plate
  - `renderSettings()` slider visual
  - `renderInputNames()` PvC vs PvP mode
  - `renderGameOver()` dark plate + outline
  - `renderMenuGeneric()` button frame + logo support
  - Wire 4 UI textures load

- `src/menu.cpp`:
  - Replace 13 chỗ Closed event → `handleCommonEvent`
  - `handleInputNames()` UTF-8 input support + PvC skip logic
  - `handleSettings()` slider drag logic
  - Save messages dùng `txt.msg*`
  - Hint hardcoded → dùng `txt.*Hint*`

- `src/language.cpp`:
  - Thêm ~15 strings VN + EN (xem Phase 4)

### Assets (created/copied)
- `assets/fonts/title_font.ttf` ← Coiny Regular (đã thay từ Bagel Fat One)
- `assets/fonts/main_font.ttf` ← Be Vietnam Pro Regular
- `assets/textures/logo_caro.png` ← AI gen + xóa nền
- `assets/textures/button_frame.png` ← AI gen + xóa nền
- `assets/textures/banner_winner.png` ← AI gen + xóa nền
- `assets/textures/banner_defeat.png` ← AI gen (đã có alpha sẵn)
- 6 sounds + mascot textures (Phase 1)

### Assets (deleted)
- `assets/textures/board.png` (không dùng nữa)
- `assets/textures/board_original_backup.png` (backup khi test trước đó)

---

## E. Known gotchas & decisions

### 1. Font Vietnamese support — đã test
| Font | Vietnamese? | Notes |
|------|------------|-------|
| Bagel Fat One | ❌ Thiếu Ơ, Ư, Ậ, ... | Đã loại |
| Coiny | ✅ Full | Đang dùng cho title |
| Be Vietnam Pro | ✅ Native | Đang dùng cho body |
| Luckiest Guy | ⚠️ Basic | Đã loại |
| Fredoka | ✅ Basic | Đã loại để chuyển sang Be Vietnam Pro |

### 2. UTF-8 → SFML rendering
- `setString(std::string)` interprets bytes as Latin-1 → mojibake với UTF-8
- ✅ ĐÚNG: `setString(sf::String::fromUtf8(s.begin(), s.end()))`
- Helper `renderTextCentered()` đã handle sẵn

### 3. PNG transparency từ AI
- AI tạo PNG có thể là 24-bit (no alpha) với checkerboard "baked in"
- Phải convert 32-bit Argb trước khi process alpha
- Script trong PowerShell, dùng `System.Drawing.Bitmap`

### 4. Save/Load + Vietnamese player name
- ✅ Safe vì text format + getline (UTF-8 multi-byte không có `\n`)
- Không dùng binary fwrite of struct (sẽ broken với std::string heap allocation)

### 5. Window resize + mouse coords
- `event.mouseButton.x/y` ở PIXEL coords (window space)
- Phải map qua `window.mapPixelToCoords()` → view coords
- `handleCommonEvent()` làm việc này IN-PLACE → code hit-test cũ vẫn work

---

## F. Pending tasks (cho deadline)

### 🔥 Critical
1. **Chụp 13 screenshots cho slide** (xem `PRESENTATION_PLAN.md` section 3.5):
   - slide05_gameplay_overview.png
   - slide06_win_moment.png ⭐
   - slide07_save_screen.png, slide07_load_screen.png
   - slide08_*.png (6 screenshots cho UI showcase)
   - slide10_bot_thinking.png
   - slide11_pattern_real.png
   - slide12_speed_timer.png ⭐

2. **Nhóm 1 (Đại & Lực) làm slide PowerPoint** với:
   - 15 slides outline (đã có trong `PRESENTATION_PLAN.md`)
   - Pseudocode TYPED (không phải image) — convention trong PRESENTATION_PLAN
   - Insert 13 screenshots vào slot tương ứng
   - Diagram (flowchart, architecture)

3. **Tập demo 6 phút** (demo script trong PRESENTATION_PLAN section 5):
   - Nhật present 1 mình
   - Cần luyện trước

### ⏳ Nice-to-have (nếu còn thời gian)
- F11 fullscreen toggle
- Animation cho VICTORY/DEFEAT (scale-in + fade)
- BGM chuyển bài khác giữa menu và gameplay
- More sound effects (button hover, undo, hint)

---

## G. Reproducible commands

### Build
```powershell
$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
$proj = "D:\HCMUS\Programming C++\TTT\Caro_Project\Caro_Game_Group_1\CaroGame\CaroGame.vcxproj"
& $msbuild $proj /p:Configuration=Debug /p:Platform=x64 /nologo /verbosity:minimal
```

### Run
```
D:\HCMUS\Programming C++\TTT\Caro_Project\Caro_Game_Group_1\CaroGame\x64\Debug\CaroGame.exe
```

### Check texture properties
```powershell
Add-Type -AssemblyName System.Drawing
$img = [System.Drawing.Image]::FromFile("path\to\file.png")
"$($img.Width) x $($img.Height) - $($img.PixelFormat)"
$img.Dispose()
```

### Remove PNG background (xóa checkerboard)
Xem code đầy đủ trong git history (commit có message about "Remove background"), hoặc tham khảo Phase 9 trên.

---

## H. Người liên hệ

- **Project Lead + Presenter:** Nhật (vanhoangnhatbp1@gmail.com)
- **Nhóm 1 (Slides + Pseudocode):** Đại & Lực
- **Nhóm 2 (Code chính):** Vũ Quang
- **Nhóm 3 (UI Polish + Demo):** Nhật & Toàn

---

**Last updated:** 24/05/2026
**Session length:** Nhiều cuộc trò chuyện trải dài qua nhiều ngày
**Total tasks completed:** 21 (xem TaskList trong Claude)
