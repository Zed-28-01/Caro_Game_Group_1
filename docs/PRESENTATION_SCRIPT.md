# SCRIPT THUYẾT TRÌNH — CARO GAME (20 phút)

**Nhóm 1 — Đồ án Cơ sở lập trình C++** | Presenter: **Nhật**

---

## ⏱️ Time Budget 20 phút

| Phần | Slide | Thời gian |
|---|---|---|
| 1. Mở đầu | 1-3 | 1:00 |
| 2. Kiến trúc + cơ bản | 4-9 | 4:30 |
| 3. UI showcase | 10-15 | 1:30 |
| 4. Kỹ thuật nâng cao | 16-21 | 5:00 |
| 5. **DEMO** ⭐ | 22 | 6:00 |
| 6. Tổng kết | 23-25 | 1:30 |
| Buffer Q&A | — | 0:30 |

---

# 🎬 PHẦN 1 — MỞ ĐẦU (1 phút)

## Slide 1 — Title (10s)
> *Chào thầy và các bạn. Em là Nhật, đại diện Nhóm 1 trình bày đồ án Caro Game — game cờ caro 5 quân thắng, phát triển bằng C++ và thư viện SFML. Buổi thuyết trình hôm nay sẽ kéo dài khoảng 20 phút, bao gồm phần demo trực tiếp 6 phút.*

## Slide 2 — Nội dung trình bày (15s)
> *Nội dung em sẽ trình bày gồm 4 phần: thứ nhất, kiến trúc tổng thể và các tính năng cơ bản theo đề bài; thứ hai, các kỹ thuật nâng cao như Bot AI và Speed Mode; thứ ba, demo trực tiếp sản phẩm; và cuối cùng là tổng kết những gì nhóm em đã học được.*

## Slide 3 — Giới thiệu game (35s)
> *Caro Game hay còn gọi là Gomoku — luật chơi đơn giản: ai đặt được **5 quân liên tiếp** trên bàn cờ **15×15** thì thắng. Nhóm em đã hiện thực hóa game này thành sản phẩm khoảng **3000 dòng C++**, chia thành **6 module** rõ ràng. Phần graphics và audio em dùng thư viện **SFML 2.6.2**. Toàn bộ quá trình phát triển nhóm em quản lý bằng Git và GitHub — đảm bảo code có lịch sử thay đổi rõ ràng, dễ phối hợp giữa các thành viên.*

💡 **Pacing tip:** Đọc chậm phần "5 quân liên tiếp", "15×15", "3000 dòng", "6 module".

---

# 🏗️ PHẦN 2 — KIẾN TRÚC + CƠ BẢN (4 phút 30 giây)

## Slide 4 — Cấu trúc module (50s)
> *Đây là sơ đồ phụ thuộc của 9 module — chỉ ra cách các file source liên kết với nhau.*
>
> *Trung tâm là `game_types.h` — chứa toàn bộ struct, enum, constant. Mọi module khác đều include file này, đóng vai trò "từ vựng chung".*
>
> *`main.cpp` là điểm vào, tạo window và gọi `gameRun()` trong `menu.cpp`. `menu.cpp` là dispatcher chính — điều phối luồng giữa các màn hình, gọi 6 module chức năng: `bot`, `board`, `timer`, `render`, `save_load`, `sound`, `language`.*
>
> *Kiến trúc phẳng kiểu C, không OOP nặng — phù hợp với class project, dễ hiểu, dễ debug.*

## Slide 5 — Sơ đồ luồng game (50s)
> *Đây là state diagram thể hiện luồng chuyển đổi giữa các màn hình. Bắt đầu từ Main Menu, người chơi có thể vào game mới qua 4 bước: chọn Mode (PvP/PvC), chọn độ khó (nếu PvC), chọn Style (Basic/Speed), nhập tên — rồi vào Playing.*
>
> *Trong Playing có 3 đường thoát: nhấn ESC vào Pause Menu, nhấn L để Save, hoặc thắng/hòa/hết giờ vào Game Over.*
>
> *Tính năng mới nhóm em vừa thêm: ở Game Over, sau khi hỏi 'Chơi tiếp?' và người chơi chọn Không, em hỏi tiếp 'Lưu ván đấu?' — giúp người chơi không mất ván hay.*

