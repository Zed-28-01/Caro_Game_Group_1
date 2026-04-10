#include "render.h"
#include "timer.h"
#include "language.h"

const float BOARD_OFFSET_X = 40.f;
const float BOARD_OFFSET_Y = 60.f;


// TIEN ICH
//(row,col) --> pixel
sf::Vector2f renderBoardToPixel(int row, int col) {
    float x = BOARD_OFFSET_X + col * CELL_SIZE + CELL_SIZE / 2.f;
    float y = BOARD_OFFSET_Y + row * CELL_SIZE + CELL_SIZE / 2.f;
    return sf::Vector2f(x, y);
}

// Chuyen pixel → (row, col) tren ban co
bool renderPixelToBoard(float x, float y, int& outRow, int& outCol) {
    int col = (int)((x - BOARD_OFFSET_X) / CELL_SIZE);
    int row = (int)((y - BOARD_OFFSET_Y) / CELL_SIZE);

    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return false;

    outRow = row;
    outCol = col;
    return true;
}


// Ve text can giua tai (centerX, centerY)
void renderTextCentered(sf::RenderWindow& window, const sf::Font& font,
    const std::string& text, int fontSize,
    float centerX, float centerY, sf::Color color) {
    sf::Text t;
    t.setFont(font);
    t.setString(sf::String::fromUtf8(text.begin(), text.end()));
    t.setCharacterSize(fontSize);
    t.setFillColor(color);

    // Tinh kich thuoc thuc te cua doan chu
    sf::FloatRect bounds = t.getLocalBounds();
    t.setOrigin(bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height / 2.f);
    t.setPosition(centerX, centerY);

    window.draw(t); // ve len cua so game
}



// KHOI TAO GAMERESOURCES
bool renderLoadResources(GameResources& res) {
    // Load font chinh
    if (!res.mainFont.loadFromFile("../assets/fonts/main_font.ttf")) return false;

    // Load font tieu de
    if (!res.titleFont.loadFromFile("../assets/fonts/title_font.ttf")) return false;

    // Texture & sound: load neu co, khong co thi bo qua (optional)
    res.backgroundTex.loadFromFile("../assets/textures/background.png");
    res.boardTex.loadFromFile("../assets/textures/board.png");

    res.moveSfx.loadFromFile("../assets/sounds/move.wav");
    res.placeSfx.loadFromFile("../assets/sounds/place.wav");
    res.winSfx.loadFromFile("../assets/sounds/win.wav");
    res.drawSfx.loadFromFile("../assets/sounds/draw.wav");
    res.menuSfx.loadFromFile("../assets/sounds/select.wav");

    res.bgMusic.openFromFile("../assets/sounds/bgm.ogg");

    return true;  
}


// VE BAN CO
void renderBoard(sf::RenderWindow& window, const GameResources& res) {
    // Ve nen ban co
    sf::RectangleShape bg(sf::Vector2f(BOARD_SIZE * CELL_SIZE, BOARD_SIZE * CELL_SIZE));
    bg.setPosition(BOARD_OFFSET_X, BOARD_OFFSET_Y);
    bg.setFillColor(COLOR_BOARD_BG);
    window.draw(bg);

    // Ve cac duong ke ngang va doc
    for (int i = 0; i <= BOARD_SIZE; i++) {
        // Duong ngang
        sf::RectangleShape hLine(sf::Vector2f(BOARD_SIZE * CELL_SIZE, 1.f));
        hLine.setPosition(BOARD_OFFSET_X, BOARD_OFFSET_Y + i * CELL_SIZE);
        hLine.setFillColor(COLOR_GRID_LINE);
        window.draw(hLine);

        // Duong doc
        sf::RectangleShape vLine(sf::Vector2f(1.f, BOARD_SIZE * CELL_SIZE));
        vLine.setPosition(BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y);
        vLine.setFillColor(COLOR_GRID_LINE);
        window.draw(vLine);
    }
}

