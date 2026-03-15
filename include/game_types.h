#pragma once
#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <mutex>
#include <thread>
#include <atomic>

// ============================================================
// HANG SO GAME (Game Constants)
// ============================================================

#define BOARD_SIZE      15          // Kich thuoc ban co 15x15
#define CELL_SIZE       40          // Kich thuoc moi o (pixel)
#define WINDOW_WIDTH    1280        // Chieu rong cua so
#define WINDOW_HEIGHT   720         // Chieu cao cua so
#define MAX_SAVE_FILES  15          // So file save toi da
#define WIN_COUNT       5           // So quan lien tiep de thang

// Thoi gian (giay)
#define MAX_GAME_TIME   300         // 5 phut moi van
#define MAX_TURN_TIME   15          // 15 giay moi luot

// Mau sac (SFML Colors) - Theme mac dinh
// Ban co the thay doi cac gia tri nay de tao theme khac
#define COLOR_BACKGROUND    sf::Color(245, 222, 179)    // Wheat - nen chinh
#define COLOR_BOARD_BG      sf::Color(222, 184, 135)    // BurlyWood - nen ban co
#define COLOR_GRID_LINE     sf::Color(139, 90, 43)      // Brown - duong ke
#define COLOR_PLAYER_X      sf::Color(220, 50, 50)      // Do - quan X
#define COLOR_PLAYER_O      sf::Color(30, 100, 200)     // Xanh - quan O
#define COLOR_HIGHLIGHT     sf::Color(255, 255, 0, 120) // Vang trong suot - o dang chon
#define COLOR_WIN_LINE      sf::Color(0, 200, 0)        // Xanh la - duong thang
#define COLOR_MENU_BG       sf::Color(44, 62, 80)       // Dark blue - nen menu
#define COLOR_MENU_TEXT      sf::Color(236, 240, 241)    // Trang - chu menu
#define COLOR_MENU_HOVER    sf::Color(52, 152, 219)     // Xanh sang - hover menu
#define COLOR_TIMER_BAR     sf::Color(46, 204, 113)     // Xanh la - timer bar day
#define COLOR_TIMER_LOW     sf::Color(231, 76, 60)      // Do - timer sap het

// ============================================================
// ENUM - Trang thai game
// ============================================================

// Trang thai man hinh hien tai
enum GameScreen {
    SCREEN_MAIN_MENU,       // Menu chinh
    SCREEN_MODE_SELECT,     // Chon che do: PvP / PvC
    SCREEN_DIFFICULTY,      // Chon do kho AI (chi PvC)
    SCREEN_STYLE_SELECT,    // Chon kieu choi: Basic / Speed
    SCREEN_INPUT_NAMES,     // Nhap ten nguoi choi
    SCREEN_PLAYING,         // Dang choi
    SCREEN_PAUSE_MENU,      // Menu tam dung (ESC)
    SCREEN_GAME_OVER,       // Ket thuc van
    SCREEN_SAVE,            // Man hinh luu game
    SCREEN_LOAD,            // Man hinh tai game
    SCREEN_SETTINGS,        // Cai dat
    SCREEN_HELP,            // Huong dan
    SCREEN_ABOUT            // Thong tin
};

// Che do choi
enum GameMode {
    MODE_PVP,               // Player vs Player
    MODE_PVC                // Player vs Computer
};

// Kieu choi
enum GameStyle {
    STYLE_BASIC,            // Khong gioi han thoi gian
    STYLE_SPEED             // Co gioi han thoi gian
};

// Do kho AI
enum BotDifficulty {
    BOT_EASY,               // Random + chan co ban
    BOT_MEDIUM,             // Heuristic scoring
    BOT_HARD,               // Minimax depth 3
    BOT_EXPERT              // Minimax depth 4 + move ordering
};

// Ket qua van dau
enum GameResult {
    RESULT_NONE,            // Chua ket thuc
    RESULT_PLAYER1_WIN,     // Nguoi choi 1 thang
    RESULT_PLAYER2_WIN,     // Nguoi choi 2 thang
    RESULT_DRAW             // Hoa
};

// Ngon ngu
enum Language {
    LANG_VIETNAMESE,
    LANG_ENGLISH
};

// ============================================================
// STRUCT - Cau truc du lieu
// ============================================================

// Mot o tren ban co
struct Cell {
    int value;              // 0: trong, -1: Player 1 (X), 1: Player 2 (O)
};

// Mot nuoc di (dung cho undo va replay)
struct Move {
    int row, col;           // Vi tri tren ban co (0-14)
    int player;             // -1: Player 1, 1: Player 2
};

// Thong tin nguoi choi
struct Player {
    std::string name;       // Ten (toi da 15 ky tu)
    int moves;              // So nuoc da di trong van hien tai
    int totalWins;          // Tong so van thang
};

// Trang thai timer
struct TimerState {
    float gameTimeLeft;     // Thoi gian con lai cua van (giay)
    float turnTimeLeft;     // Thoi gian con lai cua luot (giay)
    bool isRunning;         // Timer co dang chay khong
};

// Vi tri 5 quan thang (dung de ve hieu ung)
struct WinLine {
    int positions[WIN_COUNT][2];    // [i][0] = row, [i][1] = col
    int count;                      // So quan thang thuc te (co the > 5)
};

// Toan bo trang thai cua 1 van game (dung cho save/load)
struct GameState {
    Cell board[BOARD_SIZE][BOARD_SIZE]; // Ban co
    Player player1;
    Player player2;
    bool isPlayer1Turn;                // true = luot Player 1
    int cursorRow, cursorCol;          // Vi tri con tro hien tai
    GameMode mode;
    GameStyle style;
    BotDifficulty difficulty;
    TimerState timer;
    Move moveHistory[BOARD_SIZE * BOARD_SIZE]; // Lich su nuoc di (mang tinh)
    int moveCount;                             // So nuoc di da thuc hien
};

// ============================================================
// STRUCT - SFML Resources (tai nguyen do hoa/am thanh)
// ============================================================

// Tat ca font, texture, sound duoc load 1 lan va truyen qua tham chieu
struct GameResources {
    sf::Font mainFont;          // Font chinh cho menu, UI
    sf::Font titleFont;         // Font cho tieu de lon

    sf::Texture backgroundTex;  // Hinh nen
    sf::Texture boardTex;       // Texture ban co (neu co)
    sf::Texture xPieceTex;      // Texture quan X (neu co)
    sf::Texture oPieceTex;      // Texture quan O (neu co)

    sf::SoundBuffer moveSfx;    // Am thanh di chuyen cursor
    sf::SoundBuffer placeSfx;   // Am thanh dat quan
    sf::SoundBuffer winSfx;     // Am thanh thang
    sf::SoundBuffer drawSfx;    // Am thanh hoa
    sf::SoundBuffer menuSfx;    // Am thanh menu select

    sf::Music bgMusic;          // Nhac nen
};

#endif // GAME_TYPES_H
