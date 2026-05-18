#include "menu.h"
#include "board.h"
#include "render.h"
#include "timer.h"
#include "language.h"
#include "bot.h"
#include "save_load.h"
#include "sound.h"


// GAME LOOP CHINH
void gameRun(sf::RenderWindow& window, GameResources& res) {
    GameState state;
    boardResetAll(state);

    // Khoi tao am thanh + phat nhac nen
    soundLoadAll(res);
    soundPlayBGM(res, true);

    GameScreen currentScreen = SCREEN_MAIN_MENU;

    while (window.isOpen()) {
        GameScreen nextScreen = currentScreen;

        switch (currentScreen) {
        case SCREEN_MAIN_MENU:
            nextScreen = handleMainMenu(window, res, state);
            break;
        case SCREEN_MODE_SELECT:
            nextScreen = handleModeSelect(window, res, state);
            break;
        case SCREEN_DIFFICULTY:
            nextScreen = handleDifficultySelect(window, res, state);
            break;
        case SCREEN_STYLE_SELECT:
            nextScreen = handleStyleSelect(window, res, state);
            break;
        case SCREEN_INPUT_NAMES:
            nextScreen = handleInputNames(window, res, state);
            break;
        case SCREEN_PLAYING:
            nextScreen = handleGameplay(window, res, state);
            break;
        case SCREEN_SETTINGS:
            nextScreen = handleSettings(window, res);
            break;
        case SCREEN_HELP:
            nextScreen = handleHelp(window, res);
            break;
        case SCREEN_ABOUT:
            nextScreen = handleAbout(window, res);
            break;
        case SCREEN_LOAD:
            nextScreen = handleLoadScreen(window, res, state);
            break;
        default:
            window.close();
            break;
        }

        currentScreen = nextScreen;
    }
}



// ============================================================
// MAIN MENU
// ============================================================

GameScreen handleMainMenu(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    int menuIndex = 0;
    const int MENU_COUNT = 6;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Up:
                case sf::Keyboard::W:
                    menuIndex = (menuIndex - 1 + MENU_COUNT) % MENU_COUNT;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = (menuIndex + 1) % MENU_COUNT;
                    break;
                case sf::Keyboard::Enter:
                    soundPlaySelect(res);
                    switch (menuIndex) {
                    case 0: return SCREEN_MODE_SELECT;   // New Game
                    case 1: return SCREEN_LOAD;           // Load Game
                    case 2: return SCREEN_SETTINGS;       // Settings
                    case 3: return SCREEN_HELP;           // Help
                    case 4: return SCREEN_ABOUT;          // About
                    case 5: window.close(); return SCREEN_MAIN_MENU; // Exit
                    }
                    break;
                default: break;
                }
            }
        }

        renderMainMenu(window, res, menuIndex);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

// ============================================================
// CHON CHE DO
// ============================================================

GameScreen handleModeSelect(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    int menuIndex = 0;
    const int MENU_COUNT = 3;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Up:
                case sf::Keyboard::W:
                    menuIndex = (menuIndex - 1 + MENU_COUNT) % MENU_COUNT;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = (menuIndex + 1) % MENU_COUNT;
                    break;
                case sf::Keyboard::Enter:
                    soundPlaySelect(res);
                    if (menuIndex == 0) {
                        state.mode = MODE_PVP;
                        return SCREEN_STYLE_SELECT;
                    }
                    if (menuIndex == 1) {
                        state.mode = MODE_PVC;
                        return SCREEN_DIFFICULTY;
                    }
                    if (menuIndex == 2) return SCREEN_MAIN_MENU; // Back
                    break;
                case sf::Keyboard::Escape:
                    return SCREEN_MAIN_MENU;
                default: break;
                }
            }
        }

        renderModeSelect(window, res, menuIndex);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

// ============================================================
// CHON DO KHO AI
// ============================================================

