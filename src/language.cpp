#include "language.h"
#include "game_types.h"

// Ngôn ngữ hiện tại (mặc định tiếng Anh)
static Language currentLanguage = LANG_ENGLISH;

// Lấy ngôn ngữ hiện tại
Language langGetCurrent() {
    return currentLanguage;
}

// Đặt ngôn ngữ hiện tại
void langSetCurrent(Language lang) {
    currentLanguage = lang;
}

// Chuyển đổi ngôn ngữ (toggle)
void langToggle() {
    currentLanguage = (currentLanguage == LANG_ENGLISH) ? LANG_VIETNAMESE : LANG_ENGLISH;
}

// Build noi bo: dien toan bo string vao t theo ngon ngu.
// Tach rieng de langGetText cache 2 ban static (3.1).
static void langBuild(Language lang, TextStrings& t) {
    if (lang == LANG_ENGLISH) {
        // Main menu
        t.title = "CARO GAME";
        t.newGame = "NEW GAME";
        t.loadGame = "LOAD GAME";
        t.settings = "SETTINGS";
        t.help = "HELP";
        t.about = "ABOUT";
        t.exitGame = "EXIT";

        // Chế độ
        t.chooseMode = "CHOOSE GAME MODE";
        t.pvpMode = "Player vs Player";
        t.pvcMode = "Player vs Computer";

        // Độ khó
        t.chooseDifficulty = "CHOOSE DIFFICULTY";
        t.easy = "Easy";
        t.medium = "Medium";
        t.hard = "Hard";
        t.expert = "Expert";

        // V2 #34: man chon nhan vat
        t.chooseCharacter = "CHOOSE YOUR FIGHTER";
        t.charPickerP1 = "Player 1 - pick your fighter";
        t.charPickerP2 = "Player 2 - pick your fighter";
        t.escBack = "ESC: Back";

        // Kiểu chơi
        t.chooseStyle = "CHOOSE PLAY STYLE";
        t.basicStyle = "Basic Mode";
        t.speedStyle = "Speed Mode";

        // Nhập tên
        t.enterName1 = "Enter Player 1 Name:";
        t.enterName2 = "Enter Player 2 Name:";
        t.nameDuplicate = "Name already taken!";

        // Gameplay
        t.moves = "Moves: ";
        t.wins = "Wins: ";

        // Kết quả
        t.playerWin = " wins!";
        t.drawText = "Draw!";
        t.continueText = "Play again?";
        t.yes = "Yes";
        t.no = "No";

        // Save/Load
        t.enterSaveName = "Enter save file name:";
        t.fileNotFound = "Empty list!";

        // Cài đặt
        t.language = "Language:";
        t.bgmVolume = "Music Volume:";
        t.sfxToggle = "Sound Effects:";
        t.on = "ON";
        t.off = "OFF";
        t.back = "BACK";

        // Pause menu
        t.resume = "RESUME";
        t.saveGame = "SAVE GAME";
        t.returnMenu = "MAIN MENU";

        // Hướng dẫn điều khiển
        t.guideMove = "W/A/S/D: Move";
        t.guidePlace = "Enter: Place";
        t.guideUndo = "Z: Undo";
        t.guideHint = "H: Hint (PvC)";
        t.guidePause = "ESC: Pause";
        t.guideSave = "L: Save Game";

        // Ten ngon ngu
        t.langVietnamese = "Vietnamese";
        t.langEnglish    = "English";

        // Hint bottom Save/Load/Settings
        t.saveHintBottom = "Enter: Save | ESC: Back";
        t.saveHintMouse  = "Left click: select name | Right click: delete file";
        t.loadHintBottom = "Enter: Load | Del: Delete | ESC: Back";
        t.loadHintMouse  = "Left click: select | Double click: load | Right click: delete";
        t.settingsHint   = "Click or drag the slider to adjust volume";

        t.inputNameHintPvP = "Tab: Switch field | Enter: Confirm";
        t.inputNameHintPvC = "Enter: Confirm";
        t.nameTooLong = "Player name can be at most 15 characters!";
        t.botName = "Computer";

        // Panel + timer
        t.yourTurn = "YOUR TURN";
        t.turnTimeBar = "TURN TIME";
        t.gameTimeLabel = "Game time: ";
        t.defeatText = "DEFEAT";
        t.botThinking = "Bot is thinking...";

        // Save/Load messages
        t.msgSaveOK = "Game saved successfully!";
        t.msgSaveError = "Save file error!";
        t.msgFileExists = "File exists - press Enter again to overwrite!";
        t.msgFileDeleted = "File deleted!";
        t.msgLoadError = "Failed to load file!";
    }
    else { // VIETNAMESE
        // Main menu
        t.title = u8"TRÒ CHƠI CARO";
        t.newGame = u8"CHƠI MỚI";
        t.loadGame = u8"TẢI GAME";
        t.settings = u8"CÀI ĐẶT";
        t.help = u8"HƯỚNG DẪN";
        t.about = u8"THÔNG TIN";
        t.exitGame = u8"THOÁT";

        // Chế độ
        t.chooseMode = u8"CHỌN CHẾ ĐỘ CHƠI";
        t.pvpMode = u8"Người vs Người";
        t.pvcMode = u8"Người vs Máy";

        // Độ khó
        t.chooseDifficulty = u8"CHỌN ĐỘ KHÓ";
        t.easy = u8"Dễ";
        t.medium = u8"Trung bình";
        t.hard = u8"Khó";
        t.expert = u8"Chuyên gia";

        // V2 #34: man chon nhan vat
        t.chooseCharacter = u8"CHỌN NHÂN VẬT";
        t.charPickerP1 = u8"Người chơi 1 — chọn nhân vật";
        t.charPickerP2 = u8"Người chơi 2 — chọn nhân vật";
        t.escBack = u8"ESC: Quay lại";

        // Kiểu chơi
        t.chooseStyle = u8"CHỌN KIỂU CHƠI";
        t.basicStyle = u8"Cơ bản";
        t.speedStyle = u8"Tốc độ";

        // Nhập tên
        t.enterName1 = u8"Nhập tên Người chơi 1:";
        t.enterName2 = u8"Nhập tên Người chơi 2:";
        t.nameDuplicate = u8"Tên đã bị trùng!";

        // Gameplay
        t.moves = u8"Số bước: ";
        t.wins = u8"Thắng: ";

        // Kết quả
        t.playerWin = u8" thắng!";
        t.drawText = u8"Hòa!";
        t.continueText = u8"Chơi tiếp?";
        t.yes = u8"Có";
        t.no = u8"Không";

        // Save/Load
        t.enterSaveName = u8"Nhập tên file lưu:";
        t.fileNotFound = u8"Danh sách trống!";

        // Cài đặt
        t.language = u8"Ngôn ngữ:";
        t.bgmVolume = u8"Âm lượng nhạc:";
        t.sfxToggle = u8"Hiệu ứng âm thanh:";
        t.on = u8"BẬT";
        t.off = u8"TẮT";
        t.back = u8"TRỞ LẠI";

        // Pause menu
        t.resume = u8"TIẾP TỤC";
        t.saveGame = u8"LƯU GAME";
        t.returnMenu = u8"TRỞ VỀ MENU CHÍNH";

        // Hướng dẫn điều khiển
        t.guideMove = u8"W/A/S/D: Di chuyển";
        t.guidePlace = u8"Enter: Đặt cờ";
        t.guideUndo = u8"Z: Đi lại";
        t.guideHint = u8"H: Gợi ý (PvC)";
        t.guidePause = u8"ESC: Tạm dừng";
        t.guideSave = u8"L: Lưu Game";

        // Tên ngôn ngữ
        t.langVietnamese = u8"Tiếng Việt";
        t.langEnglish    = u8"Tiếng Anh";

        // Hint bottom Save/Load/Settings
        t.saveHintBottom = u8"Enter: Lưu | ESC: Quay lại";
        t.saveHintMouse  = u8"Click trái: chọn tên | Click phải: xóa file";
        t.loadHintBottom = u8"Enter: Tải | Del: Xóa | ESC: Quay lại";
        t.loadHintMouse  = u8"Click trái: chọn | Double-click: tải | Click phải: xóa";
        t.settingsHint   = u8"Click hoặc kéo thanh trượt để điều chỉnh âm lượng";

        t.inputNameHintPvP = u8"Tab: Chuyển ô | Enter: Xác nhận";
        t.inputNameHintPvC = u8"Enter: Xác nhận";
        t.nameTooLong = u8"Tên người chơi chỉ tối đa là 15 kí tự!";
        t.botName = u8"Máy";

        // Panel + timer
        t.yourTurn = u8"LƯỢT CỦA BẠN";
        t.turnTimeBar = u8"THỜI GIAN LƯỢT";
        t.gameTimeLabel = u8"Thời gian ván: ";
        t.defeatText = u8"THUA CUỘC";
        t.botThinking = u8"Bot đang suy nghĩ...";

        // Save/Load messages
        t.msgSaveOK = u8"Lưu thành công!";
        t.msgSaveError = u8"Lỗi lưu file!";
        t.msgFileExists = u8"File đã tồn tại — Enter lần nữa để ghi đè!";
        t.msgFileDeleted = u8"Đã xóa file!";
        t.msgLoadError = u8"Lỗi tải file!";
    }
}

// Lay bo van ban theo ngon ngu - tra const& (3.1: tranh copy ~75 string moi
// lan goi, truoc day gan ~300-400 cap phat heap/frame). 2 ban EN/VI build 1
// lan (lazy), song suot vong doi chuong trinh -> tra tham chieu an toan.
const TextStrings& langGetText(Language lang) {
    static TextStrings en;
    static TextStrings vi;
    static bool built = false;
    if (!built) {
        langBuild(LANG_ENGLISH, en);
        langBuild(LANG_VIETNAMESE, vi);
        built = true;
    }
    return (lang == LANG_VIETNAMESE) ? vi : en;
}