## Slide 6 — Cấu trúc dữ liệu (40s)
> *`GameState` là god struct chứa toàn bộ trạng thái 1 ván cờ. Có vài chi tiết đáng lưu ý:*
>
> *Thứ nhất, `board` là mảng tĩnh 2D 15×15 — nhanh hơn vector, cache friendly.*
>
> *Thứ hai, `moveHistory` là mảng tĩnh 225 phần tử + `moveCount` — em dùng pattern array+count thay cho `std::stack` để serialize save/load trivially.*
>
> *Thứ ba, `TimerState` em dùng chess-clock — mỗi người có 10 phút riêng, sẽ trình bày sau.*

## Slide 7 — Kiểm tra thắng (50s)
> *Thay vì quét toàn bộ 225 ô mỗi lượt, em chỉ check từ ô vừa đặt theo 4 hướng: ngang, dọc, 2 chéo.*
>
> *Mỗi hướng em đếm về cả 2 phía — phía dương và phía âm — rồi cộng với ô gốc. Nếu tổng ≥ 5 thì thắng. Em chỉ check sau mỗi nước đi, độ phức tạp **O(1) mỗi lượt** thay vì O(N²) — nhanh gấp ~225 lần.*

## Slide 8 — Kiểm tra hòa (20s)
> *Tương tự, em tối ưu hàm `checkDraw` xuống O(1) bằng cách dùng biến `moveCount` — luôn đồng bộ với số ô đã đặt qua các thao tác đặt/undo/reset. Bàn đầy khi `moveCount = 225`. Chỉ 1 phép so sánh int.*

## Slide 9 — Save/Load (50s)
> *Save/Load em dùng định dạng **text** thay vì binary. Lý do: text dễ debug, dễ hỗ trợ UTF-8 cho tên người chơi tiếng Việt, và không phụ thuộc layout struct.*
>
> *Quá trình save em ghi tuần tự thông tin 2 player, trạng thái ván, bàn cờ, lịch sử nước đi. Load thì đọc ngược lại theo đúng thứ tự đó.*
>
> *Tổng cộng game hỗ trợ tối đa 15 slot save.*

---

# 🎨 PHẦN 3 — UI SHOWCASE (1 phút 30 giây)

*Lướt nhanh 6 screenshots, mỗi slide ~15 giây.*

## Slide 10 — Main Menu (15s)
> *Màn hình chính: logo CARO sticker, 6 menu items với button frame bo tròn vàng kim, background ảnh anime Dragon Ball xuyên suốt mọi màn hình.*

## Slide 11 — Mode Select (15s)
> *Chọn PvP hai người hoặc PvC chơi với máy.*

## Slide 12 — Difficulty (15s)
> *4 cấp độ Bot: Easy, Medium, Hard, Expert — em sẽ giải thích chi tiết ở phần sau.*

## Slide 13 — Gameplay (20s)
> *Đây là màn hình chính: bàn cờ 15×15 ở giữa, panel 2 người chơi bên phải với mascot Goku và Vegeta. Trong Speed mode có thanh đếm ngược 20s/lượt và thời gian ván của mỗi người riêng biệt.*

## Slide 14 — Pause Menu (15s)
> *Nhấn ESC mở Pause Menu với 3 lựa chọn: tiếp tục, lưu, về Menu chính.*

## Slide 15 — Game Over (15s)
> *Khi kết thúc, banner VICTORY hiện cho người thắng, DEFEAT cho người thua. Mascot tự đổi tư thế tương ứng. Hỏi 'Chơi tiếp?' rồi nếu Không thì hỏi 'Lưu ván?'.*

---

# 🤖 PHẦN 4 — KỸ THUẬT NÂNG CAO (5 phút)

