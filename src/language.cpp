#include "language.h"
#include "game_types.h"

static Language currentLanguage = LANG_ENGLISH;

Language langGetCurrent() {
    return currentLanguage;
}

void langSetCurrent(Language lang) {
    currentLanguage = lang;
}

void langToggle() {
    currentLanguage = (currentLanguage == LANG_ENGLISH) ? LANG_VIETNAMESE : LANG_ENGLISH;
}

static void langBuild(Language lang, TextStrings& t) {
    if (lang == LANG_ENGLISH) {
        t.title = "CARO GAME";
        t.newGame = "NEW GAME";
        t.loadGame = "LOAD GAME";
        t.settings = "SETTINGS";
        t.help = "HELP";
        t.about = "ABOUT";
        t.exitGame = "EXIT";

        t.chooseMode = "CHOOSE GAME MODE";
        t.pvpMode = "Player vs Player";
        t.pvcMode = "Player vs Computer";

        t.chooseDifficulty = "CHOOSE DIFFICULTY";
        t.easy = "Easy";
        t.medium = "Medium";
        t.hard = "Hard";
        t.expert = "Expert";

        t.chooseCharacter = "CHOOSE YOUR FIGHTER";
        t.charPickerP1 = "Player 1 - pick your fighter";
        t.charPickerP2 = "Player 2 - pick your fighter";
        t.escBack = "ESC: Back";

        t.chooseStyle = "CHOOSE PLAY STYLE";
        t.basicStyle = "Basic Mode";
        t.speedStyle = "Speed Mode";

        t.enterName1 = "Enter Player 1 Name:";
        t.enterName2 = "Enter Player 2 Name:";
        t.nameDuplicate = "Name already taken!";

        t.moves = "Moves: ";
        t.wins = "Wins: ";

        t.playerWin = " wins!";
        t.drawText = "Draw!";
        t.continueText = "Play again?";
        t.yes = "Yes";
        t.no = "No";

        t.enterSaveName = "Enter save file name:";
        t.fileNotFound = "Empty list!";

        t.language = "Language:";
        t.bgmVolume = "Music Volume:";
        t.sfxToggle = "Sound Effects:";
        t.on = "ON";
        t.off = "OFF";
        t.back = "BACK";

        t.resume = "RESUME";
        t.saveGame = "SAVE GAME";
        t.returnMenu = "MAIN MENU";

        t.guideMove = "W/A/S/D: Move";
        t.guidePlace = "Enter: Place";
        t.guideUndo = "Z: Undo";
        t.guideHint = "H: Hint (PvC)";
        t.guidePause = "ESC: Pause";
        t.guideSave = "L: Save Game";

        t.langVietnamese = "Vietnamese";
        t.langEnglish    = "English";

        t.saveHintBottom = "Enter: Save | ESC: Back";
        t.saveHintMouse  = "Left click: select name | Right click: delete file";
        t.loadHintBottom = "Enter: Load | Del: Delete | ESC: Back";
        t.loadHintMouse  = "Left click: select | Double click: load | Right click: delete";
        t.settingsHint   = "Click or drag the slider to adjust volume";

        t.inputNameHintPvP = "Tab: Switch field | Enter: Confirm";
        t.inputNameHintPvC = "Enter: Confirm";
        t.nameTooLong = "Player name can be at most 15 characters!";
        t.botName = "Computer";

        t.yourTurn = "YOUR TURN";
        t.turnTimeBar = "TURN TIME";
        t.gameTimeLabel = "Game time: ";
        t.defeatText = "DEFEAT";
        t.botThinking = "Bot is thinking...";

        t.msgSaveOK = "Game saved successfully!";
        t.msgSaveError = "Save file error!";
        t.msgFileExists = "File exists - press Enter again to overwrite!";
        t.msgFileDeleted = "File deleted!";
        t.msgLoadError = "Failed to load file!";
    }
    else {
        t.title = u8"TRÒ CHƠI CARO";
        t.newGame = u8"CHƠI MỚI";
        t.loadGame = u8"TẢI GAME";
        t.settings = u8"CÀI ĐẶT";
        t.help = u8"HƯỚNG DẪN";
        t.about = u8"THÔNG TIN";
        t.exitGame = u8"THOÁT";

        t.chooseMode = u8"CHỌN CHẾ ĐỘ CHƠI";
        t.pvpMode = u8"Người vs Người";
        t.pvcMode = u8"Người vs Máy";

        t.chooseDifficulty = u8"CHỌN ĐỘ KHÓ";
        t.easy = u8"Dễ";
        t.medium = u8"Trung bình";
        t.hard = u8"Khó";
        t.expert = u8"Chuyên gia";

        t.chooseCharacter = u8"CHỌN NHÂN VẬT";
        t.charPickerP1 = u8"Người chơi 1 — chọn nhân vật";
        t.charPickerP2 = u8"Người chơi 2 — chọn nhân vật";
        t.escBack = u8"ESC: Quay lại";

        t.chooseStyle = u8"CHỌN KIỂU CHƠI";
        t.basicStyle = u8"Cơ bản";
        t.speedStyle = u8"Tốc độ";

        t.enterName1 = u8"Nhập tên Người chơi 1:";
        t.enterName2 = u8"Nhập tên Người chơi 2:";
        t.nameDuplicate = u8"Tên đã bị trùng!";

        t.moves = u8"Số bước: ";
        t.wins = u8"Thắng: ";

        t.playerWin = u8" thắng!";
        t.drawText = u8"Hòa!";
        t.continueText = u8"Chơi tiếp?";
        t.yes = u8"Có";
        t.no = u8"Không";

        t.enterSaveName = u8"Nhập tên file lưu:";
        t.fileNotFound = u8"Danh sách trống!";

        t.language = u8"Ngôn ngữ:";
        t.bgmVolume = u8"Âm lượng nhạc:";
        t.sfxToggle = u8"Hiệu ứng âm thanh:";
        t.on = u8"BẬT";
        t.off = u8"TẮT";
        t.back = u8"TRỞ LẠI";

        t.resume = u8"TIẾP TỤC";
        t.saveGame = u8"LƯU GAME";
        t.returnMenu = u8"TRỞ VỀ MENU CHÍNH";

        t.guideMove = u8"W/A/S/D: Di chuyển";
        t.guidePlace = u8"Enter: Đặt cờ";
        t.guideUndo = u8"Z: Đi lại";
        t.guideHint = u8"H: Gợi ý (PvC)";
        t.guidePause = u8"ESC: Tạm dừng";
        t.guideSave = u8"L: Lưu Game";

        t.langVietnamese = u8"Tiếng Việt";
        t.langEnglish    = u8"Tiếng Anh";

        t.saveHintBottom = u8"Enter: Lưu | ESC: Quay lại";
        t.saveHintMouse  = u8"Click trái: chọn tên | Click phải: xóa file";
        t.loadHintBottom = u8"Enter: Tải | Del: Xóa | ESC: Quay lại";
        t.loadHintMouse  = u8"Click trái: chọn | Double-click: tải | Click phải: xóa";
        t.settingsHint   = u8"Click hoặc kéo thanh trượt để điều chỉnh âm lượng";

        t.inputNameHintPvP = u8"Tab: Chuyển ô | Enter: Xác nhận";
        t.inputNameHintPvC = u8"Enter: Xác nhận";
        t.nameTooLong = u8"Tên người chơi chỉ tối đa là 15 kí tự!";
        t.botName = u8"Máy";

        t.yourTurn = u8"LƯỢT CỦA BẠN";
        t.turnTimeBar = u8"THỜI GIAN LƯỢT";
        t.gameTimeLabel = u8"Thời gian ván: ";
        t.defeatText = u8"THUA CUỘC";
        t.botThinking = u8"Bot đang suy nghĩ...";

        t.msgSaveOK = u8"Lưu thành công!";
        t.msgSaveError = u8"Lỗi lưu file!";
        t.msgFileExists = u8"File đã tồn tại — Enter lần nữa để ghi đè!";
        t.msgFileDeleted = u8"Đã xóa file!";
        t.msgLoadError = u8"Lỗi tải file!";
    }
}

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