#pragma once
#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "game_types.h"

// ============================================================
// STRUCT CHUA TAT CA VAN BAN
// ============================================================

struct TextStrings {
    // Menu chinh
    std::string title;
    std::string newGame;
    std::string loadGame;
    std::string settings;
    std::string help;
    std::string about;
    std::string exitGame;

    // Chon che do
    std::string chooseMode;
    std::string pvpMode;
    std::string pvcMode;

    // Chon do kho
    std::string chooseDifficulty;
    std::string easy;
    std::string medium;
    std::string hard;
    std::string expert;

    // Chon kieu choi
    std::string chooseStyle;
    std::string basicStyle;
    std::string speedStyle;

    // Nhap ten
    std::string enterName1;
    std::string enterName2;
    std::string nameError;
    std::string nameDuplicate;

    // Gameplay
    std::string turn;           // "Luot cua: "
    std::string moves;          // "So nuoc: "
    std::string wins;           // "Thang: "
    std::string hintText;       // "Nhan H de xem goi y"
    std::string undoText;       // "Nhan Z de huy nuoc"

    // Ket qua
    std::string playerWin;      // "{name} da thang!"
    std::string drawText;       // "Hoa!"
    std::string continueText;   // "Choi tiep?"
    std::string yes;
    std::string no;

    // Save/Load
    std::string enterSaveName;
    std::string saveSuccess;
    std::string loadSuccess;
    std::string fileNotFound;
    std::string fileExists;
    std::string maxSaveReached;
    std::string deleteFile;
    std::string renameFile;

    // Settings
    std::string language;
    std::string bgmVolume;
    std::string sfxToggle;
    std::string on;
    std::string off;
    std::string back;

    // Pause menu
    std::string resume;
    std::string saveGame;
    std::string returnMenu;

    // Timer
    std::string gameTime;       // "Thoi gian: "
    std::string turnTime;       // "Luot: "
    std::string timeUp;         // "Het gio!"

    // Guide (huong dan dieu khien)
    std::string guideMove;      // "W/A/S/D: Di chuyen"
    std::string guidePlace;     // "Enter: Dat quan"
    std::string guideUndo;      // "Z: Huy nuoc"
    std::string guideHint;      // "H: Goi y (PvC)"
    std::string guidePause;     // "ESC: Tam dung"
    std::string guideSave;      // "L: Luu game"

    // Ten ngon ngu (hien thi trong Settings)
    std::string langVietnamese; // "Tieng Viet" / "Vietnamese"
    std::string langEnglish;    // "Tieng Anh" / "English"

    // Hint hint o cuoi man hinh Save/Load
    std::string saveHintBottom; // "Enter: Luu | ESC: Quay lai"
    std::string saveHintMouse;  // "Click trai: chon ten | Click phai: xoa file"
    std::string loadHintBottom; // "Enter: Tai | Del: Xoa | ESC: Quay lai"
    std::string loadHintMouse;  // "Click trai: chon | Double-click: tai | Click phai: xoa"
    std::string settingsHint;   // "Volume: click ben trai giam, ben phai tang"

    // Input name screen
    std::string inputNameHintPvP;  // "Tab: Chuyen o | Enter: Xac nhan"
    std::string inputNameHintPvC;  // "Enter: Xac nhan"
    std::string botName;           // "May" / "Computer"

    // Panel + timer (gameplay)
    std::string yourTurn;          // ">>> LUOT CUA BAN <<<"
    std::string turnTimeBar;       // "THOI GIAN LUOT"
    std::string gameTimeLabel;     // "Thoi gian van: "
    std::string defeatText;        // "DEFEAT" / "THUA CUOC"
    std::string botThinking;       // "Bot dang suy nghi..." / "Bot is thinking..."

    // Save/Load messages
    std::string msgSaveOK;         // "Luu thanh cong!"
    std::string msgSaveError;      // "Loi luu file!"
    std::string msgFileExists;     // "File da ton tai!"
    std::string msgMaxFiles;       // "Da dat gioi han 15 file!"
    std::string msgFileDeleted;    // "Da xoa file!"
    std::string msgLoadError;      // "Loi tai file!"
};

// ============================================================
// HAM
// ============================================================

// Lay bo van ban theo ngon ngu
TextStrings langGetText(Language lang);

// Lay ngon ngu hien tai (bien global)
Language langGetCurrent();

// Dat ngon ngu hien tai
void langSetCurrent(Language lang);

// Chuyen doi ngon ngu (toggle)
void langToggle();

#endif // LANGUAGE_H