GameScreen handleDifficultySelect(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    int menuIndex = 0;
    const int MENU_COUNT = 5;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Up:
                case sf::Keyboard::W:
                    menuIndex = (menuIndex - 1 + MENU_COUNT) % MENU_COUNT;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = (menuIndex + 1) % MENU_COUNT;
                    break;
                case sf::Keyboard::Enter:
                    soundPlaySelect(res);
                    if (menuIndex == 0) { state.difficulty = BOT_EASY;   return SCREEN_STYLE_SELECT; }
                    if (menuIndex == 1) { state.difficulty = BOT_MEDIUM; return SCREEN_STYLE_SELECT; }
                    if (menuIndex == 2) { state.difficulty = BOT_HARD;   return SCREEN_STYLE_SELECT; }
                    if (menuIndex == 3) { state.difficulty = BOT_EXPERT; return SCREEN_STYLE_SELECT; }
                    if (menuIndex == 4) return SCREEN_MODE_SELECT; // Back
                    break;
                case sf::Keyboard::Escape:
                    return SCREEN_MODE_SELECT;
                default: break;
                }
            }
        }

        renderDifficultySelect(window, res, menuIndex);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

// ============================================================
// CHON KIEU CHOI
// ============================================================

GameScreen handleStyleSelect(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    int menuIndex = 0;
    const int MENU_COUNT = 3;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Up:
                case sf::Keyboard::W:
                    menuIndex = (menuIndex - 1 + MENU_COUNT) % MENU_COUNT;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = (menuIndex + 1) % MENU_COUNT;
                    break;
                case sf::Keyboard::Enter:
                    soundPlaySelect(res);
                    if (menuIndex == 0) { state.style = STYLE_BASIC; return SCREEN_INPUT_NAMES; }
                    if (menuIndex == 1) { state.style = STYLE_SPEED; return SCREEN_INPUT_NAMES; }
                    if (menuIndex == 2) {
                        return (state.mode == MODE_PVC) ? SCREEN_DIFFICULTY : SCREEN_MODE_SELECT;
                    }
                    break;
                case sf::Keyboard::Escape:
                    return (state.mode == MODE_PVC) ? SCREEN_DIFFICULTY : SCREEN_MODE_SELECT;
                default: break;
                }
            }
        }

        renderStyleSelect(window, res, menuIndex);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}


// ============================================================
// NHAP TEN
// ============================================================

GameScreen handleInputNames(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    std::string name1 = "";
    std::string name2 = "";
    bool isEditingP1 = true;
    bool showError = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    return SCREEN_STYLE_SELECT;

                if (event.key.code == sf::Keyboard::Tab)
                    isEditingP1 = !isEditingP1;

                if (event.key.code == sf::Keyboard::Enter) {
                    // Dat ten mac dinh neu de trong
                    if (name1.empty()) name1 = "Player 1";
                    if (name2.empty()) {
                        name2 = (state.mode == MODE_PVC) ? "Computer" : "Player 2";
                    }

                    state.player1.name = name1;
                    state.player2.name = name2;
                    boardResetAll(state);
                    if (state.style == STYLE_SPEED)
                        timerStart(state.timer, MAX_GAME_TIME, MAX_TURN_TIME);

                    return SCREEN_PLAYING;
                }

                // Xoa ky tu
                if (event.key.code == sf::Keyboard::BackSpace) {
                    std::string& current = isEditingP1 ? name1 : name2;
                    if (!current.empty()) current.pop_back();
                }
            }

            // Nhap ky tu
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 32 && event.text.unicode < 128) {
                    std::string& current = isEditingP1 ? name1 : name2;
                    if (current.size() < 15)
                        current += static_cast<char>(event.text.unicode);
                }
            }
        }

        renderInputNames(window, res, name1, name2, isEditingP1, showError);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

// ============================================================
// GAMEPLAY - VONG LAP CHOI GAME
// ============================================================

