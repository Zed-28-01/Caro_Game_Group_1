#include "save_load.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

static const std::string SAVE_DIR = "../saves/";
static const std::string SAVE_EXT = ".txt";

// V2 #28: Cache metadata trong RAM. Key = ten file (khong .txt).
// Build lai moi khi saveScanFiles() chay (vao man hinh Save/Load),
// hoac sau khi saveGame/saveDeleteFile/saveRenameFile thay doi disk.
static std::unordered_map<std::string, SaveMetadata> g_metaCache;
static bool g_scanned = false;

// Cac file KHONG phai save game (bo qua khi scan)
static bool isReservedFile(const std::string& stem) {
    return stem == "settings" || stem == "Gamelist";
}

// Tao thu muc saves/ neu chua co
static void ensureSaveDir() {
    if (!fs::exists(SAVE_DIR)) {
        fs::create_directories(SAVE_DIR);
    }
}

// ============================================================
// LUU GAME
// ============================================================
bool saveGame(const GameState& state, const std::string& filename) {
    ensureSaveDir();
    std::ofstream f(SAVE_DIR + filename + SAVE_EXT);
    if (!f.is_open()) return false;

    // Thong tin nguoi choi
    f << state.player1.name << "\n";
    f << state.player1.moves << " " << state.player1.totalWins << "\n";
    f << state.player2.name << "\n";
    f << state.player2.moves << " " << state.player2.totalWins << "\n";

    // Trang thai van
    f << (state.isPlayer1Turn ? 1 : 0) << "\n";
    f << state.firstPlayerOfRound << "\n"; // V2: ai di truoc van nay (1 hoac 2)
    f << state.cursorRow << " " << state.cursorCol << "\n";
    f << (int)state.mode << " " << (int)state.style << " " << (int)state.difficulty << "\n";
    f << state.timer.gameTimeLeftP1 << " " << state.timer.gameTimeLeftP2 << " "
        << state.timer.turnTimeLeft << " "
        << (state.timer.isRunning ? 1 : 0) << "\n";
    f << state.moveCount << "\n";

    // Ban co
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            f << state.board[r][c].value;
            if (c < BOARD_SIZE - 1) f << " ";
        }
        f << "\n";
    }

    // Lich su nuoc di
    for (int i = 0; i < state.moveCount; i++) {
        f << state.moveHistory[i].row << " "
            << state.moveHistory[i].col << " "
            << state.moveHistory[i].player << "\n";
    }

    // V2 #34: hero da chon (APPEND CUOI FILE de khong xo lech header -
    // parseMetadata van doc dung; file V1/V2 cu thieu dong nay -> load fallback)
    f << state.heroP1 << " " << state.heroP2 << "\n";

    f.close();

    // V2 #28: cap nhat cache ngay (khong can scan lai toan bo)
    SaveMetadata meta;
    meta.name = filename;
    meta.moveCount = state.moveCount;
    meta.mode = (int)state.mode;
    meta.style = (int)state.style;
    g_metaCache[filename] = meta;
    return true;
}

// ============================================================
// TAI GAME
// ============================================================
bool loadGame(GameState& state, const std::string& filename) {
    std::ifstream f(SAVE_DIR + filename + SAVE_EXT);
    if (!f.is_open()) return false;

    // Player 1
    std::getline(f, state.player1.name);
    f >> state.player1.moves >> state.player1.totalWins;
    f.ignore();

    // Player 2
    std::getline(f, state.player2.name);
    f >> state.player2.moves >> state.player2.totalWins;
    f.ignore();

    int turn, mode, style, diff, isRunning;
    f >> turn;          state.isPlayer1Turn = (turn == 1);
    f >> state.firstPlayerOfRound; // V2: ai di truoc van nay (1 hoac 2)
    // Bao ve: neu doc duoc gia tri rac (chac chan tu file save V1 cu),
    // fallback ve 1 (P1 di truoc - hanh vi giong V1)
    if (state.firstPlayerOfRound != 1 && state.firstPlayerOfRound != 2) {
        state.firstPlayerOfRound = 1;
    }
    f >> state.cursorRow >> state.cursorCol;
    f >> mode >> style >> diff;
    state.mode = (GameMode)mode;
    state.style = (GameStyle)style;
    state.difficulty = (BotDifficulty)diff;
    f >> state.timer.gameTimeLeftP1 >> state.timer.gameTimeLeftP2
      >> state.timer.turnTimeLeft >> isRunning;
    state.timer.isRunning = (isRunning == 1);
    f >> state.moveCount;

    // Ban co
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            f >> state.board[r][c].value;
        }
    }

    // Lich su
    for (int i = 0; i < state.moveCount; i++) {
        f >> state.moveHistory[i].row
            >> state.moveHistory[i].col
            >> state.moveHistory[i].player;
    }

    // V2 #34: hero da chon - dong cuoi file. File save cu khong co dong nay
    // -> doc fail -> fallback mac dinh Goku/Vegeta (hanh vi giong truoc V2 #34)
    int h1, h2;
    if (f >> h1 >> h2 && h1 >= 0 && h1 < 6 && h2 >= 0 && h2 < 6) {
        state.heroP1 = h1;
        state.heroP2 = h2;
    } else {
        state.heroP1 = 0;
        state.heroP2 = 1;
    }

    f.close();
    return true;
}

