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

    // Khoi tao am thanh + phat nhac nen menu
    soundLoadAll(res);
    soundPlayBGMTrack(res, BGM_MENU);

    GameScreen currentScreen = SCREEN_MAIN_MENU;

    while (window.isOpen()) {
        GameScreen nextScreen = currentScreen;

        // V2 #27: nhac nen theo man - man choi dung bgm_game, con lai dung bgm_menu
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
    int menuIndex = 0; // 0 = PvP, 1 = PvC

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

            // V2 (11/06): 2 tile ngang (hit-test rieng) + nut Back goc trai duoi
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
                    menuIndex = 1 - menuIndex; // chi 2 tile -> toggle
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
    int menuIndex = 0; // 0=Easy 1=Medium 2=Hard 3=Expert

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

            // V2 (11/06): 4 tile villain ngang + nut Back goc trai duoi
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
                    menuIndex = (menuIndex + 3) % 4;
                    break;
                case sf::Keyboard::Right: case sf::Keyboard::D:
                    menuIndex = (menuIndex + 1) % 4;
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
        // V2 #34: sau khi chon kieu choi -> sang man chon nhan vat
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
// V2 #34: CHON NHAN VAT (luoi 6 hero)
// ============================================================

GameScreen handleCharSelect(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    // PvP: P1 chon truoc roi P2 (khong duoc trung hero).
    // PvC: chi P1 chon (panel bot = villain theo do kho).
    int pickingPlayer = 1;
    int takenIndex = -1;                 // hero P1 da lay (phase P2)
    int menuIndex = state.heroP1;        // bat dau tu hero da chon lan truoc

    auto confirm = [&]() -> GameScreen {
        // Phase P2: khong cho chon trung hero cua P1
        if (pickingPlayer == 2 && menuIndex == takenIndex)
            return SCREEN_CHAR_SELECT;   // sentinel "o lai man nay"
        soundPlaySelect(res);
        if (pickingPlayer == 1) {
            state.heroP1 = menuIndex;
            if (state.mode == MODE_PVC) return SCREEN_INPUT_NAMES;
            // PvP: chuyen sang luot P2 chon
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
                    if (pickingPlayer == 2) {  // quay ve cho P1 chon lai
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
                    menuIndex = (menuIndex + 5) % 6;
                    break;
                case sf::Keyboard::Right: case sf::Keyboard::D:
                    menuIndex = (menuIndex + 1) % 6;
                    break;
                case sf::Keyboard::Up:   case sf::Keyboard::W:
                case sf::Keyboard::Down: case sf::Keyboard::S:
                    menuIndex = (menuIndex + 3) % 6; // nhay hang tren/duoi
                    break;
                case sf::Keyboard::Enter: {
                    GameScreen r = confirm();
                    if (r != SCREEN_CHAR_SELECT) return r;
                    break;
                }
                case sf::Keyboard::Escape:
                    if (pickingPlayer == 2) {  // ve buoc P1 chon lai
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
    std::string errorMsg = "";

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

            // V2: Click nut Back o goc trai duoi
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                if (backButtonContains((float)event.mouseButton.x,
                                       (float)event.mouseButton.y)) {
                    soundPlaySelect(res);
                    return SCREEN_CHAR_SELECT; // V2 #34: lui ve man chon nhan vat
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    return SCREEN_CHAR_SELECT; // V2 #34: lui ve man chon nhan vat

                // Chi cho phep Tab khi PvP (PvC chi co 1 o)
                if (!isPvC && event.key.code == sf::Keyboard::Tab) {
                    isEditingP1 = !isEditingP1;
                    errorMsg = "";
                }
                    

                if (event.key.code == sf::Keyboard::Enter) {
                    // Dat ten mac dinh neu de trong
                    if (name1.empty()) name1 = "Player 1";
                    if (isPvC) {
                        name2 = txt.botName;  // "May" / "Computer"
                    }
                    else if (name2.empty()) {
                        name2 = "Player 2";
                    }
                    if (name1 == name2) {
                        errorMsg = txt.nameDuplicate; // Trùng tên thì bật cờ báo lỗi và nằm yên tại đây
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

                // Xoa 1 ky tu UTF-8 (PvC: chi xoa name1)
                if (event.key.code == sf::Keyboard::BackSpace) {
                    std::string& current = (isPvC || isEditingP1) ? name1 : name2;
                    utf8PopBack(current);
                    errorMsg = "";
                }
            }

            // Nhap ky tu (HO TRO UNICODE - tieng Viet, etc.)
            if (event.type == sf::Event::TextEntered) {
                uint32_t cp = event.text.unicode;
                // Bo qua control chars va DEL
                if (cp >= 32 && cp != 127) {
                    std::string& current = (isPvC || isEditingP1) ? name1 : name2;
                    std::string utf8 = utf32ToUtf8(cp);
                    if (utf8Length(current) < 15) {
                        current += utf8;
                        errorMsg = "";
                    }
                    else {
                        errorMsg = u8"Tên người chơi chỉ tối đa là 15 kí tự!";
                    }
                }
            }
        }

        renderInputNames(window, res, name1, name2, isEditingP1, errorMsg, isPvC);
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

    if (state.moveCount > 0) {
        Move lastMove = state.moveHistory[state.moveCount - 1];
        result = boardEvaluateResult(state, lastMove.row, lastMove.col, winLine);

        // Nếu load lên mà thấy ván cờ đã có kết quả (thắng/thua/hòa) 
        // -> Đẩy thẳng sang màn hình Game Over luôn để hiện highlight và khóa bàn cờ
        if (result != RESULT_NONE) {
            return handleGameOver(window, res, state, result, winLine);
        }
    }
    // ============================================================
    // HELPER LAMBDA: Bot di 1 nuoc + check ket qua
    // ============================================================
    auto doBotMove = [&]() {
        // Render frame "Bot dang suy nghi..." (Bug 1 sub fix)
        renderGameplay(window, state, res, nullptr, -1, -1, false);
        if (state.style == STYLE_SPEED) renderBotThinking(window, res);
        window.display();

        // Do chinh xac thoi gian bot suy nghi (chess-clock fairness)
        sf::Clock botThinkClock;

        int botRow, botCol;
        botGetMove(state, state.difficulty, botRow, botCol);
        state.cursorRow = botRow;
        state.cursorCol = botCol;

        // Bot la P2 trong PvC. Tru thoi gian bot tu chinh game time cua P2.
        if (state.style == STYLE_SPEED) {
            float thinkSec = botThinkClock.getElapsedTime().asSeconds();
            timerConsumeP2(state.timer, thinkSec);

            // Neu bot tieu het game time trong khi nghi -> bot thua luon
            if (timerIsGameExpiredP2(state.timer)) {
                result = RESULT_PLAYER1_WIN;
                state.player1.totalWins++;
                soundPlayWin(res);
                clock.restart();
                return;
            }
        }

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

        // Bug 1 main fix: restart clock de bo thoi gian bot da troi qua,
        // tranh tru vao turn timer + game time cua P1 o frame ke tiep
        clock.restart();
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

        // Cap nhat timer (che do Speed) - chess-clock: chi tru thoi gian nguoi dang di
        if (state.style == STYLE_SPEED && result == RESULT_NONE) {
            timerUpdate(state.timer, deltaTime, state.isPlayer1Turn);

            // V2 #27: chuong canh bao khi luot sap het gio (reo 1 lan moi luot)
            if (!state.timer.turnAlarmFired &&
                timerGetTurnSecondsLeft(state.timer) <= TURN_ALARM_SECONDS) {
                soundPlayAlarm(res);
                state.timer.turnAlarmFired = true;
            }

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

            // --- KIỂM TRA HẾT GIỜ VAN CUA TUNG NGUOI (chess-clock) ---
            // Ai het thoi gian van -> nguoi do THUA (giong cờ vua)
            if (timerIsGameExpiredP1(state.timer)) {
                result = RESULT_PLAYER2_WIN;
                state.player2.totalWins++;
                soundPlayWin(res);
                return handleGameOver(window, res, state, result, winLine);
            }
            if (timerIsGameExpiredP2(state.timer)) {
                result = RESULT_PLAYER1_WIN;
                state.player1.totalWins++;
                soundPlayWin(res);
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

            // Click trai: nut Save/Exit hoac dat quan
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left
                && result == RESULT_NONE) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                // V2: Click nut Save trong panel
                if (gameplaySaveBtnContains(mx, my)) {
                    soundPlaySelect(res);
                    if (state.style == STYLE_SPEED) timerPause(state.timer);
                    handleSaveScreen(window, res, state);
                    if (state.style == STYLE_SPEED) timerResume(state.timer);
                    clock.restart();
                }
                // V2: Click nut Exit trong panel -> mo Pause menu
                // (giong nhan ESC: khong out thang ra Menu chinh nua)
                else if (gameplayExitBtnContains(mx, my)) {
                    soundPlaySelect(res);
                    if (state.style == STYLE_SPEED) timerPause(state.timer);
                    GameScreen pauseResult = handlePauseMenu(window, res, state);
                    if (state.style == STYLE_SPEED) timerResume(state.timer);
                    if (pauseResult != SCREEN_PLAYING) return pauseResult;
                    clock.restart();
                }
                else {
                    int r, c;
                    if (renderPixelToBoard(mx, my, r, c)) {
                        doPlayerPlace(r, c);
                    }
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
                    if (boardUndo(state) > 0) {
                        soundPlayUndo(res);
                        // Undo -> reset thanh thoi gian luot ve 20s (Speed mode)
                        // (timerResetTurn cung re-arm chuong canh bao cho luot moi)
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
    const int MENU_COUNT = 6;
    // V2: 0=Resume 1=Save 2=Language 3=Volume(slider) 4=SFX 5=Main Menu

    // Doc gia tri hien tai cua audio (giong handleSettings)
    int volume = soundGetBGMVolume();
    bool sfxOn = soundIsSFXEnabled();
    bool isDraggingVolume = false;

    // Hang so PHAI khop voi renderPauseMenu
    const float PAUSE_START_Y = 230.f;
    const float PAUSE_STEP    = 60.f;
    const float TRACK_W       = 300.f;
    const float TRACK_X       = WINDOW_WIDTH / 2.f - TRACK_W / 2.f;
    const float SLIDER_Y      = PAUSE_START_Y + 3 * PAUSE_STEP + 14.f;
    const float SLIDER_HIT_H  = 20.f;

    auto setVolumeFromX = [&](float mx) {
        float ratio = (mx - TRACK_X) / TRACK_W;
        if (ratio < 0.f) ratio = 0.f;
        if (ratio > 1.f) ratio = 1.f;
        int newVol = (int)(ratio * 100.f + 0.5f);
        newVol = (newVol / 5) * 5;
        if (newVol != volume) {
            volume = newVol;
            soundSetBGMVolume(res, volume);
            settingsSave();
        }
    };

    auto isOnSlider = [&](float mx, float my) -> bool {
        return my >= SLIDER_Y - SLIDER_HIT_H && my <= SLIDER_Y + SLIDER_HIT_H
            && mx >= TRACK_X - 20.f && mx <= TRACK_X + TRACK_W + 20.f;
    };

    // Confirm = thuc thi action tren menuIndex hien tai.
    // Tra ve cap (exitPauseLoop, nextScreen). exitPauseLoop=false -> stay in pause.
    auto confirm = [&](GameScreen& outNext) -> bool {
        soundPlaySelect(res);
        switch (menuIndex) {
        case 0:                                  // Resume
            outNext = SCREEN_PLAYING; return true;
        case 1:                                  // Save
            handleSaveScreen(window, res, state);
            outNext = SCREEN_PLAYING; return true;
        case 2:                                  // Language toggle - stay
            langToggle();
            settingsSave();
            return false;
        // case 3: Volume - khong dung confirm, dung mui ten
        case 4:                                  // SFX toggle - stay
            sfxOn = !sfxOn;
            soundSetSFXEnabled(sfxOn);
            settingsSave();
            return false;
        case 5:                                  // Main menu
            outNext = SCREEN_MAIN_MENU; return true;
        default: return false;
        }
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            // MOUSE MOVE - hover + drag slider
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

            // MOUSE RELEASE - thoat drag
            if (event.type == sf::Event::MouseButtonReleased
                && event.mouseButton.button == sf::Mouse::Left) {
                isDraggingVolume = false;
            }

            // MOUSE CLICK
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
                    menuIndex = (menuIndex - 1 + MENU_COUNT) % MENU_COUNT;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = (menuIndex + 1) % MENU_COUNT;
                    break;
                case sf::Keyboard::Left:
                    if (menuIndex == 3 && volume > 0) {
                        volume -= 10;
                        if (volume < 0) volume = 0;
                        soundSetBGMVolume(res, volume);
                        settingsSave();
                    }
                    break;
                case sf::Keyboard::Right:
                    if (menuIndex == 3 && volume < 100) {
                        volume += 10;
                        if (volume > 100) volume = 100;
                        soundSetBGMVolume(res, volume);
                        settingsSave();
                    }
                    break;
                case sf::Keyboard::Enter: {
                    if (menuIndex == 3) break; // volume khong dung Enter
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

// ============================================================
// GAME OVER
// ============================================================

GameScreen handleGameOver(sf::RenderWindow& window, GameResources& res,
    GameState& state, GameResult result, const WinLine& winLine) {
    int menuIndex = 0;
    bool askingSave = false; // Cờ theo dõi trạng thái đang hỏi cái gì

    const float panelX = UI_BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + UI_PANEL_GAP_LEFT;
    const float panelW = WINDOW_WIDTH - panelX - UI_PANEL_GAP_RIGHT;
    const float centerX = panelX + panelW / 2.f;
    const float yesX = centerX - UI_GAMEOVER_BTN_GAP_X;
    const float noX = centerX + UI_GAMEOVER_BTN_GAP_X;
    const float btnY = UI_GAMEOVER_START_Y + UI_GAMEOVER_BTN_DY;
    const float btnHalfW = UI_GAMEOVER_BTN_HALF_W;
    const float btnHalfH = UI_GAMEOVER_BTN_HALF_H;

    // V2 #32/#33: confetti + shockwave khi co nguoi thang (khong tinh hoa co)
    bool hasWinner = (result == RESULT_PLAYER1_WIN || result == RESULT_PLAYER2_WIN);
    // Tam hieu ung = giua duong thang thang (fallback = giua ban co)
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

            // Mouse hover
            if (event.type == sf::Event::MouseMoved) {
                float mx = (float)event.mouseMove.x;
                float my = (float)event.mouseMove.y;
                if (my > btnY - btnHalfH && my < btnY + btnHalfH) {
                    if (mx > yesX - btnHalfW && mx < yesX + btnHalfW) menuIndex = 0;
                    else if (mx > noX - btnHalfW && mx < noX + btnHalfW) menuIndex = 1;
                }
            }

            // Mouse click
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;
                if (my > btnY - btnHalfH && my < btnY + btnHalfH) {
                    if (mx > yesX - btnHalfW && mx < yesX + btnHalfW) {
                        menuIndex = 0;
                        goto DO_CONFIRM; // Dùng goto nhẹ xuống khối Enter để tận dụng code
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
                    handleSaveScreen(window, res, state); break;
                case sf::Keyboard::Escape:
                    return SCREEN_MAIN_MENU;

                case sf::Keyboard::Enter:
                DO_CONFIRM: // Điểm đến khi dùng chuột Click
                    soundPlaySelect(res);

                    if (!askingSave) {
                        // 1. ĐANG HỎI CHƠI TIẾP
                        if (menuIndex == 0) { // Có -> Reset bàn cờ và đánh tiếp
                            // V2: nguoi thua van vua roi di truoc van moi (va danh X)
                            // result == RESULT_PLAYER1_WIN -> P2 thua (loserId=2)
                            // result == RESULT_PLAYER2_WIN -> P1 thua (loserId=1)
                            // result == RESULT_DRAW        -> giu firstPlayer cu (loserId=0)
                            int loserId = 0;
                            if (result == RESULT_PLAYER1_WIN) loserId = 2;
                            else if (result == RESULT_PLAYER2_WIN) loserId = 1;
                            boardResetRound(state, loserId);
                            if (state.style == STYLE_SPEED) timerStart(state.timer, MAX_GAME_TIME, MAX_TURN_TIME);
                            return SCREEN_PLAYING;
                        }
                        else { // Không -> Sang hỏi lưu
                            askingSave = true;
                            menuIndex = 0; // Mặc định nhảy về nút "Có" cho câu hỏi sau
                        }
                    }
                    else {
                        // 2. ĐANG HỎI LƯU GAME
                        if (menuIndex == 0) { // Có -> Chuyển sang màn hình Lưu
                            handleSaveScreen(window, res, state);
                            // V2: Back/ESC trong save screen -> quay lai cau hoi
                            // "Luu game?" (KHONG out thang ra menu). Giu askingSave=true.
                            menuIndex = 0; // Mac dinh con tro ve nut "Co"
                        }
                        else { // Không -> về Menu chính
                            return SCREEN_MAIN_MENU;
                        }
                    }
                    break;
                default: break;
                }
            }
        }

        // V2 #32/#33: cap nhat hieu ung (clamp dt tranh nhay sau khi vao nested screen)
        float dt = fxClock.restart().asSeconds();
        if (dt > 0.1f) dt = 0.1f;
        fxElapsed += dt;
        confettiUpdate(confetti, dt);

        // Vẽ màn hình
        renderGameplay(window, state, res, (result != RESULT_DRAW) ? &winLine : nullptr, -1, -1, false, result);
        renderGameOver(window, state, res, result, menuIndex, askingSave); // Truyền biến askingSave vào đây
        if (hasWinner) {
            // V2 #33: vong song nang luong lan tu o thang (shader, tu tat 0.8s)
            renderShockwave(window, res, fxCx, fxCy, fxElapsed);
            // V2 #32: confetti phun tu o thang, len tren cung
            confettiDraw(window, confetti);
        }
        window.display();
    }
    return SCREEN_MAIN_MENU;
}
// ============================================================
// SAVE SCREEN
// ============================================================

GameScreen handleSaveScreen(sf::RenderWindow& window, GameResources& res,
    GameState& state) {
    // V2 #29: directory scan thay vi Gamelist.txt manifest, khong gioi han so file
    std::vector<std::string> saveList = saveScanFiles();
    int selectedIndex = 0;
    int scrollTop = 0; // V2 #30: index file dau tien dang hien thi
    std::string inputName = "";
    std::string message = "";
    sf::Clock messageClock;

    // V2 #30: dam bao item dang chon nam trong cua so cuon
    auto ensureVisible = [&]() {
        int count = (int)saveList.size();
        int maxTop = count - UI_LIST_VISIBLE; if (maxTop < 0) maxTop = 0;
        if (selectedIndex < scrollTop) scrollTop = selectedIndex;
        else if (selectedIndex >= scrollTop + UI_LIST_VISIBLE)
            scrollTop = selectedIndex - UI_LIST_VISIBLE + 1;
        if (scrollTop > maxTop) scrollTop = maxTop;
        if (scrollTop < 0) scrollTop = 0;
    };

    // V2 #30: map toa do chuot -> index file trong list (tinh ca scroll). -1 neu trat.
    auto hitTestList = [&](float mx, float my) -> int {
        int count = (int)saveList.size();
        for (int row = 0; row < UI_LIST_VISIBLE; row++) {
            int i = scrollTop + row;
            if (i >= count) break;
            float itemY = UI_SAVE_LIST_START_Y + row * UI_LIST_STEP;
            if (mx > WINDOW_WIDTH / 2.f - UI_LIST_HALF_WIDTH
                && mx < WINDOW_WIDTH / 2.f + UI_LIST_HALF_WIDTH
                && my > itemY - UI_LIST_HALF_HEIGHT && my < itemY + UI_LIST_HALF_HEIGHT) {
                return i;
            }
        }
        return -1;
    };

    // Lambda: thuc hien luu (Enter hoac click nut Save)
    auto doSave = [&]() {
        if (inputName.empty()) return;
        TextStrings txt = langGetText(langGetCurrent());
        if (saveFileExists(inputName)) {
            message = txt.msgFileExists;
        }
        else if (saveGame(state, inputName)) {
            message = txt.msgSaveOK;
            saveList = saveScanFiles(); // refresh list + cache
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

            // V2 #30: cuon bang con lan chuot
            if (event.type == sf::Event::MouseWheelScrolled) {
                int count = (int)saveList.size();
                int maxTop = count - UI_LIST_VISIBLE; if (maxTop < 0) maxTop = 0;
                scrollTop -= (int)event.mouseWheelScroll.delta;
                if (scrollTop < 0) scrollTop = 0;
                if (scrollTop > maxTop) scrollTop = maxTop;
            }

            // Mouse: click vao file trong list → copy ten vao input (de re-save / rename)
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                // V2: Click nut Back -> roi man hinh Luu (giong ESC)
                if (backButtonContains(mx, my)) {
                    soundPlaySelect(res);
                    return SCREEN_PLAYING;
                }

                int hit = hitTestList(mx, my);
                if (hit >= 0) {
                    selectedIndex = hit;
                    inputName = saveList[hit];  // Copy ten de re-save
                }
            }

            // Right click: xoa file dang chon
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Right) {
                int hit = hitTestList((float)event.mouseButton.x,
                    (float)event.mouseButton.y);
                if (hit >= 0) {
                    saveDeleteFile(saveList[hit]);
                    saveList = saveScanFiles();
                    int count = (int)saveList.size();
                    if (selectedIndex >= count && count > 0)
                        selectedIndex = count - 1;
                    ensureVisible();
                    message = langGetText(langGetCurrent()).msgFileDeleted;
                    messageClock.restart();
                }
            }

            if (event.type == sf::Event::KeyPressed) {
                int count = (int)saveList.size();
                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    return SCREEN_PLAYING;

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
                    if (count > 0 && selectedIndex < count) {
                        saveDeleteFile(saveList[selectedIndex]);
                        saveList = saveScanFiles();
                        count = (int)saveList.size();
                        if (selectedIndex >= count && count > 0)
                            selectedIndex = count - 1;
                        ensureVisible();
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

        renderSaveScreen(window, res, saveList, inputName, selectedIndex, scrollTop);

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
    // V2 #29: directory scan thay vi Gamelist.txt manifest, khong gioi han so file
    std::vector<std::string> saveList = saveScanFiles();
    int selectedIndex = 0;
    int scrollTop = 0; // V2 #30: index file dau tien dang hien thi
    std::string message = "";
    sf::Clock messageClock;

    // Double-click detection
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

    // V2 #30: dam bao item dang chon nam trong cua so cuon
    auto ensureVisible = [&]() {
        int count = (int)saveList.size();
        int maxTop = count - UI_LIST_VISIBLE; if (maxTop < 0) maxTop = 0;
        if (selectedIndex < scrollTop) scrollTop = selectedIndex;
        else if (selectedIndex >= scrollTop + UI_LIST_VISIBLE)
            scrollTop = selectedIndex - UI_LIST_VISIBLE + 1;
        if (scrollTop > maxTop) scrollTop = maxTop;
        if (scrollTop < 0) scrollTop = 0;
    };

    // V2 #30: map toa do chuot -> index file trong list (tinh ca scroll). -1 neu trat.
    auto hitTestList = [&](float mx, float my) -> int {
        int count = (int)saveList.size();
        for (int row = 0; row < UI_LIST_VISIBLE; row++) {
            int i = scrollTop + row;
            if (i >= count) break;
            float itemY = UI_LOAD_LIST_START_Y + row * UI_LIST_STEP;
            if (mx > WINDOW_WIDTH / 2.f - UI_LIST_HALF_WIDTH
                && mx < WINDOW_WIDTH / 2.f + UI_LIST_HALF_WIDTH
                && my > itemY - UI_LIST_HALF_HEIGHT && my < itemY + UI_LIST_HALF_HEIGHT) {
                return i;
            }
        }
        return -1;
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (handleCommonEvent(window, event)) continue;

            // V2 #30: cuon bang con lan chuot
            if (event.type == sf::Event::MouseWheelScrolled) {
                int count = (int)saveList.size();
                int maxTop = count - UI_LIST_VISIBLE; if (maxTop < 0) maxTop = 0;
                scrollTop -= (int)event.mouseWheelScroll.delta;
                if (scrollTop < 0) scrollTop = 0;
                if (scrollTop > maxTop) scrollTop = maxTop;
            }

            // Mouse hover: highlight item (khong auto-scroll)
            if (event.type == sf::Event::MouseMoved) {
                int hit = hitTestList((float)event.mouseMove.x,
                    (float)event.mouseMove.y);
                if (hit >= 0) selectedIndex = hit;
            }

            // Click trai: chon, double-click → load
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                float mx = (float)event.mouseButton.x;
                float my = (float)event.mouseButton.y;

                // V2: Click nut Back -> ve Menu chinh
                if (backButtonContains(mx, my)) {
                    soundPlaySelect(res);
                    return SCREEN_MAIN_MENU;
                }

                int hit = hitTestList(mx, my);
                if (hit >= 0) {
                    if (lastClickIndex == hit
                        && dblClickClock.getElapsedTime().asSeconds() < DBL_CLICK_TIME) {
                        // Double-click → load
                        selectedIndex = hit;
                        if (doLoad()) return SCREEN_PLAYING;
                        lastClickIndex = -1;
                    }
                    else {
                        // Single click → select
                        selectedIndex = hit;
                        lastClickIndex = hit;
                        dblClickClock.restart();
                    }
                }
            }

            // Click phai: xoa file
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Right) {
                int hit = hitTestList((float)event.mouseButton.x,
                    (float)event.mouseButton.y);
                if (hit >= 0) {
                    saveDeleteFile(saveList[hit]);
                    saveList = saveScanFiles();
                    int count = (int)saveList.size();
                    if (selectedIndex >= count && count > 0)
                        selectedIndex = count - 1;
                    ensureVisible();
                }
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
                    if (count > 0 && selectedIndex < count) {
                        saveDeleteFile(saveList[selectedIndex]);
                        saveList = saveScanFiles();
                        count = (int)saveList.size();
                        if (selectedIndex >= count && count > 0)
                            selectedIndex = count - 1;
                        ensureVisible();
                    }
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
                        UI_SETTINGS_START_Y, UI_SETTINGS_STEP, 3,
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

                // V2: Click nut Back goc trai duoi -> ve Menu chinh
                if (backButtonContains(mx, my)) {
                    soundPlaySelect(res);
                    return SCREEN_MAIN_MENU;
                }

                // 1) Click vao slider area → set volume + bat dau drag
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
                        // 2) Click khac → confirm action (lang, sfx)
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
                    menuIndex = (menuIndex - 1 + 3) % 3;
                    break;
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    menuIndex = (menuIndex + 1) % 3;
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
                event.key.code == sf::Keyboard::Escape) {
                soundPlaySelect(res);
                return SCREEN_MAIN_MENU;
            }
            // Click bat ky (gom nut Back) → ve menu chinh
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
            // Click bat ky (gom nut Back) → ve menu chinh
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
