# V2 Assets — AI Generation Prompts (Gemini / Veo)

> 🎨 Prompt để bạn tự gen asset cho Sprint 4. Tạo 11/06/2026.
> ✅ **Mỗi prompt dưới đây đã ghép sẵn hoàn chỉnh** — chỉ cần copy nguyên 1 ô, dán vào Gemini, xong.

---

## 0. Workflow của bạn (3 bước)

```
1. GEN ảnh ở Gemini  →  2. Xóa nền ở remove.bg  →  3. Đưa Claude (Claude resize + đặt tên + wire)
```

- Bạn **chỉ làm bước 1 + 2**. Bước 3 (resize đúng px, đổi tên, bỏ vào game) → **Claude lo**.
- Gen ở **độ phân giải cao nhất có thể**, đừng tự thu nhỏ (resize nhỏ làm mất nét — để Claude resize lúc cuối).
- Video nền (#35) dùng **Veo** — xem Section 6.

### ⚠️ 3 quy tắc vàng khi dùng Gemini (khác Midjourney/SD)

| Vấn đề | Midjourney/SD | **Gemini (bạn dùng cái này)** |
|--------|---------------|-------------------------------|
| **Nền trong suốt** | Xuất PNG alpha được | ❌ KHÔNG xuất alpha. → Xin **"nền trắng trơn"**, rồi remove.bg tách. (Các prompt dưới đã ghi sẵn nền trắng) |
| **Negative prompt** (thứ không muốn) | Có ô riêng để dán | ❌ KHÔNG có ô riêng. → Viết thẳng câu **"Do not include..."** vào cuối prompt (đã ghi sẵn) |
| **Tỉ lệ ảnh** | Có tham số `--ar` | Ghi bằng lời: "tall vertical portrait", "square", "wide horizontal" (đã ghi sẵn) |

> 💡 Tức là: **bạn không cần hiểu "style suffix" hay "negative prompt" gì cả** — tôi đã gộp hết vào từng prompt hoàn chỉnh bên dưới. Cứ copy nguyên ô là được.

---

## 1. Style chung (tham khảo — KHÔNG cần làm gì)

Mọi prompt dưới đây đã tự động bám phong cách này để asset đồng bộ với game:
- **Phong cảnh** (`background.png`): anime painterly, trời xanh, mây trắng, núi, đồng cỏ — kiểu Genshin/Makoto Shinkai.
- **Nhân vật** (Goku/Vegeta sẵn có): **Dragon Ball Z cel-shaded**, viền đen đậm, màu phẳng rực, full body.

### Palette màu (để asset hợp tông)
- Cam Goku (X / player 1): `#E8702A` → `#D9480F`
- Xanh Vegeta (O / player 2): `#2C5AA0` → `#1E3A6E`
- Vàng/gold UI: `#F2C14E` → `#E0A82E`
- Trời xanh: `#3DA9F5` · Cỏ xanh lá: `#7BC043`

---

## 2. NHÓM A — Nhân vật (Anh hùng + Phản diện)

> 🎮 **Thiết kế (chốt 11/06):** 2 pool nhân vật **TÁCH BẠCH, không trùng nhau**:
> - **Anh hùng** (2A) → **người chơi chọn** ở màn character select (#34). Vào game, panel người chơi hiện nhân vật đã chọn.
> - **Phản diện** (2B) → **đại diện độ khó bot** (#36), người chơi KHÔNG chọn.
>
> Khi chơi:
> - **PvP** → 2 panel = 2 anh hùng (mỗi người chọn 1).
> - **PvC** → panel bạn = anh hùng đã chọn; panel đối thủ = **phản diện theo độ khó** (Easy=Frieza … Expert=Broly).

> 🏆 **Về ảnh thắng/thua — KHÔNG cần gen thêm.** Mỗi nhân vật chỉ cần **1 ảnh** (tư thế đứng). Thắng/thua xử lý hoàn toàn bằng:
> - Banner sẵn có `banner_winner.png` / `banner_defeat.png` đè lên panel.
> - Hiệu ứng code Sprint 4: winner nảy + aura sáng + confetti; loser bị **làm tối/xám** (color tint).
>
> → Không gen 3 tư thế/nhân vật (vừa tốn, vừa khó đồng nhất khi AI gen). Chỉ 1 ảnh/nhân vật.

> 📐 Tất cả gen dạng **dọc, toàn thân** (tall vertical full-body), nền trắng.

### 2A — Anh hùng (người chơi chọn — #34)

> ✅ **Goku + Vegeta đã có sẵn** (tái dùng) → chỉ gen thêm **4 anh hùng** dưới đây. Tổng pool chọn = 6.

**[H1] Gohan — half-saiyan trẻ áo tím**
```
A determined young half-saiyan teenager with short spiky black hair, wearing a purple gi with a blue undershirt, confident heroic stance, in the exact art style of Dragon Ball Z anime by Akira Toriyama and Toei Animation, cel-shaded, bold black outlines, vibrant saturated flat colors with clean cel shading, dynamic full-body pose, tall vertical portrait, full body visible from head to toe, single character only on a plain solid white background. Do not include any scenery, ground, shadow, text, watermark, logo, or border.
```
> Nếu bị từ chối: *"A Gohan-style young saiyan fighter, purple gi, black hair"*.

**[H2] Piccolo — chiến binh Namek áo choàng**
```
A tall green-skinned Namekian warrior with pointed ears and antennae, wearing a purple gi with a white cape and white turban, arms crossed, calm serious expression, in the exact art style of Dragon Ball Z anime by Akira Toriyama and Toei Animation, cel-shaded, bold black outlines, vibrant saturated flat colors with clean cel shading, dynamic full-body pose, tall vertical portrait, full body visible from head to toe, single character only on a plain solid white background. Do not include any scenery, ground, shadow, text, watermark, logo, or border.
```
> Nếu bị từ chối: *"A Piccolo-style Namekian warrior, green skin, white cape and turban"*.

**[H3] Trunks — kiếm sĩ tóc lavender**
```
A cool young swordsman with lavender-purple hair, wearing a blue jacket, holding a sword, serious calm heroic expression, in the exact art style of Dragon Ball Z anime by Akira Toriyama and Toei Animation, cel-shaded, bold black outlines, vibrant saturated flat colors with clean cel shading, dynamic full-body pose, tall vertical portrait, full body visible from head to toe, single character only on a plain solid white background. Do not include any scenery, ground, shadow, text, watermark, logo, or border.
```
> Nếu bị từ chối: *"A Trunks-style swordsman, lavender hair, blue jacket, sword"*.

**[H4] Krillin — võ sĩ hói thân thiện**
```
A short friendly bald martial artist with six dots on his forehead, wearing an orange gi with a blue belt, confident stance, gentle smile, in the exact art style of Dragon Ball Z anime by Akira Toriyama and Toei Animation, cel-shaded, bold black outlines, vibrant saturated flat colors with clean cel shading, dynamic full-body pose, tall vertical portrait, full body visible from head to toe, single character only on a plain solid white background. Do not include any scenery, ground, shadow, text, watermark, logo, or border.
```
> Nếu bị từ chối: *"A Krillin-style bald martial artist in an orange dogi, friendly"*.

### 2B — Phản diện (độ khó bot — #36)

> 4 trùm theo sức mạnh tăng dần. Map: **Easy → Frieza · Medium → Cell · Hard → Majin Buu · Expert → Broly.**
> ⚠️ AI hay từ chối tên trùm có bản quyền → dùng phần *"Nếu bị từ chối"* (tả đặc điểm thay vì tên).

**[V1] EASY — Frieza (bạo chúa ngoài hành tinh)**
```
A sleek sinister alien emperor with smooth white and purple bio-armor skin, large dark eyes, a cruel confident smirk, slender body with a long tail, elegant menacing pose, in the exact art style of Dragon Ball Z anime by Akira Toriyama and Toei Animation, cel-shaded, bold black outlines, vibrant saturated flat colors with clean cel shading, dynamic full-body pose, tall vertical portrait, full body visible from head to toe, single character only on a plain solid white background. Do not include any scenery, ground, shadow, text, watermark, logo, or border.
```
> Nếu bị từ chối: *"A Frieza-style alien emperor villain, white and purple skin, cruel smirk, long tail"*.

**[V2] MEDIUM — Cell (bio-android côn trùng)**
```
A tall insectoid bio-android villain with a green and black spotted exoskeleton, dark crystalline wings, glowing pink and orange accents, a confident menacing grin, perfect form, in the exact art style of Dragon Ball Z anime by Akira Toriyama and Toei Animation, cel-shaded, bold black outlines, vibrant saturated flat colors with clean cel shading, dynamic full-body pose, tall vertical portrait, full body visible from head to toe, single character only on a plain solid white background. Do not include any scenery, ground, shadow, text, watermark, logo, or border.
```
> Nếu bị từ chối: *"A Cell-style insectoid bio-android villain, green and black exoskeleton, wings"*.

**[V3] HARD — Majin Buu (quái vật hồng)**
```
A monstrous muscular pink magical creature with a head tentacle, wearing white villain pants with a black and gold belt, bare pink torso, a wide sinister grin, evil majin demon, in the exact art style of Dragon Ball Z anime by Akira Toriyama and Toei Animation, cel-shaded, bold black outlines, vibrant saturated flat colors with clean cel shading, dynamic full-body pose, tall vertical portrait, full body visible from head to toe, single character only on a plain solid white background. Do not include any scenery, ground, shadow, text, watermark, logo, or border.
```
> Nếu bị từ chối: *"A Majin Buu-style pink demon monster, muscular, head tentacle, evil grin"*.

**[V4] EXPERT — Broly (Super Saiyan huyền thoại)**
```
A gigantic hyper-muscular legendary super saiyan berserker with glowing spiky green hair, a fierce green energy aura, rage-filled white eyes, overwhelming unstoppable power, menacing pose, in the exact art style of Dragon Ball Z anime by Akira Toriyama and Toei Animation, cel-shaded, bold black outlines, vibrant saturated flat colors with clean cel shading, dynamic full-body pose, tall vertical portrait, full body visible from head to toe, single character only on a plain solid white background. Do not include any scenery, ground, shadow, text, watermark, logo, or border.
```
> Nếu bị từ chối: *"A Broly-style legendary super saiyan, spiky green hair, massive muscular body, green aura"*.

---


## 3. NHÓM C — Mode Icons (màn chọn PvP / PvC)

> Gen dạng **vuông** (square), icon phẳng.

**[Icon PvP]**
```
A clean flat game icon: two stylized anime fighters facing each other ready to battle, an orange fighter on the left versus a blue fighter on the right, an energy spark between them, inside a circular gold-rimmed badge, vibrant anime style, simple bold shapes, centered on a plain solid white background. Do not include any text, watermark, or border outside the badge.
```

**[Icon PvC]**
```
A clean flat game icon: a human fighter facing a glowing robot AI opponent, an orange human silhouette on the left versus a blue cyber-robot head with circuit glow on the right, inside a circular gold-rimmed badge, vibrant anime style, simple bold shapes, centered on a plain solid white background. Do not include any text, watermark, or border outside the badge.
```

---

## 4. NHÓM D — Aura + Confetti (tùy chọn)

**[Aura glow]** — đặt sau mascot cho hiệu ứng "thở" (additive). *Tùy chọn, Claude tự làm bằng shader cũng được.*
```
A soft radial golden energy aura glow, a smooth gradient from a bright golden center fading to black at the edges, ethereal light only, no shapes, square image on a solid black background. Do not include any text or objects.
```
> ⚠️ Aura dùng **nền ĐEN** (không phải trắng) — vì sẽ blend kiểu "additive" (đen = trong suốt).

**[Confetti]** — hạt pháo khi thắng. *Tùy chọn, code chạy không cần.*
```
A single small soft glowing white four-pointed sparkle star, soft edges, simple, square image centered on a solid black background. Do not include any text or objects.
```
> ⚠️ Confetti cũng dùng **nền ĐEN**.

---

## 5. NHÓM E — Animated Background (#35) — A/B test 2 phương án

> 🎯 Chốt: làm **CẢ HAI**, so trực tiếp trong game, giữ cái đẹp hơn. Claude wire qua 1 flag để bật/tắt.

### 5. Phương án 1 — Parallax (Gemini gen 2 dải ảnh)

Giữ `background.png` làm nền tĩnh, thêm 2 lớp chuyển động + bụi ki vàng (Claude code phần động).

> ⚠️ 2 dải này **KHÔNG dùng nền trắng** (vì mây trắng / không tách được). Dùng **nền xanh lá chuối (green screen)** để remove.bg tách sạch.

**[Dải mây]** (gen dạng **wide horizontal**)
```
A long horizontal strip of fluffy white cumulus anime clouds, painterly anime style, clouds of varying sizes spread apart with gaps between them, wide panoramic horizontal image, clouds placed on a solid bright chroma-green background. Do not include any blue sky, scenery, text, or watermark.
```

**[Dải cỏ tiền cảnh]** (gen dạng **wide horizontal**)
```
A horizontal strip of lush green grass blades and small white and pink wildflowers seen from ground level, anime painterly style, vibrant saturated greens, solid grass along the bottom edge with an irregular grass-blade silhouette at the top, wide panoramic horizontal image, placed on a solid bright magenta background above the grass line. Do not include any sky, mountains, scenery, text, or watermark.
```
> Cỏ xanh → nền **magenta** (hồng cánh sen) để tách; mây trắng → nền **green**. Tránh nền trùng màu chủ thể.

### 5B. Phương án 2 — Veo 3.1 (image-to-video → 24 frame)

**Tool khuyên dùng:** **Veo 3.1 Fast** (model `veo-3.1-fast`) qua **Google Flow** hoặc **Google AI Studio**. (Hoặc **Gemini Omni / Omni Flash** trên gemini.google.com nếu tiện hơn — cũng giữ nét cel-shaded tốt.)

> ⭐ **Vì sao Veo 3.1 hợp task này** (info từ Gemini Pro 3): nó hỗ trợ **Seamless Loop** sẵn (giải quyết đúng nỗi lo điểm nối loop bị giật) và cho **upload tới 3 ảnh tham chiếu** để giữ đúng style. Đây là lý do PA-2 giờ bớt rủi ro hẳn so với lúc đầu.

1. Mở **Google Flow** (thường có sẵn trong gói Pro/Ultra) **hoặc AI Studio** (cần API key trả phí), chọn model **`veo-3.1-fast`**.
2. ⭐ **LOOP liền mạch:** AI Studio có **2 ô ảnh = frame ĐẦU và frame CUỐI** (2 icon ảnh, dấu `↔` ở giữa). → Up **`background.png` vào CẢ HAI ô** để video bắt đầu = kết thúc cùng 1 khung → loop khớp, không giật. (Flow có thể có toggle "Loop" riêng — bật nếu thấy.)
3. **Settings:** Aspect **16:9**, Resolution **720p**, 24 fps, duration 2-4s. (Number of results 2-3 để chọn bản ít melt nhất nếu được.)
4. Prompt (bản chống "melt" — yêu cầu giữ nguyên ảnh gốc, chỉ thêm chuyển động chậm):
```
Animate this exact anime landscape painting as a subtle living wallpaper. Keep the original art style, colors, and every detail unchanged — do not redraw, morph, or distort the mountains, trees, rocks, or path. Only add very slow, gentle motion: clouds drift slowly to the left, grass and tree leaves sway softly in a light breeze, faint sunlight shimmer. Static camera — no zoom, no pan, no rotation. Smooth seamless loop. Calm and peaceful.
```

> 💬 **Gemini chat (Omni):** không có ô chỉnh duration/fps — clip tự ra ~8s, Claude resample về 24 frame nên không cần lo. Chỉ cần set **16:9** + up `background.png`.
5. Xuất clip **2–4 giây**.
6. ✅ **Xem kỹ trước khi đưa Claude:** núi/cây/đá có bị "melt" (chảy/biến dạng) không? Điểm nối đầu-cuối loop có giật không? Nếu có → gen lại, đừng dùng bản lỗi.
7. Đưa Claude **file video** (mp4) — Claude tự chạy `ffmpeg` cắt 24 frame + resize 960×540 + ghép spritesheet (hoặc để 24 file rời).

> 📐 Claude lo phần `ffmpeg` + ép 960×540 (an toàn VRAM) + nạp vào game. Bạn chỉ cần đưa file mp4.

> 🛟 **Fallback nếu cả PA-1 lẫn PA-2 đều không ưng:** giữ `background.png` tĩnh, để **shader #33** tạo hiệu ứng cuộn màu / hào quang động đè lên nền — nhẹ máy, không lo lỗi chuyển động. (Mình vẫn có sẵn #33 trong Sprint 4 nên đây là phương án dự phòng "miễn phí".)

---

## 6. Bảng giao nộp - không cần chỉnh kích thước, nhờ AI chỉnh sau

| Asset | Bạn gen | Claude xuất ra | Cho task |
|-------|---------|----------------|----------|
| 4 anh hùng (2A) | 4 ảnh dọc, nền trắng | chân dung 250×400 (#34) + mascot 200×405 (panel in-game) | #34 |
| 4 phản diện (2B) | 4 ảnh dọc, nền trắng | mascot 200×405 (panel bot theo độ khó) | #36 |
| Icon PvP / PvC | 2 ảnh vuông, nền trắng | `icon_pvp.png` / `icon_pvc.png` 120×120 | #36 |
| Dải mây / cỏ (PA-1) | 2 ảnh ngang, nền chroma | `clouds_layer.png` / `fg_grass.png` | #35 (PA-1) |
| Video nền (PA-2) | 1 clip mp4 (Veo) | 24 frame 960×540 | #35 (PA-2) |
| Aura / Confetti | nền ĐEN | `aura_glow.png` / `confetti.png` | #33 / #32 |

> Quân **X / O giữ nguyên** bản cũ — không gen.
> Bạn chỉ cần: **gen + remove.bg** (trừ aura/confetti/video — không cần remove.bg). Rồi đưa Claude.

---

## 8. Thứ tự ưu tiên gen

1. 🥇 **4 anh hùng (2A)** — unlock #34 (character select).
2. 🥈 **4 phản diện (2B)** — unlock #36 (mascot độ khó bot).
3. 🥉 **2 mode icon** — nhanh.
4. **2 dải parallax (PA-1)** — unlock #35 chắc ăn.
5. **Video Veo (PA-2)** — thử để so A/B.
6. **Aura / confetti** — tùy chọn.

