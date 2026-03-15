# Caro Game - Hướng Dẫn Git cho Team

## 📋 MỤC LỤC

- [Thiết lập ban đầu](#thiết-lập-ban-đầu)
- [Git Workflow hàng ngày](#git-workflow-hàng-ngày)
- [Branch Management](#branch-management)
- [Merge và Pull Request](#merge-và-pull-request)
- [Troubleshooting](#troubleshooting)
- [Commit Best Practices](#commit-best-practices)
- [Quan trọng](#quan-trọng)

---

## ⚙️ THIẾT LẬP BAN ĐẦU

> Làm **một lần duy nhất** khi mới vào project.

### Clone repo về máy

**VS 2022:**
> Mở VS 2022 → **Git → Clone Repository** → paste link:
> ```
> https://github.com/Zed-28-01/Caro_Game_Group_1.git
> ```
> Chọn thư mục lưu → Clone → double click `CaroGame.sln` → **Ctrl+F5** là chạy được.

**Git Bash:**
```bash
git clone https://github.com/Zed-28-01/Caro_Game_Group_1.git
```

---

### Cấu hình tên và email (chỉ cần nếu dùng Git Bash)

> Nếu đăng nhập GitHub vào VS 2022 rồi thì **bỏ qua bước này**.

```bash
git config --global user.name "TênCủaBạn"
git config --global user.email "email@student.hcmus.edu.vn"
```

---

## 🔄 GIT WORKFLOW HÀNG NGÀY

### Bước 1 — Pull code mới về trước khi làm việc

> ⚠️ **Luôn làm bước này trước** khi bắt đầu code mỗi ngày!

**VS 2022:**
> **Git → Pull**

**Git Bash:**
```bash
git checkout main
git pull origin main
```

---

### Bước 2 — Tạo branch mới để code

> Không được code trực tiếp trên `main`!

**VS 2022:**
> **Git → New Branch** → đặt tên branch → OK

**Git Bash:**
```bash
git checkout -b TenBranch
# Ví dụ: git checkout -b feature/game-board
```

**Quy tắc đặt tên branch:**
| Loại | Tên branch | Ví dụ |
|------|-----------|-------|
| Tính năng mới | `feature/ten-tinh-nang` | `feature/game-board` |
| Sửa bug | `fix/mo-ta-bug` | `fix/win-condition` |
| Cải tiến | `refactor/ten-phan` | `refactor/ai-logic` |

---

### Bước 3 — Kiểm tra trạng thái

**VS 2022:**
> Xem panel **Git Changes** bên phải — danh sách file thay đổi hiện ở đó.

**Git Bash:**
```bash
git status
```

---

### Bước 4 — Commit code

**VS 2022:**
> **Git → Commit or Stash** → nhập message → nhấn **Commit All**

**Git Bash:**
```bash
git add .
git commit -m "Mô tả ngắn gọn những gì bạn làm"
```

---

### Bước 5 — Push lên GitHub

**VS 2022:**
> Sau khi Commit → nhấn nút **↑ (Push)** trong panel Git Changes

**Git Bash:**
```bash
# Push lần đầu cho branch mới
git push origin TenBranch

# Push tiếp cho branch đã có
git push
```

---

### Bước 6 — Tạo Pull Request để merge vào main

**VS 2022:**
> **Git → New Pull Request** → GitHub mở trên trình duyệt → Tạo PR → chờ teammate review → merge

> ⚠️ Cần ít nhất 1 người review và approve trước khi merge vào `main`.

---

## 🌿 BRANCH MANAGEMENT

### Xem danh sách branches

**VS 2022:**
> **Git → Manage Branches** — danh sách branches hiện ở panel bên trái

**Git Bash:**
```bash
# Xem local branches
git branch

# Xem tất cả (local + remote)
git branch -a
```

---

### Chuyển branch

**VS 2022:**
> **Git → Manage Branches** → double click vào branch muốn chuyển  
> Hoặc nhấn vào tên branch ở góc dưới phải của VS 2022

**Git Bash:**
```bash
git checkout TenBranch
```

---

### Xóa branch sau khi merge

**VS 2022:**
> **Git → Manage Branches** → checkout sang `main` trước → click phải branch cần xóa → **Delete**

**Git Bash:**
```bash
# Cập nhật danh sách remote
git fetch --prune

# Checkout về main trước
git checkout main

# Xóa branch local
git branch -d TenBranch

# Nếu Git từ chối, dùng -D
git branch -D TenBranch
```

---

## 🔀 MERGE VÀ PULL REQUEST

### Khi main có code mới trong lúc đang code

> Cần kéo code mới từ `main` vào branch của mình trước khi tiếp tục.

**VS 2022:**
> Bước 1: Checkout sang `main` → **Git → Pull** để cập nhật main mới nhất  
> Bước 2: Checkout lại branch của bạn  
> Bước 3: **Git → Manage Branches** → click phải vào `main` → **Merge 'main' into 'TenBranchCủaBạn'**

**Git Bash:**
```bash
# Bước 1: Cập nhật main mới nhất
git checkout main
git pull origin main

# Bước 2: Quay lại branch của bạn
git checkout TenBranch

# Bước 3: Merge main vào branch
git merge main

# Nếu có conflict, resolve xong rồi:
git add .
git commit -m "Merge main into TenBranch"
```

---

### Resolve conflict

> Conflict xảy ra khi 2 người cùng sửa một chỗ trong file.

**VS 2022:**
> File conflict sẽ được đánh dấu đỏ trong Solution Explorer  
> Double click vào file → VS 2022 hiện giao diện so sánh 2 phiên bản  
> Chọn giữ phần nào → Save → Commit

**Git Bash:**
```bash
# Sau khi resolve conflict thủ công trong file
git add .
git commit -m "Resolve merge conflicts"
```

---

### Hủy merge (khi conflict quá phức tạp)

**Git Bash:**
```bash
git merge --abort
```

---

## 🔧 TROUBLESHOOTING

### Lỡ commit vào main local

**Vấn đề:** Commit vào main nhưng không push được (vì branch protection)

**VS 2022:**
> **Git → New Branch** → tạo branch mới từ vị trí hiện tại  
> Sau đó reset main: **Git → Manage Branches** → click phải `main` → **Reset** → **Hard**

**Git Bash:**
```bash
# Tạo branch mới từ vị trí hiện tại
git checkout -b feature/ten-tinh-nang

# Quay lại main và reset
git checkout main
git reset --hard origin/main

# Push branch mới
git checkout feature/ten-tinh-nang
git push origin feature/ten-tinh-nang
```

---

### Không thể push vì remote có code mới

**Lỗi:** `Can't push refs to remote. Try running Pull first`

**VS 2022:**
> **Git → Pull** trước → resolve conflict nếu có → Push lại

**Git Bash:**
```bash
git pull origin main
# Resolve conflict nếu có
git add .
git commit -m "Resolve conflicts"
git push
```

---

### Undo thay đổi chưa commit

**VS 2022:**
> Trong panel **Git Changes** → click phải file → **Discard Changes**

**Git Bash:**
```bash
# Chưa stage
git restore .

# Đã stage, muốn hủy stage giữ code
git restore --staged .

# Đã stage, muốn xóa hết
git restore --staged .
git restore .
```

---

### Undo commit đã tạo (chưa push)

**Git Bash:**
```bash
# Undo commit, giữ changes ở staged
git reset --soft HEAD~1

# Undo commit, giữ changes ở unstaged  
git reset HEAD~1

# Undo commit, XÓA HẾT changes ⚠️ NGUY HIỂM
git reset --hard HEAD~1
```

---

### Cất code tạm thời (Stash)

> Dùng khi đang code dở mà cần Pull code mới về.

**VS 2022:**
> **Git → Commit or Stash** → nhấn **Stash All**  
> Để lấy lại: **Git Changes → Stashes** → click phải → **Pop**

**Git Bash:**
```bash
# Cất code
git stash

# Xem danh sách stash
git stash list

# Lấy lại code
git stash pop

# Lấy lại nhưng giữ stash
git stash apply
```

---

### Xóa branch remote đã bị xóa vẫn còn hiện ở local

**VS 2022:**
> **Git → Fetch** → VS 2022 tự nhận ra branch đã bị xóa

**Git Bash:**
```bash
git fetch --prune
```

---

## 📝 COMMIT BEST PRACTICES

### Commit message tốt

✅ **Tốt:**
```
Add Board class with basic draw functions
Fix win condition check for diagonal
Update GameLoop to handle pause state
Refactor AI logic for better performance
```

❌ **Không tốt:**
```
update
fix
abc
code mới
```

### Template commit message

```
# Thêm tính năng
Add <tên tính năng>

# Sửa bug
Fix <mô tả bug>

# Cập nhật
Update <cái gì> to <lý do>

# Cải tiến code
Refactor <cái gì> for <lý do>
```

---

## 🚨 QUAN TRỌNG

### ⚠️ KHÔNG BAO GIỜ:

- ❌ Code trực tiếp trên `main`
- ❌ Force push lên `main`
- ❌ Xóa thư mục `libs/SFML/`, `assets/`, `CaroGame/`
- ❌ Pull khi code đang làm dở chưa commit hoặc stash
- ❌ Push file `.exe`, thư mục `x64/`, `Debug/`

### ✅ LUÔN LUÔN:

- ✅ Pull `main` về trước khi bắt đầu làm việc
- ✅ Tạo branch mới cho mỗi tính năng
- ✅ Commit thường xuyên với message rõ ràng
- ✅ Test code trước khi push
- ✅ Chờ ít nhất 1 người review trước khi merge

---

## 🔗 LIÊN KẾT HỮU ÍCH

- [Git Documentation](https://git-scm.com/doc)
- [GitHub Guides](https://guides.github.com/)
- [SFML Documentation](https://www.sfml-dev.org/documentation/2.6.2/)

---