// Ve cac quan co da danh
void renderPieces(sf::RenderWindow& window, const GameState& state,
    const GameResources& res) {
    float radius = CELL_SIZE / 2.f - 4.f;  // Bán kính quân cờ, chừa lề 4px

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int val = state.board[r][c].value;
            if (val == 0) continue;  // O trong, bo qua

            sf::CircleShape piece(radius);
            piece.setOrigin(radius, radius);  // Goc o tam
            sf::Vector2f pos = renderBoardToPixel(r, c);
            piece.setPosition(pos);

            if (val == -1)
                piece.setFillColor(COLOR_PLAYER_X);  // Do - Player 1
            else
                piece.setFillColor(COLOR_PLAYER_O);  // Xanh - Player 2

            window.draw(piece);
        }
    }
}

// Ve highlight o cursor dang chon
void renderCursor(sf::RenderWindow& window, int row, int col) {
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
    cell.setOrigin((CELL_SIZE - 2.f) / 2.f, (CELL_SIZE - 2.f) / 2.f);
    sf::Vector2f pos = renderBoardToPixel(row, col);
    cell.setPosition(pos);
    cell.setFillColor(COLOR_HIGHLIGHT);  // Vang trong suot
    window.draw(cell);
}

// Ve highlight goi y (hint) - mau khac cursor
void renderHint(sf::RenderWindow& window, int row, int col) {
    sf::CircleShape hint(CELL_SIZE / 2.f - 8.f);
    hint.setOrigin(CELL_SIZE / 2.f - 8.f, CELL_SIZE / 2.f - 8.f);
    sf::Vector2f pos = renderBoardToPixel(row, col);
    hint.setPosition(pos);
    hint.setFillColor(sf::Color(0, 255, 0, 80));  // Xanh la nhat, trong suot
    window.draw(hint);
}

// Ve duong thang noi 5 quan
void renderWinLine(sf::RenderWindow& window, const WinLine& winLine) {
    if (winLine.count < WIN_COUNT) return;

    // Lay diem dau va cuoi
    sf::Vector2f start = renderBoardToPixel(winLine.positions[0][0],
        winLine.positions[0][1]);
    sf::Vector2f end = renderBoardToPixel(winLine.positions[WIN_COUNT - 1][0],
        winLine.positions[WIN_COUNT - 1][1]);

    // Ve duong thang day
    sf::RectangleShape line;
    sf::Vector2f diff = end - start;
    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    line.setSize(sf::Vector2f(length, 6.f));  // Dai = khoang cach, day 6px
    line.setOrigin(0.f, 3.f);                 // Can giua theo chieu doc
    line.setPosition(start);
    line.setFillColor(COLOR_WIN_LINE);

    // Xoay duong thang theo dung huong
    float angle = std::atan2(diff.y, diff.x) * 180.f / 3.14159f;
    line.setRotation(angle);

    window.draw(line);
}




// VE PLAYER PANEL

void renderPlayerPanel(sf::RenderWindow& window, const GameState& state,
    const GameResources& res) {
    float panelX = BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + 40.f;  // X = 680
    float panelW = WINDOW_WIDTH - panelX - 20.f;                     // Chieu rong panel

    // --- Ve khung 2 nguoi choi ---
    for (int i = 0; i < 2; i++) {
        const Player& p = (i == 0) ? state.player1 : state.player2;
        bool isActive = (i == 0) ? state.isPlayer1Turn : !state.isPlayer1Turn;
        sf::Color pieceColor = (i == 0) ? COLOR_PLAYER_X : COLOR_PLAYER_O;

        float boxY = 80.f + i * 200.f;  // P1 o tren, P2 o duoi

        // Nen khung nguoi choi
        sf::RectangleShape box(sf::Vector2f(panelW, 170.f));
        box.setPosition(panelX, boxY);
        box.setFillColor(isActive ? sf::Color(60, 80, 100) : sf::Color(40, 55, 70));
        box.setOutlineThickness(isActive ? 3.f : 1.f);
        box.setOutlineColor(isActive ? pieceColor : sf::Color(100, 100, 100));
        window.draw(box);

        // Quan co mau
        sf::CircleShape dot(12.f);
        dot.setOrigin(12.f, 12.f);
        dot.setPosition(panelX + 25.f, boxY + 30.f);
        dot.setFillColor(pieceColor);
        window.draw(dot);

        // Ten nguoi choi
        sf::Text nameText;
        nameText.setFont(res.mainFont);
        nameText.setString(p.name.empty() ? (i == 0 ? "Player 1" : "Player 2") : p.name);
        nameText.setCharacterSize(20);
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition(panelX + 50.f, boxY + 18.f);
        window.draw(nameText);

        // So nuoc di
        sf::Text movesText;
        movesText.setFont(res.mainFont);
        movesText.setString("Nuoc di: " + std::to_string(p.moves));
        movesText.setCharacterSize(16);
        movesText.setFillColor(sf::Color(200, 200, 200));
        movesText.setPosition(panelX + 15.f, boxY + 65.f);
        window.draw(movesText);

        // So van thang
        sf::Text winsText;
        winsText.setFont(res.mainFont);
        winsText.setString("Thang: " + std::to_string(p.totalWins));
        winsText.setCharacterSize(16);
        winsText.setFillColor(sf::Color(200, 200, 200));
        winsText.setPosition(panelX + 15.f, boxY + 95.f);
        window.draw(winsText);

        // Nhan "LUOT CUA BAN" neu dang active
        if (isActive) {
            renderTextCentered(window, res.mainFont, ">>> LUOT CUA BAN <<<",
                14, panelX + panelW / 2.f, boxY + 140.f,
                pieceColor);
        }
    }
}


