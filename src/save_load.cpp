#include "save_load.h"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

static const std::string SAVE_DIR = "../saves/";
static const std::string LIST_FILE = "../saves/Gamelist.txt";
static const std::string SAVE_EXT = ".txt";

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

    f.close();
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

    f.close();
    return true;
}

// ============================================================
// QUAN LY DANH SACH SAVE
// ============================================================
int saveGetList(std::string saveList[], int maxCount) {
    ensureSaveDir();
    std::ifstream f(LIST_FILE);
    if (!f.is_open()) return 0;

    int count = 0;
    std::string line;
    while (count < maxCount && std::getline(f, line)) {
        if (!line.empty()) saveList[count++] = line;
    }
    return count;
}

bool saveAddToList(const std::string& filename) {
    if (saveFileExists(filename)) return true; // Da co roi
    if (saveCountFiles() >= MAX_SAVE_FILES) return false;

    ensureSaveDir();
    std::ofstream f(LIST_FILE, std::ios::app);
    if (!f.is_open()) return false;
    f << filename << "\n";
    return true;
}

bool saveDeleteFile(const std::string& filename) {
    // Doc danh sach hien tai
    std::string list[MAX_SAVE_FILES];
    int count = saveGetList(list, MAX_SAVE_FILES);

    // Viet lai danh sach, bo qua file can xoa
    std::ofstream f(LIST_FILE);
    if (!f.is_open()) return false;
    bool found = false;
    for (int i = 0; i < count; i++) {
        if (list[i] != filename) f << list[i] << "\n";
        else found = true;
    }
    f.close();

    // Xoa file thuc te
    try {
        fs::remove(SAVE_DIR + filename + SAVE_EXT);
    }
    catch (...) {}
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

    // Update danh sach
    std::string list[MAX_SAVE_FILES];
    int count = saveGetList(list, MAX_SAVE_FILES);
    std::ofstream f(LIST_FILE);
    for (int i = 0; i < count; i++) {
        f << (list[i] == oldName ? newName : list[i]) << "\n";
    }
    return true;
}

bool saveFileExists(const std::string& filename) {
    std::string list[MAX_SAVE_FILES];
    int count = saveGetList(list, MAX_SAVE_FILES);
    for (int i = 0; i < count; i++) {
        if (list[i] == filename) return true;
    }
    return false;
}

int saveCountFiles() {
    std::string list[MAX_SAVE_FILES];
    return saveGetList(list, MAX_SAVE_FILES);
}