GameScreen handleGameplay(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    sf::Clock clock;
    WinLine winLine = {};
    GameResult result = RESULT_NONE;
    bool showHint = false;
    int hintRow = -1, hintCol = -1;

    // Bien cho animation dat quan (POLISH)
    int lastPlaceRow = -1, lastPlaceCol = -1;
    float placeAnimTimer = 999.0f; // 999 = khong co animation dang chay
    const float PLACE_ANIM_DURATION = 0.2f;

    // ============================================================
    // HELPER LAMBDA: Bot di 1 nuoc + check ket qua
    // ============================================================
    auto doBotMove = [&]() {
        // Render frame "dang nghi" de nguoi thay
        renderGameplay(window, state, res, nullptr, -1, -1, false);
        window.display();

        int botRow, botCol;
        botGetMove(state, state.difficulty, botRow, botCol);
        state.cursorRow = botRow;
        state.cursorCol = botCol;

        if (boardPlacePiece(state, botRow, botCol)) {
            soundPlayPlace(res);
            lastPlaceRow = botRow; lastPlaceCol = botCol; placeAnimTimer = 0.0f;

            result = boardEvaluateResult(state, botRow, botCol, winLine);
            if (result != RESULT_NONE) {
                if (result == RESULT_PLAYER1_WIN) state.player1.totalWins++;
                else if (result == RESULT_PLAYER2_WIN) state.player2.totalWins++;
                if (result == RESULT_DRAW) soundPlayDraw(res);
                else soundPlayWin(res);
            }
            else {
                boardSwitchTurn(state);
                if (state.style == STYLE_SPEED) timerResetTurn(state.timer);
            }
        }
    };

    // ============================================================
    // HELPER LAMBDA: Nguoi choi dat quan tai (row, col)
    // Goi tu ca phim Enter va Mouse Click
    // ============================================================
    auto doPlayerPlace = [&](int row, int col) {
        if (result != RESULT_NONE) return;
        if (boardPlacePiece(state, row, col)) {
            soundPlayPlace(res);
            showHint = false;
            lastPlaceRow = row; lastPlaceCol = col; placeAnimTimer = 0.0f;

            result = boardEvaluateResult(state, row, col, winLine);
            if (result != RESULT_NONE) {
                if (result == RESULT_PLAYER1_WIN) state.player1.totalWins++;
                else if (result == RESULT_PLAYER2_WIN) state.player2.totalWins++;
                if (result == RESULT_DRAW) soundPlayDraw(res);
                else soundPlayWin(res);
            }
            else {
                boardSwitchTurn(state);
                if (state.style == STYLE_SPEED) timerResetTurn(state.timer);

                // Bot phan ung neu PvC
                if (state.mode == MODE_PVC && !state.isPlayer1Turn) {
                    doBotMove();
                }
            }
        }
    };

    // ===== BOT DI TRUOC NEU LA LUOT CUA BOT (vd: van 2+ trong PvC) =====
    if (state.mode == MODE_PVC && !state.isPlayer1Turn) {
        doBotMove();
    }

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Update animation timer
        if (placeAnimTimer < PLACE_ANIM_DURATION)
            placeAnimTimer += deltaTime;

        // Cap nhat timer (che do Speed)
        if (state.style == STYLE_SPEED && result == RESULT_NONE) {
            timerUpdate(state.timer, deltaTime);

            // --- XỬ LÝ HẾT GIỜ LƯỢT NÀY ---
            if (timerIsTurnExpired(state.timer)) {
                if (state.isPlayer1Turn) {
                    result = RESULT_PLAYER2_WIN;
                    state.player2.totalWins++;
                }
                else {
                    result = RESULT_PLAYER1_WIN;
                    state.player1.totalWins++;
                }

                soundPlayWin(res);
                return handleGameOver(window, res, state, result, winLine);
            }

            // --- KIỂM TRA HẾT GIỜ CẢ VÁN ---
            if (timerIsGameExpired(state.timer) && result == RESULT_NONE) {
                if (state.player1.moves > state.player2.moves)
                    result = RESULT_PLAYER2_WIN;
                else if (state.player2.moves > state.player1.moves)
                    result = RESULT_PLAYER1_WIN;
                else
                    result = RESULT_DRAW;

                if (result == RESULT_DRAW) soundPlayDraw(res);
                else soundPlayWin(res);
                return handleGameOver(window, res, state, result, winLine);
            }
        }

        // Xu ly su kien
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // ===== MOUSE SUPPORT =====
            // Hover: di cursor theo chuot
            if (event.type == sf::Event::MouseMoved && result == RESULT_NONE) {
                int r, c;
                if (renderPixelToBoard((float)event.mouseMove.x,
                    (float)event.mouseMove.y, r, c)) {
                    state.cursorRow = r;
                    state.cursorCol = c;
                }
            }

            // Click trai: dat quan
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left
                && result == RESULT_NONE) {
                int r, c;
                if (renderPixelToBoard((float)event.mouseButton.x,
                    (float)event.mouseButton.y, r, c)) {
                    doPlayerPlace(r, c);
                }
            }

            // Click khi da ket thuc → chuyen GameOver
            if (event.type == sf::Event::MouseButtonPressed && result != RESULT_NONE) {
                return handleGameOver(window, res, state, result, winLine);
            }

            if (event.type == sf::Event::KeyPressed && result == RESULT_NONE) {
                switch (event.key.code) {
                    // Di chuyen cursor
                case sf::Keyboard::W: case sf::Keyboard::Up:
                    boardMoveCursor(state, -1, 0); break;
                case sf::Keyboard::S: case sf::Keyboard::Down:
                    boardMoveCursor(state, 1, 0); break;
                case sf::Keyboard::A: case sf::Keyboard::Left:
                    boardMoveCursor(state, 0, -1); break;
                case sf::Keyboard::D: case sf::Keyboard::Right:
                    boardMoveCursor(state, 0, 1); break;

                    // Dat quan
                case sf::Keyboard::Enter:
                    doPlayerPlace(state.cursorRow, state.cursorCol);
                    break;

                    // Undo
                case sf::Keyboard::Z:
                    boardUndo(state);
                    showHint = false;
                    break;

                case sf::Keyboard::H:
                    if (state.mode == MODE_PVC && state.isPlayer1Turn) {
                        botGetHint(state, hintRow, hintCol);
                        showHint = true;
                    }
                    break;

                    // Save nhanh
                case sf::Keyboard::L: {
                    if (state.style == STYLE_SPEED) timerPause(state.timer);
                    handleSaveScreen(window, res, state);
                    if (state.style == STYLE_SPEED) timerResume(state.timer);
                    clock.restart();
                    break;
                }

                    // Pause
                case sf::Keyboard::Escape: {
                    if (state.style == STYLE_SPEED)
                        timerPause(state.timer);

                    GameScreen pauseResult = handlePauseMenu(window, res, state);

                    if (state.style == STYLE_SPEED)
                        timerResume(state.timer);

                    if (pauseResult != SCREEN_PLAYING) return pauseResult;
                    clock.restart();
                    break;
                }

                default: break;
                }
            }

            // Nhan bat ky phim nao khi da ket thuc → chuyen GameOver
            if (event.type == sf::Event::KeyPressed && result != RESULT_NONE) {
                return handleGameOver(window, res, state, result, winLine);
            }
        }

        // Ve
        renderGameplay(window, state, res,
            (result != RESULT_NONE) ? &winLine : nullptr,
            hintRow, hintCol, showHint);

        // ===== PLACE ANIMATION =====
        if (placeAnimTimer < PLACE_ANIM_DURATION && lastPlaceRow >= 0) {
            float progress = placeAnimTimer / PLACE_ANIM_DURATION;
            int player = state.board[lastPlaceRow][lastPlaceCol].value;
            renderPlaceEffect(window, lastPlaceRow, lastPlaceCol, player, progress);
        }

        window.display();
    }
    return SCREEN_MAIN_MENU;
}



