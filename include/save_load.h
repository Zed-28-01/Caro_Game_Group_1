#pragma once
#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "game_types.h"
#include <string>
#include <vector>

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
// QUAN LY DANH SACH SAVE (V2 #29 - directory scan, bo Gamelist.txt)
// ============================================================

// Scan thu muc saves/ bang std::filesystem::directory_iterator, rebuild cache ten
// file, tra ve danh sach ten (sort theo thoi gian sua - moi nhat len dau).
// KHONG con gioi han so luong file (bo MAX_SAVE_FILES).
std::vector<std::string> saveScanFiles();

// Xoa file save (xoa khoi disk + cache)
bool saveDeleteFile(const std::string& filename);

// Kiem tra file da ton tai chua - O(1) qua cache (V2 #28)
bool saveFileExists(const std::string& filename);

#endif // SAVE_LOAD_H
