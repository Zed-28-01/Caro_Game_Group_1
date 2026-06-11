#pragma once
#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <stack> // dùng cho undo nhưng thay phương án thành mảng tính cho dễ implement
#include <string>
#include <vector> // phần lớn dùng mảng tĩnh hết

// hiện đã không dùng -> chỉ trên một main chính
#include <atomic>
#include <mutex>
#include <thread>

// ============================================================
// HANG SO GAME (Game Constants)
// ============================================================

#define BOARD_SIZE 15 // Kich thuoc ban co 15x15
#define CELL_SIZE 40 // Mỗi ô trên bàn cờ được vẽ với kích thước 40 x 40 (pixel)
// Mối quan hệ: Kích thước hiển thị thực tế của ô cờ 40 pixel sẽ phụ thuộc vào
// độ phân giải và kích thước vật lý. Trên màn hình 16 inch độ phân giải Full
// HD, ô 40px nhìn sẽ to. Nhưng nếu mang game này sang màn hình 16 inch độ phân
// giải 4K, ô 40px nhìn sẽ cực kỳ nhỏ bé (vì trên 4K các hạt pixel bị nhồi nhét
// rất dày đặc).
#define WINDOW_WIDTH 1280 // Chieu rong cua so
#define WINDOW_HEIGHT 720 // Chieu cao cua so
// V2 #29: bo gioi han so file save - dung directory_iterator scan thay vi
// Gamelist.txt manifest. So file gio chi gioi han boi o dia.
#define WIN_COUNT 5       // So quan lien tiep de thang

// Thoi gian (giay)
#define MAX_GAME_TIME 600 // 10 phut moi van
#define MAX_TURN_TIME 20  // 20 giay moi luot

// Mau sac (SFML Colors) - Theme mac dinh
#define COLOR_BACKGROUND sf::Color(245, 222, 179) // Wheat - nen chinh
#define COLOR_BOARD_BG sf::Color(222, 184, 135)   // BurlyWood - nen ban co
#define COLOR_GRID_LINE sf::Color(139, 90, 43)    // Brown - duong ke
#define COLOR_PLAYER_X sf::Color(220, 50, 50)     // Do - quan X
#define COLOR_PLAYER_O sf::Color(30, 100, 200)    // Xanh - quan O
#define COLOR_HIGHLIGHT                                                        \
  sf::Color(255, 255, 0, 120)               // Vang trong suot - o dang chon
#define COLOR_WIN_LINE sf::Color(0, 200, 0) // Xanh la - duong thang
#define COLOR_MENU_BG sf::Color(44, 62, 80) // Dark blue - nen menu
#define COLOR_MENU_TEXT sf::Color(236, 240, 241) // Trang - chu menu
#define COLOR_MENU_HOVER sf::Color(52, 152, 219) // Xanh sang - hover menu
#define COLOR_TIMER_BAR sf::Color(46, 204, 113)  // Xanh la - timer bar day
#define COLOR_TIMER_LOW sf::Color(231, 76, 60)   // Do - timer sap het

// ============================================================
// HANG SO LAYOUT UI (CHIA SE GIUA render.cpp VA menu.cpp)
// Khi redesign UI: chi can sua o day, ca 2 file kia tu khop
// Don vi su dung là pixel
// ============================================================

// Menu chuan (Main/Mode/Difficulty/Style) - items o giua man hinh
#define UI_MENU_TITLE_Y 120.f    // Y cua tieu de
#define UI_MENU_START_Y 280.f    // Y cua item dau tien
#define UI_MENU_STEP 60.f        // Khoang cach giua cac items
#define UI_MENU_HALF_WIDTH 200.f // Nua chieu rong cho hit-test chuot
#define UI_MENU_HALF_HEIGHT 22.f // Nua chieu cao cho hit-test

// Pause Menu (overlay khi nhan ESC trong game)
#define UI_PAUSE_TITLE_Y 240.f
#define UI_PAUSE_START_Y 340.f
#define UI_PAUSE_STEP 55.f

// Settings (Volume row co the rong hon cho de click)
#define UI_SETTINGS_TITLE_Y 100.f
#define UI_SETTINGS_START_Y 250.f
#define UI_SETTINGS_STEP 60.f
#define UI_SETTINGS_HALF_WIDTH 300.f

// Save Screen
#define UI_SAVE_TITLE_Y 60.f
#define UI_SAVE_INPUT_Y 130.f      // Y cua o nhap ten
#define UI_SAVE_LIST_START_Y 200.f // Y cua file dau tien trong list

