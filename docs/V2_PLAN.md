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
| #35 = A/B test 2 phương án | ✅ 11/06 | Làm CẢ parallax layers (PA-1, chắc ăn) + image-to-video 24 frame (PA-2, trần đẹp cao hơn nhưng rủi ro melt/seam). Wire qua flag toggle, so trực tiếp trong game, giữ cái đẹp hơn. |
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
| 27 | Wire moveSfx + thêm undoSfx, hintSfx, tickSfx, backSfx | 30 phút | 🤖 + 👤🎨 | None | ⏸️ Defer (chờ .wav) |
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
| 31 | RoundedRectangleShape + hover glow | 2 giờ | 🤖 | Low | ⏳ Pending |
| 32 | Particle confetti on win (DIY VertexArray) | 3 giờ | 🤖 | Low | ⏳ Pending |
| 33 | Fragment shader menu glow + victory shockwave | 4 giờ | 🤖 | Medium | ⏳ Pending |
| 36 | 4 mascot riêng cho 4 difficulty + PvP/PvC icon | 45 phút | 🤖 + 👤🎨 | None | ⏳ Pending |
| 37 | Modern input field (cursor blink + glow border) | 1 giờ | 🤖 | None | ⏳ Pending |
| 34 | Character select screen 4-5 model | 3 giờ | 🤖 + 👤🎨 | Low | ⏳ Pending |
| 35 | Animated background — A/B test: parallax layers (PA-1) vs video 24-frame (PA-2), giữ cái đẹp hơn | 4 giờ | 🤖 + 👤🎨 | **Medium-high** | ⏳ Pending |

**Owner legend:**
- 🤖 = Claude code độc lập
- 👤🎨 = User chuẩn bị asset (PNG/sound)
- 👤🧪 = User test thực tế (UI/UX feedback)

---

## 5. Scout Findings (SFML Capabilities)

Research session 01/06/2026 — agent scout 6 topic.