// VE TIMER

// Thanh dem gio luot hien tai (progress bar)
void renderTurnTimer(sf::RenderWindow& window, const GameResources& res,
    float percentage) {
    float panelX = BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + 40.f;
    float panelW = WINDOW_WIDTH - panelX - 20.f;
    float barY = 490.f;
    float barH = 24.f;

    // Nen xam
    sf::RectangleShape bg(sf::Vector2f(panelW, barH));
    bg.setPosition(panelX, barY);
    bg.setFillColor(sf::Color(60, 60, 60));
    window.draw(bg);

    // Thanh con lai - doi mau khi sap het gio
    sf::Color barColor = (percentage > 0.3f) ? COLOR_TIMER_BAR : COLOR_TIMER_LOW;
    sf::RectangleShape bar(sf::Vector2f(panelW * percentage, barH));
    bar.setPosition(panelX, barY);
    bar.setFillColor(barColor);
    window.draw(bar);

    // Label
    renderTextCentered(window, res.mainFont, "THOI GIAN LUOT",
        13, panelX + panelW / 2.f, barY - 14.f,
        sf::Color(200, 200, 200));
}

// Hien thi thoi gian van con lai
void renderGameTimer(sf::RenderWindow& window, const GameResources& res,
    float secondsLeft) {
    float panelX = BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + 40.f;
    float panelW = WINDOW_WIDTH - panelX - 20.f;

    int mins = (int)secondsLeft / 60;
    int secs = (int)secondsLeft % 60;

    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d", mins, secs);

    renderTextCentered(window, res.mainFont, "Thoi gian van: " + std::string(buf),
        16, panelX + panelW / 2.f, 545.f,
        sf::Color::White);
}


// VE MENU

// Ham noi bo: ve 1 menu chung (tieu de + danh sach lua chon)
static void renderMenuGeneric(sf::RenderWindow& window, const GameResources& res,
    const std::string& title,
    const std::string items[], int itemCount,
    int menuIndex) {
    // Nen toan man hinh
    window.clear(COLOR_MENU_BG);

    // Tieu de
    renderTextCentered(window, res.titleFont, title, 52,
        WINDOW_WIDTH / 2.f, 120.f, sf::Color::White);

    // Cac muc menu
    for (int i = 0; i < itemCount; i++) {
        bool selected = (i == menuIndex);
        sf::Color color = selected ? COLOR_MENU_HOVER : COLOR_MENU_TEXT;
        int fontSize = selected ? 30 : 26;

        renderTextCentered(window, res.mainFont, items[i], fontSize,
            WINDOW_WIDTH / 2.f, 280.f + i * 60.f, color);

        // Ve dau ">" truoc muc dang chon
        if (selected) {
            renderTextCentered(window, res.mainFont, ">", fontSize,
                WINDOW_WIDTH / 2.f - 160.f, 280.f + i * 60.f,
                color);
        }
    }
}

// Main Menu
void renderMainMenu(sf::RenderWindow& window, const GameResources& res,
    int menuIndex) {
    TextStrings txt = langGetText(langGetCurrent());
    std::string items[] = {
        txt.newGame, txt.loadGame, txt.settings,
        txt.help, txt.about, txt.exitGame
    };
    renderMenuGeneric(window, res, txt.title, items, 6, menuIndex);
}

