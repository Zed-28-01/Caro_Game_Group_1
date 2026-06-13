#pragma once
#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "game_types.h"
#include <string>
#include <vector>
#include <unordered_map>

// ============================================================
// METADATA CACHE (V2 #28)
// ============================================================

// Thong tin tom tat cua 1 file save - parse 1 lan, cache lai
// de query O(1) (khong can mo + doc lai file).
struct SaveMetadata {
    std::string name;   // Ten file (khong co duoi .txt)
    int moveCount = 0;  // So nuoc da danh trong van
    int mode = 0;       // GameMode (0 = PvP, 1 = PvC)
    int style = 0;      // GameStyle (0 = Basic, 1 = Speed)
};

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

// Scan thu muc saves/ bang std::filesystem::directory_iterator,
// rebuild metadata cache, tra ve danh sach ten file (sorted alphabet).
// KHONG con gioi han so luong file (bo MAX_SAVE_FILES).
std::vector<std::string> saveScanFiles();

// Truy van metadata cua 1 file tu cache - O(1) (V2 #28).
// Tra ve nullptr neu file khong ton tai trong cache.
const SaveMetadata* saveGetMetadata(const std::string& filename);

// Xoa file save (xoa khoi disk + cache)
bool saveDeleteFile(const std::string& filename);

// Doi ten file save
bool saveRenameFile(const std::string& oldName, const std::string& newName);

// Kiem tra file da ton tai chua - O(1) qua cache (V2 #28)
bool saveFileExists(const std::string& filename);

// Dem so file save hien tai (size cua cache)
int saveCountFiles();

#endif // SAVE_LOAD_H