### Topic 1: Spritesheet animation
- **Verdict:** ⚠️ MEDIUM
- **Memory trap:** 60-frame × 1280×720 = **211 MB VRAM** vượt GPU 8192px limit
- **Practical:** Downscale → 24-frame × 960×540 (~50 MB)
- **Code:** `setTextureRect(IntRect(col*W, row*H, W, H))` cycling via sf::Clock
- **Verify:** `sf::Texture::getMaximumSize()` trước khi quyết frame count
- **Reference:** [SFML Wiki Easy Animations](https://github.com/SFML/SFML/wiki/Easy-Animations-With-Spritesheets)

### Topic 2: Rounded rectangle + hover glow
- **Verdict:** ✅ EASY
- **Approach:** SFML Wiki có `RoundedRectangleShape` drop-in (~200 LOC)
- **Hover:** semi-transparent larger rect behind + lerp alpha
- **Reference:** [SFML Wiki RoundedRectangleShape](https://github.com/SFML/SFML/wiki/Source:-Draw-Rounded-Rectangle)

### Topic 3: Particle systems
- **Verdict:** ✅ EASY (DIY, no library)
- **Approach:** `vector<Particle>` + `sf::VertexArray(Triangles, 6*N)` batched
- **Confetti:** 200-500 particles spawn at winning piece, gravity + fade
- **WARNING:** KHÔNG dùng SFML Wiki Particle System (per-pixel `Image.setPixel()` = slow)
- **Note:** Dùng `sf::PrimitiveType::Triangles` (Quads deprecated in 2.6+)

### Topic 4: GLSL shaders
- **Verdict:** ⚠️ MEDIUM (great results, GLSL learning curve)
- **Easy effects:** color tint, single-pass blur, pulsing glow (u_time)
- **Medium effects:** bloom 2-pass, distortion, shockwave
- **Compat:** Dùng `#version 110` cho old Intel driver
- **Check:** `sf::Shader::isAvailable()` at startup
- **Reference:** [Dürrenberger fragment shader tutorial](https://duerrenberger.dev/blog/2021/08/08/basic-fragment-shader-with-sfml/)

### Topic 5: Video/GIF background
- **Verdict:** ❌ HARD — **SKIP**
- **sfeMovie:** Last push 2023-06-12, stale, 22 open issues, broken on SFML 2.6.2
- **Build complexity:** MSYS2 + FFmpeg = 4-8 giờ build hell
- **Verdict:** Spritesheet đạt 80% feel với 5% effort

### Topic 6: Character select pattern
- **Verdict:** ✅ EASY
- **Pattern:** `vector<CharacterCard>` + `hoveredIdx`/`selectedIdx` + lerp scale on hover
- **No canonical SFML demo** — build from scratch (standard pattern)
- **Tricks:** Tween scale, slide-in name label, mascot idle animation (sin bob)

### Top Recommendations (sprint order)
1. **#31 RoundedRectangleShape** — Immediate UI uplift, zero risk
2. **#32 Confetti particles** — High emotional payoff, demo gold
3. **#33 Fragment shader glow** — Biggest wow per LOC
4. **#34 Character select** — Replayability
5. **#35 Spritesheet bg** — Final polish (memory math caveat)

---

## 6. Asset Requirements (User Prep)

### Trước Sprint 1 (defer #27)
| Asset | Spec | Source gợi ý |
|-------|------|--------------|
| 4 sound .wav | 1-2 giây mỗi cái: undo, hint, tick, back | [freesound.org](https://freesound.org), [zapsplat.com](https://zapsplat.com), Suno/ElevenLabs AI |

### Trước Sprint 4 (full asset prep, ~10 giờ)
| Asset | Số lượng | Spec | Difficulty |
|-------|---------|------|---|
| Mascot 4 difficulty | 4 PNG | 200×400, transparent | Easy (AI gen) |
| PvP/PvC icon | 2 PNG | 120×120, transparent | Easy |
| Character portrait | 4-5 PNG | 250×400, transparent | Medium |
| Background spritesheet | 24 PNG | 960×540 mỗi frame, hoặc 1 atlas 5760×2160 | **Hardest** |
| Confetti texture | 1 PNG | 32×32 | Trivial |

→ **Sẽ tạo `docs/V2_ASSETS_PROMPTS.md`** với prompt cụ thể cho từng asset trước Sprint 4.

---

## 7. Workflow per Session

### 7.1 Per-task workflow

```
1. User mở VS, mở project
2. User báo Claude: "Bắt đầu task #XX"
3. 🤖 Claude code → build verify
4. 🧪 User chạy game, test feature
5. 🤝 User feedback (nếu adjust)
6. 🤖 Claude adjust nếu cần
7. ✅ Mark task completed (via TaskUpdate)
8. 🤝 Quyết định commit hay không (User explicit yêu cầu)
```

### 7.2 🔴 Sprint Completion Protocol (STANDING RULE)

**Khi hoàn thành bất kỳ Sprint nào → Claude TỰ ĐỘNG update files, KHÔNG cần User nhắc.**

**Sprint completion criteria:**
- Sprint 1 = #21 ✅ + #22 ✅ + #23 ✅ (#27 defer OK)
- Sprint 2 = #24 ✅ + #25 ✅ + #26 ✅
- Sprint 3 = #28 ✅ + #29 ✅ + #30 ✅
- Sprint 4 = #31 → #37 all ✅

**Auto-update CHỈ 3 file (rule chốt từ 09/06/2026):**

| Order | File | Update content |
|-------|------|---------------|
| 1 | `CLAUDE.md` | Tạo Section 8d/8e/... — sprint summary, files changed, decisions mới |
| 2 | `docs/HANDOVER.md` | Thêm subsection trong Section M — chi tiết session log |
| 3 | `docs/V2_PLAN.md` | Section 11 (check ✅) + Section 4 status + footer last-updated |

> ⛔ **KHÔNG check / KHÔNG update các file md khác** (build_guide.md, ASSETS_PLAN.md, ARCHITECTURE.md, SETUP.md, PRESENTATION_*.md, README.md, GitCommand.md, v.v.) **trừ khi User explicit yêu cầu.**

**Sau update, REPORT cho User:**
- Sprint X complete (task list)
- File code thay đổi
- Build status (pass/fail)
- File .md đã update (CHỈ 3 file trên)
- Next sprint preview

**Quan trọng:** KHÔNG commit tự động — chỉ update md. Commit chỉ khi User explicit yêu cầu "commit" hay "push".

### 7.3 Đặc biệt cho task 🎨 asset

- User cung cấp asset path TRƯỚC khi Claude code
- HOẶC Claude code stub với placeholder color, User wire asset sau

---

## 8. Critical Gotchas / Risks

| Risk | Phase | Mitigation |
|------|-------|---|
| Spritesheet memory blow-up (>200 MB VRAM) | #35 | Downscale frame size, check `getMaximumSize()` trước |
| VertexArray thiếu = 1000 draw calls/frame stutter | #32 | Batched VertexArray bắt buộc |
| GLSL không compile trên old Intel driver | #33 | `#version 110` + `sf::Shader::isAvailable()` check |
| Save format change breaks old saves | #29 | KHÔNG đổi format (vẫn text). Chỉ unlock limit + scan. |
| Loser→X logic break bot.cpp | #23 | Dùng hướng A (swap UI), không động vào bot logic |
| Asset chậm trễ block code | #36, #34, #35 | Code với placeholder color trước, wire asset sau |

---

## 9. Quick Reference

### Build commands
```powershell
$msbuild = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
$proj = "D:\HCMUS\Programming C++\TTT\Caro_Project\Caro_Game_Group_1\CaroGame\CaroGame.vcxproj"
& $msbuild $proj /p:Configuration=Debug /p:Platform=x64 /nologo /verbosity:minimal
```

### Key file paths
- Source: `src/` (10 .cpp file)
- Headers: `include/` (9 .h file)
- Assets: `assets/textures/`, `assets/sounds/`, `assets/fonts/`
- Saves: `saves/`
- Exe: `CaroGame/x64/Debug/CaroGame.exe`
- Project: `CaroGame/CaroGame.sln`

### Important code locations (current state)
| File | Key content |
|------|-------------|
| `include/game_types.h` | `GameState`, `GameResources`, constants (`MAX_SAVE_FILES=12`, `BOARD_SIZE=15`) |
| `src/board.cpp:48-63` | `boardResetRound()` — Phase 2 logic sẽ đụng |
| `src/bot.cpp:23-27` | `DIRS[4][2]`, `inBounds` — Phase 1.2 sẽ extract |
| `src/render.cpp` | ~900 LOC, file lớn nhất, hầu hết Phase 6 đụng đây |
| `src/menu.cpp` | ~1300 LOC, game loop dispatcher, Phase 3 đụng đây |
| `src/save_load.cpp` | Phase 5 đụng đây |

---

## 10. Decision Log (Rationale)

### Q: Tại sao không đổi sang Unity/Unreal/Godot?
A: Stardew Valley (XNA), Celeste (XNA), Hades (custom 2D) — đều dùng engine 2D đơn giản. "AAA look" là do artwork chất lượng cao, không phải engine. Đổi engine = viết lại 100% code = không đáng cho Caro game.

### Q: Tại sao skip binary save?
A: File save < 500 bytes (15×15 board + minimal metadata). Binary không nhanh hơn đáng kể. Pedagogically OK nhưng practical value low. Tập trung vào unordered_map search + scrollable list cải thiện UX nhiều hơn.

### Q: Tại sao hướng A cho Phase 2 (loser→X)?
A: Hướng B (rotate symbol thật) đụng bot.cpp (`botPlayer = -1/+1`), render (X/O texture), save/load (player[i].symbol), undo (moveHistory). Hướng A chỉ swap mascot+name+icon ở UI layer — code logic giữ nguyên. Ít risk hơn 5x.

### Q: Tại sao spritesheet không phải video?
A: Scout xác nhận sfeMovie chết từ 2023, build hell trên Windows. Spritesheet (24-frame painterly loop) đạt 80% live-wallpaper feel với 5% effort của video pipeline.

### Q: Tại sao Sprint 4 làm sau cùng?
A: Group A-C là "fix + cleanup" — foundation. Group D là "visual upgrade" — phải có foundation trước. Ví dụ: utils.h (#22) dùng được trong shader code. Navigation cleanup (#24-26) giúp character select (#34) integrate dễ hơn.

---

## 11. Progress Tracker

### Sprint 1 ✅ COMPLETE [3/3 tasks, 80 phút thực — defer #27]
- [x] #21 — Xóa board.png unused ✅ 09/06/2026
- [x] #22 — Tạo utils.h DRY ✅ 09/06/2026
- [x] #23 — Logic loser→X (hướng A swap UI) ✅ 09/06/2026
- [x] **BONUS** — Default initializers cho POD struct (silence 13 C26495 warnings) ✅ 09/06/2026
- [ ] #27 — Sounds ⏸️ DEFER (chờ User cung cấp .wav)

### Sprint 2 [3/3] ✅
- [x] #24 — Back button NewGame flow (Enter Name screen)
- [x] #25 — Exit/Save buttons gameplay
- [x] #26 — ESC menu expand (6 items inline)

### Sprint 3 [3/3] ✅
- [x] #29 — Bỏ MAX_SAVE_FILES + directory_iterator auto-scan ✅ 10/06/2026
- [x] #28 — unordered_map<string, SaveMetadata> query O(1) ✅ 10/06/2026
- [x] #30 — Scrollable list (mouse wheel + W/S + arrows + scrollbar) ✅ 10/06/2026

### Sprint 4 [0/7]
- [ ] #31 — RoundedRectangleShape + hover glow
- [ ] #32 — Particle confetti on win
- [ ] #33 — Fragment shader glow + shockwave
- [ ] #36 — 4 mascot difficulty + PvP/PvC icon
- [ ] #37 — Modern input field
- [ ] #34 — Character select 4-5 model
- [ ] #35 — Spritesheet animated background

---

## 12. Cross-References

| File | Mục đích |
|------|---------|
| `CLAUDE.md` (local) | Project overview + V1 history |
| `docs/HANDOVER.md` (local) | Detailed session log |
| `docs/ARCHITECTURE.md` | Module dependency, data flow |
| `docs/V2_ASSETS_PROMPTS.md` | ✅ Đã tạo 11/06/2026 — prompt AI gen Sprint 4 (roster 6 nhân vật, icons, #35 A/B parallax-vs-video, ki orbs, aura) |
| `docs/V2_PLAN.md` | **YOU ARE HERE** — source of truth cho V2 |

---

## 13. Recovery Protocol (nếu mất session)

Nếu Claude session mới hoặc User mất context:

1. **Đọc CLAUDE.md** — hiểu project overview
2. **Đọc V2_PLAN.md (this file)** — recover full v2 plan
3. **Check task list** (`TaskList` tool) — xem progress hiện tại
4. **Check section 11** — biết task nào đã xong
5. **Resume từ task in_progress / next pending**

**Quan trọng:**
- HANDOVER.md có thể bị mất (local-only) — V2_PLAN.md là backup tracked
- Tasks lưu trong harness — survive session switch
- Code changes đã commit vào git là an toàn nhất

---

**Last updated:** 11/06/2026 — Tạo `V2_ASSETS_PROMPTS.md`; giữ Dragon Ball theme; #35 = A/B test (parallax vs Veo 3.1 video); tách roster anh hùng (#34) vs phản diện Frieza/Cell/Buu/Broly (#36), 1 ảnh/nhân vật; GIỮ quân X/O cũ (bỏ ki-orb); reskin #32/#33 chất DB
**Created by:** Claude (Anthropic) — session với @vanhoangnhatbp1
**Status:** 🚧 V2 IN PROGRESS — Sprint 1 ✅ + Sprint 2 ✅ + Sprint 3 ✅ (#28, #29, #30 done; #27 still defer). Next: Sprint 4 (#31-#37 Visual AAA upgrade) — `include/rounded_rect.h` đã sẵn cho #31.

> 🔴 **STANDING RULE active:** Claude tự động update files .md khi hoàn thành mỗi Sprint. Xem Section 7.2.

