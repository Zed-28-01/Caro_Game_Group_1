# CẤU TRÚC SLIDES, KỊCH BẢN & PHÂN CÔNG CÔNG VIỆC
**Dự án:** Caro Game — Nhóm 1 · HCMUS
**Ngày thuyết trình:** 01/06/2026
**Deadline hoàn thành prep:** 24/05/2026 (Thứ Bảy)
**Ràng buộc đặc biệt:** ⚠️ **CHỈ 1 NGƯỜI** (Project Lead) vừa nói vừa demo trực tiếp — không có người phụ trợ

---

## YÊU CẦU TOÀN BỘ CÁC BẠN ĐỌC MỤC 4 ĐỂ XÁC ĐỊNH NHIỆM VỤ ĐƯỢC PHÂN CÔNG RỒI MỚI ĐỌC CÁC NỘI DUNG CÒN LẠI 

## MỤC LỤC
1. [Bảng phân bổ thời gian (Timeline 20 phút)](#1-bảng-phân-bổ-thời-gian-timeline-20-phút)
2. [Cấu trúc Slide chi tiết (Slide-by-Slide)](#2-cấu-trúc-slide-chi-tiết-slide-by-slide)
3. [Quy chuẩn viết Mã giả (Pseudocode)](#3-quy-chuẩn-viết-mã-giả-pseudocode) - DÀNH CHO NHÓM 1
4. [Phân công công việc & Deadline](#4-phân-công-công-việc--deadline)

---

## 1. BẢNG PHÂN BỔ THỜI GIAN (TIMELINE 20 PHÚT)

### 1.1. Tổng quan
| Phần | Thời lượng | % Tổng | Nhịp độ |
|------|-----------|--------|---------|
| **A. Mở đầu** | 1.5 phút | 7.5% | Chậm rãi, tự tin |
| **B. Kiến trúc tổng thể** | 1.5 phút | 7.5% | Vừa phải |
| **C. Kỹ thuật cơ bản** (theo đề) | 4 phút | 20% | Nhanh, dứt khoát |
| **D. Kỹ thuật nâng cao** | 5 phút | 25% | Vừa, nhấn vào Bot AI |
| **E. Demo trực tiếp** | 6 phút | 30% | Linh hoạt, vừa nói vừa thao tác |
| **F. Tổng kết** | 1.5 phút | 7.5% | Chậm, đậm dấu ấn |
| **G. Q&A buffer** | 0.5 phút | 2.5% | Dự phòng (có thể bỏ) |
| **TỔNG** | **20 phút** | **100%** | |

### 1.2. Bảng chi tiết theo phút

```
00:00 ┬─ A. MỞ ĐẦU (1.5 phút)
      │   Slide 1: Title (15s)
      │   Slide 2: Agenda (45s)
      │   Slide 3: Giới thiệu game Caro (30s)
01:30 ┼─ B. KIẾN TRÚC (1.5 phút)
      │   Slide 4: Tổng quan modules (1m30s)
03:00 ┼─ C. KỸ THUẬT CƠ BẢN (4 phút)
      │   Slide 5: Cấu trúc dữ liệu (1m)
      │   Slide 6: Win detection (1m)
      │   Slide 7: Save/Load (1m)
      │   Slide 8: Main Menu + UI (1m)
07:00 ┼─ D. KỸ THUẬT NÂNG CAO (5 phút 45 giây)
      │   Slide 9:  Bot AI tổng quan — 4 cấp độ (45s)
      │   Slide 10: Pattern Scoring (scoreLine) (1m15s) ⭐ NỀN TẢNG
      │   Slide 11: Minimax + Alpha-Beta (2m) ⭐ ĐIỂM NHẤN
      │   Slide 12: Speed Mode (chess-clock) (1m)
      │   Slide 13: Mouse Support (45s)
12:45 ┼─ E. DEMO TRỰC TIẾP (6 phút)
      │   Slide 14: Demo transition (sang game)
      │   Script demo chi tiết tại mục 2.13
18:45 ┼─ F. TỔNG KẾT (1.5 phút)
      │   Slide 15: Bảng đối chiếu yêu cầu (1m)
      │   Slide 16: Kết luận + Cảm ơn (30s)
19:30 ┼─ G. Q&A BUFFER (30s dự phòng)
      │
20:00 ┴─ KẾT THÚC
```

### 1.3. Nguyên tắc giữ nhịp khi 1 mình
- **KHÔNG để demo lố giờ** — đặt đồng hồ trên laptop, nhìn liếc mỗi 1 phút
- **Demo trước thuyết trình hết → KHÔNG được**, sẽ bị thiếu phần kết luận
- Nếu lố 1 phút ở demo → bỏ slide 14, đi thẳng tới slide 15
- Nếu thừa thời gian → mở Settings demo thêm hoặc cho bot Expert đấu lâu hơn

---

## 2. CẤU TRÚC SLIDE CHI TIẾT (SLIDE-BY-SLIDE)

> **Tổng số slide:** 15 slides (bao gồm 1 slide transition demo)
> **Số slide có mã giả (pseudocode):** 6 slides (Slides 5, 6, 7, 10, 11, 12)
> **Số slide có sơ đồ minh họa:** 8 slides (Slides 4, 6, 7, 9, 10, 11, 12, 14)
> **Số slide có screenshot game minh họa:** 6 slides (Slides 5, 6, 7, 10, 11, 12)
>
> ⚠️ **YÊU CẦU THẦY:** Mã giả phải được **GÕ TAY trực tiếp vào Text Box** trong PowerPoint — KHÔNG paste ảnh, KHÔNG chụp màn hình IDE. Sơ đồ và screenshot game được phép dùng ảnh PNG (đặt cạnh mã giả để dễ hiểu).

### 📐 Layout chuẩn cho 6 slide có mã giả (5, 6, 7, 10, 11, 12)

```
┌─────────────────────────────────────────────────┐
│ Tiêu đề slide                                   │
├──────────────────────┬──────────────────────────┤
│                      │  📊 Sơ đồ minh họa       │
│                      │  (giải thích thuật toán) │
│  💻 Mã giả           ├──────────────────────────┤
│  (Text Box, gõ tay)  │  🎮 Screenshot game      │
│                      │  (kết quả trong game)    │
│                      │                          │
└──────────────────────┴──────────────────────────┘
   ← Trái 50% →           ← Phải 50%, chia 2 →
```


---

### 🏷️ SLIDE 1 — TRANG BÌA (15 giây)

**Tiêu đề:** `CARO GAME`
**Subtitle:** Đồ án Cơ sở Lập trình · Nhóm 1
**Visual:** Logo KHTN + ảnh game (placeholder)
**Code/Sơ đồ/Screenshot:** Không
**Speaker cue:**
> *"Em chào thầy/cô. Em xin trình bày đồ án Game Caro của nhóm 1."*

---

### 🏷️ SLIDE 2 — AGENDA (45 giây)

**Tiêu đề:** `Nội dung trình bày`
**Nội dung (5 bullets, font lớn):**
- 01 · Kiến trúc & yêu cầu cơ bản
- 02 · Kỹ thuật nâng cao (Bot AI, Speed Mode)
- 03 · Demo trực tiếp
- 04 · Tổng kết

**Visual:** Icon nhỏ bên cạnh mỗi mục
**Code/Sơ đồ/Screenshot:** Không
**Speaker cue:**
> *"Phần trình bày gồm 4 mục chính. Em sẽ ưu tiên thời gian cho phần nâng cao và demo — đây là 2 phần thể hiện rõ nhất công sức của nhóm."*

---

### 🏷️ SLIDE 3 — GIỚI THIỆU GAME CARO (30 giây)

**Tiêu đề:** `Game Caro — Gomoku 5 in a row`
**Nội dung:**
- Luật chơi: Đặt 5 quân liên tiếp → thắng
- Bàn cờ: 15×15
- Quy mô dự án: ~3000 dòng C++ · 6 module · SFML 2.6.2

**Visual:** Hình bàn cờ với chuỗi 5 quân thắng (highlight)
**Code/Sơ đồ:** Không
**Screenshot game:** Có thể chèn 1 ảnh menu chính làm visual
**Speaker cue:**
> *"Caro là game cờ truyền thống. Nhóm em đã chọn nâng cấp từ console lên SFML để có giao diện đồ họa thực sự."*

---

### 🏷️ SLIDE 4 — KIẾN TRÚC TỔNG THỂ (1 phút 30 giây)

**Tiêu đề:** `Kiến trúc module — Tách bạch trách nhiệm`

**Layout:** Toàn slide là sơ đồ
**Visual:** Sơ đồ khối các module và quan hệ phụ thuộc

```
              ┌─────────────────┐
              │   game_types.h  │  ← Hằng số, struct, enum chung
              │   (Foundation)  │
              └────────┬────────┘
                       │ included by all
       ┌───────────────┼───────────────┐
       ↓               ↓               ↓
   ┌────────┐    ┌─────────┐    ┌─────────┐
   │  main  │───→│  menu   │───→│ render  │
   └────────┘    │  .cpp   │    │  .cpp   │
                 └────┬────┘    └─────────┘
                      │
              ┌───────┼────────┐
              ↓                ↓
          ┌───────┐       ┌────────┐
          │ bot   │       │ sound  │
          │ .cpp  │       │  .cpp  │
          └───────┘       └────────┘
```

**Bảng chức năng (góc dưới):**
| Module | Trách nhiệm |
|--------|-------------|
| `game_types.h` | Định nghĩa kiểu dữ liệu, hằng số UI |
| `main.cpp` | Khởi tạo SFML window, entry point |
| `menu.cpp` | Quản lý 13 màn hình, xử lý input |
| `render.cpp` | Vẽ toàn bộ UI (SFML drawing) |
| `bot.cpp` | AI 4 cấp độ |
| `sound.cpp` | Âm thanh + lưu cài đặt |

**Code:** Không (slide kiến trúc, không cần code)
**Speaker cue:**
> *"Em thiết kế theo nguyên tắc separation of concerns. Mỗi module một việc, dễ debug và mở rộng. Trung tâm là game_types.h — sửa hằng số UI một chỗ thì toàn bộ giao diện tự cập nhật."*

---

### 🏷️ SLIDE 5 — CẤU TRÚC DỮ LIỆU (1 phút)

**Tiêu đề:** `Cấu trúc dữ liệu cốt lõi — Struct GameState`

**Layout 3 vùng:**
- **Trái (50%):** Mã giả Text Box
- **Phải-trên (25%):** Sơ đồ bàn cờ 15×15
- **Phải-dưới (25%):** Screenshot gameplay

**📋 NGUỒN LỌC MÃ GIẢ:** Từ `include/game_types.h` (struct Cell, Move, GameState)

**Mã giả gõ vào slide (~16 dòng — Text Box, Consolas 16pt):**
```
// Một ô trên bàn cờ
STRUCT Cell:
    value : integer        // 0=trống, -1=X, 1=O

// Một nước đi (cho undo + replay)
STRUCT Move:
    row, col : integer
    player   : integer     // -1 hoặc 1

// Toàn bộ trạng thái ván — dùng cho save/load
STRUCT GameState:
    board       : Cell[15][15]
    player1, player2 : Player
    isPlayer1Turn : boolean
    mode        : {PvP, PvC}
    style       : {Basic, Speed}
    difficulty  : {Easy, Medium, Hard, Expert}
    timer       : TimerState
    moveHistory : Move[225]
    moveCount   : integer
```

**📊 Sơ đồ (phải-trên):** Bàn cờ 15×15 nhỏ, vài ô có X/O minh họa, mũi tên chỉ vào `board[i][j].value`

**🎮 Screenshot game (phải-dưới):** `slide05_gameplay_overview.png`
- Cảnh: Đang chơi giữa ván, có vài quân trên bàn, panel 2 người chơi rõ
- Annotation: mũi tên chỉ "đây là `board[i][j]`", "đây là `player1.name`", "đây là `moveCount`"
- → Cho hội đồng thấy struct GameState đại diện cho TOÀN BỘ những gì đang hiển thị

**Speaker cue:**
> *"Toàn bộ trạng thái 1 ván gói gọn trong struct GameState — không có con trỏ, không có khai báo động. Điểm lợi: lưu file chỉ cần 1 lệnh fwrite, sẽ thấy ở slide 7."*

---

### 🏷️ SLIDE 6 — PHÁT HIỆN THẮNG/HÒA (1 phút)

**Tiêu đề:** `Kiểm tra thắng — Quét 4 hướng từ ô vừa đặt`

**Layout 3 vùng:**
- **Trái (50%):** Mã giả Text Box
- **Phải-trên (25%):** Sơ đồ 4 hướng
- **Phải-dưới (25%):** Screenshot khoảnh khắc thắng

**📋 NGUỒN LỌC MÃ GIẢ:** Từ hàm kiểm tra thắng trong `src/menu.cpp`

**Mã giả gõ vào slide (~18 dòng — Text Box, Consolas 14-16pt):**
```
FUNCTION checkWin(board, row, col, player) → boolean:
    directions ← [(0,1), (1,0), (1,1), (1,-1)]  // ngang, dọc, 2 chéo

    FOR EACH (dx, dy) IN directions:
        count ← 1

        // Đếm về phía dương
        FOR i FROM 1 TO 4:
            r ← row + i*dx,  c ← col + i*dy
            IF NOT inBounds(r,c) OR board[r][c] ≠ player THEN
                BREAK
            count ← count + 1

        // Đếm về phía âm (tương tự, đảo dấu dx, dy)
        ...

        IF count ≥ 5 THEN
            RETURN TRUE

    RETURN FALSE
```

**📊 Sơ đồ (phải-trên):** Bàn cờ mini với mũi tên 4 hướng từ 1 ô trung tâm (→, ↓, ↘, ↗)

**🎮 Screenshot game (phải-dưới):** `slide06_win_moment.png`
- Cảnh: Khoảnh khắc thắng — **5 quân được highlight xanh lá** (win line)
- Có panel "Player 1 WINS!" hiện rõ
- → Cho hội đồng thấy thuật toán checkWin chạy → kết quả là hiệu ứng này

**Speaker cue:**
> *"Thay vì quét toàn bàn mỗi lượt — tốn 225 lần check — em chỉ quét từ ô vừa đặt theo 4 hướng. Mỗi lượt chỉ check 40 ô, nhanh gấp 5 lần."*

---

### 🏷️ SLIDE 7 — SAVE / LOAD (1 phút)

**Tiêu đề:** `Save / Load — Đóng gói trạng thái thành binary`

**Layout 3 vùng:**
- **Trái (50%):** Mã giả Text Box
- **Phải-trên (25%):** Flowchart save/load
- **Phải-dưới (25%):** 2 screenshot ghép — Save screen + Load screen

**📋 NGUỒN LỌC MÃ GIẢ:** Từ handler `SCREEN_SAVE` / `SCREEN_LOAD` trong `src/menu.cpp`

**Mã giả gõ vào slide (~14 dòng — Text Box, Consolas 16pt):**
```
FUNCTION saveGame(filename, state) → boolean:
    file ← OPEN("saves/" + filename + ".sav", BINARY_WRITE)
    IF file failed THEN RETURN FALSE

    WRITE state TO file    // Ghi nguyên struct GameState (POD serialization)
    CLOSE file
    RETURN TRUE


FUNCTION loadGame(filename, state) → boolean:
    file ← OPEN("saves/" + filename + ".sav", BINARY_READ)
    IF file failed THEN RETURN FALSE

    READ state FROM file   // Khôi phục nguyên trạng thái ván
    CLOSE file
    RETURN TRUE
```

**📊 Sơ đồ (phải-trên):** Flowchart đơn giản
```
[Đang chơi] ──ESC──→ [Pause Menu]
                        │
              ┌─────────┴─────────┐
              ↓                   ↓
         [Save: tên]         [Quay lại]
              │
              ↓
        saves/abc.sav  ← Binary file
```

**🎮 Screenshot game (phải-dưới):** Ghép 2 ảnh nhỏ
- `slide07_save_screen.png` — Màn hình Save đang nhập tên file
- `slide07_load_screen.png` — Màn hình Load có 3-5 file trong list
- → Cho thấy UI thực tế của save/load

**Speaker cue:**
> *"Vì GameState không có con trỏ, em có thể serialize bằng 1 lệnh fwrite duy nhất. Cách này gọi là POD serialization — đơn giản, nhanh, không cần thư viện JSON."*

---

### 🏷️ SLIDE 8 — MAIN MENU + GAMEPLAY UI (1 phút)

**Tiêu đề:** `13 màn hình — Đầy đủ trải nghiệm game`

**Layout:** Grid 2×3 screenshots (chỉ thuyết minh, không code, không sơ đồ)

**🎮 Visual:** 6 screenshot game ghép trong grid
- `slide08_main_menu.png` — Main Menu
- `slide08_mode_select.png` — Mode Select (PvP / PvC)
- `slide08_difficulty.png` — Difficulty (Easy/Medium/Hard/Expert)
- `slide08_gameplay.png` — Gameplay với panel
- `slide08_pause.png` — Pause Menu
- `slide08_gameover.png` — Game Over

**Code:** Không (slide UI showcase)
**Speaker cue:**
> *"Game có 13 trạng thái màn hình, đầy đủ flow từ menu vào game đến save/load/settings. Điều khiển bằng cả phím và chuột."*

---

### 🏷️ SLIDE 9 — BOT AI TỔNG QUAN (45 giây)

**Tiêu đề:** `Bot AI — 4 cấp độ thông minh`

**Layout:** Bảng so sánh lớn (chiếm toàn slide)

| Cấp | Thuật toán | Look-ahead | Phù hợp với |
|-----|-----------|------------|-------------|
| **Easy** | Random + Chặn nước thắng | 0 lượt | Người mới |
| **Medium** | Heuristic Scoring | 1 lượt | Người trung bình |
| **Hard** | Minimax depth 3 | 3 lượt | Người khá |
| **Expert** | Minimax + α-β depth 4 | 4 lượt | Cao thủ |

**Visual:** Icon độ khó (⭐ → ⭐⭐⭐⭐) cho mỗi cấp

**Bullet bổ sung (dưới bảng, font nhỏ):**
- 💡 **Tính năng Gợi ý (phím H)**: Dùng cùng thuật toán **Medium (Heuristic Scoring)** để gợi ý cho người chơi PvC — đủ thông minh để hữu ích, đủ nhanh (~5ms) để phản hồi tức thì.
- 🎯 **Tất cả cấp độ dùng chung 1 bảng điểm Pattern** (trình bày ở slide tiếp theo).

**Code/Screenshot:** Không (slide tổng quan)
**Speaker cue:**
> *"Mỗi cấp dùng chiến lược hoàn toàn khác nhau. Easy random kèm chặn cơ bản. Expert nhìn trước 4 lượt và cắt tỉa nhánh để vẫn phản hồi nhanh. Tính năng Hint cho người chơi dùng cùng thuật toán Medium để vừa nhanh vừa đủ thông minh."*

---

### 🏷️ SLIDE 10 — PATTERN SCORING (1 phút 15 giây) ⭐ NỀN TẢNG

**Tiêu đề:** `Bảng điểm Pattern — Trái tim của AI`

**Layout 3 vùng:**
- **Trái (50%):** Mã giả Text Box
- **Phải-trên (25%):** Bảng điểm pattern + minh họa
- **Phải-dưới (25%):** Screenshot bàn cờ có pattern thật

**📋 NGUỒN LỌC MÃ GIẢ:** Từ hàm `botScoreLine()` trong `src/bot.cpp`

**Mã giả gõ vào slide (~20 dòng — Text Box, Consolas 14-16pt):**
```
// Cham diem 1 chuoi quan theo 1 huong
FUNCTION scoreLine(board, row, col, dRow, dCol, player) → integer:
    forward  ← countConsecutive(row, col, +dRow, +dCol, player)
    backward ← countConsecutive(row, col, −dRow, −dCol, player)
    total ← forward + backward + 1

    // Đếm số đầu "mở" (ô trống) - quyết định pattern
    openEnds ← 0
    IF ô đầu chuỗi LÀ trống THEN openEnds++
    IF ô cuối chuỗi LÀ trống THEN openEnds++

    // Tra cứu bảng điểm
    IF total ≥ 5 THEN RETURN 1,000,000      // WIN
    IF total = 4 THEN
        IF openEnds = 2 THEN RETURN 100,000  // 4 mở 2 đầu
        IF openEnds = 1 THEN RETURN 5,000    // 4 mở 1 đầu
    IF total = 3 THEN
        IF openEnds = 2 THEN RETURN 5,000    // 3 mở 2 đầu
        IF openEnds = 1 THEN RETURN 500
    // ... (2, 1 tương tự)
    RETURN 0
```

**📊 Bảng điểm (phải-trên):**

| Pattern | Ý nghĩa | Điểm |
|---------|---------|------|
| `XXXXX` | 5 liên tiếp (WIN) | +1,000,000 |
| `_XXXX_` | 4 mở 2 đầu | +100,000 |
| `_XXXX#` | 4 mở 1 đầu | +5,000 |
| `_XXX_` | 3 mở 2 đầu | +5,000 |
| `_XXX#` | 3 mở 1 đầu | +500 |
| `_XX_` | 2 mở 2 đầu | +500 |
| `_XX#` | 2 mở 1 đầu | +50 |

*Chú thích: `_` = ô trống, `#` = bị chặn (đối thủ hoặc biên)*

**🎮 Screenshot game (phải-dưới):** `slide10_pattern_real.png` (rename từ `slide11_pattern_real.png`)
- Cảnh: Bàn cờ có chuỗi 4 liền (`_XXXX_`) — minh họa pattern điểm cao
- Annotation: vòng tròn đỏ quanh chuỗi 4 quân, mũi tên ghi "+100,000 điểm"

**Bullet QUAN TRỌNG (dưới slide):**
> 🎯 **Mọi cấp độ Bot đều dùng bảng điểm này** — Medium chấm trực tiếp, Hard/Expert chấm tại các lá của cây Minimax (slide tiếp theo). Đây là "tri thức về cờ caro" được mã hóa thành số.

**Speaker cue:**
> *"Đây là building block cho cả 4 cấp bot. Hàm scoreLine xét 1 chuỗi quân: đếm có bao nhiêu quân liên tiếp, có bao nhiêu đầu mở (ô trống), rồi tra bảng điểm. Bot Medium dùng trực tiếp — chọn nước có điểm cao nhất. Bot Hard/Expert dùng nó để đánh giá thế trận ở lá của cây Minimax — sẽ trình bày ở slide tiếp theo."*

---

### 🏷️ SLIDE 11 — MINIMAX + ALPHA-BETA ⭐ (2 phút — ĐIỂM NHẤN)

**Tiêu đề:** `Minimax + Alpha-Beta Pruning — Cốt lõi Bot Hard/Expert`

**Layout 3 vùng (slide quan trọng nhất):**
- **Trái (50%):** Mã giả Text Box (22 dòng, dày nhất)
- **Phải-trên (35%):** Cây game tree minh họa pruning (Mermaid Option A — xem chat history)
- **Phải-dưới (15%):** Screenshot nhỏ Bot Expert đang chơi

**📋 NGUỒN LỌC MÃ GIẢ:** Từ hàm `botMinimax()` trong `src/bot.cpp` (ĐIỂM NHẤN — dành 2 phút)

**Mã giả gõ vào slide (~22 dòng — Text Box, Consolas 14pt):**
```
FUNCTION minimax(board, depth, α, β, isMaximizing) → integer:
    // Điều kiện dừng: hết độ sâu hoặc ván kết thúc
    IF depth = 0 OR isGameOver(board) THEN
        RETURN evaluateBoard(board)         // dùng scoreLine (slide trước)

    candidates ← getCandidateMoves(board)   // Chỉ xét ô gần quân đã đặt

    IF isMaximizing THEN                    // Lượt Bot — tìm điểm CAO nhất
        best ← −∞
        FOR EACH move IN candidates:
            apply(move, BOT_PLAYER)
            score ← minimax(board, depth−1, α, β, FALSE)
            undo(move)

            best ← MAX(best, score)
            α ← MAX(α, best)
            IF β ≤ α THEN BREAK             // ✂ Cắt tỉa Beta
        RETURN best

    ELSE                                    // Lượt người — tìm điểm THẤP nhất
        best ← +∞
        FOR EACH move IN candidates:
            ...                              // Tương tự, đảo dấu
            IF β ≤ α THEN BREAK             // ✂ Cắt tỉa Alpha
        RETURN best
```

**📊 Sơ đồ (phải-trên):** Cây game tree với leaf values + pruning annotation

Xuất từ Mermaid (xem code trong chat history), khuyến nghị **Mermaid Option B** với giá trị 30/40/50/70:
```
              MAX root (chọn max = 50)
            /        |          \
       MIN(50)   MIN(30)    MIN(40)
       /    \    /   ✂     /    ✂
      50   70  30  pruned 40  pruned
```
Chú thích: "Alpha-Beta cắt 2/6 leaf → tiết kiệm 33%. Với depth 4 → tiết kiệm theo cấp số nhân."

**🎮 Screenshot game (phải-dưới):** `slide11_bot_thinking.png` (rename từ `slide10_bot_thinking.png`)
- Cảnh: PvC Expert đang chơi, có hiện text "Bot đang suy nghĩ..." (sau khi fix Bug 1)
- Caption nhỏ: "Bot Expert chạy thuật toán này trong 2-5s mỗi nước (depth 4)"

**Speaker cue (chia 3 phần, mỗi phần ~40 giây):**

> **Phần 1 (40s — giải thích Minimax):**
> *"Minimax giả định cả 2 người chơi tối ưu. Bot là MAX — tìm nước có điểm cao nhất. Đối thủ là MIN — Bot giả định người chơi chọn nước làm Bot bất lợi nhất. Hàm evaluateBoard ở lá của cây dùng chính bảng điểm pattern slide trước."*

> **Phần 2 (40s — giải thích Alpha-Beta):**
> *"Vấn đề: depth 4 phải duyệt rất nhiều trạng thái. Alpha-Beta giải quyết: nếu một nhánh đã chắc chắn tệ hơn nhánh tốt nhất đã tìm — cắt luôn không duyệt nữa. Trong sơ đồ: nhánh giữa và phải đều bị cắt vì leaf đầu (30, 40) đã ≤ 50."*

> **Phần 3 (40s — nhấn vào dòng `IF β ≤ α THEN BREAK`):**
> *"Đây là dòng quan trọng nhất. Khi điều kiện thoả → cắt nhánh ngay. Nhờ đó Expert depth 4 vẫn phản hồi trong 2-5 giây thay vì lâu hơn nhiều."*

---

### 🏷️ SLIDE 12 — SPEED MODE (CHESS-CLOCK) (1 phút)

**Tiêu đề:** `Speed Mode — Chess-clock per-player`

**Layout 2 vùng:**
- **Trái (60%):** Mã giả Text Box (timer chess-clock + expire logic)
- **Phải (40%):** Screenshot Speed Mode + chú thích

**📋 NGUỒN LỌC MÃ GIẢ:** Từ phần update timer trong `handleGameplay()` (`src/menu.cpp`)

**Mã giả gõ vào slide (~22 dòng — Text Box, Consolas 14pt):**
```
// Lay thoi gian troi qua giua 2 frame
deltaTime ← clock.restart().asSeconds()

// ─── Cap nhat timer (chess-clock: chi tru thoi gian nguoi dang di) ───
IF style = SPEED AND result = NONE THEN
    timer.turnTimeLeft ← timer.turnTimeLeft − deltaTime

    IF isPlayer1Turn THEN
        timer.gameTimeLeftP1 ← timer.gameTimeLeftP1 − deltaTime
    ELSE
        timer.gameTimeLeftP2 ← timer.gameTimeLeftP2 − deltaTime

    // Xu thua ngay neu het 20 giay/luot
    IF timer.turnTimeLeft ≤ 0 THEN
        result ← IF isPlayer1Turn THEN PLAYER2_WIN ELSE PLAYER1_WIN
        RETURN handleGameOver(result)

    // Ai het thoi gian van -> nguoi do THUA (chuan co vua)
    IF timer.gameTimeLeftP1 ≤ 0 THEN
        result ← PLAYER2_WIN
        RETURN handleGameOver(result)
    IF timer.gameTimeLeftP2 ≤ 0 THEN
        result ← PLAYER1_WIN
        RETURN handleGameOver(result)
```

**🎮 Screenshot game (phải):** `slide12_speed_timer.png`
- Cảnh: Speed Mode đang chạy, **2 player panel cùng hiện "Thời gian ván: MM:SS"** riêng biệt
- Có thể chụp lúc 1 bên ≤30s để thấy chữ ĐỎ
- → Cho hội đồng thấy 2 timer riêng biệt (chess-clock)

**Bullet bổ sung dưới screenshot:**
- ⚖️ **Mỗi người 10 phút riêng** — chỉ trừ thời gian khi đến lượt
- 🏆 **Ai hết time → người đó THUA** (chuẩn cờ vua, dễ giải thích)
- 🛡️ **Bot's thinking time tính cho bot** — không leak vào turn của người chơi (xem fix Bug 1 trong report)

**Speaker cue:**
> *"Nhóm em thiết kế Speed mode theo cơ chế **chess-clock**: mỗi người có 10 phút riêng, đồng hồ chỉ chạy khi đến lượt mình. Khi hết thời gian, người đó thua — quy tắc giống cờ vua truyền thống, đảm bảo công bằng. Game loop chạy 60 FPS, mỗi frame tính delta time bằng sf::Clock — đảm bảo độc lập với tốc độ máy, không cần thread, không có race condition."*

---

### 🏷️ SLIDE 13 — MOUSE SUPPORT (45 giây)

**Tiêu đề:** `Hỗ trợ chuột (Mouse Support)`

**Layout 2 vùng (60/40):**
- **Trái (60%):** Mã giả Text Box (pixelToBoard + event handling)
- **Phải (40%):** Bullet mô tả + note demo live

**📋 NGUỒN LỌC MÃ GIẢ:** Từ `renderPixelToBoard()` (`src/render.cpp`) + event loop trong `handleGameplay()` (`src/menu.cpp`)

**Mã giả gõ vào slide (~18 dòng — Text Box, Consolas 16pt):**
```
// ============================================
// HAM CHUYEN DOI PIXEL CHUOT → TOA DO BAN CO
// ============================================
FUNCTION pixelToBoard(x, y) → (row, col):
    col ← (x − BOARD_OFFSET_X) / CELL_SIZE
    row ← (y − BOARD_OFFSET_Y) / CELL_SIZE

    IF row < 0 OR row ≥ 15 OR col < 0 OR col ≥ 15 THEN
        RETURN INVALID                      // Click ngoai board

    RETURN (row, col)


// ============================================
// XU LY SU KIEN CHUOT TRONG GAME LOOP
// ============================================
WHILE pollEvent(event):

    // Di chuot → cursor theo chuot
    IF event = MouseMoved THEN
        (r, c) ← pixelToBoard(event.x, event.y)
        IF hop le THEN
            cursorRow ← r
            cursorCol ← c

    // Click trai → dat quan
    IF event = MouseLeftClick THEN
        (r, c) ← pixelToBoard(event.x, event.y)
        IF hop le THEN
            doPlayerPlace(r, c)
```

**Bullet bên phải:**
- 🖱️ **Hover** → cursor highlight di theo chuột
- 🖱️ **Click trái** → đặt quân (PvP & PvC)
- 🖱️ Tương thích **letterbox/resize window** nhờ `handleCommonEvent()` map pixel → view coords
- 🖱️ **Song song bàn phím** — người chơi tùy chọn cách điều khiển
- 🖱️ Áp dụng cho cả **menu** (`menuHitTest` để click chọn item)

**🎬 NOTE (góc dưới-phải, font nhỏ italic):**

> *"Demo trực tiếp tại buổi thuyết trình — Vì screenshot không hiển thị được con trỏ chuột, anh/chị xem demo live nhé."*

**Speaker cue (~45 giây):**

> **Mở bài (10s):** *"Bên cạnh chế độ Speed vừa trình bày, nhóm em chú trọng trải nghiệm điều khiển linh hoạt. Game hỗ trợ song song cả bàn phím và chuột — người chơi tự chọn cách thoải mái nhất."*
>
> **Kỹ thuật (20s):** *"Thử thách kỹ thuật chính là ánh xạ pixel thực của chuột về tọa độ ô bàn cờ. Hàm pixelToBoard lấy tọa độ pixel chia cho CELL_SIZE, trừ đi BOARD_OFFSET để ra row-col. Nếu nằm ngoài board, hàm trả invalid để bỏ qua click."*
>
> **Demo (15s):** *(chuyển sang cửa sổ game, di chuột + click 1-2 nước)* *"Mouse hoạt động tốt cả khi maximize window nhờ handleCommonEvent."*

---

### 🏷️ SLIDE 14 — DEMO TRANSITION (giây 0 — chỉ là slide chuyển)

**Tiêu đề:** `🎮 DEMO TRỰC TIẾP`
**Subtitle:** *Cùng xem sản phẩm hoạt động*
**Layout:** Toàn slide, font cực lớn, màu nổi bật (vàng/cam trên nền tối)

**KỊCH BẢN DEMO 6 PHÚT (rất quan trọng — học thuộc):**

```
⏱ 00:00-00:30 │ Mở game → Main Menu
              │ Nói: "Đây là main menu, có 5 lựa chọn..."
              │ Click "New Game"

⏱ 00:30-01:30 │ Chọn PvP → nhập tên → Style Basic
              │ Đánh vài quân bằng CHUỘT
              │ Nói: "Cả phím WASD và chuột đều hoạt động"
              │ Đánh 4-5 nước để thấy animation

⏱ 01:30-02:30 │ ESC → Pause → Save Game
              │ Đặt tên file → Save thành công
              │ Quit về Menu → Load Game
              │ Double-click file vừa lưu → tiếp tục ván
              │ Nói: "Trạng thái khôi phục y nguyên"

⏱ 02:30-04:00 │ Menu → New Game → PvC → Hard → Speed
              │ Chơi với bot Hard ở chế độ Speed
              │ Để thanh timer chạy gần hết → đổi màu đỏ
              │ Nói: "Speed Mode tạo áp lực thời gian"
              │ Cố tình thua bot → xem game over animation

⏱ 04:00-05:30 │ New Game → PvC → EXPERT → Basic
              │ Đánh đối đầu bot Expert
              │ Highlight: "Bot suy nghĩ rồi mới đi"
              │ Cho thấy bot chặn nước thắng + tạo bẫy

⏱ 05:30-06:00 │ Settings → Đổi volume → Tắt game
              │ Mở lại → Settings → Volume giữ nguyên
              │ Nói: "Cài đặt được persist qua file"
              │ Alt+Tab về slide 14
```

**Slide nội dung:** Vài bullet ngắn để khán giả biết sẽ demo gì:
- ✓ PvP với chuột + phím
- ✓ Save & Load
- ✓ Speed Mode với Bot Hard
- ✓ Bot Expert
- ✓ Settings persist

**Code/Sơ đồ/Screenshot:** Không (slide transition)
**Speaker cue:**
> *"Đến đây em xin tạm dừng slide để demo trực tiếp toàn bộ tính năng. Em mở game..."*
> *(Alt+Tab sang game / cửa sổ exe đã mở sẵn)*

---

### 🏷️ SLIDE 15 — BẢNG ĐỐI CHIẾU YÊU CẦU (1 phút)

**Tiêu đề:** `Đối chiếu yêu cầu đề bài`

**Layout:** Bảng lớn, chiếm toàn slide

| Yêu cầu (theo PDF đề) | Trạng thái | Mức độ |
|----------------------|-----------|--------|
| 4.1 Save/Load trò chơi | ✅ | Vượt yêu cầu (15 slots) |
| 4.2 Nhận biết thắng/thua/hòa | ✅ | Tối ưu O(1)/lượt |
| 4.3 Hiệu ứng thắng/thua/hòa | ✅ | Animation + sound |
| 4.4 Giao diện gameplay | ✅ | Panel stats đầy đủ |
| 4.5 Màn hình chính (menu) | ✅ | 13 màn hình |
| **➕ Bot AI 4 cấp + Hint** | 🌟 | Sáng tạo thêm |
| **➕ Speed Mode (chess-clock)** | 🌟 | Sáng tạo thêm |
| **➕ Mouse Support** | 🌟 | Sáng tạo thêm |
| **➕ i18n (VN/EN) + Persist Settings** | 🌟 | Sáng tạo thêm |
| **➕ Các tính năng UI/UX khác** | 🌟 | Hỗ trợ một số tính năng âm thanh, UI/UX mượt cho game |

**Code/Screenshot:** Không (slide bảng)
**Speaker cue:**
> *"5/5 yêu cầu cơ bản đạt đủ. Phần nâng cao nhóm chủ động thêm các tính năng — trong đó nổi bật nhất là Bot AI với Minimax Alpha-Beta và Speed Mode chess-clock."*

---

### 🏷️ SLIDE 16 — KẾT LUẬN + CẢM ƠN (30 giây)

**Tiêu đề:** `Tổng kết`
**Nội dung 3 bullet:**
- **Học được:** SFML, Minimax, Alpha-Beta, Game Loop, File I/O
- **Áp dụng:** Mảng 2D, struct, đệ quy, pointer-free design
- **Khó khăn:** Đồng bộ UI 13 màn hình, tối ưu bot < 200ms

**Footer (size lớn, đậm):**
> **Cảm ơn thầy/cô đã lắng nghe**
> *Nhóm sẵn sàng trả lời câu hỏi*

**Visual:** Logo KHTN ở góc + ảnh nhóm (nếu có)
**Code:** Không
**Speaker cue:**
> *"Em xin cảm ơn thầy/cô. Em sẵn sàng nhận câu hỏi ạ."*

---

## 3. QUY CHUẨN VIẾT MÃ GIẢ (PSEUDOCODE) - DÀNH CHO NHÓM 1

> ⚠️ **YÊU CẦU TỪ THẦY (đọc kỹ trước khi làm):**
> *"Thể hiện bằng các đoạn mã giả (được lọc từ source code, lưu ý mã giả trong slide và report cần gõ lại cho cẩn thận, không phải kiểu chụp hình gán vào). Bên cạnh mã giả nên đính kèm hình ảnh minh họa cho dễ hiểu."*
>
> **Diễn dịch yêu cầu:**
> 1. KHÔNG paste C++ thật → phải LỌC thành mã giả (pseudocode)
> 2. KHÔNG chụp ảnh code từ IDE / Carbon / VSCode rồi dán → phải **GÕ TAY thành text** trong Text Box của PowerPoint
> 3. PHẢI có sơ đồ hoặc hình minh họa của game đặt cạnh mã giả (sơ đồ + screenshot game được dùng ảnh PNG)
> 4.  Lưu ý: Phần mã giả ở trên tuy đã được chuẩn bị sẵn nhưng bản thân mỗi bạn trong nhóm 1 cần phải tự mở code lên + nhờ AI hỗ trợ check để viết một cách chính xác nhất và phù hợp với toàn bộ source code. Nếu sai thì sẽ phải chịu trách nhiệm toàn bộ cho phần mình làm (đánh giá không tốt trong phần làm việc ở report)

### 3.1. Nguyên tắc viết mã giả
> 💡 **Mẹo nhanh:** Xác định code cần lấy → nhờ AI (ChatGPT, Claude) sinh mã giả từ đoạn code đó là được. Sau đó verify lại logic và tinh chỉnh tên biến cho khớp.

#### ✅ NÊN làm:
- Dùng **keyword UPPERCASE** chuẩn học thuật: `FUNCTION`, `IF`, `THEN`, `ELSE`, `FOR`, `WHILE`, `RETURN`, `BREAK`
- Tên biến **CamelCase** hoặc **snake_case** tùy convention (`turnTimeLeft`, `move_count`)
- Comment bằng **tiếng Việt** giải thích "tại sao", không phải "làm gì" (vì code đã thể hiện)
- Dùng `←` cho gán, `=` cho so sánh bằng, `≤` `≥` `≠` `∞` cho biểu thức toán
- Dùng `...` để rút gọn phần lặp lại không trọng tâm
- Giữ thuật toán "compile được trên giấy" — đọc xong người khác viết lại được code thật

#### ❌ KHÔNG làm:
- KHÔNG dùng `std::`, `int`, `void`, `bool` — đó là C++
- KHÔNG dùng dấu `;` cuối dòng — không cần thiết
- KHÔNG paste include / namespace
- KHÔNG viết mã giả dài quá 25 dòng — hội đồng không kịp đọc

### 3.2. Quy tắc số dòng & font

| Loại slide | Số dòng tối đa | Font Text Box | Background |
|-----------|----------------|---------------|------------|
| Slide thường (5, 7, 11, 12) | 14-18 dòng | **Consolas 16pt** | `#1E1E1E` (tối) |
| Slide điểm nhấn (10 — Minimax) | 20-22 dòng | **Consolas 14pt** | `#1E1E1E` (tối) |
| Tuyệt đối không quá | 25 dòng | — | — |

### 3.3. Format trên slide — CỰC KỲ QUAN TRỌNG

#### ✅ ĐÚNG (Text Box gõ tay):
```
Trong PowerPoint:
1. Insert → Text Box → kéo vùng cần đặt code
2. Bên trong: Font Consolas, 14-16pt
3. Fill: Solid color #1E1E1E (đen-xám)
4. Text color: #D4D4D4 (trắng-xám)
5. Gõ tay từng dòng pseudocode
6. (Tùy chọn) Tô vàng keyword RETURN/BREAK bằng Format Text
→ Khi click vào nội dung CHỌN ĐƯỢC TEXT → chứng tỏ là chữ gõ, không phải ảnh
```

### 3.4. Bảng tổng hợp mã giả cần viết cho từng slide

| Slide | Nguồn lọc từ | Hàm/Phần | Số dòng đích | Lọc gì để gọn |
|-------|--------------|----------|--------------|----------------|
| **5** | `include/game_types.h` | struct `Cell`, `Move`, `GameState` | ~16 dòng | Bỏ comment thừa, dùng STRUCT keyword. **Lưu ý**: `TimerState` đã đổi sang per-player (`gameTimeLeftP1/P2`) |
| **6** | `src/board.cpp` (check win) | `boardCheckWin()` | ~16 dòng | Rút phần "đếm phía âm" thành `// tương tự` |
| **7** | `src/save_load.cpp` | `saveGame()` + `loadGame()` | ~14 dòng | Dùng OPEN/READ/WRITE/CLOSE thay vì `fstream` cụ thể |
| **10** | `src/bot.cpp` | `botScoreLine()` (Pattern Scoring) | ~20 dòng | Giữ logic đếm + tra bảng, bỏ chi tiết edge case |
| **11** | `src/bot.cpp` | `botMinimax()` | ~22 dòng | Giữ FULL phần Maximizing, rút Minimizing bằng `... // tương tự, đảo dấu` |
| **12** | `src/menu.cpp` | Timer chess-clock trong `handleGameplay()` | ~22 dòng | Bỏ phần animation cũ, chỉ giữ logic chess-clock + expire P1/P2 |
| **13** | `src/render.cpp` + `src/menu.cpp` | `renderPixelToBoard()` + event handling | ~18 dòng | Gọn về 2 block: hàm chuyển đổi + event loop |

### 3.5. Bảng tổng hợp screenshot game cần chụp (sau khi UI polish xong)

> ⚠️ **CẬP NHẬT 30/05/2026**: Sau khi đổi timer UI sang **per-player chess-clock** + thêm "Bot đang suy nghĩ" indicator, có **6 ảnh CẦN CHỤP LẠI**. Đánh dấu 🔴 dưới đây.

| Slide | Tên file | Cảnh cần chụp | Status |
|-------|----------|---------------|--------|
| **5** | `slide05_gameplay_overview.png` | Đang chơi giữa ván, panel 2 người chơi rõ, **có time trong mỗi panel** | 🔴 Chụp lại |
| **6** | `slide06_win_moment.png` | Khoảnh khắc thắng — 5 quân highlight + Panel "WINS!" + time | 🔴 Chụp lại |
| **7** | `slide07_save_screen.png` | Màn hình Save đang nhập tên file | ✅ Giữ |
| **7** | `slide07_load_screen.png` | Màn hình Load có 3-5 file trong list | ✅ Giữ |
| **8** | `slide08_main_menu.png` | Main Menu | ✅ Giữ |
| **8** | `slide08_mode_select.png` | Mode Select | ✅ Giữ |
| **8** | `slide08_difficulty.png` | Difficulty Select | ✅ Giữ |
| **8** | `slide08_gameplay.png` | Gameplay với panel mới (per-player time) | 🔴 Chụp lại |
| **8** | `slide08_pause.png` | Pause Menu | ✅ Giữ |
| **8** | `slide08_gameover.png` | Game Over (banner + time mới) | 🔴 Chụp lại |
| **10** | `slide10_pattern_real.png` (rename) | Bàn cờ có chuỗi `_XXXX_` — minh họa pattern | ✅ Giữ (rename từ slide11) |
| **11** | `slide11_bot_thinking.png` (rename) | PvC Expert đang chơi, **có text "Bot đang suy nghĩ..."** | 🔴 Chụp lại (rename từ slide10) |
| **12** | `slide12_speed_timer.png` ⭐ | Speed Mode, **2 timer riêng biệt trong 2 panel**, chữ đỏ khi ≤30s | 🔴 Chụp lại (BIG change) |
| **13** | (Không cần) | Mouse cursor không bắt được qua screenshot → **demo live** | — |

→ **Tổng: 13 screenshot** cần chụp. **6 ảnh cần chụp lại** sau khi fix Bug 1+2.

### 3.6. Quy ước highlight (tô màu trong Text Box)
> 💡 *Cái này team mã giả test thử trước nếu thấy màu mè quá thì bỏ.*

- **Vàng / Cam (`#FFC857`)**: Keywords quan trọng → `RETURN`, `BREAK`, `IF β ≤ α THEN BREAK` (dòng cắt tỉa)
- **Xanh lá nhạt (`#7EC699`)**: Comment giải thích thuật toán (`// Cắt tỉa Beta`)
- **Xanh dương nhạt (`#82AAFF`)**: Tên hàm khi gọi (`minimax`, `scoreLine`, `pixelToBoard`)
- **Trắng-xám (`#D4D4D4`)**: Text chính (mặc định)
- **Đỏ:** TRÁNH dùng — gây căng thẳng khi nhìn

**Cách tô màu trong PowerPoint:**
1. Bôi đen từ keyword cần tô
2. Home → Font Color → More Colors → nhập hex
3. Lặp lại — tốn ~5 phút/slide

### 3.7. Nhóm thực hiện mã giả + sơ đồ lấy slides từ nhóm tạo slides để chèn vào đúng theo các yêu cầu đã đưa ra

**Workflow chuẩn:**

```
Nhóm 2 (Vũ Quang)              Nhóm 3 (Nhật + Toàn)
   │                                │
   │ Tạo cấu trúc/template          │ Hoàn thiện UI game
   │ slides với placeholder         │ Chụp 13 screenshot
   │ (chỉ vạch vị trí, hoàn         │
   │  thiện tạo khung,              | 
      lấy ảnh minh họa              |
      code hiện có                  |
      để chỉ cho N1                 |
      - Đại và Lực )                │
   ↓                                ↓
   Caro_Template.pptx               docs/screenshots/*.png
        \                          /
         \                        /
          ↘                      ↙
            Nhóm 1 (Đại + Lực)
                  │
                  │ 1. Mở Caro_Template.pptx
                  │ 2. Cho từng slide có mã giả (5,6,7,10,11,12):
                  │    - Chèn Text Box mã giả (gõ tay) vào vị trí placeholder Trái
                  │    - Chèn sơ đồ PNG vào vị trí placeholder Phải-trên
                  │    - Chèn screenshot game PNG vào vị trí placeholder Phải-dưới
                  │ 3. Tô màu keyword theo mục 3.6
                  │ 4. Verify: click vào code → edit được text
                  ↓
            Caro_Presentation_FINAL.pptx
```

**Cấu trúc file `docs/`:** --> lập ra cho nhóm 1 có thể thuận tiện nhất khi chèn ảnh, mã giả và sơ đồ vào slides

```
docs/
├── PRESENTATION_PLAN.md      ← File này
├── ASSETS_PLAN.md            ← Plan tài nguyên
├── code_snippets.md          ← [Nhóm 1] 6 đoạn mã giả gõ tay
├── diagrams/                 ← [Nhóm 1] 7 sơ đồ PNG
│   ├── slide04_architecture.png
│   ├── slide06_4directions.png
│   ├── slide07_saveload_flow.png
│   ├── slide09_bot_comparison.png
│   ├── slide10_minimax_tree.png    ← Quan trọng nhất
│   ├── slide11_pattern_table.png
│   └── slide12_easeout_curve.png
├── screenshots/              ← [Nhóm 3] 13 screenshot game
│   ├── slide05_gameplay_overview.png
│   ├── slide06_win_moment.png
│   ├── slide07_save_screen.png
│   ├── slide07_load_screen.png
│   ├── slide08_main_menu.png
│   ├── slide08_mode_select.png
│   ├── slide08_difficulty.png
│   ├── slide08_gameplay.png
│   ├── slide08_pause.png
│   ├── slide08_gameover.png
│   ├── slide10_bot_thinking.png
│   ├── slide11_pattern_real.png
│   └── slide12_speed_timer.png
├── Caro_Template.pptx        ← [Nhóm 2] Template trống
└── Caro_Presentation.pptx    ← [Nhóm 1] File final đã chèn nội dung
```

---

## 4. PHÂN CÔNG CÔNG VIỆC & DEADLINE

### 4.1. Nguyên tắc phân công
- **3 nhóm việc song song** — không phụ thuộc nhau hoàn toàn để chạy đua deadline
- **Deadline chung cho mỗi nhóm:** Không deadline lẻ từng task → tránh áp lực vụn vặt
- **Tất cả hoàn thành trước:** ✅ **23:59 ngày 24/05/2026 (Chủ nhật)**

### 4.2. Nhóm việc 1 — Mã giả & Sơ đồ & Chèn vào slides (2 thành viên)

**Thành viên phụ trách:**
- 👤 **Đại & Lực** (focus: viết mã giả lọc từ C++ source + vẽ sơ đồ/diagram + chèn tất cả vào slides)

> ⚠️ **Lưu ý quan trọng:** Sau khi hoàn thành mã giả và sơ đồ, hai ông sẽ là người **đi chèn mã giả + ảnh + sơ đồ bản thân mình làm vào đúng slides do ông Quang tạo** nhé. Phần screenshot game thì chờ Nhật và Toàn (Nhóm 3) chụp xong mới chèn được.

**Phạm vi công việc:**

#### A. Viết mã giả (theo quy chuẩn mục 3 + bảng 3.4)

> ⚠️ **Lưu ý quan trọng:** Phần mã giả ở trên tuy đã được chuẩn bị sẵn nhưng bản thân mỗi bạn trong nhóm 1 cần phải tự mở code lên + nhờ AI hỗ trợ check để viết một cách chính xác nhất và phù hợp với toàn bộ source code. Nếu sai thì sẽ phải chịu trách nhiệm toàn bộ cho phần mình làm (đánh giá không tốt trong phần làm việc ở report) - điều này đã được nhắc kĩ ở trên mục 3 cho các bạn

**QUY TRÌNH 3 BƯỚC viết mã giả:**
1. Mở source C++ → ĐỌC kỹ logic, hiểu ý đồ thuật toán
2. Bỏ chi tiết C++ (`std::`, `int`, `;`, `{}`) → diễn đạt lại bằng keyword UPPERCASE
3. Rút gọn xuống ≤22 dòng, các phần rút gọn thì comment giải thích phần đó thực hiện chức năng gì

- [ ] Slide 5 — STRUCT GameState + TimerState per-player (lọc từ `game_types.h`)
- [ ] Slide 6 — FUNCTION boardCheckWin (lọc từ `board.cpp`)
- [ ] Slide 7 — FUNCTION saveGame + loadGame (lọc từ `save_load.cpp`)
- [ ] Slide 10 — FUNCTION scoreLine (Pattern Scoring, lọc từ `bot.cpp`) ⭐ NỀN TẢNG
- [ ] Slide 11 — FUNCTION minimax + Alpha-Beta (lọc từ `bot.cpp`) ⭐ ĐIỂM NHẤN
- [ ] Slide 12 — Timer chess-clock update + expire check (lọc từ `menu.cpp`)
- [ ] Slide 13 — pixelToBoard + event handling (lọc từ `render.cpp` + `menu.cpp`)

**Yêu cầu output:**
- File `docs/code_snippets.md` chứa 7 đoạn mã giả (KHÔNG phải C++)
- Mỗi đoạn ghi rõ: nguồn lọc từ file/hàm nào, đã đơn giản hóa thuật toán nào
  > *Cái này thì tùy, gợi ý vậy để chừng chèn vào slides đỡ mắc công xem lại file docs*

#### B. Vẽ sơ đồ minh họa (đặt cạnh mã giả)
- [ ] Slide 4 — Sơ đồ kiến trúc module (Draw.io / PowerPoint shapes)
- [ ] Slide 6 — Bàn cờ 4 hướng quét
- [ ] Slide 7 — Flowchart save/load
- [ ] Slide 9 — Bảng so sánh 4 cấp bot (icon độ khó)
- [ ] Slide 10 — Bảng pattern scoring (table style) + minh họa 3 chuỗi
- [ ] Slide 11 — Cây game tree Minimax với leaf values + pruning (Mermaid Option B, xem chat history) ⭐ ƯU TIÊN
- [ ] Slide 12 — Screenshot per-player chess-clock UI (đã có sẵn nếu chụp lại)

**Yêu cầu output:**
- 7 file ảnh `.png` độ phân giải tối thiểu 1920×1080, nền trong suốt
- Đặt trong thư mục `docs/diagrams/`
- Đặt tên theo slide: `slide04_architecture.png`, `slide10_minimax_tree.png`, v.v.

**Công cụ đề xuất:**
- **Draw.io** (https://app.diagrams.net/) — free, dễ vẽ flowchart
- **Excalidraw** (https://excalidraw.com/) — phong cách vẽ tay đẹp
- **PowerPoint native shapes** — vẽ trực tiếp trong slide, dễ chỉnh sửa

#### C. Chèn screenshot game vào slide (thực hiện lấy từ Nhóm 3 sau khi hoàn thiện đồ họa game)
- [ ] Lấy 13 screenshot từ `docs/screenshots/` (đợi Nhật Toàn hoàn thiện các bước cuối cùng của gameCaro)
- [ ] Chèn từng ảnh vào vị trí placeholder Phải-dưới của các slide tương ứng (5, 6, 7, 8, 10, 11, 12) --> Xem bảng để thấy rõ
- [ ] Resize cho vừa khung, không méo
- [ ] Thêm annotation/caption nếu cần (xem mô tả từng slide ở mục 2)

#### D. Chèn ảnh, sơ đồ và mã giả vào slide tương ứng
- [ ] Mở `Caro_Template.pptx` do Quang tạo
- [ ] Với mỗi slide 5, 6, 7, 10, 11, 12:
  - Chèn Text Box mã giả (gõ tay) vào vị trí placeholder Trái
  - Chèn sơ đồ PNG vào vị trí placeholder Phải-trên
  - Chèn screenshot PNG vào vị trí placeholder Phải-dưới
- [ ] Format Text Box theo quy chuẩn mục 3.3 + 3.6
- [ ] Verify: click vào code → edit được text (không phải ảnh)
- [ ] Save as `Caro_Presentation.pptx`

**Deadline nhóm 1:**
- ⏰ **23:59 — 21/05/2026 (Thứ Năm)** cho A và B (mã giả + sơ đồ)
- ⏰ **23:59 — 24/05/2026 (Chủ Nhật)** cho cả C + D (chèn screenshot + chèn vào slides)

---

### 4.3. Nhóm việc 2 — Thiết kế Cấu trúc/Template Slide PowerPoint (1 thành viên)

**Thành viên phụ trách:**
- 👤 **Vũ Quang** (focus: tạo template slides với layout placeholder)

> 💡 **Vai trò:** Chỉ tạo **CẤU TRÚC TRỐNG** của 15 slides. Không cần điền nội dung chính xác nhưng cần xác định rõ khung, nơi chứa (nên kèm theo ảnh minh họa, lược đồ (đối với phần ảnh và lược đồ) và khung text (nội dung tùy ý đối với phần persudocode) - Nghĩa là ông không cần tạo nội dung chính xác nhưng cần xác định rõ phần nào trong một slides để thuận cho Nhóm 1 sẽ chèn mã giả/sơ đồ/screenshot vào sau.

**Phạm vi công việc:**

#### A. Setup template
- [ ] Tạo file `docs/Caro_Template.pptx`
- [ ] Tạo master slide với:
  - Header: Logo KHTN + Tên nhóm (góc trên phải)
  - Footer: Số slide + tên dự án (góc dưới)
  - Color palette nhất quán
- [ ] Chuẩn bị 3 layout chính:
  - Layout 1: Title only (cho Slide 1, 13, 15)
  - Layout 2: Content only (cho Slide 2, 3, 9, 14)
  - Layout 3: **Code + Diagram + Screenshot** (cho Slide 5, 6, 7, 10, 11, 12) ⭐

#### B. Dựng 15 slides với LAYOUT PLACEHOLDER
- [ ] Slide 1 (Cover): chỉ tiêu đề + logo
- [ ] Slide 2 (Agenda): 5 bullets placeholder
- [ ] Slide 3 (Intro): 3 bullets placeholder
- [ ] Slide 4 (Kiến trúc): 1 placeholder lớn cho sơ đồ
- [ ] **Slide 5, 6, 7, 10, 11, 12 (mã giả):**
  - Vùng Trái (50%): placeholder ghi "MÃ GIẢ Ở ĐÂY"
  - Vùng Phải-trên (25%): placeholder ghi "SƠ ĐỒ Ở ĐÂY"
  - Vùng Phải-dưới (25%): placeholder ghi "SCREENSHOT GAME Ở ĐÂY"
- [ ] Slide 8 (UI showcase): grid 2×3 placeholder cho 6 screenshot
- [ ] Slide 9 (Bot tổng quan): placeholder cho bảng
- [ ] Slide 13 (Demo): full-screen text "DEMO TRỰC TIẾP"
- [ ] Slide 14 (Bảng đối chiếu): placeholder cho bảng
- [ ] Slide 15 (Cảm ơn): footer text

#### C. Polish template
- [ ] Font nhất quán (đề xuất: Montserrat / Segoe UI / Calibri)
- [ ] Font size: tiêu đề 32pt, body 18-20pt, code area để Consolas 14-16pt
- [ ] Color palette nhất quán (xem dưới)
- [ ] Animation chuyển slide: dùng `Fade` đơn giản, không dùng hiệu ứng lòe loẹt
- [ ] Đảm bảo mỗi slide dùng được khi chiếu trên máy chiếu (không quá tối/sáng)

**Color palette đề xuất (đồng bộ với game- Cái này có thể đổi sau sau khi tui và Toàn hoàn thành game nên ông linh hoạt):**
```
Primary  (background):  #2C3E50 (dark blue)
Accent   (highlight):   #F39C12 (cam — match màu game)
Text     (body):        #ECF0F1 (gần trắng)
Code BG  (block):       #1E1E1E (dark như VS Code)
Code FG  (text):        #D4D4D4 (xám sáng)
Player X (đỏ):          #DC3232
Player O (xanh):        #1E64C8
```

**Font đề xuất:**
- Tiêu đề: `Montserrat Bold` hoặc `Segoe UI Semibold`
- Body: `Calibri` hoặc `Segoe UI`
- Code: `Consolas` hoặc `Cascadia Code` (kích thước 14-16pt)

**Yêu cầu output:**
- File `docs/Caro_Template.pptx`
- Phải mở được trên PowerPoint 2019

**Deadline nhóm 2:** ⏰ **23:59 — 22/05/2026 (Thứ Năm)**
*(Sớm 2 ngày để Nhóm 1 có thời gian chèn nội dung)*

---

### 4.4. Nhóm việc 3 — Assets & Screenshot game (1 thành viên + Lead)

**Thành viên phụ trách:**
- 👤 **Nhật & Toàn** (Project Lead + 1 thành viên)

> 📂 **Chi tiết:** Xem file `ASSETS_PLAN.md`

**Tóm tắt phạm vi:**
- Tải assets (1 BGM + 5 SFX + 4 texture + 2 font)
- Wire textures vào `render.cpp` + UI polish
- **Chụp 13 screenshot game** theo bảng mục 3.5 (sau khi UI polish xong)
- Đặt screenshot vào `docs/screenshots/` → báo Nhóm 1 chèn vào slides

**Deadline nhóm 3:**
- Assets + UI polish: ⏰ **23:59 — 21/05/2026 (Thứ Năm)**
- 13 screenshot game: ⏰ **23:59 — 23/05/2026 (Thứ Sáu)** *(để Nhóm 1 có thời gian chèn)*

---

**Phụ thuộc:**
- Nhóm 1 (C, D) cần TEMPLATE từ Nhóm 2 + SCREENSHOT từ Nhóm 3 → mới chèn được
- Nhóm 2 và Nhóm 3 (a) chạy SONG SONG, không phụ thuộc nhau

---

## ✅ DEFINITION OF DONE

File này được coi là **hoàn tất** khi:
- [x] Cả 3 nhóm việc báo cáo done trước 24/05 23:59
- [x] File `Caro_Presentation.pptx` chạy được trên máy thuyết trình
- [x] Mã giả là Text Box gõ tay (verify click vào edit được)
- [x] Mỗi slide có mã giả đều có đủ: pseudocode + sơ đồ + screenshot
- [x] Lead đã rehearsal ít nhất 2 lần dưới 20 phút
- [x] Demo flow chạy mượt, không crash
- [x] Backup exe + save file mẫu đã chuẩn bị sẵn

---