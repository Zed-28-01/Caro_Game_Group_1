#include "save_load.h"
#include "utils.h"
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <unordered_set>

namespace fs = std::filesystem;

static const std::string SAVE_DIR = "../saves/";
static const std::string SAVE_EXT = ".txt";

static std::unordered_set<std::string> g_saveNames;
static bool g_scanned = false;

static bool isReservedFile(const std::string& stem) {
    return stem == "settings" || stem == "Gamelist";
}

static void ensureSaveDir() {
    if (!fs::exists(SAVE_DIR)) {
        fs::create_directories(SAVE_DIR);
    }
}

bool saveGame(const GameState& state, const std::string& filename) {
    ensureSaveDir();
    std::ofstream f(SAVE_DIR + filename + SAVE_EXT);
    if (!f.is_open()) return false;

    f << state.player1.name << "\n";
    f << state.player1.moves << " " << state.player1.totalWins << "\n";
    f << state.player2.name << "\n";
    f << state.player2.moves << " " << state.player2.totalWins << "\n";

    f << (state.isPlayer1Turn ? 1 : 0) << "\n";
    f << state.firstPlayerOfRound << "\n";
    f << state.cursorRow << " " << state.cursorCol << "\n";
    f << (int)state.mode << " " << (int)state.style << " " << (int)state.difficulty << "\n";
    f << state.timer.gameTimeLeftP1 << " " << state.timer.gameTimeLeftP2 << " "
        << state.timer.turnTimeLeft << " "
        << (state.timer.isRunning ? 1 : 0) << "\n";
    f << state.moveCount << "\n";

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            f << state.board[r][c].value;
            if (c < BOARD_SIZE - 1) f << " ";
        }
        f << "\n";
    }

    for (int i = 0; i < state.moveCount; i++) {
        f << state.moveHistory[i].row << " "
            << state.moveHistory[i].col << " "
            << state.moveHistory[i].player << "\n";
    }

    f << state.heroP1 << " " << state.heroP2 << "\n";

    f.close();

    g_saveNames.insert(filename);
    return true;
}

bool loadGame(GameState& state, const std::string& filename) {
    std::ifstream f(SAVE_DIR + filename + SAVE_EXT);
    if (!f.is_open()) return false;

    std::getline(f, state.player1.name);
    f >> state.player1.moves >> state.player1.totalWins;
    f.ignore();

    std::getline(f, state.player2.name);
    f >> state.player2.moves >> state.player2.totalWins;
    f.ignore();

    int turn, mode, style, diff, isRunning;
    f >> turn;          state.isPlayer1Turn = (turn == 1);
    f >> state.firstPlayerOfRound;
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

    if (state.style == STYLE_SPEED) {
        state.timer.isRunning = true;
        state.timer.turnAlarmFired = false;
    }

    f >> state.moveCount;

    if (!f) return false;
    if (mode < 0 || mode > 1 || style < 0 || style > 1 || diff < 0 || diff > 3)
        return false;
    if (!inBounds(state.cursorRow, state.cursorCol)) return false;
    if (state.moveCount < 0 || state.moveCount > BOARD_SIZE * BOARD_SIZE)
        return false;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            f >> state.board[r][c].value;
            if (state.board[r][c].value < CELL_P1 || state.board[r][c].value > CELL_P2)
                return false;
        }
    }
    if (!f) return false;

    for (int i = 0; i < state.moveCount; i++) {
        f >> state.moveHistory[i].row
            >> state.moveHistory[i].col
            >> state.moveHistory[i].player;
        if (!inBounds(state.moveHistory[i].row, state.moveHistory[i].col))
            return false;
        if (state.moveHistory[i].player != CELL_P1 && state.moveHistory[i].player != CELL_P2)
            return false;
    }
    if (!f) return false;

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

std::vector<std::string> saveScanFiles() {
    ensureSaveDir();
    g_saveNames.clear();

    std::vector<std::pair<fs::file_time_type, std::string>> items;

    try {
        for (const auto& entry : fs::directory_iterator(SAVE_DIR)) {
            if (!entry.is_regular_file()) continue;
            if (entry.path().extension() != SAVE_EXT) continue;

            std::string stem = entry.path().stem().string();
            if (isReservedFile(stem)) continue;

            g_saveNames.insert(stem);

            fs::file_time_type wt{};
            try { wt = entry.last_write_time(); } catch (...) {}
            items.push_back({ wt, stem });
        }
    }
    catch (...) {}

    std::sort(items.begin(), items.end(),
        [](const auto& a, const auto& b) { return a.first > b.first; });

    std::vector<std::string> names;
    names.reserve(items.size());
    for (const auto& it : items) names.push_back(it.second);

    g_scanned = true;
    return names;
}

bool saveDeleteFile(const std::string& filename) {
    bool found = (g_saveNames.count(filename) > 0);
    try {
        fs::remove(SAVE_DIR + filename + SAVE_EXT);
    }
    catch (...) { return false; }
    g_saveNames.erase(filename);
    return found;
}

bool saveFileExists(const std::string& filename) {
    if (!g_scanned) saveScanFiles();
    return g_saveNames.count(filename) > 0;
}