// ============================================================
// PARSE METADATA (V2 #28) - doc nhanh phan header de lay tom tat
// ============================================================
// Khop dung thu tu ghi trong saveGame():
//   line 1: ten P1            | line 6: firstPlayerOfRound
//   line 2: moves wins P1     | line 7: cursorRow cursorCol
//   line 3: ten P2            | line 8: mode style difficulty
//   line 4: moves wins P2     | line 9: timer (4 so)
//   line 5: isPlayer1Turn     | line 10: moveCount
static bool parseMetadata(const std::string& path, SaveMetadata& meta) {
    std::ifstream f(path);
    if (!f.is_open()) return false;

    std::string skip;
    for (int i = 0; i < 7; i++) std::getline(f, skip); // bo qua line 1..7

    int diff = 0;
    if (!(f >> meta.mode >> meta.style >> diff)) return false; // line 8
    std::getline(f, skip); // an phan con lai cua line 8
    std::getline(f, skip); // line 9 (timer)
    if (!(f >> meta.moveCount)) return false; // line 10
    return true;
}

// ============================================================
// SCAN THU MUC SAVE (V2 #29) - thay the Gamelist.txt manifest
// ============================================================
std::vector<std::string> saveScanFiles() {
    ensureSaveDir();
    g_metaCache.clear();

    // Thu thap (thoi gian sua, ten) de sort theo thoi gian
    std::vector<std::pair<fs::file_time_type, std::string>> items;

    try {
        for (const auto& entry : fs::directory_iterator(SAVE_DIR)) {
            if (!entry.is_regular_file()) continue;
            if (entry.path().extension() != SAVE_EXT) continue;

            std::string stem = entry.path().stem().string();
            if (isReservedFile(stem)) continue;

            SaveMetadata meta;
            meta.name = stem;
            // Best-effort: neu parse loi (file rac) van giu ten, metadata = 0
            parseMetadata(entry.path().string(), meta);
            g_metaCache[stem] = meta;

            fs::file_time_type wt{};
            try { wt = entry.last_write_time(); } catch (...) {}
            items.push_back({ wt, stem });
        }
    }
    catch (...) {
        // Thu muc loi - tra ve nhung gi da scan duoc
    }

    // Sort theo thoi gian sua file: moi nhat len dau (giam dan)
    std::sort(items.begin(), items.end(),
        [](const auto& a, const auto& b) { return a.first > b.first; });

    std::vector<std::string> names;
    names.reserve(items.size());
    for (const auto& it : items) names.push_back(it.second);

    g_scanned = true;
    return names;
}

// ============================================================
// QUERY METADATA O(1) (V2 #28)
// ============================================================
const SaveMetadata* saveGetMetadata(const std::string& filename) {
    auto it = g_metaCache.find(filename);
    if (it == g_metaCache.end()) return nullptr;
    return &it->second;
}

// ============================================================
// QUAN LY FILE
// ============================================================
bool saveDeleteFile(const std::string& filename) {
    bool found = (g_metaCache.find(filename) != g_metaCache.end());
    try {
        fs::remove(SAVE_DIR + filename + SAVE_EXT);
    }
    catch (...) { return false; }
    g_metaCache.erase(filename); // cap nhat cache
    return found;
}

bool saveRenameFile(const std::string& oldName, const std::string& newName) {
    if (!saveFileExists(oldName)) return false;
    if (saveFileExists(newName)) return false;

    try {
        fs::rename(SAVE_DIR + oldName + SAVE_EXT,
            SAVE_DIR + newName + SAVE_EXT);
    }
    catch (...) { return false; }

    // Cap nhat cache: doi key, giu metadata
    auto it = g_metaCache.find(oldName);
    if (it != g_metaCache.end()) {
        SaveMetadata meta = it->second;
        meta.name = newName;
        g_metaCache.erase(it);
        g_metaCache[newName] = meta;
    }
    return true;
}

bool saveFileExists(const std::string& filename) {
    if (!g_scanned) saveScanFiles(); // lazy build neu chua scan lan nao
    return g_metaCache.find(filename) != g_metaCache.end();
}

int saveCountFiles() {
    if (!g_scanned) saveScanFiles();
    return (int)g_metaCache.size();
}