// ============================================================
// PAUSE MENU
// ============================================================

GameScreen handlePauseMenu(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    int menuIndex = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Up:
                case sf::Keyboard::W:
                    menuIndex = (menuIndex - 1 + 3) % 3;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = (menuIndex + 1) % 3;
                    break;
                case sf::Keyboard::Enter:
                    soundPlaySelect(res);
                    if (menuIndex == 0) return SCREEN_PLAYING;     // Resume
                    if (menuIndex == 1) {
                        handleSaveScreen(window, res, state);
                        return SCREEN_PLAYING;                      // Save roi tiep tuc
                    }
                    if (menuIndex == 2) return SCREEN_MAIN_MENU;    // Quit
                    break;
                case sf::Keyboard::Escape:
                    return SCREEN_PLAYING; // Resume
                default: break;
                }
            }
        }

        // Ve gameplay phia sau + overlay pause
        renderGameplay(window, state, res, nullptr, -1, -1, false);
        renderPauseMenu(window, res, menuIndex);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

// ============================================================
// GAME OVER
// ============================================================

GameScreen handleGameOver(sf::RenderWindow& window, GameResources& res,
    GameState& state, GameResult result, const WinLine& winLine) {
    int menuIndex = 0; // 0 = Yes (choi tiep), 1 = No (ve menu)

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Left:
                case sf::Keyboard::A:
                    menuIndex = 0;
                    break;
                case sf::Keyboard::Right:
                case sf::Keyboard::D:
                    menuIndex = 1;
                    break;
                case sf::Keyboard::Enter:
                    soundPlaySelect(res);
                    if (menuIndex == 0) {
                        // Choi van tiep theo
                        boardResetRound(state);
                        if (state.style == STYLE_SPEED)
                            timerStart(state.timer, MAX_GAME_TIME, MAX_TURN_TIME);
                        return SCREEN_PLAYING;
                    }
                    else {
                        return SCREEN_MAIN_MENU;
                    }
                case sf::Keyboard::Escape:
                    return SCREEN_MAIN_MENU;
                default: break;
                }
            }
        }

        renderGameplay(window, state, res, (result != RESULT_DRAW) ? &winLine : nullptr, -1, -1, false);
        renderGameOver(window, state, res, result, menuIndex);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

