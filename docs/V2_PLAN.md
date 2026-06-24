# Caro Game V2 — Development Plan

> 📌 **File này là source of truth cho V2 development.**
> Tạo ngày 01/06/2026 sau khi V1 hoàn thành thuyết trình.
> Mục đích: backup kế hoạch khi switch session / compact / branch.

---

## 1. Status & Goal

| Field | Value |
|-------|-------|
| **V1 status** | ✅ COMPLETE — đã thuyết trình 1/6/2026 |
| **V2 status** | 🚧 IN PROGRESS — Sprint 1 task #21 done |
| **Goal** | Portfolio-grade polished game (đẹp + ổn) |
| **Engine** | KHÔNG đổi — giữ SFML 2.6.2 + Visual Studio 2022 |
| **Architecture** | KHÔNG đổi — giữ procedural C-style (free functions + POD structs) |
| **Total estimated effort** | ~24 giờ Claude code + ~10 giờ User asset prep + ~5 giờ User test |

---

## 2. Decisions Locked

Các quyết định đã chốt trong session ngày 09/06/2026. **KHÔNG đề xuất lại trừ khi User nói rõ.**

| Decision | Chốt | Rationale |
|----------|------|-----------|
| Skip binary save format | ✅ | File save < 500 bytes, binary không cải thiện thực tế |
| Skip engine change (Unity/Unreal/Godot) | ✅ | Stardew/Celeste/Hades đều 2D engine + artwork chất lượng cao. Đổi engine = viết lại 100%. |
| Skip sfeMovie / video background | ✅ | Lib stale từ 2023, build hell trên Windows. Dùng spritesheet thay. |
| Giữ procedural architecture | ✅ | Code đã ổn định, không refactor sang OOP |
| Phase 2 (loser→X) dùng hướng A (swap UI display) | ✅ | Logic giữ nguyên P1=X, P2=O; chỉ swap mascot/name khi loser=P2. Ít risk hơn rotate symbol. |
| Sprint 4 làm full 7 task | ✅ | Maximum visual upgrade |
| Asset strategy: Claude viết prompt AI gen cụ thể | ✅ | Sẽ tạo `docs/V2_ASSETS_PROMPTS.md` trước Sprint 4 |
| Giữ Dragon Ball theme cho V2 | ✅ 11/06 | Đồ án môn học, không kinh doanh → bản quyền không phải vấn đề. Theme ki/aura cộng hưởng mạnh với shader/particle Sprint 4. KHÔNG đổi art style. |
| #35 = CHỐT PA-2 (video), BỎ PA-1 | ✅ 11/06 | Ban đầu định A/B parallax vs video. User test PA-2 (image-to-video Veo → 106 frame JPG 1280×720) thấy đẹp → **bỏ hẳn PA-1 parallax**. #35 DONE. F2 toggle nền động/tĩnh giữ lại. |
| Reskin hiệu ứng #32/#33 theo chất DB | ✅ 11/06 | Confetti → vụ nổ tia ki vàng; victory flash kiểu Super Saiyan; aura glow theo màu player (cam X / xanh O). Không thêm task mới, chỉ reskin trong scope sẵn có. |
| ~~Nâng cấp quân X/O → ki orb glossy~~ | ❌ ĐẢO 11/06 | User thấy ki-orb không hợp gu → **GIỮ quân X/O cũ** (`x_piece.png` đỏ / `o_piece.png` xanh). Không gen mới. |
| Tách roster: anh hùng vs phản diện | ✅ 11/06 | **Anh hùng** (Goku/Vegeta sẵn + Gohan/Piccolo/Trunks/Krillin) = người chơi chọn (#34). **Phản diện** = độ khó bot (#36): Easy=Frieza, Medium=Cell, Hard=Buu, Expert=Broly. 2 pool KHÔNG trùng. PvC: panel bạn = anh hùng, panel đối thủ = phản diện theo độ khó. |
| 1 ảnh/nhân vật (bỏ 3-state mascot) | ✅ 11/06 | Mỗi nhân vật chỉ 1 tư thế. Thắng/thua xử lý bằng code (banner sẵn có + confetti + aura + tint tối loser), KHÔNG gen 3 trạng thái/nhân vật. Đổi nhẹ so với V1 (mascot 3-state) đổi lấy đa dạng nhân vật. |

---

## 3. Sprint Plan Overview

```
Sprint 1 (Foundation):   #21, #22, #23 (skip #27 sounds defer)   → ~2.5 giờ
Sprint 2 (Navigation):   #24, #25, #26                            → ~3 giờ
Sprint 3 (Save/Load):    #29, #28, #30                            → ~3.5 giờ
Sprint 4 (Visual AAA):   #31, #32, #33, #36, #37, #34, #35        → ~14 giờ
```

**Critical path:** Sprint 1 → 2 → 3 → 4. Sprint 4 cần Sprint 1 (utils.h) làm prerequisite.

---

## 4. Task Detail Table

### Sprint 1 — Foundation Cleanup ✅ COMPLETE 09/06/2026

| # | Task | Effort | Owner | Risk | Status |
|---|------|--------|-------|------|--------|
| 21 | Xóa load `board.png` unused | 5 phút | 🤖 Claude | None | ✅ **DONE 09/06** |
| 22 | Tạo `utils.h` DRY (`inBounds`, `DIRS`, `countConsecutive`) | 25 phút | 🤖 Claude | Low | ✅ **DONE 09/06** |
| 27 | Sounds: undo + hint + alarm (SFX) + BGM theo màn (menu/gameplay) | 30 phút | 🤖 + 👤🎨 | None | ✅ **DONE 12/06** |
| 23 | Logic loser→X (hướng A: swap UI display) | 45 phút | 🤖 + 👤🧪 | Medium | ✅ **DONE 09/06** |

### Sprint 2 — Navigation Polish

| # | Task | Effort | Owner | Risk | Status |
|---|------|--------|-------|------|--------|
| 24 | Back button đồng bộ NewGame flow (Choose Mode/Style/Name) | 45 phút | 🤖 + 👤🧪 | Low | ✅ **DONE 10/06** |
| 25 | Exit/Save buttons góc phải dưới gameplay | 1 giờ | 🤖 + 👤🧪 | Low | ✅ **DONE 10/06** |
| 26 | ESC pause menu expand (Language + Volume + SFX inline) | 1.5 giờ | 🤖 + 👤🧪 | Low | ✅ **DONE 10/06** |

### Sprint 3 — Save/Load Modernize (SKIP binary) ✅ COMPLETE 10/06/2026

| # | Task | Effort | Owner | Risk | Status |
|---|------|--------|-------|------|--------|
| 29 | Bỏ MAX_SAVE_FILES + `filesystem::directory_iterator` | 1 giờ | 🤖 + 👤🧪 | Low | ✅ **DONE 10/06** |
| 28 | `unordered_map<string, SaveMetadata>` cho query O(1) | 1 giờ | 🤖 | Low | ✅ **DONE 10/06** |
| 30 | Scrollable list (mouse wheel + W/S key) | 1.5 giờ | 🤖 + 👤🧪 | Medium | ✅ **DONE 10/06** |

### Sprint 4 — Visual AAA Upgrade

| # | Task | Effort | Owner | Risk | Status |
|---|------|--------|-------|------|--------|
| 31 | RoundedRectangleShape + hover glow | 2 giờ | 🤖 | Low | ✅ **DONE 11/06** (glow toàn UI) |
| 32 | Particle confetti on win (DIY VertexArray) | 3 giờ | 🤖 | Low | ✅ **DONE** (gỡ 11/06 rồi **KHÔI PHỤC 12/06** — chạy cùng shockwave) |
| 33 | Fragment shader victory shockwave | 4 giờ | 🤖 | Medium | ✅ **DONE 11/06** (shockwave.frag, guard isAvailable; bỏ menu-glow trùng #31) |
| 36 | Villain theo độ khó + Mode tiles | 45 phút | 🤖 + 👤🎨 | None | ✅ **DONE 11/06** (Frieza/Cell/Buu/Broly + 2 mode tile, bỏ icon khung vàng) |
| 37 | Modern input field (caret + glow border) | 1 giờ | 🤖 | None | ✅ **DONE 11/06** (caret thở + pop gõ + glow focus) |
| 34 | Character select screen 6 hero | 3 giờ | 🤖 + 👤🎨 | Low | ✅ **DONE 11/06** (lưới 3×2, PvP 2 phase, save/load hero) |
| 35 | Animated background (chốt PA-2 video, bỏ PA-1 parallax) | 4 giờ | 🤖 + 👤🎨 | **Medium-high** | ✅ **DONE 11/06** (106f JPG 1280×720, ping-pong 18fps, F2 toggle) |

**Owner legend:**
- 🤖 = Claude code độc lập
- 👤🎨 = User chuẩn bị asset (PNG/sound)
- 👤🧪 = User test thực tế (UI/UX feedback)


---

## 5. 🔍 BÁO CÁO AUDIT KỸ THUẬT TOÀN DIỆN (12/06/2026)

> Senior C++ Game Dev review toàn bộ ~6.300 dòng (11 header + 10 .cpp + shader), kiểm tra chéo từng hàm/chuỗi/hằng số. **Báo cáo = phân tích, KHÔNG tự sửa** (trừ phần đã thực thi ở §5.8+). Mọi mục ghi đích danh `file:dòng`.

### 5.1 — PHẦN A: Kiến trúc tổng thể

**Sơ đồ phụ thuộc (một chiều, KHÔNG vòng):**
```
main.cpp → menu.cpp (orchestrator)
  → board.cpp / bot.cpp / timer.cpp / save_load.cpp → utils.h/cpp (DIRS, inBounds, countConsecutive)
  → sound.cpp / language.cpp / render.cpp → rounded_rect.h
game_types.h = từ điển chung (hằng số/enum/struct), mọi module include
```

**Tốt ✅:** Phân tầng sạch, logic cờ tách hẳn render, không leak, utils.h gom DRY đúng chỗ, tài nguyên load 1 lần truyền tham chiếu.

**Trừ ⚠️:**
- `render.cpp` ~2050 dòng = "god file" (load + letterbox + 25 hàm vẽ + confetti + shader). Nên tách: resources / render_screens / render_fx.
- `renderLoadResources` load CẢ âm thanh ([render.cpp:220-226]) — sai module. `soundLoadAll` lại không load gì.
- Trạng thái global rải rác: currentLanguage, bgmVolume/sfxEnabled/soundPool/currentBGMTrack, g_bgAnimated + nhiều sf::Clock static, g_metaCache.

**Vòng lặp lồng nhau:** mỗi màn = 1 `handle*()` có event loop riêng; màn con (Pause/Save/GameOver) gọi LỒNG trực tiếp từ gameplay (3 tầng). Hệ quả: gameRun chỉ thấy chuyển màn cấp cao → BGM chỉ đổi ở đó; 3 enum `SCREEN_PAUSE_MENU/GAME_OVER/SAVE` không bao giờ được dispatch (bẫy R7).

### 5.2 — PHẦN B: Main Flow

```
main: srand→window 1280×720→letterbox→renderLoadResources→settingsLoad→gameRun
gameRun: BGM theo màn (PLAYING→bgm_game, else bgm_menu) → dispatch
MAIN_MENU→MODE(2 tile)→[DIFFICULTY 4 villain nếu PvC]→STYLE→CHAR_SELECT(PvP 2 phase)
  →INPUT_NAMES(boardResetAll+timerStart Speed)→PLAYING
PLAYING/frame: dt→timerUpdate→chuông≤5s→check hết giờ→events→render. Bot chạy ĐỒNG BỘ (block).
GAME OVER (lồng): confetti 280 + shockwave + mascot win/lose → "Chơi tiếp?" Có→boardResetRound(loser đi trước=X) / Không→"Lưu?"→SaveScreen / MAIN_MENU
```
**~~Không nhất quán~~ → ĐÃ KIỂM CHỨNG KHÔNG PHẢI LỖI (13/06, §5.18):** thắng do hết giờ vào Game Over thẳng; thắng đủ 5 quân (hoặc hòa) cũng vào **NGAY trong cùng 1 sự kiện** — chính cú đặt quân đi qua guard `result==NONE` (đặt quân) rồi guard `result!=NONE` ngay sau đó ([menu.cpp:826](src/menu.cpp:826)/[886](src/menu.cpp:886)) → `return handleGameOver`. KHÔNG cần click/phím thừa. Phân tích cũ ở dòng này là SAI; User test xác nhận đúng.

### 5.3 — 🎯 MỤC TIÊU 1: Dead Code & Redundant (đã verify 0 nơi gọi)

> ✅ **ĐÃ DỌN XONG 12/06 (Ưu tiên 3)** — toàn bộ 1A/1B/1C/1D/1E bên dưới đã xóa khỏi code (re-verify bằng grep trước khi xóa từng món). 1F giữ nguyên theo ghi chú "giữ cũng được". Chi tiết §5.11.

**1A. Hàm chết (11) — xóa cả khai báo + định nghĩa:**
| Hàm | Định nghĩa |
|-----|-----------|
| `renderScreen` (dispatcher không ai gọi, comment gây hiểu lầm) | render.cpp:2025 |
| `renderWinEffect` / `renderDrawEffect` (thay bằng confetti+shockwave) | render.cpp:1998 / 2013 |
| `confettiActive` | render.cpp:1524 |
| `timerGetGamePercentP1` / `P2` (panel dùng giây, không % ) | timer.cpp:64 / 68 |
| `boardCountEmpty` (checkDraw đã O(1) bằng moveCount) | board.cpp:15 |
| `soundPlayBGM` (chết khi chuyển soundPlayBGMTrack 12/06) | sound.cpp:34 |
| `saveGetMetadata` (UI metadata gỡ ở Sprint 3) | save_load.cpp:222 |
| `saveCountFiles` (hết giới hạn file) | save_load.cpp:267 |
| `saveRenameFile` (KHÔNG có UI đổi tên) | save_load.cpp:241 |

**1B. Hàm "xác sống":** `soundLoadAll` (chỉ setLoop/setVolume lên Music chưa mở file — vô tác dụng, soundPlayBGMTrack làm lại) [sound.cpp:23, gọi menu.cpp:17]; `animClock` trong renderGameOver (tạo + restart rồi `(void)` — di tích animation cũ) [render.cpp:1563-1583].

**1C. Chuỗi i18n chết (14 field × 2 ngôn ngữ):** `turn, hintText, undoText, nameError, saveSuccess, loadSuccess, fileExists, maxSaveReached, msgMaxFiles, deleteFile, renameFile, gameTime, turnTime, timeUp` — 0 nơi đọc. Xóa khỏi language.h:11-130 + 28 dòng gán language.cpp.

**1D. #define chết (game_types.h):** `COLOR_BOARD_BG/GRID_LINE/WIN_LINE` (39,40,45 — render hardcode màu khác); `UI_PAUSE_TITLE_Y/START_Y/STEP` (66-68 — ❗GIÁ TRỊ SAI: thật là 150/230/60, bị khai báo cục bộ 2 nơi render.cpp:1188 + menu.cpp:904); `UI_MENU_HALF_WIDTH/HEIGHT` (62-63 — menuHitTest hardcode 200/22).

**1E. Include thừa game_types.h (7-13):** `<algorithm><cmath><cstdlib><ctime><fstream><vector>` (header chỉ cần `<string>`+2 SFML). ⚠️ Khi xóa: render.cpp:589 dùng `snprintf` mà KHÔNG include `<cstdio>` (sống nhờ include gián tiếp) → phải thêm `<cstdio>` vào render.cpp.

**1F. Redundant nhỏ (giữ cũng được):** `struct Cell{int value;}` (bọc 1 int, .value ở ~40 chỗ — **CÓ DÙNG** khắp nơi, chỉ là wrapper quanh int, KHÔNG phải dead code); `WinLine.count` = số quân thắng thực tế, **CÓ THỂ > 5** (đường thắng dài hơn 5 quân). Claim cũ "luôn 0 hoặc 5" là SAI — comment trong code (game_types.h:210) vốn đã đúng. (đính chính 13/06, §5.18)

### 5.4 — 🎯 MỤC TIÊU 2: Naming Convention

| # | Hiện tại | Vấn đề | Đề xuất |
|---|----------|--------|---------|
| 1 | `soundLoadAll` | hứa load all, không load gì | ✅ ĐÃ XÓA (Ưu tiên 3) |
| 2 | `mascotGoku/Vegeta` vs `heroGohan/Piccolo/Trunks/Krillin` | cùng vai 6 hero, 2 tiền tố | ✅ ĐÃ ĐỔI `heroGoku/heroVegeta` (Ưu tiên 6 — kèm heroSetByIndex + renderLoadResources) |
| 3 | comment Cell.value "−1:P1(X), 1:P2(O)" | SAI từ #23 (X/O động theo firstPlayerOfRound) | ✅ ĐÃ SỬA comment (Ưu tiên 6) |
| 4 | Move.player=−1/+1 vs firstPlayerOfRound/loserId=1/2 | 2 hệ mã hóa người chơi song song; bot.cpp lại định nghĩa BOT_PLAYER=1/HUMAN=−1 | ✅ ĐÃ THÊM `CELL_P1=-1, CELL_P2=+1` game_types.h (Ưu tiên 6 — bot/board/save_load dùng thống nhất) |
| 5 | `#define BOARD_OFFSET_X UI_BOARD_OFFSET_X` render.cpp:22 | 1 hằng 2 tên | ✅ ĐÃ BỎ alias, dùng thẳng UI_ (Ưu tiên 7) |
| 6 | `renderLoadResources` load sound+shader | sai module | ✅ ĐÃ chuyển load SFX sang `soundLoadResources()` sound.cpp, main() gọi (Ưu tiên 7) |
| 7 | comment BG_FRAME_COUNT "120@24fps 237MB" | sai (106 frame, 18fps, ~390MB) | ✅ ĐÃ cập nhật comment (Ưu tiên 7) |
| 8 | comment "bg_frame_00..23.png" render.cpp:151 | thật là .jpg 00..105 | ✅ ĐÃ cập nhật comment (Ưu tiên 7) |
| 9 | handleMainMenu literal 280.f/60.f (menu.cpp:118,126) | = UI_MENU_START_Y/STEP | ✅ ĐÃ dùng hằng UI_MENU_START_Y/STEP (Ưu tiên 7) |
| 10 | lỗi 15 ký tự hardcode `u8"Tên người chơi..."` menu.cpp:567 | VI PHẠM i18n (English vẫn ra tiếng Việt) | ✅ ĐÃ THÊM field `nameTooLong` (Ưu tiên 6) |

### 5.5 — 🎯 MỤC TIÊU 3: Tối ưu luồng (CHỈ phân tích, chưa sửa)

- **3.1 ✅ ĐÃ SỬA (Ưu tiên 4) `langGetText` copy cả struct ~75 string MỖI lần gọi**, gọi nhiều lần/frame → ~300-400 cấp phát heap/frame. → đổi `const TextStrings&` + 2 bản static. Rủi ro ~0. [§5.12]
- **3.2 ✅ ĐÃ SỬA (Ưu tiên 7) Minimax không check thắng ở node giữa** [bot.cpp] → thêm `botIsWinningMove` + node terminal trả `±(SCORE_WIN−depth)` (ưu tiên thắng nhanh / hoãn thua, không để minimax đào sâu làm loãng điểm). Verify đối kháng: dấu/reset/overflow/collision đúng, không kịch bản chọn nước tệ hơn. [§5.15]
- **3.3 ✅ ĐÃ SỬA (Ưu tiên 4) "Bot đang suy nghĩ" chỉ hiện ở Speed** [menu.cpp:612] → Basic+Expert đứng im, có thể "Not Responding". → bỏ điều kiện STYLE_SPEED. [§5.12]
- **3.4 ✅ ĐÃ SỬA (Ưu tiên 4) Click lọt khe khi bot nghĩ** (hàng đợi xả sau → đặt nhầm). → xả event chuột sau doBotMove. [§5.12]
- **3.5 ⏸️ User LOẠI TRỪ (Ưu tiên 8)** — `goto DO_CONFIRM` nhảy giữa switch [menu.cpp] → tách lambda confirmChoice. (Chạy đúng, chỉ là style; để sau.)
- **3.6 ✅ KHÔNG PHẢI LỖI (kiểm chứng 13/06, §5.18)** — tưởng "thắng 5 quân cần click thừa" nhưng guard `result!=NONE` ([menu.cpp:826](src/menu.cpp:826)/[886](src/menu.cpp:886)) chạy ngay sau guard đặt quân trong CÙNG 1 sự kiện → game-over hiện ngay. User test xác nhận. Phân tích §5.2 cũ sai → đã đính chính. KHÔNG cần làm gì.
- **3.7 ✅ ĐÃ SỬA (Ưu tiên 8)** Ô nhập tên save → dùng `drawInputField` (bo góc + glow + caret, luôn focused), đồng bộ #37. [§5.16]
- **3.8 parseMetadata lệch với file V1** (vô hại, metadata không hiển thị).

### 5.6 — 🎯 MỤC TIÊU 4: Rủi ro & Lỗ hổng

**🔴 CAO (người chơi gặp được):**
- **R1** ✅ ĐÃ SỬA — Save Speed → load = đồng hồ chết vĩnh viễn (mọi save timerPause→isRunning=0→load không resume). [save_load.cpp:51,118]
- **R2** ✅ ĐÃ SỬA — Click trượt mép trái/trên bàn cờ → đặt nhầm hàng/cột 0 (`(int)` cắt về 0). [render.cpp:95]
- **R3** ✅ ĐÃ SỬA — loadGame tin file tuyệt đối → moveCount rác ghi TRÀN moveHistory[225] (an toàn bộ nhớ). [save_load.cpp:119-133]

**🟠 TRUNG BÌNH:**
- **R4** ⏸️ **HOÃN — xử lý lúc nộp bài** (User quyết 13/06). Đường dẫn `../assets` phụ thuộc CWD; double-click exe trong Debug/ → font fail → main trả −1 IM LẶNG [main.cpp:21]. → lấy path từ exe / MessageBox khi fail.
- **R5** ✅ ĐÃ SỬA (12/06) — KHÔNG ghi đè được save: click file "copy tên để re-save" nhưng doSave từ chối tên trùng. UI mâu thuẫn. → **User chọn (a): CHO GHI ĐÈ** — Enter lần 1 tên trùng = hỏi xác nhận, Enter lần 2 = ghi đè. Chi tiết §5.10.
- **R6** — Bot block UI (= 3.3/3.4).

**🟡 THẤP:**
- **R7** ✅ ĐÃ SỬA (Ưu tiên 7) — `default` gameRun về `SCREEN_MAIN_MENU` thay vì `window.close()`. Nút Thoát Main Menu tự `window.close()` riêng (case 5) nên không ảnh hưởng. [§5.15]
- **R8** ✅ ĐÃ SỬA (Ưu tiên 7) — thêm `soundShutdown()` (stop + `resetBuffer()` 8 sound trong pool) gọi cuối `main()` trước khi `res`/audio device teardown. [§5.15]
- **R9** — `snprintf` thiếu `<cstdio>` [render.cpp:589] (= 1E).
- **R10** — KHÔNG có memory leak. Tất cả tài nguyên trong GameResources (RAII). VRAM tĩnh ~390MB (106 frame nền) — máy yếu load fail nhưng đã fallback nền tĩnh ✅.

### 5.7 — 🎯 MỤC TIÊU 5: Code lặp chưa gom hàm chung

> ✅ **Ưu tiên 5 (12/06): D2, D3, D4, D5, D6, D8.** ✅ **Ưu tiên 7 (13/06): D1, D7.** Mỗi đợt verify đối kháng (5 agent, verdict `preserved`, 0 finding lỗi) — chi tiết §5.13 + §5.15. **D9/D10 chưa làm** (giá trị thấp / D10 đụng define game_types.h).

- **D1 ✅ ĐÃ GOM (Ưu tiên 7) Slider âm lượng nhân bản 4 nơi:** logic → 3 static `sliderSetFromX`/`sliderContains`/`sliderNudge` (menu.cpp); vẽ → 1 `sliderDraw` static (render.cpp). Lambda mỗi màn + phím ±10 giờ gọi helper (truyền geometry riêng: Pause TRACK_W=300, Settings=360). 2 thay đổi chủ ý: handle Settings 12/10→11/9 (~1-2px); `sliderNudge` clamp local var Settings (sửa bug fill âm tại volume=5 — giá trị LƯU không đổi). [§5.15]
- **D2 ✅ ĐÃ GOM `ensureVisible`+`hitTestList` chép 2 lần:** Save vs Load, chỉ khác startY. → 2 static free func `ensureVisibleIn(...)` + `hitTestListAt(..., startY)`; lambda mỗi màn giờ là wrapper 1 dòng (truyền UI_SAVE/UI_LOAD_LIST_START_Y).
- **D3 ✅ ĐÃ GOM Khối xóa file lặp 4 lần.** → lambda `deleteSaveAt(idx)` mỗi màn (guard `idx<0||idx>=size`); bản Save có toast, bản Load không (giữ hành vi cũ). 4 block → 4 lời gọi 1 dòng.
- **D4 ✅ ĐÃ GOM Cuộn con lăn lặp 2 lần.** → static `scrollListBy(scrollTop, delta, count)`; 2 handler gọi chung.
- **D5 ✅ ĐÃ GOM "pause→màn con→resume→clock.restart" lặp 4 lần** trong handleGameplay. → lambda `openNestedScreen(fn)` (generic `auto&&`, gói STYLE_SPEED pause/resume + restart, trả GameScreen để call site tự forward).
- **D6 ✅ ĐÃ GOM "ghi nhận kết quả" (totalWins+++soundPlayWin/Draw) lặp 5 nơi.** → lambda `applyResult(GameResult)` (chỉ cộng điểm + sound, KHÔNG đụng firstPlayerOfRound).
- **D7 ✅ ĐÃ GOM (Ưu tiên 7) Bộ 3 màn tile (Mode 2/Diff 4/Char 6) giống ~85%** → 4 static: `tileGridCenter` (1 công thức lưới căn giữa thỏa cả 3 layout — verify **bit-exact** mọi index), `tileGridHitTest`, `drawTileFrame` (glow+panel), `drawTileSprite` (fit-scale). Loop label/badge/fallback-text giữ riêng từng màn (divergent). KHÔNG gộp thành 1 hàm khổng lồ (giảm readability + rủi ro). [§5.15]
- **D8 ✅ ĐÃ GOM Đuôi "lấy chuột + renderBackButton" — thực tế 9 hàm render (audit đếm thiếu, ghi 7).** renderBackButton tự đọc chuột khi `mx<0` (default sẵn ở header) → 9 chỗ còn `renderBackButton(window, res)`.
- **D9 ✅ ĐÃ GOM (Ưu tiên 8) W/S/Up/Down modulo lặp 7 site / 6 handler.** → static `menuNavStep(index,count,delta)` = `((i+delta)%n+n)%n`. delta ±1 (dọc/ngang), +3 (lưới Char nhảy hàng), +1 (toggle Mode 2 tile). Verify đại số: tất cả tương đương bản cũ. [§5.16]
- **D10 ✅ ĐÃ GOM (Ưu tiên 8) Hằng pause 230/60 khai báo cục bộ 2 FILE** → thêm `UI_PAUSE_TITLE_Y=150 / UI_PAUSE_START_Y=230 / UI_PAUSE_STEP=60` vào game_types.h; render.cpp + menu.cpp derive từ macro (1 nguồn). `ROW_W=500` vs `UI_LIST_HALF_WIDTH=250` vẫn khớp ngầm (chưa gom, giá trị thấp). [§5.16]

### 5.8 — 🎯 TỔNG KẾT + BẢNG ƯU TIÊN (tiến độ thực thi)

| Ưu tiên | Việc | Trạng thái |
|---------|------|-----------|
| 🔴 **1** | Sửa 3 bug thật R1+R2+R3 | ✅ **DONE 12/06** (build pass 0 warning) |
| 🔴 **2** | Quyết định R5 (ghi đè save hay bỏ "click copy tên") | ✅ **DONE 12/06** — User chọn (a) cho ghi đè, xác nhận 1 lần |
| 🟠 **3** | Dọn dead code Mục tiêu 1 (11 hàm + 14 chuỗi + define chết + soundLoadAll + animClock + include thừa) | ✅ **DONE 12/06** (build pass 0 warning, §5.11) |
| 🟠 **4** | langGetText trả ref (3.1) + bot thinking cả Basic (3.3) + xả click sau bot (3.4) | ✅ **DONE 12/06** (build pass 0 warning, §5.12) |
| 🟡 **5** | DRY rẻ: D8, D5, D6, D2-D4 | ✅ **DONE 12/06** (build pass 0 warning, verify đối kháng pass, §5.13) |
| 🟡 **6** | Naming M2 (#2 hero prefix, #3-4 CELL_P1, #10 i18n lỗi 15 ký tự) | ✅ **DONE 12/06** (build pass 0 warning, §5.14) |
| 🟡 **7** | Naming #5-9 + minimax (3.2) + tile-grid (D7) + slider (D1) + R7 + R8 (**R4 hoãn tới lúc nộp bài**) | ✅ **DONE 13/06** (build pass 0 warning, verify đối kháng 5 agent pass, §5.15) |
| 🟡 **8** | D9 `menuNavStep` + D10 hằng pause→game_types.h + 3.7 ô save `drawInputField` (User loại trừ 3.5, R4) | ✅ **DONE 13/06** (build pass 0 warning, §5.16) |
| ⚪ **9** | Hoãn/loại trừ: R4 path exe (xử lý lúc nộp bài), 3.5 `goto`→lambda (User loại trừ) | ⬜ chưa |

**Đánh giá chung:** codebase TỐT so với quy mô đồ án — phân tầng sạch, không leak, không vòng, quyết định lớn nhất quán. Nợ kỹ thuật tập trung: (1) di tích V1→V2 chưa quét (dead code 5.3 gần như toàn "xác" tính năng đã thay), (2) 3 bug đã sửa ở ưu tiên 1, (3) lặp slider/list trong menu.cpp.

### 5.9 — CHI TIẾT FIX ƯU TIÊN 1 (đã thực thi 12/06)

**R1 — [save_load.cpp] loadGame, sau đọc dòng timer:** nếu `style==STYLE_SPEED` → ép `isRunning=true` + `turnAlarmFired=false`. (File save luôn có isRunning=0 do timerPause trước khi ghi.)

**R2 — [render.cpp] renderPixelToBoard:** thêm chặn sớm `if (x < BOARD_OFFSET_X || y < BOARD_OFFSET_Y) return false;` TRƯỚC khi chia (chống `(int)` cắt số âm về 0).

**R3 — [save_load.cpp] loadGame:** include `utils.h`; validate sau khi đọc: `if(!f) return false` (3 chỗ), mode/style/diff trong miền, `inBounds(cursor)`, **`moveCount ∈ [0,225]`** (chốt chống tràn), mỗi ô board ∈ {−1,0,1}, mỗi nước lịch sử inBounds + player ∈ {−1,1}. Sai → return false → màn Load hiện "Lỗi tải file!".

**Verify:** Build ✅ 0 warning. `saves/test.txt` (Speed thật, `...19.3786 0`) pass validation + R1 kích hoạt. 10 file V1 cũ (`file1..11`, chứa rác `-858993460`) giờ bị từ chối lịch sự thay vì nạp rác (khớp quyết định "save V1 không hỗ trợ").

### 5.10 — CHI TIẾT FIX ƯU TIÊN 2 / R5 (đã thực thi 12/06 — User chọn phương án a)

**Quyết định:** cho phép ghi đè file save khi tên trùng, hỏi xác nhận 1 lần (không thêm popup riêng — dùng dòng message có sẵn).

**Cơ chế (state `pendingOverwrite` trong handleSaveScreen):**
- Enter lần 1 với tên trùng → CHƯA ghi, set `pendingOverwrite = inputName`, hiện câu hỏi.
- Enter lần 2 (tên không đổi) → `saveGame` ghi đè (ofstream truncate sẵn), cache cập nhật, list refresh.
- Đổi tên (gõ thêm/xóa/click file khác) → `pendingOverwrite != inputName` → tự hỏi lại từ đầu.
- Xóa file đang chờ (Delete/right-click) → `saveFileExists` false → Enter sau lưu thẳng, không hỏi.
- Câu xác nhận **giữ trên màn hình** khi đang chờ (không tự tắt sau 2s như message thường).

**File sửa:**
- `src/menu.cpp` — handleSaveScreen: thêm `pendingOverwrite`, sửa `doSave()`, sửa điều kiện hiện message.
- `src/language.cpp` — `msgFileExists` đổi nội dung 2 ngôn ngữ: VN `"File đã tồn tại — Enter lần nữa để ghi đè!"` / EN `"File exists - press Enter again to overwrite!"` (tái dùng string, không tạo string mới).
- `include/language.h` — cập nhật comment field.

**Verify:** Build ✅ 0 warning.

### 5.11 — CHI TIẾT FIX ƯU TIÊN 3 / Dọn dead code (đã thực thi 12/06)

Mỗi món đều **re-verify bằng grep 0 nơi gọi** ngay trước khi xóa (code đã đổi từ lúc audit). `renderPlaceEffect` nằm cạnh nhóm bị xóa nhưng CÓ caller (menu.cpp) → giữ, khớp audit.

**1A — 11 hàm chết (xóa cả khai báo + định nghĩa):**
- `render.h/cpp`: `renderScreen` (dispatcher + comment gây hiểu lầm), `renderWinEffect`, `renderDrawEffect`, `confettiActive`
- `timer.h/cpp`: `timerGetGamePercentP1/P2`
- `board.h/cpp`: `boardCountEmpty`
- `sound.h/cpp`: `soundPlayBGM`
- `save_load.h/cpp`: `saveGetMetadata`, `saveCountFiles`, `saveRenameFile` (+ sửa comment đầu file nhắc saveRenameFile). **GIỮ** `SaveMetadata` struct + `parseMetadata` + `g_metaCache` — `saveFileExists` O(1) vẫn dùng.

**1B — Xác sống:** `soundLoadAll` xóa hàm + call menu.cpp:17 (chỉ setLoop/setVolume lên Music CHƯA mở file — `soundPlayBGMTrack` ngay dòng sau làm lại đầy đủ); `animClock` + `lastResult` + `(void)animClock` trong renderGameOver xóa sạch. → Naming #1 (soundLoadAll) tự giải quyết.

**1C — 14 chuỗi i18n chết ×2 ngôn ngữ:** `turn, hintText, undoText, nameError, saveSuccess, loadSuccess, fileExists, maxSaveReached, msgMaxFiles, deleteFile, renameFile, gameTime, turnTime, timeUp` — xóa field language.h + 28 dòng gán language.cpp (section "Timer" biến mất hẳn). `fileNotFound`, `nameDuplicate`, `msgFileExists`... còn dùng → giữ.

**1D — 8 #define chết (game_types.h):** `COLOR_BOARD_BG`, `COLOR_GRID_LINE`, `COLOR_WIN_LINE`, `UI_MENU_HALF_WIDTH/HEIGHT`, `UI_PAUSE_TITLE_Y/START_Y/STEP` (block "Pause Menu" — giá trị vốn SAI so với code thật).

**1E — Include:** game_types.h rút từ 7 → 1 std header (`<string>`; xóa `<algorithm><cmath><cstdlib><ctime><fstream><vector>`). Hệ quả đã xử lý: `render.h` +`<vector>` (ConfettiSystem/saveList); `render.cpp` +`<cstdio>` (snprintf sống nhờ include gián tiếp). Đã kiểm tra cả 10 .cpp + rounded_rect.h (tự có `<cmath>`) + utils.h — không file nào khác sống nhờ include gián tiếp.

**Verify:** Build ✅ 0 warning (cả 10 .cpp recompile do game_types.h đổi). Grep cuối: 0 reference sót.

### 5.12 — CHI TIẾT FIX ƯU TIÊN 4 / Tối ưu luồng (đã thực thi 12/06)

**3.1 — `langGetText` trả `const TextStrings&` thay vì copy:**
- `language.cpp`: tách thân hàm cũ thành `static void langBuild(Language, TextStrings&)`; `langGetText` mới giữ **2 bản static EN/VI** build 1 lần (lazy, cờ `built`), trả tham chiếu. Static sống suốt vòng đời → tham chiếu an toàn (không dangling).
- `language.h`: đổi khai báo `const TextStrings& langGetText(Language)`.
- 20 call site `TextStrings txt = langGetText(...)` → `const TextStrings& txt = ...` (18 render.cpp + 2 menu.cpp, replace_all; giữ đúng indent 8-space ở doSave). 3 chỗ inline `langGetText(...).msgX` giữ nguyên (chỉ copy 1 string field).
- Lợi: trước mỗi lần gọi build + copy ~75 std::string (≈300-400 alloc/frame, nhiều hàm render gọi/frame); giờ 0 build, 0 copy struct. Hành vi text KHÔNG đổi.

**3.3 — "Bot đang suy nghĩ" hiện ở MỌI mode:** `menu.cpp` `doBotMove` bỏ điều kiện `if (state.style == STYLE_SPEED)` trước `renderBotThinking`. Basic + Expert giờ cũng hiện plate → không bị tưởng treo ("Not Responding") khi bot nghĩ lâu.

**3.4 — Xả input queue sau bot move:** thêm lambda `flushPendingInput` (drain `pollEvent` + `handleCommonEvent` để vẫn nuốt Close/Resize, bỏ click/phím). Gọi ở 2 lối ra của `doBotMove`: nhánh bot hết giờ (trước khi sang game over) + cuối hàm (trước `clock.restart`). Chống click/phím người chơi lỡ bấm lúc bot nghĩ bị xử lý nhầm ở frame sau (đặt quân ngoài ý muốn).

**Verify:** Build ✅ 0 warning (language.h đổi → render/menu/sound recompile).

### 5.13 — CHI TIẾT FIX ƯU TIÊN 5 / Gom code lặp DRY (đã thực thi 12/06)

Quy trình: 1 workflow **điều tra song song** (6 agent map đủ call site + sắc thái) → implement tuần tự (tránh xung đột file) → 1 workflow **verify đối kháng** (5 agent, mỗi pattern 1 agent + 1 holistic). **Kết quả: cả 5 verdict = `preserved`, 0 finding** (1 agent đối chiếu trực tiếp `git diff HEAD`).

**D8 (`src/render.cpp`):** renderBackButton thêm khối `if (mx < 0.f) { mp = mapPixelToCoords(getPosition); mx/my = mp; }` ở đầu (dùng đúng biểu thức letterbox cũ). 9 call site (audit ghi 7 — đếm thiếu) gọn còn `renderBackButton(window, res);`, xóa biến `mp`/`pix` mồ côi. Header giữ nguyên (default `-1.f` đã có sẵn). Sentinel `mx<0` an toàn: nút ở x∈[30,160], không bao giờ âm khi hover thật.

**D5/D6 (`src/menu.cpp`, handleGameplay):**
- `openNestedScreen(auto&& fn)`: gói pause(SPEED)→fn→resume(SPEED)→clock.restart, trả GameScreen. 4 site (Save-btn/Exit-btn/L/ESC). `soundPlaySelect` để NGOÀI (chỉ 2 site chuột). clock.restart chạy cả lối return của Pause site — đã chứng minh vô hại (clock local, không đọc lại sau return).
- `applyResult(GameResult r)`: `if NONE return; P1/P2 totalWins++; DRAW→drawSound else winSound`. 5 site (bot hết giờ / doBotMove / doPlayerPlace / turn-timer / game-timer ×2). Site turn-timer rút if/else thành ternary. KHÔNG đụng `firstPlayerOfRound` (việc của handleGameOver).

**D2/D3/D4 (`src/menu.cpp`, save+load screen):**
- D2: static `ensureVisibleIn(sel, scrollTop&, count)` + `hitTestListAt(mx,my,scrollTop,count,startY)`; lambda mỗi màn thành wrapper 1 dòng. Save truyền `UI_SAVE_LIST_START_Y`, Load truyền `UI_LOAD_LIST_START_Y` (KHÔNG hoán đổi — verify kỹ).
- D3: lambda `deleteSaveAt(idx)` mỗi màn, guard `idx<0||idx>=size` (tương đương guard cũ); Save có toast `msgFileDeleted`, Load không (giữ hành vi cũ). 4 block → 4 lời gọi.
- D4: static `scrollListBy(scrollTop&, delta, count)`; 2 wheel handler gọi chung.

**Verify:** Build ✅ 0 warning (chỉ menu.cpp + render.cpp recompile). 5 agent đối kháng đều `preserved`.

**⚠️ Quirk tiền tồn — ✅ ĐÃ SỬA (12/06, đầu session Ưu tiên 6 theo yêu cầu User):** trên màn Save, xóa file đang chờ ghi đè trước đây KHÔNG reset `pendingOverwrite`. Đã thêm `pendingOverwrite = "";` trong lambda `deleteSaveAt` (bản Save) → xóa file giờ hủy trạng thái chờ ghi đè (Enter sau lưu thẳng, không hỏi). Bản Load không có `pendingOverwrite` nên không đụng.

### 5.14 — CHI TIẾT FIX ƯU TIÊN 6 / Naming M2 (đã thực thi 12/06)

Phạm vi (theo bảng §5.8): **#2 hero prefix, #3-4 hằng CELL_P1/P2, #10 i18n lỗi 15 ký tự.** (#1 soundLoadAll đã xong ở Ưu tiên 3; #5-9 để Ưu tiên 7.)

**#2 — Hero prefix thống nhất `heroGoku`/`heroVegeta`:** 6 hero giờ cùng tiền tố `hero*`. Đổi `mascotGoku→heroGoku`, `mascotVegeta→heroVegeta` ở: khai báo `game_types.h` (2), load `renderLoadResources` render.cpp (2), `heroSetByIndex` (case 1 + default), fallback villain PvP `renderPlayerPanel`. File ảnh `hero_goku*.png`/`hero_vegeta*.png` KHÔNG đổi (chỉ đổi tên field C++).

**#3 — Comment Cell.value:** sửa "−1:P1(X), 1:P2(O)" (SAI từ #23) → "CELL_P1(-1): Player 1, CELL_P2(+1): Player 2 (X/O hiển thị động theo firstPlayerOfRound)". Comment Move.player cũng cập nhật sang CELL_P1/CELL_P2.

**#4 — Hằng `CELL_P1=-1, CELL_P2=+1` (game_types.h) — 1 nguồn mã hóa người chơi:**
- `game_types.h`: thêm 2 `#define` ngay sau `WIN_COUNT` (CELL_P1 dùng `(-1)` có ngoặc).
- `bot.cpp`: `BOT_PLAYER = CELL_P2` (máy = P2 +1), `HUMAN_PLAYER = CELL_P1` (người = P1 −1) — bỏ magic 1/−1 tự định nghĩa, tie vào nguồn trung tâm.
- `board.cpp`: nguồn sinh giá trị `isPlayer1Turn ? CELL_P1 : CELL_P2` (boardPlaceMove); 2 phép so `.value == CELL_P1`, `lastMove.player == CELL_P1`.
- `save_load.cpp`: validation R3 dùng `< CELL_P1 || > CELL_P2` (range ô) + `!= CELL_P1 && != CELL_P2` (player).
- KHÔNG đụng: `heroP2=1` (index hero), `isPlayer1Turn?1:0` (serialize bool), `firstPlayerOfRound 1/2` (player-id 1-based — hệ khác). Mỗi swap là **giá trị-tương đương tuyệt đối** (macro ≡ literal) → compiler đảm bảo bất biến.

**#10 — i18n `nameTooLong`:** thêm field `std::string nameTooLong` (language.h, sau inputNameHintPvC); gán EN `"Player name can be at most 15 characters!"` + VN `u8"Tên người chơi chỉ tối đa là 15 kí tự!"` (language.cpp); `menu.cpp:567` thay hardcode → `errorMsg = txt.nameTooLong;` (txt đã có ở scope hàm handleInputNames). Sửa lỗi English vẫn hiện tiếng Việt.

**Verify:** Build ✅ 0 warning (toàn bộ 10 .cpp recompile do game_types.h đổi). Grep: 0 reference `mascotGoku/Vegeta` sót trong code; hardcode `u8"Tên người chơi"` chỉ còn ở language.cpp (đúng chỗ giá trị VN).

### 5.15 — CHI TIẾT FIX ƯU TIÊN 7 (đã thực thi 13/06) — **R4 HOÃN**

Phạm vi: naming #5-9 + tối ưu 3.2 + DRY D1/D7 + R7 + R8. **R4 (path exe) HOÃN tới lúc nộp bài theo yêu cầu User.** Làm theo cụm, build giữa các cụm; cụm đụng hành vi (D1/D7/3.2) verify đối kháng 5 agent → **tất cả `preserved`, 0 finding lỗi**.

**Cụm trivial (#5,#7,#8,#9,R7):**
- **#5** `render.cpp`: bỏ 2 `#define BOARD_OFFSET_X/Y` alias (xóa khối trước) rồi `replace_all` 11 chỗ bare → `UI_BOARD_OFFSET_X/Y`. menu.cpp đã dùng `UI_` sẵn (không đụng). Verify: 0 bare sót, 0 `UI_UI_` hỏng.
- **#7** comment `BG_FRAME_COUNT` game_types.h: "120@24fps 237MB" → "120 = sức chứa tối đa; thực tế ~106 frame .jpg @ BG_FPS=18 (~390MB)".
- **#8** comment render.cpp: `bg_frame_00..23.png` → `bg_frame_00..105.jpg`.
- **#9** menu.cpp `handleMainMenu`: 2 literal `280.f/60.f` → `UI_MENU_START_Y`/`UI_MENU_STEP` (= đúng giá trị cũ).
- **R7** menu.cpp `gameRun` `default`: `window.close()` → `nextScreen = SCREEN_MAIN_MENU`. Verify: nút Thoát (handleMainMenu case 5) tự `window.close()` nên Exit vẫn chạy.

**#6 — chuyển load SFX sang sound.cpp:** thêm `soundLoadResources(GameResources&)` (sound.cpp + sound.h) load 7 SFX; gỡ 7 dòng `loadFromFile` khỏi `renderLoadResources`; `main()` gọi `soundLoadResources(res)` sau `renderLoadResources`, trước `settingsLoad`. Shader giữ ở render.cpp (đúng module đồ họa).

**R8 — `soundShutdown()`:** sound.cpp + sound.h; loop 8 phần tử `soundPool`: `stop()` + `resetBuffer()` (xác nhận có trong SFML 2.6.2). `main()` gọi cuối, sau `gameRun`, trước `return 0`.

**3.2 — minimax terminal check:** thêm `static bool botIsWinningMove(board, r, c)` (giống inner-loop `botCheckImmediateWin`). Trong `botMinimax` cả 2 nhánh: sau khi đặt thử, nếu thắng → `eval = ±(SCORE_WIN − depth)` (max:+, min:−) thay vì đệ quy; vẫn reset `board[r][c]=0`, vẫn feed alpha-beta cũ. Verify: dấu đúng, không leak mutation, `−depth` ưu tiên thắng nhanh/hoãn thua, không overflow/collision, không kịch bản chọn nước tệ hơn.

**D1 — slider widget:** `render.cpp` `sliderDraw(window, trackX, trackY, trackW, volume, selected)` static (track+fill+handle) thay 2 khối vẽ. `menu.cpp` 3 static `sliderSetFromX`/`sliderContains`/`sliderNudge`; lambda `setVolumeFromX`/`isOnSlider` + phím ±10 ở cả handleSettings + handlePauseMenu delegate qua helper. 2 thay đổi chủ ý (verify xác nhận không phải regression): handle Settings 12/10→11/9 outline 3→2.5 (~1-2px); `sliderNudge` clamp local var (sửa bug Settings fill âm ở volume=5, giá trị LƯU không đổi vì `soundSetBGMVolume` đã clamp global).

**D7 — tile-grid:** 4 static trong render.cpp: `tileGridCenter(i,cols,rows,W,H,gapX,gapY,gridCy,&cx,&cy)` (công thức lưới căn giữa, thỏa Mode 2×1 / Diff 4×1 / Char 3×2 — verify bit-exact mọi index, `(cols-1)/2.f` là float-division), `tileGridHitTest`, `drawTileFrame` (glow halo + panel bo góc, param cornerR), `drawTileSprite` (fit-scale + tint). 3 hit-test → wrapper 1 dòng; 3 render loop dùng helper, giữ phần label/badge/fallback-text riêng. KHÔNG gộp 1 hàm khổng lồ (tránh ~12 param + giảm readability).

**Verify đối kháng (5 agent, 13/06):** d7-geometry `preserved` (bit-exact, click khớp tile); d7-appearance `preserved` (mọi màu/góc/margin/tint/badge khớp HEAD); d1-slider `preserved` (2 finding = 2 thay đổi chủ ý đã ghi); minimax-3.2 `preserved` (đúng + cải thiện); holistic `preserved` (1 finding doc cũ soundLoadAll/soundPlayBGM trong ARCHITECTURE.md/build_guide.md — **không sửa** theo standing rule). Build ✅ 0 warning sau mỗi cụm.

### 5.16 — CHI TIẾT FIX ƯU TIÊN 8 (đã thực thi 13/06) — **User loại trừ 3.5 + R4**

Phạm vi: **D9 + D10 + 3.7.** (3.5 `goto`→lambda User loại trừ; R4 path exe hoãn tới lúc nộp bài.) Tất cả thuần code-quality/thẩm mỹ — 0 ảnh hưởng chức năng.

**D9 — `menuNavStep` (menu.cpp):** static `int menuNavStep(index, count, delta)` = `((index+delta)%count + count)%count` (chèn trước handleMainMenu). Thay 7 call site / 6 handler — verify đại số từng cái tương đương bản cũ:
- Dọc (Up/W = −1, Down/S = +1): handleMainMenu, handleStyleSelect, handlePauseMenu (count=MENU_COUNT), handleSettings (count=3). `(i−1+n)%n` ≡ `menuNavStep(i,n,−1)`.
- Ngang (Left/A = −1, Right/D = +1): handleDifficultySelect (count=4): `(i+3)%4` ≡ `menuNavStep(i,4,−1)`.
- Lưới Char (count=6): Left `(i+5)%6` ≡ `(.,6,−1)`; Right `(.,6,+1)`; Up/Down `(i+3)%6` ≡ `(.,6,+3)` (nhảy hàng, +3≡−3 mod 6).
- Toggle Mode (2 tile): `1−i` ≡ `menuNavStep(i,2,+1)` ((i+1)%2). Dòng `(takenIndex+1)%6` trong confirm KHÔNG đụng (không phải nav).

**D10 — hằng Pause (game_types.h):** thêm `UI_PAUSE_TITLE_Y=150 / UI_PAUSE_START_Y=230 / UI_PAUSE_STEP=60`. `renderPauseMenu` (render.cpp) + `handlePauseMenu` (menu.cpp) đổi local const derive từ macro (`= UI_PAUSE_START_Y` …) + title `"PAUSE"` dùng `UI_PAUSE_TITLE_Y`. Giá trị không đổi (230/60/150) → hit-test vẫn khớp item vẽ; chỉ khác: giờ 1 nguồn, sửa 1 chỗ.

**3.7 — ô nhập tên save (render.cpp):** `renderSaveScreen` bỏ `RectangleShape` vuông + text rời, thay bằng 1 lời gọi `drawInputField(window, res, WW/2, UI_SAVE_INPUT_Y, 400, 48, inputName, txt.enterSaveName, true, 1.f)` — bo góc + glow + caret thở, luôn focused (ô nhập duy nhất). Đồng bộ thẩm mỹ với màn Nhập tên (#37). Chức năng save không đổi.

**Verify:** Build ✅ 0 warning (toàn bộ recompile do game_types.h đổi). D9 verify đại số tay (tautological); D10/3.7 rủi ro ~0 (giá trị/chức năng không đổi).

### 5.17 — FIX ĐIỀU HƯỚNG màn Save từ Game Over + input-bleed phím L (13/06)

> ⚠️ **Bug fix riêng do User báo trực tiếp** (KHÔNG nằm trong bảng ưu tiên audit 1→9). Đảo quyết định Sprint 2 cũ "Save sau game over → quay lại câu hỏi Lưu game?".

**Lỗi User báo:** Ván đã KẾT THÚC → "Lưu game? → Có" → màn Save → Thoát thì quay ngược về gameplay (re-render vòng game over đã chết, có thể kích lại confetti/shockwave) thay vì ra Menu chính.

**Nguyên nhân:** `handleSaveScreen` luôn `return SCREEN_PLAYING` khi Back/ESC — không biết ngữ cảnh gọi. `handleGameOver` bỏ qua return + giữ `askingSave=true` → lặp lại câu hỏi "Lưu game?" trong vòng lặp game over đã chết.

**Fix chính — context-aware:** thêm param `GameScreen exitTarget = SCREEN_PLAYING` (default CHỈ ở `menu.h`, không lặp ở `.cpp`). Back-click + ESC → `return exitTarget`.
- 3 site trong-ván (Save button, L gameplay, pause-menu Save) dùng default `SCREEN_PLAYING` → về bàn cờ (hành vi cũ không đổi).
- 2 site game-over (Save?→Có, L game-over) truyền `SCREEN_MAIN_MENU` + **forward return** (chain `handleGameOver → handleGameplay → gameRun`).
- ⚠️ **Gotcha C++:** 2 site gameplay đi qua template `openNestedScreen(auto&& fn)` → `fn` decay thành con trỏ hàm → **default arg KHÔNG sống qua function-pointer decay** → phải wrap lambda 3-arg `[](w,r,s){ return handleSaveScreen(w,r,s); }`. (Pause-menu + game-over là direct call nên default/explicit arg áp bình thường.)
- Dọn dead code: bỏ `menuIndex=0` + comment "Giữ askingSave=true" cũ ở nhánh Save?→Có.

**Quyết định User — phím L màn Game Over:** chỉ cho L hoạt động khi `askingSave==true` (đã qua "Chơi tiếp?", đang ở "Lưu game?") → save xong ra Menu chính, đồng bộ "Lưu game?→Có". Còn đang hỏi "Chơi tiếp?" thì L = no-op (`break`). L trong-ván (đang chơi) KHÔNG đổi.

**Fix lỗi phụ — input-bleed phím L:** nhấn L → SFML sinh CẢ `KeyPressed` lẫn `TextEntered('l')`. Caller ăn `KeyPressed` mở màn Save, nhưng `TextEntered('l')` còn trong queue → ô tên file tự thêm 'l' đầu. Fix: **drain hàng đợi 1 lần ở đầu `handleSaveScreen`** (qua `handleCommonEvent` nên vẫn xử Close/Resize) = "bỏ input cũ khi chuyển màn". Bắt được chắc chắn trên Windows vì `TranslateMessage(WM_KEYDOWN)` post `WM_CHAR` và cùng vòng `PeekMessage` của `processEvents()` bơm nó vào queue trước khi `pollEvent` trả `KeyPressed`.

**Verify:** 2 workflow đối kháng — (a) 4 lens (routing / completeness / memory-lifecycle / regression) ALL pass cho fix chính; (b) 2 lens (drain-correctness Win32 event-model / gate-logic) ALL pass cho L-gate + drain. RAII unwind sạch (ConfettiSystem/clock/vector), timer Speed cân bằng (site game-over đúng là KHÔNG dùng openNestedScreen vì ván đã kết thúc). Build ✅ 0 warning. **File:** `include/menu.h`, `src/menu.cpp`.

### 5.18 — REGRESSION TEST + FIX vòng 2 (13/06) — sau phản biện của User

> Phiên regression test User yêu cầu (đóng vai Senior C++ review). Workflow 5-lens adversarial (21 finding: 3 confirmed, 10 refuted, 8 verifier chết do session limit → Claude tự kiểm chứng tay) + Claude đọc trực tiếp navigation core. User phản biện 3 finding → kết quả chỉnh như dưới.

**3 finding XÁC NHẬN + ĐÃ SỬA (code):**
- **A1 — `pendingOverwrite` thủng hợp đồng (medium):** comment hứa "đổi tên → hỏi lại từ đầu" nhưng `pendingOverwrite` chỉ reset trong doSave/deleteSaveAt, KHÔNG reset khi sửa `inputName`. → sửa tên (gõ/xóa) rồi gõ lại đúng tên cũ, hoặc click file khác → click lại file đó: Enter ghi đè chỉ 1 lần (bỏ qua double-confirm). KHÔNG phải "ghi đè im lặng" — prompt "Enter lần nữa để ghi đè" vẫn hiện (waitingConfirm dùng chung phép so `pendingOverwrite==inputName`) → finding `pending-2` bị refute đúng. **Fix:** thêm `pendingOverwrite = "";` vào 3 điểm đổi inputName (BackSpace, TextEntered-append, click-file). Giờ code khớp comment.
- **side-effect-1 — slider ghi đĩa mỗi bước kéo (low):** `sliderSetFromX` gọi `settingsSave()` mỗi 5% → tới ~20 lần ghi settings.txt/lần kéo (hành vi cũ giữ từ D1). **Fix:** bỏ `settingsSave()` khỏi `sliderSetFromX` (chỉ giữ `soundSetBGMVolume` để preview ngay), gọi `settingsSave()` 1 lần khi `MouseButtonReleased` (cả handleSettings + handlePauseMenu). `sliderNudge` (phím) vẫn save ngay (rời rạc 1/phím). Trade-off nhỏ: thả chuột NGOÀI cửa sổ giữa lúc kéo → có thể không lưu (chấp nhận, low).
- **side-effect-2 — `currentBGMTrack` không reset lúc shutdown (info):** thêm `currentBGMTrack = -1;` cuối `soundShutdown()` để giữ bất biến "track phản ánh nhạc đang phát".

**2 finding User PHẢN BIỆN ĐÚNG — Claude SỬA LẠI BÁO CÁO (KHÔNG đụng code):**
- **A2 / 3.6 — KHÔNG phải lỗi.** Claude đọc sai event loop. Cú đặt quân thắng qua guard `result==NONE` (đặt) rồi guard `result!=NONE` ngay sau ([menu.cpp:826](src/menu.cpp:826)/[886](src/menu.cpp:886)) → vào Game Over NGAY cùng sự kiện, KHÔNG cần click thừa. User test đúng. → đã sửa §5.2 + §5.5/3.6.
- **A3 / metadata — không mâu thuẫn + ĐÃ DỌN.** Load đúng mode/style/moves là nhờ `loadGame()` ghi thẳng vào `GameState` ([save_load.cpp:113-130](src/save_load.cpp:113)) — KHÔNG liên quan cache. Field CHẾT chỉ là `SaveMetadata.mode/style/moveCount` (cache RAM, KHÔNG UI nào đọc — list save/load chỉ hiện tên file). → **ĐÃ DỌN (13/06, User yêu cầu):** gỡ hẳn `struct SaveMetadata` + hàm `parseMetadata`; thay `unordered_map<string,SaveMetadata> g_metaCache` → `unordered_set<string> g_saveNames` (đúng nhu cầu duy nhất của `saveFileExists` O(1)); bỏ `<sstream>` thừa + thêm `<unordered_set>`; sửa luôn comment header save_load.h sai "sorted alphabet" (thực tế sort theo mtime). Build 0 warning, load/save chạy đúng như cũ.

**Cleanup khác (User duyệt):**
- `soundShutdown` + `playSfx` + khai báo pool dùng hằng `SOUND_POOL_SIZE = 8` thay 3 magic number.
- Đính chính claim sai trong doc này: §5.3 1F (`WinLine.count` có thể > 5; `Cell` có dùng), §5.2 + §5.5/3.6 (không phải lỗi). **Comment trong CODE (Cell/WinLine/render) vốn đã đúng — không có comment code nào sai phải xóa.** Comment `pendingOverwrite` (menu.cpp:1302-1304) giờ khớp code sau fix A1.

**File sửa:** `src/menu.cpp` (A1 ×3 + slider save-on-release ×2 + comment D1), `src/sound.cpp` (SOUND_POOL_SIZE + currentBGMTrack reset), `src/save_load.cpp` + `include/save_load.h` (dọn metadata A3: bỏ struct + parseMetadata, cache → `unordered_set<string>`, fix include + comment header). Build ✅ 0 warning.

---

**Last updated:** 13/06/2026 — **V2 100% COMPLETE [17/17]** + **AUDIT KỸ THUẬT (mục 5)** + **fix điều hướng Save/Game-Over (§5.17)** + **regression test vòng 2 (§5.18)**. Ưu tiên 1→8 ✅ DONE. §5.18: fix A1 `pendingOverwrite` + 2 side-effect (slider save-on-release, currentBGMTrack reset) + `SOUND_POOL_SIZE` + DỌN metadata chết (struct/parseMetadata → `unordered_set`); reclassify A2/3.6 = KHÔNG phải lỗi (User phản biện đúng). **CÒN LẠI (Ưu tiên 9): R4 path exe — xử lý lúc nộp bài; 3.5 goto→lambda — User loại trừ.** Build pass 0 warning.
**Created by:** Claude (Anthropic) — session với @vanhoangnhatbp1
**Status:** 🎉 V2 CODE COMPLETE — Sprint 1-4 ✅ (**17/17 task** — #27 sounds DONE 12/06). Toàn bộ asset nhân vật + mode tile + âm thanh wired đủ. Sẵn sàng chạy + commit khi User yêu cầu.