## Slide 16 — Bot AI 4 cấp độ (45s)
> *Bot AI có 4 cấp độ tăng dần:*
> - *Easy: random với check thắng/chặn ngay — dành cho người mới làm quen*
> - *Medium: heuristic scoring — nhìn xa 1 lượt*
> - *Hard: Minimax + Alpha-Beta pruning, độ sâu 3*
> - *Expert: tương tự Hard nhưng độ sâu 4 với Move Ordering*
>
> *Cả Hard và Expert đều dùng Minimax kết hợp Alpha-Beta. Khác biệt là Expert nhìn sâu hơn và có cơ chế sắp xếp nước đi để cắt tỉa hiệu quả hơn.*
>
> *Tính năng Hint — phím H — dùng chính thuật toán Medium để gợi ý cho người chơi. Đủ thông minh nhưng đủ nhanh để phản hồi tức thì.*

## Slide 17 — botGetCandidates (45s)
> *Trước khi chấm điểm, em lọc danh sách ô cần xét. Thay vì xét cả 225 ô, em chỉ xét ô trống có quân nào đó trong bán kính `radius`.*
>
> *Lý do: nước đi tiềm năng chỉ có nghĩa khi gần quân hiện có — đặt ngoài rìa khi chưa có gì là vô nghĩa.*
>
> *Trường hợp đặc biệt: bàn cờ trống → em trả về ô trung tâm (7,7) làm nước khai cuộc.*
>
> *Kết quả: candidates giảm từ 225 xuống 20-50 → bot Expert chạy được ở depth 4 trong thời gian chấp nhận được.*

## Slide 18 — Pattern Scoring ⭐ (75s)
> *Đây là building block cho 3 cấp bot có heuristic: Medium, Hard, Expert.*
>
> *Hàm `scoreLine` xét 1 chuỗi quân theo 1 hướng. Đếm số quân liên tiếp về 2 phía, cộng với ô đang xét. Đồng thời đếm số đầu mở — tức là 2 đầu chuỗi có phải ô trống không.*
>
> *Rồi tra bảng điểm: 5 liên tiếp đương nhiên thắng — 1 triệu điểm. 4 mở 2 đầu chắc thắng vì không thể chặn cả 2 đầu trong 1 lượt — 100 ngàn điểm. 4 mở 1 đầu có thể chặn được — 5 ngàn điểm. Tương tự cho 3, 2, 1 quân.*
>
> *Bậc thang điểm này em thiết kế để bot phân biệt rõ giữa các nước — chênh nhau 10-20 lần để bot không bị nhầm giữa nước tốt vừa và nước cực tốt.*
>
> *Medium dùng trực tiếp `scoreLine` để chấm. Hard và Expert dùng nó tại lá của cây Minimax — slide tiếp theo.*

## Slide 19 — Minimax + Alpha-Beta ⭐⭐ ĐIỂM NHẤN (2 phút)

⚠️ **ĐOẠN QUAN TRỌNG NHẤT** — đọc chậm + nhấn mạnh tác giả gốc.

> *Đây là điểm nhấn của phần Bot AI.*
>
> *Minimax là thuật toán cổ điển trong game theory — được **John von Neumann** chứng minh năm 1928, **Claude Shannon** áp dụng vào AI cờ vua năm 1950, và IBM **Deep Blue** dùng để đánh bại Garry Kasparov năm 1997. Nhóm em **không phát minh** thuật toán này — em **triển khai** lại cho game Caro.*
>
> *Ý tưởng: Bot giả định cả 2 người chơi đều tối ưu. Bot là MAX — tìm nước có điểm cao nhất. Đối thủ là MIN — bot giả định đối thủ sẽ chọn nước làm bot bất lợi nhất. Đệ quy này lan truyền đến độ sâu maxDepth, đến lá em gọi `evaluateBoard` đánh giá toàn cảnh bằng `scoreLine`.*
>
> *Vấn đề: depth 4 phải duyệt rất nhiều trạng thái. Alpha-Beta giải quyết: nếu một nhánh đã chắc chắn tệ hơn nhánh tốt nhất đã tìm — cắt luôn không duyệt nữa. Cụ thể: `beta ≤ alpha` thì break.*
>
> *Đóng góp riêng của nhóm em là: thiết kế bảng điểm pattern phù hợp riêng cho Caro, chọn depth 4 cân bằng giữa thông minh và tốc độ — bot phản hồi dưới 200ms — và tích hợp với chess-clock của Speed mode.*