// ============================================================
// SAVE SCREEN
// ============================================================

GameScreen handleSaveScreen(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    std::string saveList[MAX_SAVE_FILES];
    int count = saveGetList(saveList, MAX_SAVE_FILES);
    int selectedIndex = 0;
    std::string inputName = "";
    std::string message = "";
    sf::Clock messageClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    return SCREEN_PLAYING; // Quay lai game

                case sf::Keyboard::Up:
                    if (selectedIndex > 0) selectedIndex--;
                    break;
                case sf::Keyboard::Down:
                    if (selectedIndex < count - 1) selectedIndex++;
                    break;

                case sf::Keyboard::Enter:
                    if (!inputName.empty()) {
                        if (saveFileExists(inputName)) {
                            message = "File da ton tai!";
                        }
                        else if (saveCountFiles() >= MAX_SAVE_FILES) {
                            message = "Da dat gioi han 15 file!";
                        }
                        else if (saveGame(state, inputName)) {
                            saveAddToList(inputName);
                            message = "Luu thanh cong!";
                            count = saveGetList(saveList, MAX_SAVE_FILES);
                            inputName = "";
                        }
                        else {
                            message = "Loi luu file!";
                        }
                        messageClock.restart();
                    }
                    break;

                case sf::Keyboard::Delete:
                    if (count > 0 && selectedIndex < count) {
                        saveDeleteFile(saveList[selectedIndex]);
                        count = saveGetList(saveList, MAX_SAVE_FILES);
                        if (selectedIndex >= count && count > 0)
                            selectedIndex = count - 1;
                        message = "Da xoa file!";
                        messageClock.restart();
                    }
                    break;

                case sf::Keyboard::BackSpace:
                    if (!inputName.empty()) inputName.pop_back();
                    break;

                default: break;
                }
            }

            // Nhap ten file
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 32 && event.text.unicode < 128
                    && event.text.unicode != '\r' && event.text.unicode != '\b') {
                    char ch = static_cast<char>(event.text.unicode);
                    // Loai bo ky tu khong hop le cho ten file Windows
                    if (ch != '\\' && ch != '/' && ch != ':' && ch != '*'
                        && ch != '?' && ch != '"' && ch != '<' && ch != '>'
                        && ch != '|') {
                        if (inputName.size() < 20) inputName += ch;
                    }
                }
            }
        }

        renderSaveScreen(window, res, saveList, count, inputName, selectedIndex);

        // Hien thong bao 2 giay
        if (!message.empty() && messageClock.getElapsedTime().asSeconds() < 2.0f) {
            renderTextCentered(window, res.mainFont, message, 18,
                WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 80.f,
                sf::Color::Yellow);
        }
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