// Chon che do PvP / PvC
void renderModeSelect(sf::RenderWindow& window, const GameResources& res,
    int menuIndex) {
    TextStrings txt = langGetText(langGetCurrent());
    std::string items[] = { txt.pvpMode, txt.pvcMode, txt.back };
    renderMenuGeneric(window, res, txt.chooseMode, items, 3, menuIndex);
}

// Chon do kho AI
void renderDifficultySelect(sf::RenderWindow& window, const GameResources& res,
    int menuIndex) {
    TextStrings txt = langGetText(langGetCurrent());
    std::string items[] = { txt.easy, txt.medium, txt.hard, txt.expert, txt.back };
    renderMenuGeneric(window, res, txt.chooseDifficulty, items, 5, menuIndex);
}

// Chon kieu choi Basic / Speed
void renderStyleSelect(sf::RenderWindow& window, const GameResources& res,
    int menuIndex) {
    TextStrings txt = langGetText(langGetCurrent());
    std::string items[] = { txt.basicStyle, txt.speedStyle, txt.back };
    renderMenuGeneric(window, res, txt.chooseStyle, items, 3, menuIndex);
}

// Menu tam dung (ESC in-game)
void renderPauseMenu(sf::RenderWindow& window, const GameResources& res,
    int menuIndex) {
    // Lop phu toi ban trong (overlay)
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    TextStrings txt = langGetText(langGetCurrent());
    std::string items[] = { txt.resume, txt.saveGame, txt.returnMenu };

    // Tieu de
    renderTextCentered(window, res.titleFont, "PAUSE", 42,
        WINDOW_WIDTH / 2.f, 240.f, sf::Color::White);

    // Cac muc
    for (int i = 0; i < 3; i++) {
        bool selected = (i == menuIndex);
        sf::Color color = selected ? COLOR_MENU_HOVER : COLOR_MENU_TEXT;
        renderTextCentered(window, res.mainFont, items[i],
            selected ? 28 : 24,
            WINDOW_WIDTH / 2.f, 340.f + i * 55.f, color);
    }
}


// VE MAN HINH NHAP TEN


void renderInputNames(sf::RenderWindow& window, const GameResources& res,
    const std::string& name1, const std::string& name2,
    bool isEditingPlayer1, bool showError) {
    window.clear(COLOR_MENU_BG);
    TextStrings txt = langGetText(langGetCurrent());

    renderTextCentered(window, res.titleFont, txt.enterName1, 30,
        WINDOW_WIDTH / 2.f, 150.f, sf::Color::White);

    // O nhap ten Player 1
    sf::RectangleShape box1(sf::Vector2f(400.f, 50.f));
    box1.setOrigin(200.f, 25.f);
    box1.setPosition(WINDOW_WIDTH / 2.f, 220.f);
    box1.setFillColor(sf::Color(60, 60, 80));
    box1.setOutlineThickness(isEditingPlayer1 ? 3.f : 1.f);
    box1.setOutlineColor(isEditingPlayer1 ? COLOR_MENU_HOVER : sf::Color(100, 100, 100));
    window.draw(box1);

    renderTextCentered(window, res.mainFont,
        name1.empty() ? "Player 1" : name1, 22,
        WINDOW_WIDTH / 2.f, 220.f,
        name1.empty() ? sf::Color(120, 120, 120) : sf::Color::White);

    // O nhap ten Player 2
    renderTextCentered(window, res.titleFont, txt.enterName2, 30,
        WINDOW_WIDTH / 2.f, 320.f, sf::Color::White);

    sf::RectangleShape box2(sf::Vector2f(400.f, 50.f));
    box2.setOrigin(200.f, 25.f);
    box2.setPosition(WINDOW_WIDTH / 2.f, 390.f);
    box2.setFillColor(sf::Color(60, 60, 80));
    box2.setOutlineThickness(!isEditingPlayer1 ? 3.f : 1.f);
    box2.setOutlineColor(!isEditingPlayer1 ? COLOR_MENU_HOVER : sf::Color(100, 100, 100));
    window.draw(box2);

    renderTextCentered(window, res.mainFont,
        name2.empty() ? "Player 2" : name2, 22,
        WINDOW_WIDTH / 2.f, 390.f,
        name2.empty() ? sf::Color(120, 120, 120) : sf::Color::White);

    // Thong bao loi
    if (showError) {
        renderTextCentered(window, res.mainFont, txt.nameError, 18,
            WINDOW_WIDTH / 2.f, 460.f, sf::Color::Red);
    }

    // Huong dan
    renderTextCentered(window, res.mainFont, "Tab: Chuyen o | Enter: Xac nhan", 16,
        WINDOW_WIDTH / 2.f, 520.f, sf::Color(180, 180, 180));
}



