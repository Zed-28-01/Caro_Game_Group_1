# KẾ HOẠCH TÀI NGUYÊN (ASSETS) & TINH CHỈNH UI/UX
**Dự án:** Caro Game — Nhóm 1 · HCMUS
**Deadline tổng:** 26/05/2026 (Chủ nhật)
**Phụ trách:** Nhóm việc 3 (1 thành viên + Project Lead)

> 🎯 **MỤC TIÊU THẨM MỸ:** Giao diện game phải **đồng bộ theme** từ background → font → button → quân cờ → nhân vật phụ — giống phong cách của game Caro Doraemon, Caro Space, hoặc Caro Forest tham khảo. KHÔNG đi theo hướng pixel-art vẽ tay vì SFML cho phép dùng asset chất lượng cao.

---

## MỤC LỤC
1. [Chiến lược chọn Theme đồng bộ](#1-chiến-lược-chọn-theme-đồng-bộ)
2. [Danh mục Font chữ](#2-danh-mục-font-chữ)
3. [Danh mục Âm thanh (Sounds)](#3-danh-mục-âm-thanh-sounds)
4. [Danh mục Hình ảnh/Textures](#4-danh-mục-hình-ảnhtextures)
5. [Công cụ AI tạo Asset tuỳ chỉnh](#5-công-cụ-ai-tạo-asset-tuỳ-chỉnh)
6. [Kế hoạch Tinh chỉnh UI lần cuối](#6-kế-hoạch-tinh-chỉnh-ui-lần-cuối)
7. [Cấu trúc Thư mục & Tracking](#7-cấu-trúc-thư-mục--tracking)

---

## 1. CHIẾN LƯỢC CHỌN THEME ĐỒNG BỘ

### 1.1. Vì sao phải chọn theme TRƯỚC khi tải asset?

**Kết luận:** Phải chọn 1 theme cố định → tải toàn bộ asset **từ CÙNG 1 PACK / CÙNG 1 ARTIST** thay vì gom từ nhiều nguồn rời rạc.

### 1.2. 3 lựa chọn theme đề xuất

| Theme | Phong cách | Phù hợp với | Độ khó tìm asset |
|-------|-----------|-------------|------------------|
| **A. Cartoon/Anime** | Như Doraemon — màu pastel, nhân vật chibi | Khán giả trẻ, thân thiện | ⭐⭐ Dễ |
| **B. Space/Sci-fi** | Như ảnh 5 — neon, vũ trụ, robot | Nhìn hiện đại, "ngầu" | ⭐⭐⭐ Trung bình |
| **C. Fantasy/Medieval** | Hiệp sĩ vs quái, gỗ + đá | Cổ điển, hợp game cờ | ⭐⭐ Dễ |

> 💡 **Khuyến nghị:** Chọn **Theme A (Cartoon)** vì:
> - Nhiều pack free trên Craftpix/itch.io
> - AI tools tạo asset cartoon rất tốt
> - Font cartoon đa dạng (Fredoka, Luckiest Guy...)
> - Khán giả hội đồng dễ tiếp nhận

### 1.3. Nguyên tắc đồng bộ (CỰC KỲ QUAN TRỌNG)
- **1 theme = 1 nguồn chính** (vd: tải 80% asset từ Craftpix bundle "Cartoon Game UI")
- **Asset thiếu → dùng AI tạo bổ sung** với prompt mô tả CHÍNH XÁC style của pack chính
- **Không trộn:** Đừng lấy background pixel + button cartoon — sẽ "lệch tông" ngay
- **Test trước:** Đặt 2-3 asset cạnh nhau trên Figma/PowerPoint xem có hợp không trước khi import vào code

---

## 2. DANH MỤC FONT CHỮ

### 2.1. Tổng quan
- **Số font cần tải:** 2 file `.ttf`
  - `assets/fonts/main.ttf` — Body text (menu, panel, settings)
  - `assets/fonts/title.ttf` — Tiêu đề lớn (CARO GAME, WIN!, ...)
- **Định dạng:** `.ttf` (TrueType) — SFML hỗ trợ trực tiếp qua `sf::Font::loadFromFile`
- **Tránh:** `.otf` (đôi khi gây lỗi SFML), font có license thương mại

### 2.2. Đề xuất font theo theme

#### 🎨 Theme A — Cartoon/Anime (như Doraemon)
| Vai trò | Font đề xuất | Đặc điểm |
|---------|--------------|----------|
| `title.ttf` | **Luckiest Guy** | Bo tròn, đậm, vui nhộn — giống chữ "CARO WITH DORAEMON" |
| `title.ttf` (alt) | **Fredoka One** | Bo tròn, đậm, dễ đọc |
| `main.ttf` | **Fredoka** (Regular) | Cùng họ với title, đồng bộ |
| `main.ttf` (alt) | **Comic Neue** | Phong cách comic nhẹ nhàng |

#### 🚀 Theme B — Space/Sci-fi
| Vai trò | Font đề xuất | Đặc điểm |
|---------|--------------|----------|
| `title.ttf` | **Orbitron** | Geometric, hi-tech |
| `title.ttf` (alt) | **Audiowide** | Neon, sci-fi |
| `main.ttf` | **Exo 2** | Hiện đại, dễ đọc |

#### ⚔️ Theme C — Fantasy/Medieval
| Vai trò | Font đề xuất | Đặc điểm |
|---------|--------------|----------|
| `title.ttf` | **MedievalSharp** | Cổ điển, kiếm hiệp |
| `main.ttf` | **Cinzel** | Trang trọng, serif |

### 2.3. Nguồn tải font — Sắp xếp theo độ ưu tiên

#### 🥇 Ưu tiên 1: Google Fonts
- **URL:** https://fonts.google.com
- **Vì sao đứng đầu:**
  - ✅ 100% miễn phí, license SIL Open Font / Apache → dùng thương mại OK
  - ✅ Tải `.ttf` trực tiếp, không cần convert
  - ✅ Chất lượng đồng đều, có preview ngay
  - ✅ Có TẤT CẢ font đề xuất ở mục 2.2 (Luckiest Guy, Fredoka, Orbitron, Cinzel)
- **Cách dùng:** Search → click font → "Download family" → giải nén lấy file `.ttf`

#### 🥈 Ưu tiên 2: DaFont
- **URL:** https://www.dafont.com
- **Vì sao chọn:**
  - ✅ Kho font theme game khổng lồ, có category "Cartoon", "Pixel", "Sci-fi"
  - ⚠️ Cần KIỂM TRA license từng font (cột "License" trên trang) — chọn "100% Free" hoặc "Public Domain"
  - ✅ Tải trực tiếp `.ttf`
- **Filter hay:** https://www.dafont.com/theme.php?cat=503 (Cartoon)

#### 🥉 Ưu tiên 3: 1001fonts.com
- **URL:** https://www.1001fonts.com
- **Vì sao chọn:**
  - ✅ Có category dành riêng "Games" — đã filter sẵn font phù hợp
  - ✅ License rõ ràng ngay trên trang chi tiết
- **Filter hay:** https://www.1001fonts.com/games-fonts.html

#### Phương án 4: FontSpace
- **URL:** https://www.fontspace.com
- **Đặc điểm:** Lọc theo "Commercial Free Use" hoặc "CC license"
- Dùng khi 3 nguồn trên không có font ưng ý

### 2.4. Lưu ý kỹ thuật
- **Test font hỗ trợ tiếng Việt:** Mở file `.ttf` → gõ thử "Người chơi 1: Nguyễn Văn A" → kiểm tra dấu có hiển thị không
  - Nếu KHÔNG có dấu Việt → chỉ dùng cho TITLE (tiếng Anh), main text dùng font khác
  - **Font có dấu Việt tốt:** Fredoka, Roboto, Be Vietnam Pro, Inter
- **Đổi tên file ngắn gọn:** `LuckiestGuy-Regular.ttf` → đổi thành `title.ttf` để khớp code
- **Kích thước file:** Font tốt thường dưới 200KB, nếu file > 500KB là có nhiều glyph không cần thiết

---

## 3. DANH MỤC ÂM THANH (SOUNDS)

### 3.1. Tổng quan
- **Tổng số file cần tải:** 6 file (1 BGM + 5 SFX)
- **Định dạng chuẩn:**
  - **BGM:** `.ogg` — SFML stream được, nhẹ
  - **SFX:** `.wav` — load nhanh, không decode runtime

### 3.2. Chi tiết từng file

| # | Tên file | Loại | Chức năng | Độ dài | Volume gốc |
|---|----------|------|-----------|--------|------------|
| 1 | `bgm.ogg` | BGM | Nhạc nền menu + gameplay (loop) | 1-3 phút loop | 30-50% |
| 2 | `place.wav` | SFX | Đặt quân X/O xuống bàn | 0.1-0.3s | 80% |
| 3 | `move.wav` | SFX | Di chuyển cursor bằng WASD | 0.05-0.1s | 50% |
| 4 | `win.wav` | SFX | Có người thắng | 1-2s | 100% |
| 5 | `draw.wav` | SFX | Hòa cờ | 0.5-1s | 80% |
| 6 | `menu.wav` | SFX | Click menu/button | 0.1-0.2s | 70% |

### 3.3. Nguồn tải — Sắp xếp lại theo độ ưu tiên

#### 🥇 Ưu tiên 1: Kenney Audio
- **URL:** https://kenney.nl/assets/category:Audio
- **Vì sao ĐỨNG ĐẦU (đẩy lên từ vị trí 3):**
  - ✅ License **CC0** (public domain) — KHÔNG cần ghi credit, KHÔNG có ràng buộc nào
  - ✅ **Đồng bộ theme** — mỗi pack được Kenney tự thiết kế đồng nhất → tất cả SFX trong 1 pack đều "ăn rơ" về tone âm
  - ✅ Pack hay: "Interface Sounds", "Casino Audio", "Sci-Fi Sounds"
  - ✅ Tải nguyên ZIP — có sẵn cả `.wav` và `.ogg`

#### 🥈 Ưu tiên 2: Zapsplat (MỚI THÊM)
- **URL:** https://www.zapsplat.com
- **Vì sao quan trọng:**
  - ✅ **Chất lượng chuyên nghiệp** — SFX được mix bởi sound designer chuyên nghiệp, sạch hơn cộng đồng Freesound nhiều
  - ✅ Có category riêng "Game Sounds" filter chuẩn
  - ⚠️ Cần đăng ký free → tải `.mp3` hoặc `.wav` (account miễn phí giới hạn 100 file/ngày — đủ dùng)
  - ⚠️ Yêu cầu ghi credit "Sound from Zapsplat.com" trong project (chấp nhận được cho đồ án)
- **Link tắt:** https://www.zapsplat.com/sound-effect-category/game-sounds/

#### 🥉 Ưu tiên 3: Mixkit
- **URL:** https://mixkit.co/free-sound-effects/game/
- **Vì sao chọn:**
  - ✅ Không cần đăng ký, tải trực tiếp `.wav`
  - ✅ License Mixkit Free — dùng thương mại miễn phí
  - ⚠️ Số lượng SFX game không nhiều bằng 2 site trên

#### Phương án 4: OpenGameArt — Audio Section
- **URL:** https://opengameart.org/art-search?keys=&field_art_type_tid%5B%5D=13
- **Đặc điểm:** Cộng đồng game dev, chất lượng đa dạng (cần lọc kỹ)
- Dùng khi cần SFX rất đặc thù

#### Phương án 5: Freesound
- **URL:** https://freesound.org
- **Đặc điểm:** Kho khổng lồ nhưng chất lượng không đồng đều, cần đăng ký
- **Demoted vì:** Cần lọc kỹ, nhiều file recording lo-fi không chuyên nghiệp

#### Phương án 6: Pixabay Audio
- **URL:** https://pixabay.com/sound-effects/search/game/
- **Đặc điểm:** Royalty-free, tải trực tiếp
- Dùng khi 5 nguồn trên không có

### 3.4. Từ khóa tìm kiếm

| File | Keywords (Tiếng Anh) |
|------|---------------------|
| `bgm.ogg` | `cute game music loop`, `casual game bgm`, `cartoon background music` |
| `place.wav` | `wood click`, `stone place`, `tap pop`, `chess piece sound` |
| `move.wav` | `ui tick`, `subtle blip`, `menu hover` |
| `win.wav` | `victory fanfare short`, `cartoon win`, `level complete cute` |
| `draw.wav` | `game neutral end`, `soft notification` |
| `menu.wav` | `ui select cartoon`, `button click pop`, `cartoon confirm` |

### 3.5. Lưu ý kỹ thuật
- **Convert format:** Nếu tải `.mp3`, dùng [Audacity](https://www.audacityteam.org/) free → Export sang `.wav` 16-bit
- **Chuẩn hoá volume:** Audacity → Effect → Normalize → -1dB
- **Cắt ngắn nếu cần:** SFX không nên dài quá 2s, BGM cần loop seamless (Audacity → Effect → Crossfade Tracks)

---

## 4. DANH MỤC HÌNH ẢNH/TEXTURES

### 4.1. Tổng quan
- **Số file cần:** 4 file `.png` cốt lõi + (tuỳ chọn) 2 nhân vật mascot
- **Định dạng:** `.png` 32-bit (RGBA — có alpha channel)
- **Cửa sổ game:** 1280×720
- **Ô bàn cờ:** 40×40 pixel

### 4.2. Chi tiết từng texture

| # | Tên file | Kích thước (px) | Chức năng | Yêu cầu thiết kế |
|---|----------|------------------|-----------|------------------|
| 1 | `background.png` | 1280×720 | Nền toàn cửa sổ | Theme cố định, không quá rối, không gradient gây chói |
| 2 | `board.png` | 600×600 | Texture đè bàn cờ | Cùng tông với background, có vân/kết cấu nhẹ |
| 3 | `x_piece.png` | 36×36 | Quân X Player 1 | Màu nổi (đỏ/xanh dương), có viền/glow |
| 4 | `o_piece.png` | 36×36 | Quân O Player 2 / Bot | Màu tương phản với X |
| 5 *(tuỳ chọn)* | `mascot_p1.png` | ~200×400 | Nhân vật trang trí góc trái panel | Như nhân vật bên trái ảnh Doraemon |
| 6 *(tuỳ chọn)* | `mascot_p2.png` | ~200×400 | Nhân vật trang trí góc phải panel | Như nhân vật bên phải ảnh Doraemon |

> 💡 **Mascot là điểm cộng lớn:** Nhìn ảnh Doraemon và Space — 2 nhân vật ở 2 bên panel làm game **sống động** hẳn lên. Đây là điểm khác biệt giữa "đồ án sinh viên" và "game thực sự".

### 4.3. Nguồn tải texture — Sắp xếp lại theo độ ưu tiên

#### 🥇 Ưu tiên 1: Craftpix.net (MỚI THÊM — QUAN TRỌNG NHẤT)
- **URL:** https://craftpix.net/categorys/free/
- **Vì sao ĐỨNG ĐẦU:**
  - ✅ **CHUYÊN DỤNG cho game asset 2D** — đây là điểm khác biệt then chốt với stock photo sites
  - ✅ Bán/tặng theo **BUNDLE THEME** — 1 pack có đủ background + UI + characters + icons **cùng style** → đạt được vẻ đồng bộ như Doraemon
  - ✅ Có section "FREE" — đủ chất lượng cho đồ án
  - ✅ License: Free Asset Use License — dùng được cho đồ án sinh viên
- **Pack đề xuất tải về:**
  - "Free Cartoon Game GUI" — buttons + panels + icons đồng bộ
  - "Free Game Backgrounds" — nhiều theme: forest, space, dungeon
  - "Free Game Characters" — mascot nhân vật chibi
- **Link tắt:**
  - GUI: https://craftpix.net/categorys/gui/?filter_by=featured&show=free
  - Backgrounds: https://craftpix.net/categorys/backgrounds/?show=free
  - Characters: https://craftpix.net/categorys/characters/?show=free

#### 🥈 Ưu tiên 2: Kenney.nl
- **URL:** https://kenney.nl/assets/category:2D
- **Vì sao chọn:**
  - ✅ License **CC0** (mạnh nhất — public domain)
  - ✅ Cũng tổ chức theo bundle theme đồng bộ
  - ✅ Pack hay: "UI Pack", "Background Elements", "Toon Characters 1"
  - ⚠️ Style hơi đơn giản hơn Craftpix nhưng MIỄN PHÍ 100% không phải lo license

#### 🥉 Ưu tiên 3: itch.io — Free Game Assets
- **URL:** https://itch.io/game-assets/free/tag-2d
- **Vì sao chọn:**
  - ✅ Cộng đồng artist độc lập, nhiều bundle theme cực đẹp tặng miễn phí
  - ✅ Filter "Free" + sort "Most Downloaded" → tìm pack chất lượng nhanh
  - ⚠️ Phải kiểm tra license từng pack (artist tự quy định)
- **Mẹo tìm:** Search "complete game UI" hoặc "game art pack"

#### Phương án 4: OpenGameArt — 2D Section
- **URL:** https://opengameart.org/art-search-advanced?field_art_type_tid%5B%5D=9
- **Đặc điểm:** Cộng đồng game dev, miễn phí 100%
- ⚠️ Chất lượng không đồng đều — cần lọc kỹ

#### Phương án 5: Unsplash (MỚI — CHỈ DÙNG CHO BACKGROUND ẢNH THẬT)
- **URL:** https://unsplash.com
- **Vì sao quan trọng:**
  - ✅ Ảnh chụp thật **CHẤT LƯỢNG CAO** (4K, độ phân giải lớn)
  - ✅ Hoàn toàn free, không cần đăng ký
  - ✅ Phù hợp khi muốn background **realistic** (vd: bàn gỗ thật, mây trời thật)
- ⚠️ **LƯU Ý:** Unsplash KHÔNG có UI elements / character — chỉ dùng nó cho background, các phần khác phải lấy từ Craftpix
- **Từ khóa search:**
  - "wooden table top" — nếu muốn background gỗ
  - "starry night sky" — nếu theme Space
  - "japanese garden" — nếu theme Anime

#### Phương án 6: Pexels (MỚI — TƯƠNG TỰ UNSPLASH)
- **URL:** https://www.pexels.com
- **Đặc điểm:** Giống Unsplash, đôi khi có ảnh đẹp hơn
- Dùng khi Unsplash không có

#### Phương án 7: Pixabay
- **URL:** https://pixabay.com
- **Đặc điểm:** Có cả ảnh thật, vector, illustration — đa dạng
- Dùng khi 6 nguồn trên không có

#### Phương án 8: Freepik (DEMOTED — chỉ dùng khi cần)
- **URL:** https://freepik.com
- **Lý do tụt hạng:**
  - ⚠️ Yêu cầu ghi credit phức tạp (link về author)
  - ⚠️ Watermark gói free
  - ⚠️ Không chuyên về game asset — chủ yếu cho thiết kế in ấn
- Chỉ dùng khi tìm icon UI cụ thể không có nơi khác

### 4.4. Từ khóa tìm kiếm cụ thể theo theme

#### Theme Cartoon (như Doraemon)
| Asset | Keywords cho Craftpix/itch.io | Keywords cho Unsplash |
|-------|-------------------------------|-----------------------|
| Background | `cartoon game background`, `cute landscape 2D` | `cartoon sky illustration`, `pastel landscape` |
| Board | `wooden game board cartoon`, `cute boardgame` | `wood texture seamless light` |
| X/O piece | `game tokens cartoon`, `cute X O sprites` | (không có — phải tạo bằng AI hoặc Figma) |
| Mascot | `chibi character free`, `cartoon mascot 2D` | (không phù hợp) |

#### Theme Space
| Asset | Keywords |
|-------|----------|
| Background | `space game background`, `nebula 2D pixel art free` |
| Board | `sci-fi grid panel`, `neon grid game` |
| Mascot | `astronaut sprite`, `robot character 2D` |

### 4.5. Chiến lược đồng bộ theme (BƯỚC THỰC HIỆN)

```
BƯỚC 1: Chọn 1 theme cố định (vd: Cartoon)
   ↓
BƯỚC 2: Vào Craftpix.net section Free → tìm 1 BUNDLE chứa GUI + BG cùng style
   ↓
BƯỚC 3: Tải nguyên bundle (vd: "Free Cartoon GUI Pack")
   ↓
BƯỚC 4: Kiểm tra trong bundle có:
   - Background ✓
   - Buttons (cho menu) ✓
   - Panel/Box (cho player info) ✓
   - Icons (sound on/off, settings...) ✓
   ↓
BƯỚC 5: Asset CÒN THIẾU (X piece, O piece, mascot) → DÙNG AI TẠO MỚI
   với prompt: "in style of [tên pack]" để đồng bộ
   ↓
BƯỚC 6: Test ghép 4-5 asset trên Figma/PPT xem có hợp tông không
```

---

## 5. CÔNG CỤ AI TẠO ASSET TUỲ CHỈNH

> 🤖 **Khi nào cần AI?**
> Khi không tìm được asset đúng theme — đặc biệt là quân X, O, mascot nhân vật phải khớp với background đã chọn. AI giải quyết được vấn đề "style matching" mà các website asset không có.

### 5.1. Công cụ AI đề xuất — Theo độ ưu tiên

#### 🥇 Ưu tiên 1: Leonardo.ai
- **URL:** https://leonardo.ai
- **Vì sao đứng đầu:**
  - ✅ **CHUYÊN GAME ASSET** — có preset "Pixel Art", "Game UI", "RPG Character"
  - ✅ Free 150 token/ngày — đủ tạo ~30 ảnh
  - ✅ Hỗ trợ tạo PNG nền trong suốt (transparent background)
  - ✅ Negative prompt giúp loại bỏ artifact
- **Cách dùng:**
  1. Đăng ký free account
  2. Vào "Image Generation"
  3. Chọn preset "RPG v5" hoặc "Cinematic Kino" tuỳ theme
  4. Bật "Transparent" nếu cần PNG trong suốt
  5. Nhập prompt (xem 5.2)

#### 🥈 Ưu tiên 2: Microsoft Designer / Bing Image Creator
- **URL:** https://designer.microsoft.com/image-creator hoặc https://www.bing.com/images/create
- **Vì sao chọn:**
  - ✅ Free 100% — dùng DALL-E 3 mạnh
  - ✅ Tài khoản Microsoft là đủ
  - ✅ Chất lượng cao, hiểu prompt phức tạp
  - ⚠️ Không xuất transparent PNG (cần remove background sau)

#### 🥉 Ưu tiên 3: ChatGPT Free (DALL-E 3)
- **URL:** https://chat.openai.com (gói free hiện cho 3 ảnh/ngày)
- **Vì sao chọn:**
  - ✅ Có thể "chat" để refine prompt qua nhiều lượt
  - ✅ Chất lượng tương đương Bing
  - ⚠️ Giới hạn ảnh/ngày, đôi khi bị queue

#### Phương án 4: Recraft.ai
- **URL:** https://www.recraft.ai
- **Đặc điểm:**
  - ✅ Chuyên vector/icon — cực hợp tạo X/O piece, icon UI
  - ✅ Free 50 credit/ngày
  - ✅ Xuất SVG được — scale không vỡ
- Dùng cho: icon UI, X/O piece, decorations

#### Phương án 5: Playground.ai
- **URL:** https://playground.com
- **Đặc điểm:** Free 500 ảnh/ngày, Stable Diffusion
- Dùng khi cần số lượng lớn để chọn

#### Phương án 6: Civitai.com
- **URL:** https://civitai.com
- **Đặc điểm:** Có nhiều model fine-tune theo style cụ thể (anime, pixel, fantasy)
- Khó dùng hơn nhưng kết quả style chính xác hơn

### 5.2. Prompt mẫu cho từng asset (COPY-PASTE ĐƯỢC)

#### 🎨 Background (Theme Cartoon — như Doraemon/Forest)
```
A wide cartoon game background, sunset over rolling green hills, 
small cute wooden house on the left, fluffy trees, blue sky with 
soft clouds, vibrant warm colors, 2D illustration style, no characters, 
no text, no UI elements, suitable as game menu background, 16:9 aspect ratio, 
high quality digital art

Negative prompt: text, logo, characters, people, ugly, blurry, low quality, 
watermark, signature
```

#### 🚀 Background (Theme Space)
```
2D game background, deep space nebula with stars and planets, 
two distant planets on left, asteroid belt, neon cyan accents, 
no characters, no UI, suitable for game menu, vibrant cosmic colors, 
digital painting style, 16:9 wide aspect ratio

Negative prompt: text, logo, characters, spaceships, ugly, watermark
```

#### ❌ Quân X (X piece)
```
A bold cartoon X game piece, glowing red color with darker red outline, 
chunky brushstroke style, slight 3D depth, transparent background, 
centered, no shadow, simple clean design, suitable as gomoku game token, 
square aspect ratio, vector-like illustration

Negative prompt: realistic, photo, complex details, background, multiple X, 
text, ugly
```

#### ⭕ Quân O (O piece)
```
A bold cartoon O game piece, glowing deep blue color with darker blue outline, 
chunky brushstroke style, hollow circle (ring shape), slight 3D depth, 
transparent background, centered, clean design, suitable as gomoku game token, 
square aspect ratio

Negative prompt: realistic, photo, filled circle, complex details, background, 
text, multiple circles, ugly
```

#### 🪵 Board texture (gỗ cartoon)
```
Seamless cartoon wooden plank texture for a game board, light warm brown 
wood grain, top-down view, painted style with visible brush strokes, 
clean and simple, no edges visible, tileable, suitable for 5-in-a-row board, 
square aspect ratio

Negative prompt: realistic photo, grid, lines, characters, text, dark, gloomy
```

#### 🧙 Mascot nhân vật trái (kiểu Doraemon ấm áp)
```
A cute chibi cartoon hero character standing facing slightly right, 
holding a small sword, friendly smiling face, samurai inspired warm color outfit 
(white and red), full body shot, transparent background, simple 2D illustration 
in vibrant cartoon style, no shadow on ground

Negative prompt: realistic, anime sketch, 3D render, complex armor, background, 
text, weapons in both hands, mature features
```

#### 👹 Mascot nhân vật phải (kiểu đối thủ)
```
A cute chibi cartoon monster villain standing facing slightly left, 
holding a spiked club, slightly grumpy face, red ogre style with horns, 
full body shot, transparent background, simple 2D illustration in vibrant 
cartoon style matching a hero character, no shadow on ground

Negative prompt: realistic, scary, gore, blood, complex details, background, 
text, dark theme
```

#### 🔘 Button UI (như button "NEW GAME" của Doraemon)
```
A horizontal cartoon game button, light blue rounded rectangle with darker 
blue border, slight 3D depth with soft shadow underneath, clean clickable 
appearance, transparent background, blank center (no text), suitable for 
game menu button, modern cartoon UI style

Negative prompt: realistic, text inside, photo, dark theme, complex pattern, 
multiple buttons
```

### 5.3. Tips để AI trả về kết quả tốt
- **Generate 4 lần / prompt** rồi chọn cái đẹp nhất (đa số AI tự đưa 4 variants)
- **Refine bằng từ khoá thêm:** "highly detailed", "professional quality", "by [tên artist style nổi tiếng]"
- **Style consistency:** Luôn thêm cùng cụm "cartoon illustration style, vibrant colors, clean lines" cho mọi asset trong cùng theme
- **Tham chiếu pack chính:** Nếu tải pack chính từ Craftpix → upload 1 ảnh trong pack lên Leonardo → bật "Image Guidance" → AI sẽ học style của pack
- **Remove background:** Nếu AI ra ảnh có nền:
  - Dùng https://www.remove.bg (free 1 ảnh/giờ)
  - Hoặc Photopea (https://www.photopea.com) — Photoshop free trên web

### 5.4. Cảnh báo về AI asset
- ⚠️ **License không hoàn toàn rõ:** Một số nước coi AI-generated là public domain, một số yêu cầu disclaimer
  - **Cho đồ án sinh viên:** OK, nhưng nên ghi rõ trong report "Sử dụng AI generative tools"
- ⚠️ **Chất lượng không đồng đều:** Nên tạo 5-10 ảnh cho mỗi asset, chọn cái tốt nhất
- ⚠️ **Không thay thế được toàn bộ:** Background AI rất tốt, nhưng UI button đôi khi méo — vẫn nên ưu tiên pack có sẵn cho UI

---

## 6. KẾ HOẠCH TINH CHỈNH UI LẦN CUỐI

### 6.1. Checklist sau khi có đủ assets

#### ☐ Wire fonts vào code (NEW)
- [ ] Đặt `main.ttf` và `title.ttf` vào `assets/fonts/`
- [ ] Kiểm tra `renderLoadResources()` load đúng 2 font
- [ ] Test tiếng Việt có hiển thị đầy đủ dấu không
- [ ] Đảm bảo title font không quá rộng (vỡ layout)

#### ☐ Wire textures vào `render.cpp`
- [ ] Load 4 texture từ `assets/textures/`
- [ ] `renderBackground()`: vẽ sprite background full window
- [ ] `renderBoard()`: vẽ `board.png` đè rồi vẽ lưới
- [ ] `renderPiece()`: vẽ X/O piece thay shape thuần
- [ ] (Tuỳ chọn) Vẽ 2 mascot ở 2 bên panel
- [ ] Test fallback: rename file → game phải vẫn chạy

#### ☐ Đồng bộ màu sắc theo background mới
- [ ] Nếu background sáng → đổi `COLOR_MENU_TEXT` sang màu tối
- [ ] Nếu background tối → giữ chữ sáng
- [ ] Test contrast trên mọi screen

#### ☐ Audio polish
- [ ] BGM volume mặc định 40%
- [ ] Test BGM không cắt khi đổi screen
- [ ] Test sound pool: spam đặt quân nhanh
- [ ] Test persist: tắt-mở game → cài đặt còn

#### ☐ UI Animation & Feedback
- [ ] Hover button đổi màu khi rê chuột
- [ ] Animation đặt quân 0.2s mượt
- [ ] Win line nhấp nháy đẹp
- [ ] Cursor rõ ràng trên ô đang chọn

#### ☐ Spacing & Alignment
- [ ] Text căn giữa đúng pixel
- [ ] Menu item cách đều
- [ ] Panel căn theo bàn cờ
- [ ] Game Over panel không đè text khác

#### ☐ Screen transitions
- [ ] Menu → Gameplay: không flash
- [ ] Pause → Resume: timer không nhảy
- [ ] Save/Load: cập nhật list file đúng

#### ☐ Edge cases
- [ ] Bàn cờ đầy → hiện Draw đúng
- [ ] Hết giờ Speed Mode → xử lý thua đúng
- [ ] Load file không tồn tại → không crash
- [ ] Bot không còn nước → fallback hợp lý

### 6.2. Quy trình test cuối
1. Build Release clean rebuild
2. Chạy lần đầu → kiểm tra load assets không lỗi
3. Test full flow: Menu → New PvP → Save → Quit → Load → Play tiếp → Win → Continue
4. Test PvC 4 cấp độ
5. Test Speed Mode hết giờ lượt + giờ ván
6. Test Settings persist
7. Test toàn bộ bằng CHUỘT, sau đó bằng PHÍM

---

## 7. CẤU TRÚC THƯ MỤC & TRACKING

### 7.1. Cấu trúc `assets/` chuẩn

```
Caro_Game_Group_1/
├── assets/
│   ├── fonts/
│   │   ├── main.ttf            ← [TODO] Body text (Fredoka / Roboto)
│   │   └── title.ttf           ← [TODO] Title (Luckiest Guy / Orbitron)
│   ├── sounds/
│   │   ├── bgm.ogg             ← [TODO] Nhạc nền
│   │   ├── place.wav           ← [TODO] Đặt quân
│   │   ├── move.wav            ← [TODO] Di cursor
│   │   ├── win.wav             ← [TODO] Thắng
│   │   ├── draw.wav            ← [TODO] Hòa
│   │   └── menu.wav            ← [TODO] Click menu
│   └── textures/
│       ├── background.png      ← [TODO] Nền 1280×720
│       ├── board.png           ← [TODO] Bàn cờ 600×600
│       ├── x_piece.png         ← [TODO] Quân X 36×36
│       ├── o_piece.png         ← [TODO] Quân O 36×36
│       ├── mascot_p1.png       ← [Optional] Nhân vật trái ~200×400
│       └── mascot_p2.png       ← [Optional] Nhân vật phải ~200×400
├── saves/                       ← Tự tạo runtime
└── settings.dat                 ← Tự tạo runtime
```

### 7.2. Quy ước đặt tên
- **Snake_case**, không khoảng trắng
- **Tên đúng chính xác** — code load theo tên
- Đổi tên file ngay khi tải về

### 7.3. Bảng tracking

| Asset | Trạng thái | Người PT | Nguồn dự kiến | Ngày commit | Ghi chú |
|-------|-----------|----------|---------------|-------------|---------|
| `main.ttf` | ⬜ | _______ | Google Fonts | / / | Fredoka Regular |
| `title.ttf` | ⬜ | _______ | Google Fonts | / / | Luckiest Guy |
| `bgm.ogg` | ⬜ | _______ | Kenney/Zapsplat | / / | |
| `place.wav` | ⬜ | _______ | Kenney | / / | |
| `move.wav` | ⬜ | _______ | Kenney | / / | |
| `win.wav` | ⬜ | _______ | Zapsplat | / / | |
| `draw.wav` | ⬜ | _______ | Mixkit | / / | |
| `menu.wav` | ⬜ | _______ | Kenney | / / | |
| `background.png` | ⬜ | _______ | Craftpix/AI | / / | Theme: ________ |
| `board.png` | ⬜ | _______ | Craftpix/AI | / / | |
| `x_piece.png` | ⬜ | _______ | AI (Leonardo) | / / | |
| `o_piece.png` | ⬜ | _______ | AI (Leonardo) | / / | |
| `mascot_p1.png` | ⬜ | _______ | AI / Craftpix | / / | Optional |
| `mascot_p2.png` | ⬜ | _______ | AI / Craftpix | / / | Optional |
| **Wire fonts** | ⬜ | Lead | — | / / | Sau khi có 2 font |
| **Wire textures** | ⬜ | Lead | — | / / | Sau khi có đủ PNG |
| **UI Polish (6.1)** | ⬜ | Lead | — | / / | |
| **Test cuối (6.2)** | ⬜ | Lead | — | / / | |

**Trạng thái:** ⬜ TODO → 🟡 Đang làm → 🟢 Done → ✅ Commit

### 7.4. Quy trình bàn giao file
1. Tìm/tạo asset → **đổi tên chuẩn ngay** (`bgm.ogg`, không `my_song_final_v2.ogg`)
2. Test mở file (VLC cho audio, Windows Photo cho ảnh)
3. Bỏ vào folder tương ứng
4. Commit ngay: `assets: add bgm.ogg`
5. Báo nhóm qua group chat

### 7.5. Mốc kiểm tra
- **20/05 (T3):** Chốt theme (A/B/C) — quan trọng nhất
- **22/05 (T5):** Báo cáo đã tải được bao nhiêu, có asset nào AI tạo
- **24/05 (T6):** Deadline có đủ assets
- **25/05 (T7):** Wire vào code + UI polish
- **26/05 (CN):** Test cuối + chốt — không sửa nữa
- **27/05 - 31/05:** Luyện thuyết trình
- **01/06 (CN):** Thuyết trình

---

## ✅ DEFINITION OF DONE
- [x] 2 file font có trong `assets/fonts/`, test tiếng Việt OK
- [x] 6 file âm thanh có trong `assets/sounds/`, đã test phát được
- [x] Tối thiểu 4 file texture trong `assets/textures/`, đúng kích thước
- [x] Toàn bộ asset CÙNG 1 THEME, không lệch tông
- [x] Game build & chạy load đủ assets, không warning thiếu file
- [x] Checklist 6.1 tick xong
- [x] Quy trình test 6.2 chạy xong, không lỗi

---