## Slide 20 — Speed Mode Chess-Clock (50s)
> *Speed Mode em thiết kế theo cơ chế **chess-clock** — mỗi người có 10 phút riêng, đồng hồ chỉ chạy khi đến lượt mình.*
>
> *Mỗi frame, game loop tính `deltaTime` từ `sf::Clock`, trừ vào thời gian lượt và thời gian ván của người đang đi. Có 3 điều kiện kết thúc: hết 20 giây của lượt — người đang đi thua; P1 hết 10 phút — P1 thua; P2 hết 10 phút — P2 thua.*
>
> *Quan trọng: nhóm em xử lý cẩn thận trường hợp PvC + Speed. Khi bot Expert suy nghĩ 2-5 giây, em đo bằng `sf::Clock` riêng và trừ thủ công vào thời gian của bot — đảm bảo chess-clock công bằng.*

## Slide 21 — Mouse Support (30s)
> *Mouse Support em làm bằng cách chuyển đổi tọa độ pixel chuột về tọa độ ô trên bàn cờ.*
>
> *Hàm `pixelToBoard` chia tọa độ cho `CELL_SIZE` (40px) sau khi trừ offset bàn cờ — ra (row, col) từ 0-14.*
>
> *Trong game loop: di chuột làm cursor follow chuột, click trái thì đặt quân. Ngoài ra hàm `menuHitTest` xác định chuột đang nằm trên nút bấm nào trong menu để highlight.*

---

# 🎮 PHẦN 5 — DEMO TRỰC TIẾP (6 phút) ⭐ CRITICAL

## Mở đầu demo
> *Bây giờ em xin phép chuyển sang demo trực tiếp sản phẩm.*

## Phần 1 — Main Menu + Settings (45s)

**Các bước thực hiện:**
- Mở exe → Main Menu hiện ra
- Vào Settings → kéo slider volume → đổi VN/EN
- ESC về Main

**Câu nói:**
> *Đây là màn hình chính. Em show qua menu Cài đặt...*
> *Game hỗ trợ tiếng Việt và tiếng Anh, settings được lưu vào file `saves/settings.txt`.*

## Phần 2 — PvP Basic (1 phút)

**Các bước thực hiện:**
- Chọn: Chơi mới → PvP → Basic → nhập tên 2 player tiếng Việt
- Đặt vài quân với WASD + Enter, cố ý tạo chuỗi 4 quân
- Tiếp tục đến khi thắng → highlight 5 quân thắng màu vàng
- Chọn "Không" ở "Chơi tiếp?" → hiện câu hỏi mới về lưu ván
- Chọn "Không" → về Main Menu

**Câu nói:**
> *Em sẽ chơi nhanh 1 ván PvP để show gameplay và mascot.*
> *Cursor di bằng WASD, đặt quân bằng Enter, hoặc dùng chuột — em sẽ show chuột ở ván sau.*
> *5 quân thắng được highlight, mascot Goku đổi tư thế chiến thắng, banner VICTORY hiện ra.*
> *Đây là tính năng nhóm em vừa thêm: hỏi có muốn lưu ván đấu không.*

## Phần 3 — PvC Expert + Speed (2 phút 30 giây) ⭐ ĐIỂM NHẤN DEMO

**Các bước thực hiện:**
- Chọn: Chơi mới → PvC → Expert → Speed → nhập tên
- Đặt quân vài lần — show cả bàn phím và chuột
- Cố ý nhấn H để gọi Hint → highlight ô gợi ý
- Tiếp tục đánh, để bot thắng nếu có thể (Expert thường thắng)