// VE TOAN BO MAN HINH GAMEPLAY

void renderGameplay(sf::RenderWindow& window, const GameState& state,
    const GameResources& res, const WinLine* winLine,
    int hintRow, int hintCol, bool showHint) {
    window.clear(COLOR_BACKGROUND);

    // Ve ban co + quan co
    renderBoard(window, res);
    renderPieces(window, state, res);

    // Ve cursor o dang chon
    renderCursor(window, state.cursorRow, state.cursorCol);

    // Ve hint neu dang bat
    if (showHint && hintRow >= 0 && hintCol >= 0)
        renderHint(window, hintRow, hintCol);

    // Ve duong thang neu co nguoi thang
    if (winLine != nullptr && winLine->count >= WIN_COUNT)
        renderWinLine(window, *winLine);

    // Ve panel thong tin nguoi choi
    renderPlayerPanel(window, state, res);

    // Ve timer (chi khi che do Speed)
    if (state.style == STYLE_SPEED) {
        renderTurnTimer(window, res, timerGetTurnPercent(state.timer));
        renderGameTimer(window, res, timerGetGameSecondsLeft(state.timer));
    }
}



// VE MAN HINH KET THUC

void renderGameOver(sf::RenderWindow& window, const GameState& state,
    const GameResources& res, GameResult result,
    int menuIndex) {
    // Overlay toi
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    TextStrings txt = langGetText(langGetCurrent());

    // Ket qua
    std::string resultStr;
    sf::Color resultColor;
    if (result == RESULT_PLAYER1_WIN) {
        resultStr = state.player1.name + " " + txt.playerWin;
        resultColor = COLOR_PLAYER_X;
    }
    else if (result == RESULT_PLAYER2_WIN) {
        resultStr = state.player2.name + " " + txt.playerWin;
        resultColor = COLOR_PLAYER_O;
    }
    else {
        resultStr = txt.drawText;
        resultColor = sf::Color::Yellow;
    }

    renderTextCentered(window, res.titleFont, resultStr, 42,
        WINDOW_WIDTH / 2.f, 250.f, resultColor);

    // Thong ke
    std::string stats = state.player1.name + " " + std::to_string(state.player1.totalWins)
        + "  -  "
        + std::to_string(state.player2.totalWins) + " " + state.player2.name;
    renderTextCentered(window, res.mainFont, stats, 22,
        WINDOW_WIDTH / 2.f, 330.f, sf::Color::White);

    // Lua chon tiep
    std::string items[] = { txt.yes, txt.no };
    renderTextCentered(window, res.mainFont, txt.continueText, 20,
        WINDOW_WIDTH / 2.f, 410.f, sf::Color(200, 200, 200));

    for (int i = 0; i < 2; i++) {
        bool selected = (i == menuIndex);
        renderTextCentered(window, res.mainFont, items[i],
            selected ? 26 : 22,
            WINDOW_WIDTH / 2.f - 80.f + i * 160.f, 470.f,
            selected ? COLOR_MENU_HOVER : COLOR_MENU_TEXT);
    }
}

// VE SAVE / LOAD

