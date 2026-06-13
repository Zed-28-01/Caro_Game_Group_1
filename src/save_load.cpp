#include "save_load.h"
#include "utils.h" // Fix R3: inBounds - validate toa do doc tu file save
#include <fstream>
#include <algorithm>
#include <filesystem>
#include <unordered_set> // cache ten file (g_saveNames) cho saveFileExists O(1)

namespace fs = std::filesystem;

static const std::string SAVE_DIR = "../saves/";
static const std::string SAVE_EXT = ".txt";

// V2 #28 (don §5.18): cache TEN cac file save trong RAM -> saveFileExists() query
// O(1) (khong mo + doc lai disk). Truoc la unordered_map<string,SaveMetadata> nhung
// 3 field mode/style/moveCount khong UI nao doc (preview da go o Sprint 3) -> rut gon
// con set ten file. Rebuild khi saveScanFiles() chay, hoac sau saveGame/saveDeleteFile.
static std::unordered_set<std::string> g_saveNames;
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

    // V2 #34: hero da chon (APPEND CUOI FILE - file V1/V2 cu thieu dong nay ->
    // loadGame se fallback Goku/Vegeta)
    f << state.heroP1 << " " << state.heroP2 << "\n";

    f.close();

    // V2 #28: cap nhat cache ten file ngay (khong can scan lai toan bo)
    g_saveNames.insert(filename);
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

    // Fix R1 (audit 12/06): moi duong save deu timerPause truoc khi ghi file
    // -> isRunning trong file LUON la 0. Neu giu nguyen, van Speed sau khi
    // load se dung dong ho vinh vien (khong dem nguoc, khong het gio, chuong
    // khong reo). Load la de choi tiep -> ep timer chay lai.
    if (state.style == STYLE_SPEED) {
        state.timer.isRunning = true;
        state.timer.turnAlarmFired = false; // re-arm chuong cho luot dang do
    }

    f >> state.moveCount;

    // Fix R3 (audit 12/06): validate du lieu doc tu file - file hong/sua tay
    // co the chua so rac. Nguy hiem nhat: moveCount qua lon lam vong doc lich
    // su ghi TRAN MANG moveHistory[225]. Sai -> return false, man Load hien
    // msgLoadError (co che bao loi co san).
    if (!f) return false;
    if (mode < 0 || mode > 1 || style < 0 || style > 1 || diff < 0 || diff > 3)
        return false;
    if (!inBounds(state.cursorRow, state.cursorCol)) return false;
    if (state.moveCount < 0 || state.moveCount > BOARD_SIZE * BOARD_SIZE)
        return false;

    // Ban co
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            f >> state.board[r][c].value;
            // Fix R3: o hop le chi co CELL_P1 (quan P1) / 0 (trong) / CELL_P2 (quan P2)
            if (state.board[r][c].value < CELL_P1 || state.board[r][c].value > CELL_P2)
                return false;
        }
    }
    if (!f) return false;

    // Lich su
    for (int i = 0; i < state.moveCount; i++) {
        f >> state.moveHistory[i].row
            >> state.moveHistory[i].col
            >> state.moveHistory[i].player;
        // Fix R3: toa do phai nam trong ban co, player chi co CELL_P1/CELL_P2
        if (!inBounds(state.moveHistory[i].row, state.moveHistory[i].col))
            return false;
        if (state.moveHistory[i].player != CELL_P1 && state.moveHistory[i].player != CELL_P2)
            return false;
    }
    if (!f) return false;

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
// SCAN THU MUC SAVE (V2 #29) - thay the Gamelist.txt manifest
// ============================================================
std::vector<std::string> saveScanFiles() {
    ensureSaveDir();
    g_saveNames.clear();

    // Thu thap (thoi gian sua, ten) de sort theo thoi gian
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
// QUAN LY FILE
// ============================================================
bool saveDeleteFile(const std::string& filename) {
    bool found = (g_saveNames.count(filename) > 0);
    try {
        fs::remove(SAVE_DIR + filename + SAVE_EXT);
    }
    catch (...) { return false; }
    g_saveNames.erase(filename); // cap nhat cache
    return found;
}

bool saveFileExists(const std::string& filename) {
    if (!g_scanned) saveScanFiles(); // lazy build neu chua scan lan nao
    return g_saveNames.count(filename) > 0;
}
