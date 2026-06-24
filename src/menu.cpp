#include "menu.h"
#include "board.h"
#include "render.h"
#include "timer.h"
#include "language.h"
#include "bot.h"
#include "save_load.h"
#include "sound.h"

void gameRun(sf::RenderWindow& window, GameResources& res) {
    GameState state;
    boardResetAll(state);

    soundPlayBGMTrack(res, BGM_MENU);

    GameScreen currentScreen = SCREEN_MAIN_MENU;

    while (window.isOpen()) {
        GameScreen nextScreen = currentScreen;

        soundPlayBGMTrack(res, (currentScreen == SCREEN_PLAYING) ? BGM_GAME : BGM_MENU);

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
        case SCREEN_CHAR_SELECT:
            nextScreen = handleCharSelect(window, res, state);
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

            nextScreen = SCREEN_MAIN_MENU;
            break;
        }

        currentScreen = nextScreen;
    }
}

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

static int menuNavStep(int index, int count, int delta) {
    return ((index + delta) % count + count) % count;
}

GameScreen handleMainMenu(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    int menuIndex = 0;
    const int MENU_COUNT = 6;

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

            if (event.type == sf::Event::MouseMoved) {
                int hit = menuHitTest((float)event.mouseMove.x,
                    (float)event.mouseMove.y, UI_MENU_START_Y, UI_MENU_STEP, MENU_COUNT);
                if (hit >= 0) menuIndex = hit;
            }

            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                int hit = menuHitTest((float)event.mouseButton.x,
                    (float)event.mouseButton.y, UI_MENU_START_Y, UI_MENU_STEP, MENU_COUNT);
                if (hit >= 0) {
                    menuIndex = hit;
                    return confirm();
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Up:
                case sf::Keyboard::W:
                    menuIndex = menuNavStep(menuIndex, MENU_COUNT, -1);
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = menuNavStep(menuIndex, MENU_COUNT, +1);
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

GameScreen handleModeSelect(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    int menuIndex = 0;

    auto confirm = [&]() -> GameScreen {
        soundPlaySelect(res);
        if (menuIndex == 0) { state.mode = MODE_PVP; return SCREEN_STYLE_SELECT; }
        state.mode = MODE_PVC;
        return SCREEN_DIFFICULTY;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseMoved) {
                int hit = modeSelectHitTest((float)event.mouseMove.x,
                    (float)event.mouseMove.y);
                if (hit >= 0) menuIndex = hit;
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;
                if (backButtonContains(mx, my)) {
                    soundPlaySelect(res);
                    return SCREEN_MAIN_MENU;
                }
                int hit = modeSelectHitTest(mx, my);
                if (hit >= 0) { menuIndex = hit; return confirm(); }
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Left:  case sf::Keyboard::A:
                case sf::Keyboard::Right: case sf::Keyboard::D:
                    menuIndex = menuNavStep(menuIndex, 2, +1);
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

GameScreen handleDifficultySelect(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    int menuIndex = 0;

    auto confirm = [&]() -> GameScreen {
        soundPlaySelect(res);
        switch (menuIndex) {
        case 0:  state.difficulty = BOT_EASY;   break;
        case 1:  state.difficulty = BOT_MEDIUM; break;
        case 2:  state.difficulty = BOT_HARD;   break;
        default: state.difficulty = BOT_EXPERT; break;
        }
        return SCREEN_STYLE_SELECT;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseMoved) {
                int hit = diffSelectHitTest((float)event.mouseMove.x,
                    (float)event.mouseMove.y);
                if (hit >= 0) menuIndex = hit;
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;
                if (backButtonContains(mx, my)) {
                    soundPlaySelect(res);
                    return SCREEN_MODE_SELECT;
                }
                int hit = diffSelectHitTest(mx, my);
                if (hit >= 0) { menuIndex = hit; return confirm(); }
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Left: case sf::Keyboard::A:
                    menuIndex = menuNavStep(menuIndex, 4, -1);
                    break;
                case sf::Keyboard::Right: case sf::Keyboard::D:
                    menuIndex = menuNavStep(menuIndex, 4, +1);
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

GameScreen handleStyleSelect(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    int menuIndex = 0;
    const int MENU_COUNT = 3;

    auto confirm = [&]() -> GameScreen {
        soundPlaySelect(res);

        if (menuIndex == 0) { state.style = STYLE_BASIC; return SCREEN_CHAR_SELECT; }
        if (menuIndex == 1) { state.style = STYLE_SPEED; return SCREEN_CHAR_SELECT; }
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
                    menuIndex = menuNavStep(menuIndex, MENU_COUNT, -1);
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = menuNavStep(menuIndex, MENU_COUNT, +1);
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

GameScreen handleCharSelect(sf::RenderWindow& window, GameResources& res,
    GameState& state) {

    int pickingPlayer = 1;
    int takenIndex = -1;
    int menuIndex = state.heroP1;

    auto confirm = [&]() -> GameScreen {

        if (pickingPlayer == 2 && menuIndex == takenIndex)
            return SCREEN_CHAR_SELECT;
        soundPlaySelect(res);
        if (pickingPlayer == 1) {
            state.heroP1 = menuIndex;
            if (state.mode == MODE_PVC) return SCREEN_INPUT_NAMES;

            pickingPlayer = 2;
            takenIndex = state.heroP1;
            menuIndex = (state.heroP2 != takenIndex) ? state.heroP2
                                                     : (takenIndex + 1) % 6;
            return SCREEN_CHAR_SELECT;
        }
        state.heroP2 = menuIndex;
        return SCREEN_INPUT_NAMES;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseMoved) {
                int hit = charSelectHitTest((float)event.mouseMove.x,
                    (float)event.mouseMove.y);
                if (hit >= 0) menuIndex = hit;
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;
                if (backButtonContains(mx, my)) {
                    soundPlaySelect(res);
                    if (pickingPlayer == 2) {
                        pickingPlayer = 1; takenIndex = -1;
                        menuIndex = state.heroP1;
                        continue;
                    }
                    return SCREEN_STYLE_SELECT;
                }
                int hit = charSelectHitTest(mx, my);
                if (hit >= 0) {
                    menuIndex = hit;
                    GameScreen r = confirm();
                    if (r != SCREEN_CHAR_SELECT) return r;
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Left: case sf::Keyboard::A:
                    menuIndex = menuNavStep(menuIndex, 6, -1);
                    break;
                case sf::Keyboard::Right: case sf::Keyboard::D:
                    menuIndex = menuNavStep(menuIndex, 6, +1);
                    break;
                case sf::Keyboard::Up:   case sf::Keyboard::W:
                case sf::Keyboard::Down: case sf::Keyboard::S:
                    menuIndex = menuNavStep(menuIndex, 6, +3);
                    break;
                case sf::Keyboard::Enter: {
                    GameScreen r = confirm();
                    if (r != SCREEN_CHAR_SELECT) return r;
                    break;
                }
                case sf::Keyboard::Escape:
                    if (pickingPlayer == 2) {
                        pickingPlayer = 1; takenIndex = -1;
                        menuIndex = state.heroP1;
                        break;
                    }
                    return SCREEN_STYLE_SELECT;
                default: break;
                }
            }
        }

        renderCharSelect(window, res, menuIndex, pickingPlayer, takenIndex);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

GameScreen handleInputNames(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    const bool isPvC = (state.mode == MODE_PVC);
    const TextStrings& txt = langGetText(langGetCurrent());

    std::string name1 = "";
    std::string name2 = "";
    bool isEditingP1 = true;
    std::string errorMsg = "";

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

    auto utf8PopBack = [](std::string& s) {
        if (s.empty()) return;

        while (!s.empty() && ((unsigned char)s.back() & 0xC0) == 0x80) {
            s.pop_back();
        }

        if (!s.empty()) s.pop_back();
    };

    auto utf8Length = [](const std::string& s) -> size_t {
        size_t len = 0;
        for (char c : s) {
            if ((c & 0xC0) != 0x80) len++;
        }
        return len;
        };

    const size_t MAX_NAME_BYTES = 30;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                if (backButtonContains((float)event.mouseButton.x,
                                       (float)event.mouseButton.y)) {
                    soundPlaySelect(res);
                    return SCREEN_CHAR_SELECT;
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    return SCREEN_CHAR_SELECT;

                if (!isPvC && event.key.code == sf::Keyboard::Tab) {
                    isEditingP1 = !isEditingP1;
                    errorMsg = "";
                }

                if (event.key.code == sf::Keyboard::Enter) {

                    if (name1.empty()) name1 = "Player 1";
                    if (isPvC) {
                        name2 = txt.botName;
                    }
                    else if (name2.empty()) {
                        name2 = "Player 2";
                    }
                    if (name1 == name2) {
                        errorMsg = txt.nameDuplicate;
                    }
                    else {
                        errorMsg = "";
                        state.player1.name = name1;
                        state.player2.name = name2;
                        boardResetAll(state);
                        if (state.style == STYLE_SPEED)
                            timerStart(state.timer, MAX_GAME_TIME, MAX_TURN_TIME);

                        return SCREEN_PLAYING;
                    }
                }

                if (event.key.code == sf::Keyboard::BackSpace) {
                    std::string& current = (isPvC || isEditingP1) ? name1 : name2;
                    utf8PopBack(current);
                    errorMsg = "";
                }
            }

            if (event.type == sf::Event::TextEntered) {
                uint32_t cp = event.text.unicode;

                if (cp >= 32 && cp != 127) {
                    std::string& current = (isPvC || isEditingP1) ? name1 : name2;
                    std::string utf8 = utf32ToUtf8(cp);
                    if (utf8Length(current) < 15) {
                        current += utf8;
                        errorMsg = "";
                    }
                    else {
                        errorMsg = txt.nameTooLong;
                    }
                }
            }
        }

        renderInputNames(window, res, name1, name2, isEditingP1, errorMsg, isPvC);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

GameScreen handleGameplay(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    sf::Clock clock;
    WinLine winLine = {};
    GameResult result = RESULT_NONE;
    bool showHint = false;
    int hintRow = -1, hintCol = -1;

    int lastPlaceRow = -1, lastPlaceCol = -1;
    float placeAnimTimer = 999.0f;
    const float PLACE_ANIM_DURATION = 0.2f;

    if (state.moveCount > 0) {
        Move lastMove = state.moveHistory[state.moveCount - 1];
        result = boardEvaluateResult(state, lastMove.row, lastMove.col, winLine);

        if (result != RESULT_NONE) {
            return handleGameOver(window, res, state, result, winLine);
        }
    }

    auto flushPendingInput = [&]() {
        sf::Event ev;
        while (window.pollEvent(ev)) handleCommonEvent(window, ev);
    };

    auto applyResult = [&](GameResult r) {
        if (r == RESULT_NONE) return;
        if (r == RESULT_PLAYER1_WIN) state.player1.totalWins++;
        else if (r == RESULT_PLAYER2_WIN) state.player2.totalWins++;
        if (r == RESULT_DRAW) soundPlayDraw(res);
        else soundPlayWin(res);
    };

    auto doBotMove = [&]() {

        renderGameplay(window, state, res, nullptr, -1, -1, false);

        renderBotThinking(window, res);
        window.display();

        sf::Clock botThinkClock;

        int botRow, botCol;
        botGetMove(state, state.difficulty, botRow, botCol);
        state.cursorRow = botRow;
        state.cursorCol = botCol;

        if (state.style == STYLE_SPEED) {
            float thinkSec = botThinkClock.getElapsedTime().asSeconds();
            timerConsumeP2(state.timer, thinkSec);

            if (timerIsGameExpiredP2(state.timer)) {
                result = RESULT_PLAYER1_WIN;
                applyResult(result);
                flushPendingInput();
                clock.restart();
                return;
            }
        }

        if (boardPlacePiece(state, botRow, botCol)) {
            soundPlayPlace(res);
            lastPlaceRow = botRow; lastPlaceCol = botCol; placeAnimTimer = 0.0f;

            result = boardEvaluateResult(state, botRow, botCol, winLine);
            if (result != RESULT_NONE) {
                applyResult(result);
            }
            else {
                boardSwitchTurn(state);
                if (state.style == STYLE_SPEED) timerResetTurn(state.timer);
            }
        }

        flushPendingInput();

        clock.restart();
    };

    auto doPlayerPlace = [&](int row, int col) {
        if (result != RESULT_NONE) return;
        if (boardPlacePiece(state, row, col)) {
            soundPlayPlace(res);
            showHint = false;
            lastPlaceRow = row; lastPlaceCol = col; placeAnimTimer = 0.0f;

            result = boardEvaluateResult(state, row, col, winLine);
            if (result != RESULT_NONE) {
                applyResult(result);
            }
            else {
                boardSwitchTurn(state);
                if (state.style == STYLE_SPEED) timerResetTurn(state.timer);

                if (state.mode == MODE_PVC && !state.isPlayer1Turn) {
                    doBotMove();
                }
            }
        }
    };

    auto openNestedScreen = [&](auto&& fn) -> GameScreen {
        if (state.style == STYLE_SPEED) timerPause(state.timer);
        GameScreen next = fn(window, res, state);
        if (state.style == STYLE_SPEED) timerResume(state.timer);
        clock.restart();
        return next;
    };

    if (state.mode == MODE_PVC && !state.isPlayer1Turn) {
        doBotMove();
    }

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        if (placeAnimTimer < PLACE_ANIM_DURATION)
            placeAnimTimer += deltaTime;

        if (state.style == STYLE_SPEED && result == RESULT_NONE) {
            timerUpdate(state.timer, deltaTime, state.isPlayer1Turn);

            if (!state.timer.turnAlarmFired &&
                timerGetTurnSecondsLeft(state.timer) <= TURN_ALARM_SECONDS) {
                soundPlayAlarm(res);
                state.timer.turnAlarmFired = true;
            }

            if (timerIsTurnExpired(state.timer)) {
                result = state.isPlayer1Turn ? RESULT_PLAYER2_WIN : RESULT_PLAYER1_WIN;
                applyResult(result);
                return handleGameOver(window, res, state, result, winLine);
            }

            if (timerIsGameExpiredP1(state.timer)) {
                result = RESULT_PLAYER2_WIN;
                applyResult(result);
                return handleGameOver(window, res, state, result, winLine);
            }
            if (timerIsGameExpiredP2(state.timer)) {
                result = RESULT_PLAYER1_WIN;
                applyResult(result);
                return handleGameOver(window, res, state, result, winLine);
            }

        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseMoved && result == RESULT_NONE) {
                int r, c;
                if (renderPixelToBoard((float)event.mouseMove.x,
                    (float)event.mouseMove.y, r, c)) {
                    state.cursorRow = r;
                    state.cursorCol = c;
                }
            }

            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left
                && result == RESULT_NONE) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                if (gameplaySaveBtnContains(mx, my)) {
                    soundPlaySelect(res);

                    openNestedScreen([](sf::RenderWindow& w, GameResources& r,
                        GameState& s) { return handleSaveScreen(w, r, s); });
                }

                else if (gameplayExitBtnContains(mx, my)) {
                    soundPlaySelect(res);
                    GameScreen pauseResult = openNestedScreen(handlePauseMenu);
                    if (pauseResult != SCREEN_PLAYING) return pauseResult;
                }
                else {
                    int r, c;
                    if (renderPixelToBoard(mx, my, r, c)) {
                        doPlayerPlace(r, c);
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed && result != RESULT_NONE) {
                return handleGameOver(window, res, state, result, winLine);
            }

            if (event.type == sf::Event::KeyPressed && result == RESULT_NONE) {
                switch (event.key.code) {

                case sf::Keyboard::W: case sf::Keyboard::Up:
                    boardMoveCursor(state, -1, 0); break;
                case sf::Keyboard::S: case sf::Keyboard::Down:
                    boardMoveCursor(state, 1, 0); break;
                case sf::Keyboard::A: case sf::Keyboard::Left:
                    boardMoveCursor(state, 0, -1); break;
                case sf::Keyboard::D: case sf::Keyboard::Right:
                    boardMoveCursor(state, 0, 1); break;

                case sf::Keyboard::Enter:
                    doPlayerPlace(state.cursorRow, state.cursorCol);
                    break;

                case sf::Keyboard::Z:
                    if (boardUndo(state) > 0) {
                        soundPlayUndo(res);

                        if (state.style == STYLE_SPEED) timerResetTurn(state.timer);
                    }
                    showHint = false;
                    break;

                case sf::Keyboard::H:
                    if (state.mode == MODE_PVC && state.isPlayer1Turn) {
                        botGetHint(state, hintRow, hintCol);
                        showHint = true;
                        soundPlayHint(res);
                    }
                    break;

                case sf::Keyboard::L:

                    openNestedScreen([](sf::RenderWindow& w, GameResources& r,
                        GameState& s) { return handleSaveScreen(w, r, s); });
                    break;

                case sf::Keyboard::Escape: {
                    GameScreen pauseResult = openNestedScreen(handlePauseMenu);
                    if (pauseResult != SCREEN_PLAYING) return pauseResult;
                    break;
                }

                default: break;
                }
            }

            if (event.type == sf::Event::KeyPressed && result != RESULT_NONE) {
                return handleGameOver(window, res, state, result, winLine);
            }
        }

        renderGameplay(window, state, res,
            (result != RESULT_NONE) ? &winLine : nullptr,
            hintRow, hintCol, showHint);

        if (placeAnimTimer < PLACE_ANIM_DURATION && lastPlaceRow >= 0) {
            float progress = placeAnimTimer / PLACE_ANIM_DURATION;
            int player = state.board[lastPlaceRow][lastPlaceCol].value;
            renderPlaceEffect(window, lastPlaceRow, lastPlaceCol, player, progress);
        }

        window.display();
    }
    return SCREEN_MAIN_MENU;
}

static bool sliderSetFromX(GameResources& res, float trackX, float trackW,
                           int& volume, float mx) {
    float ratio = (mx - trackX) / trackW;
    if (ratio < 0.f) ratio = 0.f;
    if (ratio > 1.f) ratio = 1.f;
    int newVol = (int)(ratio * 100.f + 0.5f);
    newVol = (newVol / 5) * 5;
    if (newVol != volume) {
        volume = newVol;
        soundSetBGMVolume(res, volume);
        return true;
    }
    return false;
}

static bool sliderContains(float trackX, float trackW, float y, float hitH,
                           float mx, float my) {
    return my >= y - hitH && my <= y + hitH
        && mx >= trackX - 20.f && mx <= trackX + trackW + 20.f;
}

static void sliderNudge(GameResources& res, int& volume, int delta) {
    int v = volume + delta;
    if (v < 0) v = 0;
    if (v > 100) v = 100;
    if (v != volume) {
        volume = v;
        soundSetBGMVolume(res, volume);
        settingsSave();
    }
}

GameScreen handlePauseMenu(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    int menuIndex = 0;
    const int MENU_COUNT = 6;

    int volume = soundGetBGMVolume();
    bool sfxOn = soundIsSFXEnabled();
    bool isDraggingVolume = false;

    const float PAUSE_START_Y = UI_PAUSE_START_Y;
    const float PAUSE_STEP    = UI_PAUSE_STEP;
    const float TRACK_W       = 300.f;
    const float TRACK_X       = WINDOW_WIDTH / 2.f - TRACK_W / 2.f;
    const float SLIDER_Y      = PAUSE_START_Y + 3 * PAUSE_STEP + 14.f;
    const float SLIDER_HIT_H  = 20.f;

    auto setVolumeFromX = [&](float mx) {
        sliderSetFromX(res, TRACK_X, TRACK_W, volume, mx);
    };

    auto isOnSlider = [&](float mx, float my) -> bool {
        return sliderContains(TRACK_X, TRACK_W, SLIDER_Y, SLIDER_HIT_H, mx, my);
    };

    auto confirm = [&](GameScreen& outNext) -> bool {
        soundPlaySelect(res);
        switch (menuIndex) {
        case 0:
            outNext = SCREEN_PLAYING; return true;
        case 1:
            handleSaveScreen(window, res, state);
            outNext = SCREEN_PLAYING; return true;
        case 2:
            langToggle();
            settingsSave();
            return false;

        case 4:
            sfxOn = !sfxOn;
            soundSetSFXEnabled(sfxOn);
            settingsSave();
            return false;
        case 5:
            outNext = SCREEN_MAIN_MENU; return true;
        default: return false;
        }
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseMoved) {
                float mx = (float)event.mouseMove.x;
                float my = (float)event.mouseMove.y;
                if (isDraggingVolume) {
                    setVolumeFromX(mx);
                    menuIndex = 3;
                }
                else {
                    int hit = menuHitTest(mx, my, PAUSE_START_Y, PAUSE_STEP, MENU_COUNT);
                    if (hit >= 0) menuIndex = hit;
                    else if (isOnSlider(mx, my)) menuIndex = 3;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased
                && event.mouseButton.button == sf::Mouse::Left) {
                if (isDraggingVolume) settingsSave();
                isDraggingVolume = false;
            }

            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                if (isOnSlider(mx, my)) {
                    menuIndex = 3;
                    isDraggingVolume = true;
                    setVolumeFromX(mx);
                }
                else {
                    int hit = menuHitTest(mx, my, PAUSE_START_Y, PAUSE_STEP, MENU_COUNT);
                    if (hit >= 0) {
                        menuIndex = hit;
                        if (menuIndex != 3) {
                            GameScreen next;
                            if (confirm(next)) return next;
                        }
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Up:
                case sf::Keyboard::W:
                    menuIndex = menuNavStep(menuIndex, MENU_COUNT, -1);
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = menuNavStep(menuIndex, MENU_COUNT, +1);
                    break;
                case sf::Keyboard::Left:
                    if (menuIndex == 3) sliderNudge(res, volume, -10);
                    break;
                case sf::Keyboard::Right:
                    if (menuIndex == 3) sliderNudge(res, volume, +10);
                    break;
                case sf::Keyboard::Enter: {
                    if (menuIndex == 3) break;
                    GameScreen next;
                    if (confirm(next)) return next;
                    break;
                }
                case sf::Keyboard::Escape:
                    return SCREEN_PLAYING;
                default: break;
                }
            }
        }

        renderGameplay(window, state, res, nullptr, -1, -1, false);
        renderPauseMenu(window, res, menuIndex, volume, sfxOn);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

GameScreen handleGameOver(sf::RenderWindow& window, GameResources& res,
    GameState& state, GameResult result, const WinLine& winLine) {
    int menuIndex = 0;
    bool askingSave = false;

    const float panelX = UI_BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + UI_PANEL_GAP_LEFT;
    const float panelW = WINDOW_WIDTH - panelX - UI_PANEL_GAP_RIGHT;
    const float centerX = panelX + panelW / 2.f;
    const float yesX = centerX - UI_GAMEOVER_BTN_GAP_X;
    const float noX = centerX + UI_GAMEOVER_BTN_GAP_X;
    const float btnY = UI_GAMEOVER_START_Y + UI_GAMEOVER_BTN_DY;
    const float btnHalfW = UI_GAMEOVER_BTN_HALF_W;
    const float btnHalfH = UI_GAMEOVER_BTN_HALF_H;

    bool hasWinner = (result == RESULT_PLAYER1_WIN || result == RESULT_PLAYER2_WIN);

    float fxCx = UI_BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE * 0.5f;
    float fxCy = UI_BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE * 0.5f;
    if (hasWinner && winLine.count > 0) {
        int mid = winLine.count / 2;
        sf::Vector2f wp = renderBoardToPixel(winLine.positions[mid][0],
                                             winLine.positions[mid][1]);
        fxCx = wp.x; fxCy = wp.y;
    }
    ConfettiSystem confetti;
    if (hasWinner) confettiSpawnBurst(confetti, fxCx, fxCy, 280);
    sf::Clock fxClock;
    float fxElapsed = 0.f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

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
                        menuIndex = 0;
                        goto DO_CONFIRM;
                    }
                    if (mx > noX - btnHalfW && mx < noX + btnHalfW) {
                        menuIndex = 1;
                        goto DO_CONFIRM;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Left: case sf::Keyboard::A:
                    menuIndex = 0; break;
                case sf::Keyboard::Right: case sf::Keyboard::D:
                    menuIndex = 1; break;
                case sf::Keyboard::L:

                    if (askingSave)
                        return handleSaveScreen(window, res, state, SCREEN_MAIN_MENU);
                    break;
                case sf::Keyboard::Escape:
                    return SCREEN_MAIN_MENU;

                case sf::Keyboard::Enter:
                DO_CONFIRM:
                    soundPlaySelect(res);

                    if (!askingSave) {

                        if (menuIndex == 0) {

                            int loserId = 0;
                            if (result == RESULT_PLAYER1_WIN) loserId = 2;
                            else if (result == RESULT_PLAYER2_WIN) loserId = 1;
                            boardResetRound(state, loserId);
                            if (state.style == STYLE_SPEED) timerStart(state.timer, MAX_GAME_TIME, MAX_TURN_TIME);
                            return SCREEN_PLAYING;
                        }
                        else {
                            askingSave = true;
                            menuIndex = 0;
                        }
                    }
                    else {

                        if (menuIndex == 0) {

                            return handleSaveScreen(window, res, state,
                                SCREEN_MAIN_MENU);
                        }
                        else {
                            return SCREEN_MAIN_MENU;
                        }
                    }
                    break;
                default: break;
                }
            }
        }

        float dt = fxClock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;
        fxElapsed += dt;
        confettiUpdate(confetti, dt);

        renderGameplay(window, state, res, (result != RESULT_DRAW) ? &winLine : nullptr, -1, -1, false, result);
        renderGameOver(window, state, res, result, menuIndex, askingSave);
        if (hasWinner) {

            renderShockwave(window, res, fxCx, fxCy, fxElapsed);

            confettiDraw(window, confetti);
        }
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

static void ensureVisibleIn(int selectedIndex, int& scrollTop, int count) {
    int maxTop = count - UI_LIST_VISIBLE; if (maxTop < 0) maxTop = 0;
    if (selectedIndex < scrollTop) scrollTop = selectedIndex;
    else if (selectedIndex >= scrollTop + UI_LIST_VISIBLE)
        scrollTop = selectedIndex - UI_LIST_VISIBLE + 1;
    if (scrollTop > maxTop) scrollTop = maxTop;
    if (scrollTop < 0) scrollTop = 0;
}

static int hitTestListAt(float mx, float my, int scrollTop, int count, float startY) {
    for (int row = 0; row < UI_LIST_VISIBLE; row++) {
        int i = scrollTop + row;
        if (i >= count) break;
        float itemY = startY + row * UI_LIST_STEP;
        if (mx > WINDOW_WIDTH / 2.f - UI_LIST_HALF_WIDTH
            && mx < WINDOW_WIDTH / 2.f + UI_LIST_HALF_WIDTH
            && my > itemY - UI_LIST_HALF_HEIGHT && my < itemY + UI_LIST_HALF_HEIGHT) {
            return i;
        }
    }
    return -1;
}

static void scrollListBy(int& scrollTop, int delta, int count) {
    int maxTop = count - UI_LIST_VISIBLE; if (maxTop < 0) maxTop = 0;
    scrollTop -= delta;
    if (scrollTop < 0) scrollTop = 0;
    if (scrollTop > maxTop) scrollTop = maxTop;
}

GameScreen handleSaveScreen(sf::RenderWindow& window, GameResources& res,
    GameState& state, GameScreen exitTarget) {

    { sf::Event drain; while (window.pollEvent(drain)) handleCommonEvent(window, drain); }

    std::vector<std::string> saveList = saveScanFiles();
    int selectedIndex = 0;
    int scrollTop = 0;
    std::string inputName = "";
    std::string message = "";
    sf::Clock messageClock;

    std::string pendingOverwrite = "";

    auto ensureVisible = [&]() {
        ensureVisibleIn(selectedIndex, scrollTop, (int)saveList.size());
    };

    auto hitTestList = [&](float mx, float my) -> int {
        return hitTestListAt(mx, my, scrollTop, (int)saveList.size(),
            UI_SAVE_LIST_START_Y);
    };

    auto doSave = [&]() {
        if (inputName.empty()) return;
        const TextStrings& txt = langGetText(langGetCurrent());
        if (saveFileExists(inputName) && pendingOverwrite != inputName) {

            pendingOverwrite = inputName;
            message = txt.msgFileExists;
        }
        else if (saveGame(state, inputName)) {
            message = txt.msgSaveOK;
            saveList = saveScanFiles();
            inputName = "";
            pendingOverwrite = "";
        }
        else {
            message = txt.msgSaveError;
            pendingOverwrite = "";
        }
        messageClock.restart();
    };

    auto deleteSaveAt = [&](int idx) {
        if (idx < 0 || idx >= (int)saveList.size()) return;
        saveDeleteFile(saveList[idx]);
        saveList = saveScanFiles();
        int count = (int)saveList.size();
        if (selectedIndex >= count && count > 0) selectedIndex = count - 1;
        ensureVisible();

        pendingOverwrite = "";
        message = langGetText(langGetCurrent()).msgFileDeleted;
        messageClock.restart();
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseWheelScrolled) {
                scrollListBy(scrollTop, (int)event.mouseWheelScroll.delta,
                    (int)saveList.size());
            }

            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                if (backButtonContains(mx, my)) {
                    soundPlaySelect(res);
                    return exitTarget;
                }

                int hit = hitTestList(mx, my);
                if (hit >= 0) {
                    selectedIndex = hit;
                    inputName = saveList[hit];
                    pendingOverwrite = "";
                }
            }

            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Right) {
                deleteSaveAt(hitTestList((float)event.mouseButton.x,
                    (float)event.mouseButton.y));
            }

            if (event.type == sf::Event::KeyPressed) {
                int count = (int)saveList.size();
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    return exitTarget;

                case sf::Keyboard::Up:
                    if (selectedIndex > 0) { selectedIndex--; ensureVisible(); }
                    break;
                case sf::Keyboard::Down:
                    if (selectedIndex < count - 1) { selectedIndex++; ensureVisible(); }
                    break;

                case sf::Keyboard::Enter:
                    doSave();
                    break;

                case sf::Keyboard::Delete:
                    deleteSaveAt(selectedIndex);
                    break;

                case sf::Keyboard::BackSpace:
                    if (!inputName.empty()) inputName.pop_back();
                    pendingOverwrite = "";
                    break;

                default: break;
                }
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode >= 32 && event.text.unicode < 128
                    && event.text.unicode != '\r' && event.text.unicode != '\b') {
                    char ch = static_cast<char>(event.text.unicode);
                    if (ch != '\\' && ch != '/' && ch != ':' && ch != '*'
                        && ch != '?' && ch != '"' && ch != '<' && ch != '>'
                        && ch != '|') {
                        if (inputName.size() < 20) {
                            inputName += ch;
                            pendingOverwrite = "";
                        }
                    }
                }
            }
        }

        renderSaveScreen(window, res, saveList, inputName, selectedIndex, scrollTop);

        bool waitingConfirm = !pendingOverwrite.empty()
            && pendingOverwrite == inputName;
        if (!message.empty() && (waitingConfirm
            || messageClock.getElapsedTime().asSeconds() < 2.0f)) {
            renderTextCentered(window, res.mainFont, message, 18,
                WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 90.f,
                sf::Color::Yellow);
        }
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

GameScreen handleLoadScreen(sf::RenderWindow& window, GameResources& res,
    GameState& state) {

    std::vector<std::string> saveList = saveScanFiles();
    int selectedIndex = 0;
    int scrollTop = 0;
    std::string message = "";
    sf::Clock messageClock;

    sf::Clock dblClickClock;
    int lastClickIndex = -1;
    const float DBL_CLICK_TIME = 0.4f;

    auto doLoad = [&]() -> bool {
        int count = (int)saveList.size();
        if (count > 0 && selectedIndex < count) {
            if (loadGame(state, saveList[selectedIndex])) {
                return true;
            }
            message = langGetText(langGetCurrent()).msgLoadError;
            messageClock.restart();
        }
        return false;
    };

    auto ensureVisible = [&]() {
        ensureVisibleIn(selectedIndex, scrollTop, (int)saveList.size());
    };

    auto hitTestList = [&](float mx, float my) -> int {
        return hitTestListAt(mx, my, scrollTop, (int)saveList.size(),
            UI_LOAD_LIST_START_Y);
    };

    auto deleteSaveAt = [&](int idx) {
        if (idx < 0 || idx >= (int)saveList.size()) return;
        saveDeleteFile(saveList[idx]);
        saveList = saveScanFiles();
        int count = (int)saveList.size();
        if (selectedIndex >= count && count > 0) selectedIndex = count - 1;
        ensureVisible();
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseWheelScrolled) {
                scrollListBy(scrollTop, (int)event.mouseWheelScroll.delta,
                    (int)saveList.size());
            }

            if (event.type == sf::Event::MouseMoved) {
                int hit = hitTestList((float)event.mouseMove.x,
                    (float)event.mouseMove.y);
                if (hit >= 0) selectedIndex = hit;
            }

            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                if (backButtonContains(mx, my)) {
                    soundPlaySelect(res);
                    return SCREEN_MAIN_MENU;
                }

                int hit = hitTestList(mx, my);
                if (hit >= 0) {
                    if (lastClickIndex == hit
                        && dblClickClock.getElapsedTime().asSeconds() < DBL_CLICK_TIME) {

                        selectedIndex = hit;
                        if (doLoad()) return SCREEN_PLAYING;
                        lastClickIndex = -1;
                    }
                    else {

                        selectedIndex = hit;
                        lastClickIndex = hit;
                        dblClickClock.restart();
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Right) {
                deleteSaveAt(hitTestList((float)event.mouseButton.x,
                    (float)event.mouseButton.y));
            }

            if (event.type == sf::Event::KeyPressed) {
                int count = (int)saveList.size();
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    return SCREEN_MAIN_MENU;

                case sf::Keyboard::Up: case sf::Keyboard::W:
                    if (selectedIndex > 0) { selectedIndex--; ensureVisible(); }
                    break;
                case sf::Keyboard::Down: case sf::Keyboard::S:
                    if (selectedIndex < count - 1) { selectedIndex++; ensureVisible(); }
                    break;

                case sf::Keyboard::Enter:
                    if (doLoad()) return SCREEN_PLAYING;
                    break;

                case sf::Keyboard::Delete:
                    deleteSaveAt(selectedIndex);
                    break;

                default: break;
                }
            }
        }

        renderLoadScreen(window, res, saveList, selectedIndex, scrollTop);

        if (!message.empty() && messageClock.getElapsedTime().asSeconds() < 2.0f) {
            renderTextCentered(window, res.mainFont, message, 18,
                WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 90.f,
                sf::Color::Red);
        }
        window.display();
    }
    return SCREEN_MAIN_MENU;
}