// Load Screen
#define UI_LOAD_TITLE_Y 60.f
#define UI_LOAD_LIST_START_Y 150.f

// Chung cho ca Save va Load list
#define UI_LIST_STEP 35.f // Khoang cach giua cac file
#define UI_LIST_HALF_WIDTH 250.f
#define UI_LIST_HALF_HEIGHT 15.f
// V2 #30: so file hien thi cung luc (scrollable list). Neu nhieu hon thi cuon.
#define UI_LIST_VISIBLE 10

// ============================================================
// GAMEPLAY LAYOUT - VI TRI BAN CO + PANEL + TIMER
// ============================================================

// Ban co (goc trai-tren)
#define UI_BOARD_OFFSET_X 40.f
#define UI_BOARD_OFFSET_Y 60.f

// Panel nguoi choi (ben phai ban co)
#define UI_PANEL_GAP_LEFT 40.f    // Khoang cach ban co → panel
#define UI_PANEL_GAP_RIGHT 20.f   // Khoang cach panel → mep cua so
#define UI_PANEL_BOX_Y_START 60.f // Y cua box player 1 dau tien
#define UI_PANEL_BOX_HEIGHT 170.f // Chieu cao moi box player
#define UI_PANEL_BOX_STEP 190.f   // Khoang cach giua 2 box (gap 20)

// Timer (chi che do SPEED)
#define UI_TIMER_BAR_Y 450.f // Y cua thanh tien trinh timer luot
#define UI_TIMER_BAR_HEIGHT 24.f
#define UI_BOT_THINKING_Y 505.f // Y cua text "Bot dang suy nghi..." (PvC + Speed mode)

// Man hinh Game Over - text "Player X wins!" + nut Yes/No
#define UI_GAMEOVER_START_Y 550.f    // Y cua text ket qua
#define UI_GAMEOVER_STATS_DY 40.f    // Cach text ket qua bao nhieu (cho stats)
#define UI_GAMEOVER_QUESTION_DY 75.f // Cach text ket qua (cho "Choi tiep?")
#define UI_GAMEOVER_BTN_DY 110.f     // Cach text ket qua (cho nut Yes/No)
#define UI_GAMEOVER_BTN_GAP_X 95.f   // Khoang cach Yes/No tu tam panel (gian de glow khong de nhau)
#define UI_GAMEOVER_BTN_HALF_W 55.f  // Half-width cho hit-test (khop be rong pill)
#define UI_GAMEOVER_BTN_HALF_H 22.f  // Half-height cho hit-test

// ============================================================
// ENUM - Trang thai game
// ============================================================

// Trang thai man hinh hien tai
enum GameScreen {
  SCREEN_MAIN_MENU,    // Menu chinh
  SCREEN_MODE_SELECT,  // Chon che do: PvP / PvC
  SCREEN_DIFFICULTY,   // Chon do kho AI (chi PvC)
  SCREEN_STYLE_SELECT, // Chon kieu choi: Basic / Speed
  SCREEN_INPUT_NAMES,  // Nhap ten nguoi choi
  SCREEN_PLAYING,      // Dang choi
  SCREEN_PAUSE_MENU,   // Menu tam dung (ESC)
  SCREEN_GAME_OVER,    // Ket thuc van
  SCREEN_SAVE,         // Man hinh luu game
  SCREEN_LOAD,         // Man hinh tai game
  SCREEN_SETTINGS,     // Cai dat
  SCREEN_HELP,         // Huong dan
  SCREEN_ABOUT         // Thong tin
};

// Che do choi
enum GameMode {
  MODE_PVP, // Player vs Player
  MODE_PVC  // Player vs Computer
};

// Kieu choi
enum GameStyle {
  STYLE_BASIC, // Khong gioi han thoi gian
  STYLE_SPEED  // Co gioi han thoi gian
};

// Do kho AI
enum BotDifficulty {
  BOT_EASY,   // Random + chan co ban
  BOT_MEDIUM, // Heuristic scoring
  BOT_HARD,   // Minimax depth 3
  BOT_EXPERT  // Minimax depth 4 + move ordering
};

// Ket qua van dau
enum GameResult {
  RESULT_NONE,        // Chua ket thuc
  RESULT_PLAYER1_WIN, // Nguoi choi 1 thang
  RESULT_PLAYER2_WIN, // Nguoi choi 2 thang
  RESULT_DRAW         // Hoa
};

// Ngon ngu
enum Language { LANG_VIETNAMESE, LANG_ENGLISH };

// ============================================================
// STRUCT - Cau truc du lieu
// ============================================================

