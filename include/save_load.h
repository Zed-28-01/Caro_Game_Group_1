#pragma once
#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "game_types.h"

// ============================================================
// LUU GAME
// ============================================================

// Luu toan bo trang thai game vao file
// filename: ten file (khong can duoi .txt, se tu them)
// Tra ve true neu luu thanh cong
bool saveGame(const GameState& state, const std::string& filename);

// ============================================================
// TAI GAME
// ============================================================

// Tai trang thai game tu file
// Tra ve true neu tai thanh cong, false neu file loi
bool loadGame(GameState& state, const std::string& filename);

// ============================================================
// QUAN LY DANH SACH SAVE
// ============================================================

// Doc danh sach ten cac file save tu Gamelist.txt
// Tra ve so luong file, dien vao mang saveList
int saveGetList(std::string saveList[], int maxCount);

// Them ten file vao danh sach
bool saveAddToList(const std::string& filename);

// Xoa file save va xoa khoi danh sach
bool saveDeleteFile(const std::string& filename);

// Doi ten file save
bool saveRenameFile(const std::string& oldName, const std::string& newName);

// Kiem tra file da ton tai trong danh sach chua
bool saveFileExists(const std::string& filename);

// Dem so file save hien tai
int saveCountFiles();

#endif // SAVE_LOAD_H