GameScreen handleSettings(sf::RenderWindow& window, GameResources& res) {
    int menuIndex = 0;

    int volume = soundGetBGMVolume();
    bool sfxOn = soundIsSFXEnabled();
    bool isDraggingVolume = false;

    const float TRACK_W = 360.f;
    const float TRACK_X = WINDOW_WIDTH / 2.f - TRACK_W / 2.f;
    const float SLIDER_Y = UI_SETTINGS_START_Y + 1 * UI_SETTINGS_STEP + 15.f;
    const float SLIDER_HIT_HEIGHT = 20.f;

    auto setVolumeFromX = [&](float mx) {
        sliderSetFromX(res, TRACK_X, TRACK_W, volume, mx);
    };

    auto isOnSlider = [&](float mx, float my) -> bool {
        return sliderContains(TRACK_X, TRACK_W, SLIDER_Y, SLIDER_HIT_HEIGHT, mx, my);
    };

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
        return SCREEN_SETTINGS;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            if (event.type == sf::Event::MouseMoved) {
                float mx = (float)event.mouseMove.x;
                float my = (float)event.mouseMove.y;

                if (isDraggingVolume) {
                    setVolumeFromX(mx);
                    menuIndex = 1;
                }
                else {
                    int hit = menuHitTest(mx, my,
                        UI_SETTINGS_START_Y, UI_SETTINGS_STEP, 3,
                        UI_SETTINGS_HALF_WIDTH, 25.f);
                    if (hit >= 0) menuIndex = hit;

                    else if (isOnSlider(mx, my)) menuIndex = 1;
                }
            }

            if (event.type == sf::Event::MouseButtonReleased
                && event.mouseButton.button == sf::Mouse::Left) {
                if (isDraggingVolume) settingsSave();
                isDraggingVolume = false;
            }

            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                if (backButtonContains(mx, my)) {
                    soundPlaySelect(res);
                    return SCREEN_MAIN_MENU;
                }

                if (isOnSlider(mx, my)) {
                    menuIndex = 1;
                    isDraggingVolume = true;
                    setVolumeFromX(mx);
                }
                else {
                    int hit = menuHitTest(mx, my,
                        UI_SETTINGS_START_Y, UI_SETTINGS_STEP, 3,
                        UI_SETTINGS_HALF_WIDTH, 25.f);
                    if (hit >= 0) {
                        menuIndex = hit;

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
                    menuIndex = menuNavStep(menuIndex, 3, -1);
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = menuNavStep(menuIndex, 3, +1);
                    break;
                case sf::Keyboard::Enter: {
                    GameScreen next = confirm();
                    if (next != SCREEN_SETTINGS) return next;
                    break;
                }
                case sf::Keyboard::Left:
                    if (menuIndex == 1) sliderNudge(res, volume, -10);
                    break;
                case sf::Keyboard::Right:
                    if (menuIndex == 1) sliderNudge(res, volume, +10);
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

GameScreen handleHelp(sf::RenderWindow& window, GameResources& res) {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) {
                soundPlaySelect(res);
                return SCREEN_MAIN_MENU;
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                soundPlaySelect(res);
                return SCREEN_MAIN_MENU;
            }
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

            if (event.type == sf::Event::MouseButtonPressed) {
                soundPlaySelect(res);
                return SCREEN_MAIN_MENU;
            }
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape) {
                soundPlaySelect(res);
                return SCREEN_MAIN_MENU;
            }
        }
        renderAbout(window, res);
        window.display();
    }
    return SCREEN_MAIN_MENU;
}