// ============================================================
// LOAD SCREEN
// ============================================================

GameScreen handleLoadScreen(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    std::string saveList[MAX_SAVE_FILES];
    int count = saveGetList(saveList, MAX_SAVE_FILES);
    int selectedIndex = 0;
    std::string message = "";
    sf::Clock messageClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    return SCREEN_MAIN_MENU;

                case sf::Keyboard::Up: case sf::Keyboard::W:
                    if (selectedIndex > 0) selectedIndex--;
                    break;
                case sf::Keyboard::Down: case sf::Keyboard::S:
                    if (selectedIndex < count - 1) selectedIndex++;
                    break;

                case sf::Keyboard::Enter:
                    if (count > 0 && selectedIndex < count) {
                        if (loadGame(state, saveList[selectedIndex])) {
                            return SCREEN_PLAYING; // Resume game
                        }
                        else {
                            message = "Loi tai file!";
                            messageClock.restart();
                        }
                    }
                    break;

                case sf::Keyboard::Delete:
                    if (count > 0 && selectedIndex < count) {
                        saveDeleteFile(saveList[selectedIndex]);
                        count = saveGetList(saveList, MAX_SAVE_FILES);
                        if (selectedIndex >= count && count > 0)
                            selectedIndex = count - 1;
                    }
                    break;

                default: break;
                }
            }
        }

        renderLoadScreen(window, res, saveList, count, selectedIndex);

        // Hien thong bao loi
        if (!message.empty() && messageClock.getElapsedTime().asSeconds() < 2.0f) {
            renderTextCentered(window, res.mainFont, message, 18,
                WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 80.f,
                sf::Color::Red);
        }
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

// ============================================================
// SETTINGS
// ============================================================

GameScreen handleSettings(sf::RenderWindow& window, GameResources& res) {
    int menuIndex = 0;
    // Doc gia tri hien tai tu sound module (da load tu file lúc khoi dong)
    int volume = soundGetBGMVolume();
    bool sfxOn = soundIsSFXEnabled();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Up:
                case sf::Keyboard::W:
                    menuIndex = (menuIndex - 1 + 4) % 4;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = (menuIndex + 1) % 4;
                    break;
                case sf::Keyboard::Enter:
                    soundPlaySelect(res);
                    if (menuIndex == 0) {
                        langToggle();
                        settingsSave();           // Persist language
                    }
                    if (menuIndex == 2) {
                        sfxOn = !sfxOn;
                        soundSetSFXEnabled(sfxOn);
                        settingsSave();           // Persist SFX
                    }
                    if (menuIndex == 3) return SCREEN_MAIN_MENU; // Back
                    break;
                case sf::Keyboard::Left:
                    if (menuIndex == 1 && volume > 0) {
                        volume -= 10;
                        soundSetBGMVolume(res, volume);
                        settingsSave();           // Persist volume
                    }
                    break;
                case sf::Keyboard::Right:
                    if (menuIndex == 1 && volume < 100) {
                        volume += 10;
                        soundSetBGMVolume(res, volume);
                        settingsSave();           // Persist volume
                    }
                    break;
                case sf::Keyboard::Escape:
                    return SCREEN_MAIN_MENU;
                default: break;
                }
            }
        }

        renderSettings(window, res, menuIndex, langGetCurrent(), volume, sfxOn);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

// ============================================================
// HELP & ABOUT
// ============================================================

GameScreen handleHelp(sf::RenderWindow& window, GameResources& res) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                return SCREEN_MAIN_MENU;
        }
        renderHelp(window, res);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

GameScreen handleAbout(sf::RenderWindow& window, GameResources& res) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                return SCREEN_MAIN_MENU;
        }
        renderAbout(window, res);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}