void renderSaveScreen(sf::RenderWindow& window, const GameResources& res,
    const std::string saveList[], int saveCount,
    const std::string& inputName, int selectedIndex) {
    window.clear(COLOR_MENU_BG);
    TextStrings txt = langGetText(langGetCurrent());

    renderTextCentered(window, res.titleFont, txt.saveGame, 38,
        WINDOW_WIDTH / 2.f, 60.f, sf::Color::White);

    // O nhap ten save
    sf::RectangleShape inputBox(sf::Vector2f(400.f, 45.f));
    inputBox.setOrigin(200.f, 22.5f);
    inputBox.setPosition(WINDOW_WIDTH / 2.f, 130.f);
    inputBox.setFillColor(sf::Color(60, 60, 80));
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(COLOR_MENU_HOVER);
    window.draw(inputBox);

    renderTextCentered(window, res.mainFont,
        inputName.empty() ? txt.enterSaveName : inputName, 18,
        WINDOW_WIDTH / 2.f, 130.f,
        inputName.empty() ? sf::Color(120, 120, 120) : sf::Color::White);

    // Danh sach file save
    for (int i = 0; i < saveCount; i++) {
        bool selected = (i == selectedIndex);
        float y = 200.f + i * 35.f;

        if (selected) {
            sf::RectangleShape highlight(sf::Vector2f(500.f, 30.f));
            highlight.setOrigin(250.f, 15.f);
            highlight.setPosition(WINDOW_WIDTH / 2.f, y);
            highlight.setFillColor(sf::Color(50, 70, 90));
            window.draw(highlight);
        }

        renderTextCentered(window, res.mainFont, saveList[i], 16,
            WINDOW_WIDTH / 2.f, y,
            selected ? COLOR_MENU_HOVER : COLOR_MENU_TEXT);
    }

    // Huong dan
    renderTextCentered(window, res.mainFont,
        "Enter: Luu | ESC: Quay lai", 14,
        WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 40.f,
        sf::Color(150, 150, 150));
}

void renderLoadScreen(sf::RenderWindow& window, const GameResources& res,
    const std::string saveList[], int saveCount,
    int selectedIndex) {
    window.clear(COLOR_MENU_BG);
    TextStrings txt = langGetText(langGetCurrent());

    renderTextCentered(window, res.titleFont, txt.loadGame, 38,
        WINDOW_WIDTH / 2.f, 60.f, sf::Color::White);

    if (saveCount == 0) {
        renderTextCentered(window, res.mainFont, txt.fileNotFound, 20,
            WINDOW_WIDTH / 2.f, 300.f, sf::Color(180, 180, 180));
    }
    else {
        for (int i = 0; i < saveCount; i++) {
            bool selected = (i == selectedIndex);
            float y = 150.f + i * 35.f;

            if (selected) {
                sf::RectangleShape highlight(sf::Vector2f(500.f, 30.f));
                highlight.setOrigin(250.f, 15.f);
                highlight.setPosition(WINDOW_WIDTH / 2.f, y);
                highlight.setFillColor(sf::Color(50, 70, 90));
                window.draw(highlight);
            }

            renderTextCentered(window, res.mainFont, saveList[i], 16,
                WINDOW_WIDTH / 2.f, y,
                selected ? COLOR_MENU_HOVER : COLOR_MENU_TEXT);
        }
    }

    renderTextCentered(window, res.mainFont,
        "Enter: Tai | Del: Xoa | ESC: Quay lai", 14,
        WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 40.f,
        sf::Color(150, 150, 150));
}

// VE SETTINGS / HELP / ABOUT

void renderSettings(sf::RenderWindow& window, const GameResources& res,
    int menuIndex, Language lang, int volume, bool sfxOn) {
    window.clear(COLOR_MENU_BG);
    TextStrings txt = langGetText(langGetCurrent());

    renderTextCentered(window, res.titleFont, txt.settings, 38,
        WINDOW_WIDTH / 2.f, 100.f, sf::Color::White);

    // Ngon ngu
    std::string langStr = txt.language + ": " +
        (lang == LANG_VIETNAMESE ? "Tieng Viet" : "English");
    // Am luong BGM
    std::string volStr = txt.bgmVolume + ": " + std::to_string(volume) + "%";
    // SFX
    std::string sfxStr = txt.sfxToggle + ": " + (sfxOn ? txt.on : txt.off);

    std::string items[] = { langStr, volStr, sfxStr, txt.back };

    for (int i = 0; i < 4; i++) {
        bool selected = (i == menuIndex);
        renderTextCentered(window, res.mainFont, items[i],
            selected ? 26 : 22,
            WINDOW_WIDTH / 2.f, 250.f + i * 60.f,
            selected ? COLOR_MENU_HOVER : COLOR_MENU_TEXT);
    }
}

