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

// Helper: hit-test menu items can giua man hinh (dung cho menu chuan)
// Tra ve index cua item duoc hover, hoac -1 neu khong trung
static int menuHitTest(float mx, float my, float startY, float step,
    int itemCount, float halfW = 200.f, float halfH = 22.f) {
    if (mx < WINDOW_WIDTH / 2.f - halfW || mx > WINDOW_WIDTH / 2.f + halfW)
        return -1;
    for (int i = 0; i < itemCount; i++) {
        float itemY = startY + i * step;
        if (my > itemY - halfH && my < itemY + halfH)
            return i;
    }
    return -1;
}

GameScreen handleMainMenu(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    int menuIndex = 0;
    const int MENU_COUNT = 6;

    // Lambda: thuc thi nuoc duoc chon
    auto confirm = [&]() -> GameScreen {
        soundPlaySelect(res);
        switch (menuIndex) {
        case 0: return SCREEN_MODE_SELECT;
        case 1: return SCREEN_LOAD;
        case 2: return SCREEN_SETTINGS;
        case 3: return SCREEN_HELP;
        case 4: return SCREEN_ABOUT;
        case 5: window.close(); return SCREEN_MAIN_MENU;
        }
        return SCREEN_MAIN_MENU;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            // MOUSE HOVER
            if (event.type == sf::Event::MouseMoved) {
                int hit = menuHitTest((float)event.mouseMove.x,
                    (float)event.mouseMove.y, 280.f, 60.f, MENU_COUNT);
                if (hit >= 0) menuIndex = hit;
            }

            // MOUSE CLICK
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                int hit = menuHitTest((float)event.mouseButton.x,
                    (float)event.mouseButton.y, 280.f, 60.f, MENU_COUNT);
                if (hit >= 0) {
                    menuIndex = hit;
                    return confirm();
                }
            }

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
                    return confirm();
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

    auto confirm = [&]() -> GameScreen {
        soundPlaySelect(res);
        if (menuIndex == 0) { state.mode = MODE_PVP; return SCREEN_STYLE_SELECT; }
        if (menuIndex == 1) { state.mode = MODE_PVC; return SCREEN_DIFFICULTY; }
        if (menuIndex == 2) return SCREEN_MAIN_MENU;
        return SCREEN_MAIN_MENU;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseMoved) {
                int hit = menuHitTest((float)event.mouseMove.x,
                    (float)event.mouseMove.y, UI_MENU_START_Y, UI_MENU_STEP, MENU_COUNT);
                if (hit >= 0) menuIndex = hit;
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                int hit = menuHitTest((float)event.mouseButton.x,
                    (float)event.mouseButton.y, UI_MENU_START_Y, UI_MENU_STEP, MENU_COUNT);
                if (hit >= 0) { menuIndex = hit; return confirm(); }
            }

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
                    return confirm();
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

    auto confirm = [&]() -> GameScreen {
        soundPlaySelect(res);
        if (menuIndex == 0) { state.difficulty = BOT_EASY;   return SCREEN_STYLE_SELECT; }
        if (menuIndex == 1) { state.difficulty = BOT_MEDIUM; return SCREEN_STYLE_SELECT; }
        if (menuIndex == 2) { state.difficulty = BOT_HARD;   return SCREEN_STYLE_SELECT; }
        if (menuIndex == 3) { state.difficulty = BOT_EXPERT; return SCREEN_STYLE_SELECT; }
        if (menuIndex == 4) return SCREEN_MODE_SELECT;
        return SCREEN_MODE_SELECT;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseMoved) {
                int hit = menuHitTest((float)event.mouseMove.x,
                    (float)event.mouseMove.y, UI_MENU_START_Y, UI_MENU_STEP, MENU_COUNT);
                if (hit >= 0) menuIndex = hit;
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                int hit = menuHitTest((float)event.mouseButton.x,
                    (float)event.mouseButton.y, UI_MENU_START_Y, UI_MENU_STEP, MENU_COUNT);
                if (hit >= 0) { menuIndex = hit; return confirm(); }
            }

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
                    return confirm();
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

    auto confirm = [&]() -> GameScreen {
        soundPlaySelect(res);
        if (menuIndex == 0) { state.style = STYLE_BASIC; return SCREEN_INPUT_NAMES; }
        if (menuIndex == 1) { state.style = STYLE_SPEED; return SCREEN_INPUT_NAMES; }
        if (menuIndex == 2) {
            return (state.mode == MODE_PVC) ? SCREEN_DIFFICULTY : SCREEN_MODE_SELECT;
        }
        return SCREEN_MAIN_MENU;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseMoved) {
                int hit = menuHitTest((float)event.mouseMove.x,
                    (float)event.mouseMove.y, UI_MENU_START_Y, UI_MENU_STEP, MENU_COUNT);
                if (hit >= 0) menuIndex = hit;
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                int hit = menuHitTest((float)event.mouseButton.x,
                    (float)event.mouseButton.y, UI_MENU_START_Y, UI_MENU_STEP, MENU_COUNT);
                if (hit >= 0) { menuIndex = hit; return confirm(); }
            }

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
                    return confirm();
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
    const bool isPvC = (state.mode == MODE_PVC);
    TextStrings txt = langGetText(langGetCurrent());

    std::string name1 = "";
    std::string name2 = "";
    bool isEditingP1 = true;       // PvC: luon edit P1
    bool showError = false;

    // Helper: encode Unicode codepoint thanh UTF-8 bytes
    auto utf32ToUtf8 = [](uint32_t cp) -> std::string {
        std::string r;
        if (cp < 0x80) {
            r += (char)cp;
        } else if (cp < 0x800) {
            r += (char)(0xC0 | (cp >> 6));
            r += (char)(0x80 | (cp & 0x3F));
        } else if (cp < 0x10000) {
            r += (char)(0xE0 | (cp >> 12));
            r += (char)(0x80 | ((cp >> 6) & 0x3F));
            r += (char)(0x80 | (cp & 0x3F));
        } else {
            r += (char)(0xF0 | (cp >> 18));
            r += (char)(0x80 | ((cp >> 12) & 0x3F));
            r += (char)(0x80 | ((cp >> 6) & 0x3F));
            r += (char)(0x80 | (cp & 0x3F));
        }
        return r;
    };

    // Helper: xoa 1 ky tu UTF-8 (co the 1-4 bytes) tu cuoi chuoi
    auto utf8PopBack = [](std::string& s) {
        if (s.empty()) return;
        // Xoa cac continuation bytes (10xxxxxx) o cuoi
        while (!s.empty() && ((unsigned char)s.back() & 0xC0) == 0x80) {
            s.pop_back();
        }
        // Xoa byte mo dau
        if (!s.empty()) s.pop_back();
    };

    // Gioi han byte (~15-20 ky tu Viet hoac 30 ky tu ASCII)
    const size_t MAX_NAME_BYTES = 30;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    return SCREEN_STYLE_SELECT;

                // Chi cho phep Tab khi PvP (PvC chi co 1 o)
                if (!isPvC && event.key.code == sf::Keyboard::Tab)
                    isEditingP1 = !isEditingP1;

                if (event.key.code == sf::Keyboard::Enter) {
                    // Dat ten mac dinh neu de trong
                    if (name1.empty()) name1 = "Player 1";
                    if (isPvC) {
                        name2 = txt.botName;  // "May" / "Computer"
                    } else if (name2.empty()) {
                        name2 = "Player 2";
                    }

                    state.player1.name = name1;
                    state.player2.name = name2;
                    boardResetAll(state);
                    if (state.style == STYLE_SPEED)
                        timerStart(state.timer, MAX_GAME_TIME, MAX_TURN_TIME);

                    return SCREEN_PLAYING;
                }

                // Xoa 1 ky tu UTF-8 (PvC: chi xoa name1)
                if (event.key.code == sf::Keyboard::BackSpace) {
                    std::string& current = (isPvC || isEditingP1) ? name1 : name2;
                    utf8PopBack(current);
                }
            }

            // Nhap ky tu (HO TRO UNICODE - tieng Viet, etc.)
            if (event.type == sf::Event::TextEntered) {
                uint32_t cp = event.text.unicode;
                // Bo qua control chars va DEL
                if (cp >= 32 && cp != 127) {
                    std::string& current = (isPvC || isEditingP1) ? name1 : name2;
                    std::string utf8 = utf32ToUtf8(cp);
                    if (current.size() + utf8.size() <= MAX_NAME_BYTES) {
                        current += utf8;
                    }
                }
            }
        }

        renderInputNames(window, res, name1, name2, isEditingP1, showError, isPvC);
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
            if (handleCommonEvent(window, event)) continue;

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
    const int MENU_COUNT = 3;

    auto confirm = [&]() -> GameScreen {
        soundPlaySelect(res);
        if (menuIndex == 0) return SCREEN_PLAYING;
        if (menuIndex == 1) {
            handleSaveScreen(window, res, state);
            return SCREEN_PLAYING;
        }
        if (menuIndex == 2) return SCREEN_MAIN_MENU;
        return SCREEN_PLAYING;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            // Pause menu
            if (event.type == sf::Event::MouseMoved) {
                int hit = menuHitTest((float)event.mouseMove.x,
                    (float)event.mouseMove.y, UI_PAUSE_START_Y, UI_PAUSE_STEP, MENU_COUNT);
                if (hit >= 0) menuIndex = hit;
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                int hit = menuHitTest((float)event.mouseButton.x,
                    (float)event.mouseButton.y, UI_PAUSE_START_Y, UI_PAUSE_STEP, MENU_COUNT);
                if (hit >= 0) { menuIndex = hit; return confirm(); }
            }

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
                    return confirm();
                case sf::Keyboard::Escape:
                    return SCREEN_PLAYING;
                default: break;
                }
            }
        }

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

    // Vi tri nut Yes/No - TINH TU CUNG CONG THUC voi renderGameOver
    // de bao dam dong bo khi user thay doi layout
    const float panelX = UI_BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + UI_PANEL_GAP_LEFT;
    const float panelW = WINDOW_WIDTH - panelX - UI_PANEL_GAP_RIGHT;
    const float centerX = panelX + panelW / 2.f;
    const float yesX = centerX - UI_GAMEOVER_BTN_GAP_X;
    const float noX = centerX + UI_GAMEOVER_BTN_GAP_X;
    const float btnY = UI_GAMEOVER_START_Y + UI_GAMEOVER_BTN_DY;
    const float btnHalfW = UI_GAMEOVER_BTN_HALF_W;
    const float btnHalfH = UI_GAMEOVER_BTN_HALF_H;

    auto confirm = [&]() -> GameScreen {
        soundPlaySelect(res);
        if (menuIndex == 0) {
            boardResetRound(state);
            if (state.style == STYLE_SPEED)
                timerStart(state.timer, MAX_GAME_TIME, MAX_TURN_TIME);
            return SCREEN_PLAYING;
        }
        return SCREEN_MAIN_MENU;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            // Mouse hover: detect Yes/No
            if (event.type == sf::Event::MouseMoved) {
                float mx = (float)event.mouseMove.x;
                float my = (float)event.mouseMove.y;
                if (my > btnY - btnHalfH && my < btnY + btnHalfH) {
                    if (mx > yesX - btnHalfW && mx < yesX + btnHalfW) menuIndex = 0;
                    else if (mx > noX - btnHalfW && mx < noX + btnHalfW) menuIndex = 1;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;
                if (my > btnY - btnHalfH && my < btnY + btnHalfH) {
                    if (mx > yesX - btnHalfW && mx < yesX + btnHalfW) {
                        menuIndex = 0; return confirm();
                    }
                    if (mx > noX - btnHalfW && mx < noX + btnHalfW) {
                        menuIndex = 1; return confirm();
                    }
                }
            }

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
                    return confirm();
                case sf::Keyboard::Escape:
                    return SCREEN_MAIN_MENU;
                default: break;
                }
            }
        }

        renderGameplay(window, state, res, (result != RESULT_DRAW) ? &winLine : nullptr, -1, -1, false, result);
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

    // Lambda: thuc hien luu (Enter hoac click nut Save)
    auto doSave = [&]() {
        if (inputName.empty()) return;
        TextStrings txt = langGetText(langGetCurrent());
        if (saveFileExists(inputName)) {
            message = txt.msgFileExists;
        }
        else if (saveCountFiles() >= MAX_SAVE_FILES) {
            message = txt.msgMaxFiles;
        }
        else if (saveGame(state, inputName)) {
            saveAddToList(inputName);
            message = txt.msgSaveOK;
            count = saveGetList(saveList, MAX_SAVE_FILES);
            inputName = "";
        }
        else {
            message = txt.msgSaveError;
        }
        messageClock.restart();
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            // Mouse: click vao file trong list → copy ten vao input (de re-save / rename)
            // Save list: Y = 200 + i * 35, width 500, height 30, X center WINDOW_WIDTH/2
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;
                // Hit-test save list
                for (int i = 0; i < count; i++) {
                    float itemY = UI_SAVE_LIST_START_Y + i * UI_LIST_STEP;
                    if (mx > WINDOW_WIDTH / 2.f - UI_LIST_HALF_WIDTH
                        && mx < WINDOW_WIDTH / 2.f + UI_LIST_HALF_WIDTH
                        && my > itemY - UI_LIST_HALF_HEIGHT && my < itemY + UI_LIST_HALF_HEIGHT) {
                        selectedIndex = i;
                        inputName = saveList[i];  // Copy ten de re-save
                        break;
                    }
                }
            }

            // Right click: xoa file dang chon
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Right) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;
                for (int i = 0; i < count; i++) {
                    float itemY = UI_SAVE_LIST_START_Y + i * UI_LIST_STEP;
                    if (mx > WINDOW_WIDTH / 2.f - UI_LIST_HALF_WIDTH
                        && mx < WINDOW_WIDTH / 2.f + UI_LIST_HALF_WIDTH
                        && my > itemY - UI_LIST_HALF_HEIGHT && my < itemY + UI_LIST_HALF_HEIGHT) {
                        saveDeleteFile(saveList[i]);
                        count = saveGetList(saveList, MAX_SAVE_FILES);
                        if (selectedIndex >= count && count > 0)
                            selectedIndex = count - 1;
                        message = langGetText(langGetCurrent()).msgFileDeleted;
                        messageClock.restart();
                        break;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    return SCREEN_PLAYING;

                case sf::Keyboard::Up:
                    if (selectedIndex > 0) selectedIndex--;
                    break;
                case sf::Keyboard::Down:
                    if (selectedIndex < count - 1) selectedIndex++;
                    break;

                case sf::Keyboard::Enter:
                    doSave();
                    break;

                case sf::Keyboard::Delete:
                    if (count > 0 && selectedIndex < count) {
                        saveDeleteFile(saveList[selectedIndex]);
                        count = saveGetList(saveList, MAX_SAVE_FILES);
                        if (selectedIndex >= count && count > 0)
                            selectedIndex = count - 1;
                        message = langGetText(langGetCurrent()).msgFileDeleted;
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
                    if (ch != '\\' && ch != '/' && ch != ':' && ch != '*'
                        && ch != '?' && ch != '"' && ch != '<' && ch != '>'
                        && ch != '|') {
                        if (inputName.size() < 20) inputName += ch;
                    }
                }
            }
        }

        renderSaveScreen(window, res, saveList, count, inputName, selectedIndex);

        // (Hint mouse da duoc render trong renderSaveScreen)

        if (!message.empty() && messageClock.getElapsedTime().asSeconds() < 2.0f) {
            renderTextCentered(window, res.mainFont, message, 18,
                WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 90.f,
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

    // Double-click detection
    sf::Clock dblClickClock;
    int lastClickIndex = -1;
    const float DBL_CLICK_TIME = 0.4f;

    auto doLoad = [&]() -> bool {
        if (count > 0 && selectedIndex < count) {
            if (loadGame(state, saveList[selectedIndex])) {
                return true;
            }
            message = langGetText(langGetCurrent()).msgLoadError;
            messageClock.restart();
        }
        return false;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            // Mouse hover: highlight item
            if (event.type == sf::Event::MouseMoved) {
                float mx = (float)event.mouseMove.x;
                float my = (float)event.mouseMove.y;
                for (int i = 0; i < count; i++) {
                    float itemY = UI_LOAD_LIST_START_Y + i * UI_LIST_STEP;
                    if (mx > WINDOW_WIDTH / 2.f - UI_LIST_HALF_WIDTH
                        && mx < WINDOW_WIDTH / 2.f + UI_LIST_HALF_WIDTH
                        && my > itemY - UI_LIST_HALF_HEIGHT && my < itemY + UI_LIST_HALF_HEIGHT) {
                        selectedIndex = i;
                        break;
                    }
                }
            }

            // Click trai: chon, double-click → load
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;
                for (int i = 0; i < count; i++) {
                    float itemY = UI_LOAD_LIST_START_Y + i * UI_LIST_STEP;
                    if (mx > WINDOW_WIDTH / 2.f - UI_LIST_HALF_WIDTH
                        && mx < WINDOW_WIDTH / 2.f + UI_LIST_HALF_WIDTH
                        && my > itemY - UI_LIST_HALF_HEIGHT && my < itemY + UI_LIST_HALF_HEIGHT) {
                        if (lastClickIndex == i
                            && dblClickClock.getElapsedTime().asSeconds() < DBL_CLICK_TIME) {
                            // Double-click → load
                            selectedIndex = i;
                            if (doLoad()) return SCREEN_PLAYING;
                            lastClickIndex = -1;
                        }
                        else {
                            // Single click → select
                            selectedIndex = i;
                            lastClickIndex = i;
                            dblClickClock.restart();
                        }
                        break;
                    }
                }
            }

            // Click phai: xoa file
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Right) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;
                for (int i = 0; i < count; i++) {
                    float itemY = UI_LOAD_LIST_START_Y + i * UI_LIST_STEP;
                    if (mx > WINDOW_WIDTH / 2.f - UI_LIST_HALF_WIDTH
                        && mx < WINDOW_WIDTH / 2.f + UI_LIST_HALF_WIDTH
                        && my > itemY - UI_LIST_HALF_HEIGHT && my < itemY + UI_LIST_HALF_HEIGHT) {
                        saveDeleteFile(saveList[i]);
                        count = saveGetList(saveList, MAX_SAVE_FILES);
                        if (selectedIndex >= count && count > 0)
                            selectedIndex = count - 1;
                        break;
                    }
                }
            }

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
                    if (doLoad()) return SCREEN_PLAYING;
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

        // (Hint mouse da duoc render trong renderLoadScreen)

        if (!message.empty() && messageClock.getElapsedTime().asSeconds() < 2.0f) {
            renderTextCentered(window, res.mainFont, message, 18,
                WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 90.f,
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
    bool isDraggingVolume = false;

    // Hang so slider (PHAI khop voi renderSettings)
    const float TRACK_W = 360.f;
    const float TRACK_X = WINDOW_WIDTH / 2.f - TRACK_W / 2.f;
    const float SLIDER_Y = UI_SETTINGS_START_Y + 1 * UI_SETTINGS_STEP + 15.f;
    const float SLIDER_HIT_HEIGHT = 20.f;  // hit-area generous cho click slider

    // Lambda: dat volume theo vi tri x cua chuot, snap moi 5%
    auto setVolumeFromX = [&](float mx) {
        float ratio = (mx - TRACK_X) / TRACK_W;
        if (ratio < 0.f) ratio = 0.f;
        if (ratio > 1.f) ratio = 1.f;
        int newVol = (int)(ratio * 100.f + 0.5f);
        newVol = (newVol / 5) * 5;  // snap moi 5%
        if (newVol != volume) {
            volume = newVol;
            soundSetBGMVolume(res, volume);
            settingsSave();
        }
    };

    // Lambda: kiem tra (mx,my) co nam trong vung slider khong
    auto isOnSlider = [&](float mx, float my) -> bool {
        return my >= SLIDER_Y - SLIDER_HIT_HEIGHT
            && my <= SLIDER_Y + SLIDER_HIT_HEIGHT
            && mx >= TRACK_X - 20.f && mx <= TRACK_X + TRACK_W + 20.f;
    };

    // Lambda: thuc hien action tren menuIndex hien tai
    auto confirm = [&]() -> GameScreen {
        soundPlaySelect(res);
        if (menuIndex == 0) {
            langToggle();
            settingsSave();
        }
        else if (menuIndex == 2) {
            sfxOn = !sfxOn;
            soundSetSFXEnabled(sfxOn);
            settingsSave();
        }
        else if (menuIndex == 3) return SCREEN_MAIN_MENU;
        return SCREEN_SETTINGS;  // stay in settings
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            // Mouse hover: update menuIndex - Settings items
            if (event.type == sf::Event::MouseMoved) {
                float mx = (float)event.mouseMove.x;
                float my = (float)event.mouseMove.y;

                // Neu dang keo slider: update volume theo chuot
                if (isDraggingVolume) {
                    setVolumeFromX(mx);
                    menuIndex = 1;
                }
                else {
                    int hit = menuHitTest(mx, my,
                        UI_SETTINGS_START_Y, UI_SETTINGS_STEP, 4,
                        UI_SETTINGS_HALF_WIDTH, 25.f);
                    if (hit >= 0) menuIndex = hit;
                    // Hover len slider area cung select volume row
                    else if (isOnSlider(mx, my)) menuIndex = 1;
                }
            }

            // Mouse release: thoat che do keo
            if (event.type == sf::Event::MouseButtonReleased
                && event.mouseButton.button == sf::Mouse::Left) {
                isDraggingVolume = false;
            }

            // Mouse click trai
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                // 1) Click vao slider area → set volume + bat dau drag
                if (isOnSlider(mx, my)) {
                    menuIndex = 1;
                    isDraggingVolume = true;
                    setVolumeFromX(mx);
                }
                else {
                    int hit = menuHitTest(mx, my,
                        UI_SETTINGS_START_Y, UI_SETTINGS_STEP, 4,
                        UI_SETTINGS_HALF_WIDTH, 25.f);
                    if (hit >= 0) {
                        menuIndex = hit;
                        // 2) Click khac → confirm action (lang, sfx, back)
                        if (menuIndex != 1) {
                            GameScreen next = confirm();
                            if (next != SCREEN_SETTINGS) return next;
                        }
                    }
                }
            }

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
                case sf::Keyboard::Enter: {
                    GameScreen next = confirm();
                    if (next != SCREEN_SETTINGS) return next;
                    break;
                }
                case sf::Keyboard::Left:
                    if (menuIndex == 1 && volume > 0) {
                        volume -= 10;
                        soundSetBGMVolume(res, volume);
                        settingsSave();
                    }
                    break;
                case sf::Keyboard::Right:
                    if (menuIndex == 1 && volume < 100) {
                        volume += 10;
                        soundSetBGMVolume(res, volume);
                        settingsSave();
                    }
                    break;
                case sf::Keyboard::Escape:
                    return SCREEN_MAIN_MENU;
                default: break;
                }
            }
        }

        renderSettings(window, res, menuIndex, langGetCurrent(), volume, sfxOn);

        // (Hint Volume da duoc render trong renderSettings)

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
            if (handleCommonEvent(window, event)) continue;
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                return SCREEN_MAIN_MENU;
            // Click bat ky → ve menu chinh
            if (event.type == sf::Event::MouseButtonPressed)
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
            if (handleCommonEvent(window, event)) continue;
            // Click bat ky → ve menu chinh
            if (event.type == sf::Event::MouseButtonPressed)
                return SCREEN_MAIN_MENU;
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                return SCREEN_MAIN_MENU;
        }
        renderAbout(window, res);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}
