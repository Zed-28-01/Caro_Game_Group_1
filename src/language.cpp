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

// Lấy bộ văn bản theo ngôn ngữ đã chọn
TextStrings langGetText(Language lang) {
    TextStrings t;

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

        // Kiểu chơi
        t.chooseStyle = "CHOOSE PLAY STYLE";
        t.basicStyle = "Basic Mode";
        t.speedStyle = "Speed Mode";

        // Nhập tên
        t.enterName1 = "Enter Player 1 Name:";
        t.enterName2 = "Enter Player 2 Name:";
        t.nameError = "Name cannot be empty!";
        t.nameDuplicate = "Name already taken!";

        // Gameplay
        t.turn = "Turn: ";
        t.moves = "Moves: ";
        t.wins = "Wins: ";
        t.hintText = "Press H for Hint";
        t.undoText = "Press Z to Undo";

        // Kết quả
        t.playerWin = " wins!";
        t.drawText = "Draw!";
        t.continueText = "Play again?";
        t.yes = "Yes";
        t.no = "No";

        // Save/Load
        t.enterSaveName = "Enter save file name:";
        t.saveSuccess = "Game saved successfully!";
        t.loadSuccess = "Game loaded successfully!";
        t.fileNotFound = "File not found!";
        t.fileExists = "File already exists!";
        t.maxSaveReached = "Max save slots reached!";
        t.deleteFile = "Delete File";
        t.renameFile = "Rename File";

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

        // Timer
        t.gameTime = "Time: ";
        t.turnTime = "Turn: ";
        t.timeUp = "Time's up!";

        // Hướng dẫn điều khiển
        t.guideMove = "W/A/S/D: Move";
        t.guidePlace = "Enter: Place";
        t.guideUndo = "Z: Undo";
        t.guideHint = "H: Hint (PvC)";
        t.guidePause = "ESC: Pause";
        t.guideSave = "L: Save Game";
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

        // Kiểu chơi
        t.chooseStyle = u8"CHỌN KIỂU CHƠI";
        t.basicStyle = u8"Cơ bản";
        t.speedStyle = u8"Tốc độ";

        // Nhập tên
        t.enterName1 = u8"Nhập tên Người chơi 1:";
        t.enterName2 = u8"Nhập tên Người chơi 2:";
        t.nameError = u8"Tên không được để trống!";
        t.nameDuplicate = u8"Tên đã bị trùng!";

        // Gameplay
        t.turn = u8"Lượt: ";
        t.moves = u8"Số bước: ";
        t.wins = u8"Thắng: ";
        t.hintText = u8"Nhấn H để xem gợi ý";
        t.undoText = u8"Nhấn Z để đi lại";

        // Kết quả
        t.playerWin = u8" chiến thắng!";
        t.drawText = u8"Hòa!";
        t.continueText = u8"Chơi tiếp?";
        t.yes = u8"Có";
        t.no = u8"Không";

        // Save/Load
        t.enterSaveName = u8"Nhập tên file lưu:";
        t.saveSuccess = u8"Lưu game thành công!";
        t.loadSuccess = u8"Tải game thành công!";
        t.fileNotFound = u8"Không tìm thấy file!";
        t.fileExists = u8"File đã tồn tại!";
        t.maxSaveReached = u8"Đã đạt giới hạn số lượng file lưu tối đa!";
        t.deleteFile = u8"Xóa File";
        t.renameFile = u8"Đổi tên File";

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

        // Timer
        t.gameTime = u8"Thời gian: ";
        t.turnTime = u8"Lượt: ";
        t.timeUp = u8"Hết giờ!";

        // Hướng dẫn điều khiển
        t.guideMove = u8"W/A/S/D: Di chuyển";
        t.guidePlace = u8"Enter: Đặt cờ";
        t.guideUndo = u8"Z: Đi lại";
        t.guideHint = u8"H: Gợi ý (PvC)";
        t.guidePause = u8"ESC: Tạm dừng";
        t.guideSave = u8"L: Lưu Game";
    }

    return t;
}