**Câu nói:**
> *Bây giờ em đánh với bot cấp Expert ở chế độ Speed.*
> *Mỗi người có 10 phút riêng, hiển thị trong panel. Khi đến lượt mình, đồng hồ chạy. Khi bot suy nghĩ, có text "Bot đang suy nghĩ" báo cho người dùng biết.*
> *Phím H gọi gợi ý — dùng thuật toán Medium đánh giá theo góc nhìn người chơi.*
> *Bot Expert nhìn xa 4 lượt và sort candidates trước khi search — phản hồi vẫn dưới 200ms.*

## Phần 4 — Save/Load (1 phút)

**Các bước thực hiện:**
- Chơi tiếp → Chọn Có → reset round (loser starts)
- Đặt 5-6 quân, nhấn L → Save Screen
- Nhập tên save "demo_test" → lưu
- ESC về Playing → ESC nữa → Pause Menu → Về Menu
- Main Menu → Tải game → chọn "demo_test"

**Câu nói:**
> *Theo quy tắc cờ vua truyền thống, người thua ván trước đi trước ván sau.*
> *Save lưu dưới định dạng text trong thư mục saves.*
> *Load thành công — toàn bộ trạng thái khôi phục đúng: vị trí quân, thời gian từng người, lượt đang đi.*

## Phần 5 — Resize window (30s)

**Các bước thực hiện:**
- Kéo to window hoặc nhấn nút Maximize
- Chuột vẫn click đúng ô

**Câu nói:**
> *Game hỗ trợ resize giữ tỷ lệ 16:9 bằng letterbox — chi tiết kỹ thuật em có ghi trong báo cáo.*
> *Mouse hit-test vẫn chính xác sau khi resize.*

## Kết thúc demo (15s)
- ESC về Main Menu

> *Đó là toàn bộ demo. Em xin trở lại slide tổng kết.*

---

# 🏁 PHẦN 6 — TỔNG KẾT (1 phút 30 giây)

## Slide 23 — Đối chiếu yêu cầu cốt lõi (45s)
> *Em đối chiếu với 5 yêu cầu cốt lõi của đề bài:*
> - *Save/Load: vượt yêu cầu với 15 slot*
> - *Nhận biết thắng/thua/hòa: tối ưu O(1) mỗi lượt*
> - *Hiệu ứng: có animation cho đặt quân, win, draw + sound effects*
> - *Giao diện gameplay: có panel stats đầy đủ — tên, số bước, thắng, thời gian*
> - *Màn hình chính: 13 màn hình tổng cộng*
>
> *Tất cả 5 yêu cầu cốt lõi đạt mức vượt.*

## Slide 24 — Đối chiếu yêu cầu mở rộng (30s)
> *Ngoài ra nhóm em sáng tạo thêm:*
> - *Bot AI 4 cấp + Hint + Undo*
> - *Speed Mode với chess-clock*
> - *Mouse Support*
> - *Đa ngôn ngữ Việt-Anh và lưu settings*
> - *Một số tính năng âm thanh, UI/UX mượt cho game*

## Slide 25 — Tổng kết (15s)
> *Tổng kết — kỹ thuật em đã áp dụng: mảng 2D, struct, đệ quy, thiết kế kiểu pointer-free. Học được: SFML, Minimax, Alpha-Beta, Game Loop, File I/O. Khó khăn lớn nhất là đồng bộ UI 13 màn hình và tối ưu bot dưới 200ms phản hồi.*
>
> *Em xin kết thúc phần trình bày. Cảm ơn thầy và các bạn đã lắng nghe. Em sẵn sàng nhận câu hỏi từ Hội đồng.*

---

# 🎯 PACING CHEATSHEET (in sẵn liếc khi present)