// Mot o tren ban co
// Default 0 (o trong) - tranh warning C26495 (uninitialized member)
struct Cell {
  int value = 0; // 0: trong, -1: Player 1 (X), 1: Player 2 (O)
};

// Mot nuoc di (dung cho undo va replay)
// Default zero-init - tranh warning C26495
struct Move {
  int row = 0, col = 0; // Vi tri tren ban co (0-14)
  int player = 0;       // -1: Player 1, 1: Player 2
};

// Thong tin nguoi choi
// Default zero-init - tranh warning C26495
struct Player {
  std::string name;   // Ten (toi da 15 ky tu) - std::string co default empty
  int moves = 0;      // So nuoc da di trong van hien tai
  int totalWins = 0;  // Tong so van thang
};

// Trang thai timer (chess-clock style: moi nguoi co thoi gian rieng)
// Default zero-init - tranh warning C26495
struct TimerState {
  float gameTimeLeftP1 = 0.f; // Thoi gian van con lai cua Player 1 (giay)
  float gameTimeLeftP2 = 0.f; // Thoi gian van con lai cua Player 2 (giay)
  float turnTimeLeft = 0.f;   // Thoi gian con lai cua luot hien tai (giay)
  bool isRunning = false;     // Timer co dang chay khong
};

// Vi tri 5 quan thang (dung de ve hieu ung)
// Default zero-init - tranh warning C26495
struct WinLine {
  int positions[WIN_COUNT][2] = {}; // [i][0] = row, [i][1] = col
  int count = 0;                    // So quan thang thuc te (co the > 5)
};

// Toan bo trang thai cua 1 van game (dung cho save/load)
// Default values cho moi field - tranh warning C26495 va dam bao safe
// neu code quen goi boardInit/boardResetAll truoc khi doc
struct GameState {
  Cell board[BOARD_SIZE][BOARD_SIZE] = {}; // Ban co (zero-init het)
  Player player1;                          // Default-construct
  Player player2;                          // Default-construct
  bool isPlayer1Turn = true;               // Mac dinh P1 di truoc
  int cursorRow = BOARD_SIZE / 2;          // Mac dinh cursor o tam
  int cursorCol = BOARD_SIZE / 2;
  GameMode mode = MODE_PVP;                // Mac dinh PvP
  GameStyle style = STYLE_BASIC;           // Mac dinh Basic mode
  BotDifficulty difficulty = BOT_EASY;     // Mac dinh Easy
  TimerState timer;                        // Default-construct
  Move moveHistory[BOARD_SIZE * BOARD_SIZE] = {}; // Zero-init history
  int moveCount = 0;

  // V2: Ai di truoc van nay (1 = P1, 2 = P2)
  // Mac dinh 1. Sau khi ket thuc 1 van, set = id cua nguoi thua
  // (nguoi thua di truoc van sau, va luon danh ky hieu X)
  // Render logic dung field nay de swap X/O texture + icon
  int firstPlayerOfRound = 1;
};

// ============================================================
// STRUCT - SFML Resources (tai nguyen do hoa/am thanh)
// ============================================================

// Tat ca font, texture, sound duoc load 1 lan va truyen qua tham chieu
struct GameResources {
  sf::Font mainFont;  // Font chinh cho menu, UI
  sf::Font titleFont; // Font cho tieu de lon

  sf::Texture backgroundTex; // Hinh nen
  sf::Texture xPieceTex;     // Texture quan X (neu co)
  sf::Texture oPieceTex;     // Texture quan O (neu co)

  // Mascot textures - V2: moi nhan vat chi 1 tu the (idle).
  // Thang/thua the hien bang banner + hieu ung code (tint toi loser), khong doi tu the.
  sf::Texture mascotP1Idle; // Player 1
  sf::Texture mascotP2Idle; // Player 2

  // UI decoration textures (style polish)
  sf::Texture logoCaroTex;     // Logo "CARO" sticker cho Main Menu
  sf::Texture bannerWinTex;    // Banner "VICTORY" cho winner
  sf::Texture bannerDefeatTex; // Banner "DEFEAT" cho loser
  sf::Texture buttonFrameTex;  // Khung bo tron cho menu items

  sf::SoundBuffer moveSfx;  // Am thanh di chuyen cursor
  sf::SoundBuffer placeSfx; // Am thanh dat quan
  sf::SoundBuffer winSfx;   // Am thanh thang
  sf::SoundBuffer drawSfx;  // Am thanh hoa
  sf::SoundBuffer menuSfx;  // Am thanh menu select

  sf::Music bgMusic; // Nhac nen
};

#endif // GAME_TYPES_H
