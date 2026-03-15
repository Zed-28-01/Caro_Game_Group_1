# Caro Game - Hướng Dẫn Setup cho Thành Viên Mới

## 📋 MỤC LỤC

- [Yêu cầu](#yêu-cầu)
- [Clone repo về máy](#clone-repo-về-máy)
- [Chạy thử project](#chạy-thử-project)
- [Quy tắc thêm file khi code](#quy-tắc-thêm-file-khi-code)
- [Cấu trúc thư mục](#cấu-trúc-thư-mục)

---

## ✅ YÊU CẦU

Chỉ cần cài **một thứ duy nhất**:

| Phần mềm | Ghi chú |
|----------|---------|
| **Visual Studio 2022** | Phải có workload "Desktop development with C++" |

> ⚠️ **Không cần cài** Git Bash, SFML, hay bất kỳ thư viện nào khác — tất cả đã có trong repo.

---

## 📥 CLONE REPO VỀ MÁY

### Cách 1 — Dùng VS 2022 (khuyên dùng)

1. Mở VS 2022
2. **Git → Clone Repository**
3. Paste link repo:
   ```
   https://github.com/Zed-28-01/Caro_Game_Group_1.git
   ```
4. Chọn thư mục muốn lưu → nhấn **Clone**

### Cách 2 — Dùng Git Bash

```bash
git clone https://github.com/Zed-28-01/Caro_Game_Group_1.git
```

---

## ▶️ CHẠY THỬ PROJECT

1. Vào thư mục `Caro_Game_Group_1/CaroGame/`
2. Double click file **`CaroGame.sln`**
3. VS 2022 tự mở lên
4. Nhấn **Ctrl+F5** để build và chạy

> Nếu hiện cửa sổ game lên = thành công, không cần cài thêm gì!

---

## 📝 QUY TẮC THÊM FILE KHI CODE

> ⚠️ **Quan trọng** — Làm đúng theo quy tắc này để tránh lỗi build.

### File `.cpp` — để trong thư mục `src/`

Trong VS 2022, Solution Explorer:

> Click phải **Source Files** → **Add** → **New Item** → chọn **C++ File (.cpp)** → **đổi ô Location thành đường dẫn đến thư mục `src/`** → đặt tên → **Add**

Ví dụ Location:
```
D:\...\Caro_Game_Group_1\src\
```

---

### File `.h` — để trong thư mục `include/`

Trong VS 2022, Solution Explorer:

> Click phải **Header Files** → **Add** → **New Item** → chọn **Header File (.h)** → **đổi ô Location thành đường dẫn đến thư mục `include/`** → đặt tên → **Add**

Ví dụ Location:
```
D:\...\Caro_Game_Group_1\include\
```

---

### Tại sao phải làm vậy?

```
Caro_Game_Group_1/
├── src/        ← tất cả file .cpp của team để ở đây
├── include/    ← tất cả file .h của team để ở đây
├── assets/     ← ảnh, nhạc, font
├── docs/       ← tài liệu
└── CaroGame/   ← cấu hình VS 2022, KHÔNG code trực tiếp ở đây
```

Nếu tạo file thẳng vào `CaroGame/` thay vì `src/` hoặc `include/` thì code vẫn chạy được trên máy bạn nhưng **thành viên khác clone về sẽ bị lỗi đường dẫn**.

---

## 📁 CẤU TRÚC THƯ MỤC

```
Caro_Game_Group_1/
├── CaroGame/                  ← VS 2022 project
│   ├── CaroGame.sln           ← double click vào đây để mở
│   ├── CaroGame.vcxproj       ← cấu hình build (không sửa tay)
│   └── [các file .dll SFML]   ← thư viện runtime
├── src/                       ← FILE .CPP của team
├── include/                   ← FILE .H của team
├── assets/                    ← ảnh, nhạc, font chữ
├── libs/
│   └── SFML/                  ← thư viện SFML (không sửa)
└── docs/                      ← tài liệu nhóm
    ├── SETUP.md               ← file này
    └── GitCommand.md          ← hướng dẫn Git
```

---

## ❓ GẶP VẤN ĐỀ?

**Build lỗi "cannot open source file SFML/Graphics.hpp"**
> Kiểm tra thư mục `libs/SFML/` có đủ 3 thư mục `bin/`, `lib/`, `include/` chưa.

**Game chạy nhưng crash ngay lập tức**
> Kiểm tra thư mục `CaroGame/` có đủ các file `.dll` chưa.

**Push lên GitHub bị từ chối**
> Không được push thẳng lên `main` — tạo branch mới và tạo Pull Request. Xem `GitCommand.md` để biết thêm.

---