void renderHelp(sf::RenderWindow& window, const GameResources& res) {
    window.clear(COLOR_MENU_BG);
    TextStrings txt = langGetText(langGetCurrent());

    renderTextCentered(window, res.titleFont, txt.help, 38,
        WINDOW_WIDTH / 2.f, 80.f, sf::Color::White);

    std::string guides[] = {
        txt.guideMove, txt.guidePlace, txt.guideUndo,
        txt.guideHint, txt.guidePause, txt.guideSave
    };

    for (int i = 0; i < 6; i++) {
        renderTextCentered(window, res.mainFont, guides[i], 20,
            WINDOW_WIDTH / 2.f, 200.f + i * 50.f,
            sf::Color(220, 220, 220));
    }

    renderTextCentered(window, res.mainFont, "ESC: " + txt.back, 16,
        WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 50.f,
        sf::Color(150, 150, 150));
}

void renderAbout(sf::RenderWindow& window, const GameResources& res) {
    window.clear(COLOR_MENU_BG);
    TextStrings txt = langGetText(langGetCurrent());

    renderTextCentered(window, res.titleFont, txt.about, 38,
        WINDOW_WIDTH / 2.f, 100.f, sf::Color::White);

    renderTextCentered(window, res.mainFont, "Caro Game - Group 1", 24,
        WINDOW_WIDTH / 2.f, 230.f, sf::Color::White);

    renderTextCentered(window, res.mainFont, "HCMUS - 2026", 20,
        WINDOW_WIDTH / 2.f, 280.f, sf::Color(200, 200, 200));

    renderTextCentered(window, res.mainFont, "Made with SFML 2.6.2", 18,
        WINDOW_WIDTH / 2.f, 340.f, sf::Color(150, 150, 150));

    renderTextCentered(window, res.mainFont, "ESC: " + txt.back, 16,
        WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 50.f,
        sf::Color(150, 150, 150));
}


// HIEU UNG

void renderPlaceEffect(sf::RenderWindow& window, int row, int col,
    int player, float progress) {
    // Hieu ung vong tron mo rong khi dat quan
    float maxRadius = CELL_SIZE / 2.f;
    float radius = maxRadius * progress;
    int alpha = (int)(255 * (1.f - progress));  // Mo dan

    sf::CircleShape effect(radius);
    effect.setOrigin(radius, radius);
    effect.setPosition(renderBoardToPixel(row, col));
    effect.setFillColor(sf::Color(255, 255, 255, alpha));
    window.draw(effect);
}

void renderWinEffect(sf::RenderWindow& window, const WinLine& winLine,
    const GameResources& res, float progress) {
    // Nhap nhay 5 quan thang
    int alpha = (int)(128 + 127 * std::sin(progress * 6.28f));  // 0→255 lien tuc

    for (int i = 0; i < winLine.count && i < WIN_COUNT; i++) {
        sf::CircleShape glow(CELL_SIZE / 2.f);
        glow.setOrigin(CELL_SIZE / 2.f, CELL_SIZE / 2.f);
        glow.setPosition(renderBoardToPixel(winLine.positions[i][0],
            winLine.positions[i][1]));
        glow.setFillColor(sf::Color(255, 255, 0, alpha));
        window.draw(glow);
    }
}

void renderDrawEffect(sf::RenderWindow& window, const GameResources& res,
    float progress) {
    // Man hinh mo dan khi hoa
    int alpha = (int)(150 * progress);
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(128, 128, 128, alpha));
    window.draw(overlay);
}


// DISPATCHER - GOI HAM VE TUONG UNG VOI MAN HINH

void renderScreen(sf::RenderWindow& window, const GameState& state,
    const GameResources& res, GameScreen screen) {
    switch (screen) {
    case SCREEN_MAIN_MENU:
        renderMainMenu(window, res, 0);
        break;
    case SCREEN_MODE_SELECT:
        renderModeSelect(window, res, 0);
        break;
    case SCREEN_DIFFICULTY:
        renderDifficultySelect(window, res, 0);
        break;
    case SCREEN_STYLE_SELECT:
        renderStyleSelect(window, res, 0);
        break;
    case SCREEN_PLAYING:
        renderGameplay(window, state, res, nullptr, -1, -1, false);
        break;
    default:
        break;
    }
}