| Mốc thời gian | Đang ở slide nào | Nếu lệch |
|---|---|---|
| **02:00** | Hết slide 4-5 | Lệch >30s → rút gọn UI showcase |
| **05:30** | Hết slide 9 | Lệch >1min → bỏ slide 14 (Pause) |
| **07:00** | Hết slide 15 | Đang đúng tiến độ |
| **12:00** | Hết slide 21 (chuyển demo) | Demo 6 phút từ đây |
| **18:00** | Hết demo (slide 22) | Còn 2 phút cho 3 slide kết |
| **19:30** | Hết slide 25 | OK — buffer Q&A 30s |

---

# 💡 TIPS DEMO CRITICAL

- **Trước demo:** Xóa `saves/Gamelist.txt` (do format thay đổi) — chỉ giữ `settings.txt`
- **Chuẩn bị sẵn** 1 save game test trước để show Load nhanh
- **Tập demo 3 lần** trước thuyết trình thật — đếm giờ

**Phím tắt cần nhớ:**
- W/A/S/D: di cursor
- Enter: đặt quân
- H: hint (chỉ PvC)
- L: save
- Z: undo
- ESC: pause

**Backup plan:**
- Nếu game crash: *"Em có quay video demo dự phòng"* (chuẩn bị sẵn nếu có thể)
- Nếu lệch thời gian: cắt UI showcase (slide 14 Pause + 1 slide khác)

---

# 📚 Q&A FLASHCARDS — TOP 10 CÂU HỎI THƯỜNG GẶP

### Q1: Minimax là của các em phát minh à?
> *Không. John von Neumann chứng minh năm 1928, Claude Shannon áp dụng AI cờ vua 1950. Em TRIỂN KHAI lại cho Caro. Đóng góp riêng: bảng điểm pattern + depth=4 + tích hợp chess-clock.*

### Q2: Tại sao chọn Minimax mà không phải MCTS?
> *Caro là zero-sum + deterministic + perfect-info → Minimax tối ưu lý thuyết. Branching factor nhỏ hơn cờ vua → depth 4 đủ thông minh. Có scoreLine chính xác → không cần simulation như MCTS.*

### Q3: Hard và Expert khác nhau gì?
> *Cả 2 đều Minimax + Alpha-Beta. Khác: Hard depth 3 / Expert depth 4 + Move Ordering (sort candidates) + Top-12 filter.*

### Q4: evaluatePosition vs evaluateBoard?
> *evaluatePosition chấm 1 nước đi (mô phỏng đặt quân, attack + 0.9×defense + center bonus). Dùng cho Medium, Move Ordering Expert, Hint. evaluateBoard chấm toàn bàn cờ. Dùng tại lá Minimax (Hard + Expert).*

### Q5: Tại sao defense = 0.9 × attack?
> *Em test 3 tỷ lệ: 1.0 → bot quá phòng thủ. 0.5 → bot bỏ qua đối thủ. 0.9 → cân bằng — ưu tiên tấn công nhưng vẫn block.*

### Q6: Tại sao chess-clock thay vì shared timer?
> *Công bằng — người chậm tự bị phạt, không phụ thuộc đối thủ. Ai hết time → người đó thua (giống cờ vua truyền thống).*

### Q7: UTF-8 Vietnamese hiển thị thế nào?
> *Dùng `sf::String::fromUtf8(s.begin(), s.end())`. Lý do: `setString(std::string)` interpret bytes là Latin-1 → mojibake với UTF-8.*

### Q8: Sao mouse hit-test vẫn đúng sau resize?
> *`handleCommonEvent` map pixel chuột → view coord (1280×720 chuẩn) trước khi đến code hit-test. Letterbox giữ tỷ lệ 16:9.*

### Q9: Save/Load dùng binary hay text?
> *Text. Lý do: dễ debug, UTF-8 friendly cho tên VN, không phụ thuộc struct layout.*

### Q10: Undo dùng `std::stack` hay mảng?
> *Mảng tĩnh + count. Lý do: kích thước max biết chắc (225), cache locality, save/load đơn giản, random access cho PvC undo 2 nước.*
