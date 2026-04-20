#include "menu.h"
#include "board.h"
#include "render.h"
#include "timer.h"
#include "language.h"


// GAME LOOP CHINH
void gameRun(sf::RenderWindow& window, GameResources& res) {
    GameState state;
    boardResetAll(state);

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

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Cap nhat timer (che do Speed)
 // Cap nhat timer (che do Speed)
        if (state.style == STYLE_SPEED && result == RESULT_NONE) {
            timerUpdate(state.timer, deltaTime);

            // --- XỬ LÝ HẾT GIỜ LƯỢT NÀY ---
            if (timerIsTurnExpired(state.timer)) {
                if (state.isPlayer1Turn) {
                    result = RESULT_PLAYER2_WIN; // P1 hết giờ -> P2 thắng
                    state.player2.totalWins++;
                }
                else {
                    result = RESULT_PLAYER1_WIN; // P2 hết giờ -> P1 thắng
                    state.player1.totalWins++;
                }

                // FIX: Trả thẳng về màn hình Game Over ngay lập tức!
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

                // FIX: Trả thẳng về màn hình Game Over ngay lập tức!
                return handleGameOver(window, res, state, result, winLine);
            }
        }

        // Xu ly su kien
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

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
                    if (boardPlacePiece(state, state.cursorRow, state.cursorCol)) {
                        showHint = false;
                        result = boardEvaluateResult(state, state.cursorRow,
                            state.cursorCol, winLine);
                        if (result != RESULT_NONE) {
                            if (result == RESULT_PLAYER1_WIN)
                                state.player1.totalWins++;
                            else if (result == RESULT_PLAYER2_WIN)
                                state.player2.totalWins++;
                        }
                        else {
                            boardSwitchTurn(state);
                            if (state.style == STYLE_SPEED)
                                timerResetTurn(state.timer);
                        }
                    }
                    break;

                    // Undo
                case sf::Keyboard::Z:
                    boardUndo(state);
                    showHint = false;
                    break;

                    // Pause
                case sf::Keyboard::Escape: {
                    if (state.style == STYLE_SPEED)
                        timerPause(state.timer);

                    GameScreen pauseResult = handlePauseMenu(window, res, state);

                    if (state.style == STYLE_SPEED)
                        timerResume(state.timer);

                    if (pauseResult != SCREEN_PLAYING) return pauseResult;
                    clock.restart(); // Reset clock sau khi pause
                    break;
                }

                default: break;
                }
            }

            // Nhan bat ky phim nao khi da ket thuc → chuyen sang GameOver
            if (event.type == sf::Event::KeyPressed && result != RESULT_NONE) {
                // Truyền thêm &winLine vào đây
                return handleGameOver(window, res, state, result, winLine);
            }
        }

        // Ve
        renderGameplay(window, state, res,
            (result != RESULT_NONE) ? &winLine : nullptr,
            hintRow, hintCol, showHint);
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
// SAVE / LOAD (don gian - chua co save_load.cpp)
// ============================================================

GameScreen handleSaveScreen(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    // Tam thoi: hien thong bao roi quay lai
    sf::Clock timer;
    while (timer.getElapsedTime().asSeconds() < 1.5f && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) return SCREEN_PLAYING;
        }
        window.clear(COLOR_MENU_BG);
        renderTextCentered(window, res.mainFont, "Save - Coming soon!", 24,
            WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f, sf::Color::White);
        window.display();
    }
    return SCREEN_PLAYING;
}

GameScreen handleLoadScreen(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    sf::Clock timer;
    while (timer.getElapsedTime().asSeconds() < 1.5f && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed) return SCREEN_MAIN_MENU;
        }
        window.clear(COLOR_MENU_BG);
        renderTextCentered(window, res.mainFont, "Load - Coming soon!", 24,
            WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f, sf::Color::White);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

// ============================================================
// SETTINGS
// ============================================================

GameScreen handleSettings(sf::RenderWindow& window, GameResources& res) {
    int menuIndex = 0;
    int volume = 50;
    bool sfxOn = true;

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
                    if (menuIndex == 0) langToggle();         // Doi ngon ngu
                    if (menuIndex == 2) sfxOn = !sfxOn;       // Toggle SFX
                    if (menuIndex == 3) return SCREEN_MAIN_MENU; // Back
                    break;
                case sf::Keyboard::Left:
                    if (menuIndex == 1 && volume > 0) volume -= 10;
                    break;
                case sf::Keyboard::Right:
                    if (menuIndex == 1 && volume < 100) volume += 10;
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
