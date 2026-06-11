#include "render.h"
#include "timer.h"
#include "language.h"
#include "rounded_rect.h"

// V2 #31: forward declaration - dinh nghia o duoi (muc VE SAVE / LOAD).
// Khai bao truoc de cac helper menu (drawMenuButton...) goi duoc.
// Default args dat o day, KHONG lap lai o phan dinh nghia.
static void drawRoundedCentered(sf::RenderWindow& window, float cx, float cy,
    float w, float h, float radius, sf::Color fill,
    sf::Color outline = sf::Color::Transparent, float outlineThick = 0.f);

// Su dung hang so chia se tu game_types.h cho de chinh sua sau nay
// (UI_BOARD_OFFSET_X, UI_BOARD_OFFSET_Y)
#define BOARD_OFFSET_X  UI_BOARD_OFFSET_X
#define BOARD_OFFSET_Y  UI_BOARD_OFFSET_Y

// ============================================================
// WINDOW RESIZE / LETTERBOX
// ============================================================

// Letterbox: giu ty le 16:9 cua game, them vien den khi window khong dung ty le
void applyLetterbox(sf::RenderWindow& window, unsigned int newWidth, unsigned int newHeight) {
    sf::View view(sf::FloatRect(0.f, 0.f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));

    float windowRatio = (float)newWidth / (float)newHeight;
    float gameRatio = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;

    float sizeX = 1.f, sizeY = 1.f, posX = 0.f, posY = 0.f;
    if (windowRatio < gameRatio) {
        // Window cao hon ty le 16:9 -> letterbox tren/duoi
        sizeY = windowRatio / gameRatio;
        posY = (1.f - sizeY) / 2.f;
    } else {
        // Window rong hon ty le 16:9 -> letterbox trai/phai
        sizeX = gameRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
    }

    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    window.setView(view);
}

// Xu ly event chung: map mouse coords + Closed + Resized
bool handleCommonEvent(sf::RenderWindow& window, sf::Event& event) {
    // 1. Map mouse coords tu pixel -> view (de hit-test menu hoat dong sau resize)
    if (event.type == sf::Event::MouseButtonPressed ||
        event.type == sf::Event::MouseButtonReleased) {
        sf::Vector2f v = window.mapPixelToCoords(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        event.mouseButton.x = (int)v.x;
        event.mouseButton.y = (int)v.y;
    }
    else if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f v = window.mapPixelToCoords(
            sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
        event.mouseMove.x = (int)v.x;
        event.mouseMove.y = (int)v.y;
    }

    // 2. Xu ly Closed/Resized
    if (event.type == sf::Event::Closed) {
        window.close();
        return true;
    }
    if (event.type == sf::Event::Resized) {
        applyLetterbox(window, event.size.width, event.size.height);
        return true;
    }
    return false;
}


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
// SNAP origin va position vao integer pixel de tranh sub-pixel blur
// Co the them outline de chu noi bat tren background phuc tap
void renderTextCentered(sf::RenderWindow& window, const sf::Font& font,
    const std::string& text, int fontSize,
    float centerX, float centerY, sf::Color color,
    sf::Color outlineColor, float outlineThickness) {
    sf::Text t;
    t.setFont(font);
    t.setString(sf::String::fromUtf8(text.begin(), text.end()));
    t.setCharacterSize(fontSize);
    t.setFillColor(color);

    // Outline (neu co) - giup chu noi bat tren background
    if (outlineThickness > 0.f) {
        t.setOutlineColor(outlineColor);
        t.setOutlineThickness(outlineThickness);
    }

    // Tinh kich thuoc thuc te cua doan chu
    sf::FloatRect bounds = t.getLocalBounds();
    // SNAP origin va position vao integer pixel -> chu ro net hon
    t.setOrigin(std::round(bounds.left + bounds.width / 2.f),
        std::round(bounds.top + bounds.height / 2.f));
    t.setPosition(std::round(centerX), std::round(centerY));

    window.draw(t); // ve len cua so game
}



// KHOI TAO GAMERESOURCES
bool renderLoadResources(GameResources& res) {
    // Load font chinh
    if (!res.mainFont.loadFromFile("../assets/fonts/main_font.ttf")) return false;

    // Load font tieu de
    if (!res.titleFont.loadFromFile("../assets/fonts/title_font.ttf")) return false;

    // Texture: load neu co, khong co thi bo qua (optional)
    res.backgroundTex.loadFromFile("../assets/textures/background.png");
    res.xPieceTex.loadFromFile("../assets/textures/x_piece.png");
    res.xPieceTex.setSmooth(true);
    res.oPieceTex.loadFromFile("../assets/textures/o_piece.png");
    res.oPieceTex.setSmooth(true);

    // Mascot textures - V2: chi load 1 tu the (idle) moi player
    res.mascotP1Idle.loadFromFile("../assets/textures/mascot_p1.png");
    res.mascotP1Idle.setSmooth(true);
    res.mascotP2Idle.loadFromFile("../assets/textures/mascot_p2.png");
    res.mascotP2Idle.setSmooth(true);

    // UI decoration textures (smooth de scale dep)
    res.logoCaroTex.loadFromFile("../assets/textures/logo_caro.png");
    res.logoCaroTex.setSmooth(true);
    res.bannerWinTex.loadFromFile("../assets/textures/banner_winner.png");
    res.bannerWinTex.setSmooth(true);
    res.bannerDefeatTex.loadFromFile("../assets/textures/banner_defeat.png");
    res.bannerDefeatTex.setSmooth(true);
    res.buttonFrameTex.loadFromFile("../assets/textures/button_frame.png");
    res.buttonFrameTex.setSmooth(true);

    // Sound
    res.moveSfx.loadFromFile("../assets/sounds/move.wav");
    res.placeSfx.loadFromFile("../assets/sounds/place.wav");
    res.winSfx.loadFromFile("../assets/sounds/win.wav");
    res.drawSfx.loadFromFile("../assets/sounds/draw.wav");
    res.menuSfx.loadFromFile("../assets/sounds/menu.wav");

    res.bgMusic.openFromFile("../assets/sounds/bgm.ogg");

    return true;
}


// VE BAN CO
// Ban co duoc ve = panel ban trong suot (de thay background) + grid procedural 15x15
void renderBoard(sf::RenderWindow& window, const GameResources& res) {
    const float boardPx = BOARD_SIZE * CELL_SIZE;

    // Panel ban trong suot (wood-tone) - cho phep thay background xuyen qua
    sf::RectangleShape panel(sf::Vector2f(boardPx, boardPx));
    panel.setPosition(BOARD_OFFSET_X, BOARD_OFFSET_Y);
    panel.setFillColor(sf::Color(245, 222, 179, 150));  // wheat ~59% opaque
    panel.setOutlineThickness(3.f);
    panel.setOutlineColor(sf::Color(101, 67, 33, 230));  // dark wood border
    window.draw(panel);

    // Grid 15x15 - duong ke ngang va doc (luon mau dam de noi bat)
    for (int i = 0; i <= BOARD_SIZE; i++) {
        sf::RectangleShape hLine(sf::Vector2f(boardPx, 1.f));
        hLine.setPosition(BOARD_OFFSET_X, BOARD_OFFSET_Y + i * CELL_SIZE);
        hLine.setFillColor(sf::Color(101, 67, 33, 210));
        window.draw(hLine);

        sf::RectangleShape vLine(sf::Vector2f(1.f, boardPx));
        vLine.setPosition(BOARD_OFFSET_X + i * CELL_SIZE, BOARD_OFFSET_Y);
        vLine.setFillColor(sf::Color(101, 67, 33, 210));
        window.draw(vLine);
    }
}

// VE BACKGROUND TOAN MAN HINH
// dimForMenu=true: them lop overlay toi de chu menu de doc
static void renderBackdrop(sf::RenderWindow& window, const GameResources& res,
                            bool dimForMenu) {
    if (res.backgroundTex.getSize().x == 0) {
        window.clear(dimForMenu ? COLOR_MENU_BG : COLOR_BACKGROUND);
        return;
    }
    sf::Sprite bg(res.backgroundTex);
    sf::Vector2u texSize = res.backgroundTex.getSize();
    bg.setScale((float)WINDOW_WIDTH / texSize.x,
                (float)WINDOW_HEIGHT / texSize.y);
    bg.setPosition(0.f, 0.f);
    window.draw(bg);

    if (dimForMenu) {
        sf::RectangleShape dim(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        dim.setFillColor(sf::Color(15, 25, 45, 170));  // dark blue ~67% opaque
        window.draw(dim);
    }
}

// Ve cac quan co da danh
// V2: Texture X/O duoc map dong theo state.firstPlayerOfRound
// - Nguoi di truoc van nay luon danh X (luat co vua)
// - val=-1 la quan cua P1, val=+1 la quan cua P2 (khong doi)
// - Nhung renderng X/O thi swap dua tren firstPlayerOfRound
void renderPieces(sf::RenderWindow& window, const GameState& state,
    const GameResources& res) {

    const bool hasXTex = res.xPieceTex.getSize().x > 0;
    const bool hasOTex = res.oPieceTex.getSize().x > 0;
    const int fontSize = (int)(CELL_SIZE - 10.f);
    const float pieceTarget = CELL_SIZE - 6.f;  // quan co nho hon 1 chut so voi o

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int val = state.board[r][c].value;
            if (val == 0) continue;

            sf::Vector2f pos = renderBoardToPixel(r, c);

            // V2: xac dinh quan nay co phai cua nguoi di truoc khong
            // (nguoi di truoc danh X, nguoi di sau danh O)
            int pieceOwnerId = (val == -1) ? 1 : 2; // val=-1 la P1, val=+1 la P2
            bool isPlayerPlaysX = (pieceOwnerId == state.firstPlayerOfRound);

            const sf::Texture* tex = isPlayerPlaysX ? (hasXTex ? &res.xPieceTex : nullptr)
                                                    : (hasOTex ? &res.oPieceTex : nullptr);

            if (tex != nullptr) {
                sf::Sprite sprite(*tex);
                sf::Vector2u texSize = tex->getSize();
                float scale = pieceTarget / (float)std::max(texSize.x, texSize.y);
                sprite.setScale(scale, scale);
                sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
                sprite.setPosition(pos);
                window.draw(sprite);
            }
            else {
                // Fallback: text X/O neu thieu texture
                sf::Text pieceText;
                pieceText.setFont(res.mainFont);
                pieceText.setCharacterSize(fontSize);
                pieceText.setString(isPlayerPlaysX ? "X" : "O");
                pieceText.setFillColor(isPlayerPlaysX ? COLOR_PLAYER_X : COLOR_PLAYER_O);
                sf::FloatRect bounds = pieceText.getLocalBounds();
                pieceText.setOrigin(bounds.left + bounds.width / 2.f,
                    bounds.top + bounds.height / 2.f);
                pieceText.setPosition(pos);
                window.draw(pieceText);
            }
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


void renderHint(sf::RenderWindow& window, int row, int col) {
    sf::CircleShape hint(CELL_SIZE / 2.f - 8.f);
    hint.setOrigin(CELL_SIZE / 2.f - 8.f, CELL_SIZE / 2.f - 8.f);
    sf::Vector2f pos = renderBoardToPixel(row, col);
    hint.setPosition(pos);
    hint.setFillColor(sf::Color(0, 255, 0, 80));  // Xanh la nhat, trong suot
    window.draw(hint);
}

void renderWinLine(sf::RenderWindow& window, const WinLine& winLine) {
    if (winLine.count < WIN_COUNT) return;

    // Duyệt qua từng tọa độ của 5 quân chiến thắng
    for (int i = 0; i < WIN_COUNT; i++) {
        int r = winLine.positions[i][0];
        int c = winLine.positions[i][1];

        // Tạo một hình vuông vừa vặn với ô cờ (trừ đi 2 pixel để không đè mất lưới bàn cờ)
        sf::RectangleShape cellBg(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
        cellBg.setOrigin((CELL_SIZE - 2.f) / 2.f, (CELL_SIZE - 2.f) / 2.f);

        // Lấy tọa độ tâm của ô cờ
        cellBg.setPosition(renderBoardToPixel(r, c));

        // Tô màu nền: Vàng sáng với độ trong suốt (Alpha = 150) để chữ X/O vẫn nổi bật
        cellBg.setFillColor(sf::Color(255, 255, 0, 150));

        window.draw(cellBg);
    }
}



// VE PLAYER PANEL
void renderPlayerPanel(sf::RenderWindow& window, const GameState& state,
    const GameResources& res, GameResult result) {
    TextStrings txt = langGetText(langGetCurrent());
    float panelX = BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + UI_PANEL_GAP_LEFT;
    float panelW = WINDOW_WIDTH - panelX - UI_PANEL_GAP_RIGHT;

    for (int i = 0; i < 2; i++) {
        const Player& p = (i == 0) ? state.player1 : state.player2;
        bool isActive = (i == 0) ? state.isPlayer1Turn : !state.isPlayer1Turn;

        // V2: Mau + ky hieu phu thuoc vao ai di truoc van nay
        // - Nguoi di truoc danh X (mau do)
        // - Nguoi di sau danh O (mau xanh)
        int playerId = i + 1; // i=0 -> P1, i=1 -> P2
        bool playsX = (playerId == state.firstPlayerOfRound);
        sf::Color pieceColor = playsX ? COLOR_PLAYER_X : COLOR_PLAYER_O;

        // Player 1 o tren, Player 2 o duoi cach UI_PANEL_BOX_STEP
        float boxY = UI_PANEL_BOX_Y_START + i * UI_PANEL_BOX_STEP;

        sf::RectangleShape box(sf::Vector2f(panelW, UI_PANEL_BOX_HEIGHT));
        box.setPosition(panelX, boxY);
        box.setFillColor(isActive ? sf::Color(60, 80, 100) : sf::Color(40, 55, 70));
        box.setOutlineThickness(isActive ? 3.f : 1.f);
        box.setOutlineColor(isActive ? pieceColor : sf::Color(100, 100, 100));
        window.draw(box);

        // VE MASCOT - V2: luon dung 1 tu the (idle); thang/thua the hien
        // qua banner + tint (lam toi/xam mascot nguoi thua).
        const sf::Texture* mascotTex = (i == 0) ? &res.mascotP1Idle : &res.mascotP2Idle;

        // Nguoi nay co thua khong? (co ket qua, khong hoa, va khong phai nguoi thang)
        bool thisPlayerLost = (result != RESULT_NONE && result != RESULT_DRAW)
            && !((i == 0 && result == RESULT_PLAYER1_WIN) ||
                 (i == 1 && result == RESULT_PLAYER2_WIN));

        float mascotLeftX = panelX + panelW;  // fallback: rightmost
        if (mascotTex != nullptr && mascotTex->getSize().x > 0) {
            sf::Sprite mascot(*mascotTex);
            sf::Vector2u texSize = mascotTex->getSize();
            const float targetH = UI_PANEL_BOX_HEIGHT - 10.f;
            float scale = targetH / (float)texSize.y;
            mascot.setScale(scale, scale);
            float mascotW = texSize.x * scale;
            // Dat mascot ben phai box, can giua chieu doc
            float mascotX = panelX + panelW - mascotW - 8.f;
            mascot.setPosition(mascotX, boxY + 5.f);
            // Loser: lam toi + am xam de the hien thua bang code (thay anh _Over)
            if (thisPlayerLost) mascot.setColor(sf::Color(110, 110, 130));
            window.draw(mascot);
            mascotLeftX = mascotX;
        }

        // VICTORY / DEFEAT banner giua panel box (giua text info va mascot)
        if (result != RESULT_NONE && result != RESULT_DRAW) {
            bool thisPlayerWon = (i == 0 && result == RESULT_PLAYER1_WIN) ||
                                 (i == 1 && result == RESULT_PLAYER2_WIN);

            // Vung dat banner: giua khu text (left ~200) va mascot
            float bannerCenterX = (panelX + 200.f + mascotLeftX) / 2.f;
            float bannerCenterY = boxY + UI_PANEL_BOX_HEIGHT / 2.f;
            const float bannerW = 200.f;

            const sf::Texture* bannerTex = thisPlayerWon ? &res.bannerWinTex
                                                          : &res.bannerDefeatTex;

            if (bannerTex != nullptr && bannerTex->getSize().x > 0) {
                // Texture VICTORY hoac DEFEAT
                sf::Sprite banner(*bannerTex);
                sf::Vector2u s = bannerTex->getSize();
                banner.setOrigin(s.x / 2.f, s.y / 2.f);
                float bScale = bannerW / s.x;
                banner.setScale(bScale, bScale);
                banner.setPosition(std::round(bannerCenterX), std::round(bannerCenterY));
                window.draw(banner);
            }
            else if (!thisPlayerWon) {
                // Fallback: neu thieu texture defeat -> dung text do
                renderTextCentered(window, res.titleFont, txt.defeatText, 32,
                    bannerCenterX, bannerCenterY,
                    sf::Color(255, 70, 70),
                    sf::Color(0, 0, 0, 230), 3.f);
            }
        }

        // Chữ X/O (V2: swap theo firstPlayerOfRound - nguoi di truoc luon X)
        sf::Text pieceIcon;
        pieceIcon.setFont(res.mainFont);
        pieceIcon.setString(playsX ? "X" : "O");
        pieceIcon.setCharacterSize(28);
        pieceIcon.setFillColor(pieceColor);

        sf::FloatRect bounds = pieceIcon.getLocalBounds();
        pieceIcon.setOrigin(bounds.left + bounds.width / 2.f,
            bounds.top + bounds.height / 2.f);
        pieceIcon.setPosition(panelX + 25.f, boxY + 30.f);
        window.draw(pieceIcon);

        // Ten nguoi choi (PHAI dung fromUtf8 de hien thi tieng Viet)
        std::string nameStr = p.name.empty() ? (i == 0 ? "Player 1" : "Player 2") : p.name;
        sf::Text nameText;
        nameText.setFont(res.mainFont);
        nameText.setString(sf::String::fromUtf8(nameStr.begin(), nameStr.end()));
        nameText.setCharacterSize(20);
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition(panelX + 50.f, boxY + 18.f);
        window.draw(nameText);

        // So nuoc di
        std::string movesStr = txt.moves + std::to_string(p.moves);
        sf::Text movesText;
        movesText.setFont(res.mainFont);
        movesText.setString(sf::String::fromUtf8(movesStr.begin(), movesStr.end()));
        movesText.setCharacterSize(16);
        movesText.setFillColor(sf::Color(200, 200, 200));
        movesText.setPosition(panelX + 15.f, boxY + 65.f);
        window.draw(movesText);

        // So van thang
        std::string winsStr = txt.wins + std::to_string(p.totalWins);
        sf::Text winsText;
        winsText.setFont(res.mainFont);
        winsText.setString(sf::String::fromUtf8(winsStr.begin(), winsStr.end()));
        winsText.setCharacterSize(16);
        winsText.setFillColor(sf::Color(200, 200, 200));
        winsText.setPosition(panelX + 15.f, boxY + 95.f);
        window.draw(winsText);

        // Thoi gian van CUA NGUOI NAY (chi che do Speed)
        if (state.style == STYLE_SPEED) {
            float secLeft = (i == 0)
                ? timerGetGameSecondsLeftP1(state.timer)
                : timerGetGameSecondsLeftP2(state.timer);
            int mins = (int)secLeft / 60;
            int secs = (int)secLeft % 60;
            char tbuf[24];
            snprintf(tbuf, sizeof(tbuf), "%02d:%02d", mins, secs);
            std::string timeStr = txt.gameTimeLabel + std::string(tbuf);
            sf::Color timeColor = (secLeft <= 30.f) ? sf::Color(255, 100, 100)
                                                    : sf::Color(220, 220, 220);
            sf::Text timeText;
            timeText.setFont(res.mainFont);
            timeText.setString(sf::String::fromUtf8(timeStr.begin(), timeStr.end()));
            timeText.setCharacterSize(16);
            timeText.setFillColor(timeColor);
            timeText.setPosition(panelX + 15.f, boxY + 122.f);
            window.draw(timeText);
        }

        if (isActive && result == RESULT_NONE) {
            renderTextCentered(window, res.mainFont, txt.yourTurn,
                14, panelX + panelW / 2.f, boxY + 152.f,
                pieceColor,                     // Tự động đổi Đỏ/Xanh theo lượt
                sf::Color(0, 0, 0, 200), 1.5f);
        }
    }
}


// VE TIMER

// Thanh dem gio luot hien tai (progress bar)
void renderTurnTimer(sf::RenderWindow& window, const GameResources& res,
    float percentage) {
    float panelX = BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + UI_PANEL_GAP_LEFT;
    float panelW = WINDOW_WIDTH - panelX - UI_PANEL_GAP_RIGHT;

    float barY = UI_TIMER_BAR_Y;
    float barH = UI_TIMER_BAR_HEIGHT;

    // PLATE den mo phia sau ca khu vuc timer (label + bar + game time)
    // Cao 90px de bao trum tu "THOI GIAN LUOT" den "Thoi gian van"
    sf::RectangleShape timerPlate(sf::Vector2f(panelW, 90.f));
    timerPlate.setPosition(panelX, barY - 32.f);
    timerPlate.setFillColor(sf::Color(20, 30, 50, 180));  // dark blue, 70% opacity
    timerPlate.setOutlineThickness(2.f);
    timerPlate.setOutlineColor(sf::Color(80, 100, 140, 200));
    window.draw(timerPlate);

    // Background thanh bar (track)
    sf::RectangleShape bg(sf::Vector2f(panelW - 20.f, barH));
    bg.setPosition(panelX + 10.f, barY);
    bg.setFillColor(sf::Color(40, 50, 70));
    window.draw(bg);

    // Progress bar fill
    sf::Color barColor = (percentage > 0.3f) ? COLOR_TIMER_BAR : COLOR_TIMER_LOW;
    sf::RectangleShape bar(sf::Vector2f((panelW - 20.f) * percentage, barH));
    bar.setPosition(panelX + 10.f, barY);
    bar.setFillColor(barColor);
    window.draw(bar);

    // Text "THOI GIAN LUOT" - trang tinh + outline den day -> noi bat
    TextStrings txt = langGetText(langGetCurrent());
    renderTextCentered(window, res.mainFont, txt.turnTimeBar,
        16, panelX + panelW / 2.f, barY - 18.f,
        sf::Color::White,
        sf::Color(0, 0, 0, 220), 2.f);  // black outline 2px
}


// Text "Bot dang suy nghi..." trong khi botGetMove() block main loop
void renderBotThinking(sf::RenderWindow& window, const GameResources& res) {
    float panelX = BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + UI_PANEL_GAP_LEFT;
    float panelW = WINDOW_WIDTH - panelX - UI_PANEL_GAP_RIGHT;

    TextStrings txt = langGetText(langGetCurrent());

    // Plate xanh nhe phia sau text
    sf::RectangleShape plate(sf::Vector2f(panelW - 20.f, 40.f));
    plate.setPosition(panelX + 10.f, UI_BOT_THINKING_Y - 15.f);
    plate.setFillColor(sf::Color(20, 30, 50, 220));
    plate.setOutlineThickness(2.f);
    plate.setOutlineColor(sf::Color(255, 220, 100, 220));
    window.draw(plate);

    // Text "Bot dang suy nghi..."
    renderTextCentered(window, res.mainFont, txt.botThinking,
        18, panelX + panelW / 2.f, UI_BOT_THINKING_Y + 5.f,
        sf::Color(255, 220, 100),
        sf::Color(0, 0, 0, 220), 2.f);
}
// VE MENU

// V2 #31: nhip dap (pulse) cho glow - dao dong muot 0.56 .. 1.0 theo thoi gian.
// Dung sf::Clock module-static (doc moi frame) -> animation khong can doi signature.
static float menuGlowPulse() {
    static sf::Clock c;
    float t = c.getElapsedTime().asSeconds();
    return 0.78f + 0.22f * std::sin(t * 3.0f);
}

// V2 #31: ve quang sang (glow halo) bo tron quanh (cx, cy).
// Nhieu lop rounded-rect dang vien thuoc (pill), lop ngoai to + mo dan
// -> gia lap hieu ung blur sang. intensity: cuong do tong (0..1).
static void drawGlowHalo(sf::RenderWindow& window, float cx, float cy,
    float w, float h, sf::Color col, float intensity) {
    // grow ti le theo chieu cao button -> nut nho glow nho, nut to glow to.
    struct Layer { float growFrac; float alpha; };
    const Layer layers[] = { { 0.70f, 0.10f }, { 0.45f, 0.15f }, { 0.22f, 0.22f } };
    for (const Layer& l : layers) {
        float grow = h * l.growFrac;
        sf::Uint8 a = (sf::Uint8)(255.f * l.alpha * intensity);
        drawRoundedCentered(window, cx, cy, w + grow, h + grow,
            (h + grow) / 2.f, sf::Color(col.r, col.g, col.b, a));
    }
}

// Helper: ve 1 button menu (V2 #31: glow halo + nen bo tron + khung gold texture)
// centerY: vi tri Y trung tam button
// selected: true = button highlight/hover (glow vang + sang hon)
static void drawMenuButton(sf::RenderWindow& window, const GameResources& res,
    float centerY, bool selected) {
    float btnW = selected ? 420.f : 380.f;
    float btnH = selected ? 64.f : 56.f;
    float cx = std::round(WINDOW_WIDTH / 2.f);
    float cy = std::round(centerY);

    // 1) Glow halo vang nhap nhay sau item dang chon/hover
    if (selected) {
        drawGlowHalo(window, cx, cy, btnW, btnH,
            sf::Color(255, 205, 90), menuGlowPulse());
    }

    // 2) Nen button bo tron mo (tat ca button) - tang chieu sau + tuong phan chu
    drawRoundedCentered(window, cx, cy, btnW, btnH, btnH / 2.f,
        sf::Color(16, 24, 40, selected ? 165 : 120),
        selected ? sf::Color(255, 220, 120, 220)
                 : sf::Color(120, 150, 190, 90),
        selected ? 2.f : 1.5f);

    // 3) Khung gold texture (button_frame.png) ben tren - giu phong cach cu
    if (res.buttonFrameTex.getSize().x == 0) return;
    sf::Sprite frame(res.buttonFrameTex);
    sf::Vector2u s = res.buttonFrameTex.getSize();
    frame.setOrigin(s.x / 2.f, s.y / 2.f);
    frame.setScale(btnW / s.x, btnH / s.y);
    frame.setPosition(cx, cy);
    frame.setColor(selected ? sf::Color(255, 240, 180, 255)   // golden warm
                            : sf::Color(200, 200, 220, 200)); // soft white
    window.draw(frame);
}

// V2 #31: ve 1 "pill" bo tron lam nen cho 1 muc UI (pause / game over / back...).
// Khong dung texture - dung cho cac man co layout rieng (slider, nut nho...).
// selected -> them glow halo nhap nhay + vien sang mau accent.
static void drawGlowPill(sf::RenderWindow& window, float cx, float cy,
    float w, float h, bool selected, sf::Color accent) {
    if (selected) {
        drawGlowHalo(window, cx, cy, w, h, accent, menuGlowPulse());
    }
    drawRoundedCentered(window, cx, cy, w, h, h / 2.f,
        sf::Color(16, 24, 40, selected ? 175 : 120),
        selected ? sf::Color(accent.r, accent.g, accent.b, 220)
                 : sf::Color(120, 150, 190, 80),
        selected ? 2.f : 1.5f);
}

// Helper: ve logo CARO o vi tri tieu de Main Menu
static void drawCaroLogo(sf::RenderWindow& window, const GameResources& res) {
    if (res.logoCaroTex.getSize().x == 0) return;
    sf::Sprite logo(res.logoCaroTex);
    sf::Vector2u s = res.logoCaroTex.getSize();
    logo.setOrigin(s.x / 2.f, s.y / 2.f);
    float targetW = 380.f;
    float scale = targetW / s.x;
    logo.setScale(scale, scale);
    logo.setPosition(std::round(WINDOW_WIDTH / 2.f), 150.f);
    window.draw(logo);
}

// Ham noi bo: ve 1 menu chung (tieu de + danh sach lua chon co button frame)
// useLogo: true → ve logo CARO thay text title (Main Menu)
static void renderMenuGeneric(sf::RenderWindow& window, const GameResources& res,
    const std::string& title,
    const std::string items[], int itemCount,
    int menuIndex,
    bool useLogo = false) {
    // Background + dim overlay (chu menu de doc)
    renderBackdrop(window, res, true);

    // Tieu de: logo hoac text
    if (useLogo) {
        drawCaroLogo(window, res);
    } else {
        renderTextCentered(window, res.titleFont, title, 52,
            WINDOW_WIDTH / 2.f, UI_MENU_TITLE_Y, sf::Color::White,
            sf::Color(0, 0, 0, 200), 2.f);
    }

    // Cac muc menu - moi item co button frame bo tron + text outline
    for (int i = 0; i < itemCount; i++) {
        bool selected = (i == menuIndex);
        float itemY = UI_MENU_START_Y + i * UI_MENU_STEP;

        // Button frame bo tron lam nen
        drawMenuButton(window, res, itemY, selected);

        // Text tren button
        sf::Color textColor = selected ? sf::Color(255, 230, 100) : sf::Color::White;
        int fontSize = selected ? 28 : 24;
        renderTextCentered(window, res.mainFont, items[i], fontSize,
            WINDOW_WIDTH / 2.f, itemY, textColor,
            sf::Color(0, 0, 0, 220), 1.5f);
    }
}

// Main Menu - dung logo CARO thay text title
void renderMainMenu(sf::RenderWindow& window, const GameResources& res,
    int menuIndex) {
    TextStrings txt = langGetText(langGetCurrent());
    std::string items[] = {
        txt.newGame, txt.loadGame, txt.settings,
        txt.help, txt.about, txt.exitGame
    };
    renderMenuGeneric(window, res, txt.title, items, 6, menuIndex, true);
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

// Menu tam dung (ESC in-game) - V2: 6 items voi Language/Volume/SFX inline
void renderPauseMenu(sf::RenderWindow& window, const GameResources& res,
    int menuIndex, int volume, bool sfxOn) {
    // Lop phu toi ban trong (overlay)
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(overlay);

    TextStrings txt = langGetText(langGetCurrent());
    Language lang = langGetCurrent();

    // V2: 6 items
    // 0: Resume   1: Save   2: Language toggle   3: Volume slider   4: SFX toggle   5: Main Menu
    std::string langStr = txt.language + " " +
        (lang == LANG_VIETNAMESE ? txt.langVietnamese : txt.langEnglish);
    std::string sfxStr = txt.sfxToggle + " " + (sfxOn ? txt.on : txt.off);

    std::string items[6] = {
        txt.resume, txt.saveGame, langStr, "", sfxStr, txt.returnMenu
    };

    // Tieu de - day len de co cho cho 6 items
    renderTextCentered(window, res.titleFont, "PAUSE", 40,
        WINDOW_WIDTH / 2.f, 150.f, sf::Color::White);

    // Step ngan hon de fit 6 items
    const float PAUSE_START_Y = 230.f;
    const float PAUSE_STEP    = 60.f;

    for (int i = 0; i < 6; i++) {
        bool selected = (i == menuIndex);
        sf::Color color = selected ? COLOR_MENU_HOVER : COLOR_MENU_TEXT;
        int fontSize = selected ? 26 : 22;
        float itemY = PAUSE_START_Y + i * PAUSE_STEP;

        // V2 #31: nen pill bo tron + glow vang khi selected/hover.
        // Row volume (i==3) cao hon de bao ca label + thanh slider.
        drawGlowPill(window, WINDOW_WIDTH / 2.f, itemY, 380.f,
            (i == 3) ? 74.f : 46.f, selected, COLOR_MENU_HOVER);

        if (i == 3) {
            // VOLUME ROW: label + slider bar (giong renderSettings)
            std::string volLabel = txt.bgmVolume + " " + std::to_string(volume) + "%";
            renderTextCentered(window, res.mainFont, volLabel, fontSize,
                WINDOW_WIDTH / 2.f, itemY - 14.f, color);

            const float trackW = 300.f;
            const float trackH = 8.f;
            const float trackX = WINDOW_WIDTH / 2.f - trackW / 2.f;
            const float trackY = itemY + 14.f;

            sf::RectangleShape track(sf::Vector2f(trackW, trackH));
            track.setPosition(trackX, trackY);
            track.setFillColor(sf::Color(80, 90, 110));
            track.setOutlineThickness(1.f);
            track.setOutlineColor(sf::Color(140, 150, 170));
            window.draw(track);

            sf::RectangleShape fill(sf::Vector2f(trackW * volume / 100.f, trackH));
            fill.setPosition(trackX, trackY);
            fill.setFillColor(selected ? sf::Color(80, 200, 255) : sf::Color(60, 150, 220));
            window.draw(fill);

            float handleR = selected ? 11.f : 9.f;
            sf::CircleShape handle(handleR);
            handle.setOrigin(handleR, handleR);
            handle.setPosition(trackX + trackW * volume / 100.f, trackY + trackH / 2.f);
            handle.setFillColor(sf::Color::White);
            handle.setOutlineColor(selected ? sf::Color(80, 200, 255) : sf::Color(60, 150, 220));
            handle.setOutlineThickness(2.5f);
            window.draw(handle);
        }
        else {
            renderTextCentered(window, res.mainFont, items[i], fontSize,
                WINDOW_WIDTH / 2.f, itemY, color);
        }
    }
}


// ============================================================
// NUT BACK CHUNG (V2)
// ============================================================
// Hitbox cua nut Back: goc trai duoi (~ y = WINDOW_HEIGHT - 60)
static const float BACK_BTN_X = 30.f;          // top-left x
static const float BACK_BTN_Y = 640.f;          // top-left y
static const float BACK_BTN_W = 130.f;
static const float BACK_BTN_H = 48.f;

bool backButtonContains(float mx, float my) {
    return mx >= BACK_BTN_X && mx <= BACK_BTN_X + BACK_BTN_W
        && my >= BACK_BTN_Y && my <= BACK_BTN_Y + BACK_BTN_H;
}

// Vi tri nut Save / Exit trong gameplay (duoi panel P2)
// Save rong hon Exit vi chu "LUU GAME" dai hon "THOAT" (tranh bi cat chu khi hover)
static const float ACT_BTN_H  = 44.f;
static const float ACT_BTN_Y  = 620.f;
static const float ACT_SAVE_X = 815.f;
static const float ACT_SAVE_W = 150.f;
static const float ACT_EXIT_X = 985.f;
static const float ACT_EXIT_W = 110.f;

bool gameplaySaveBtnContains(float mx, float my) {
    return mx >= ACT_SAVE_X && mx <= ACT_SAVE_X + ACT_SAVE_W
        && my >= ACT_BTN_Y && my <= ACT_BTN_Y + ACT_BTN_H;
}
bool gameplayExitBtnContains(float mx, float my) {
    return mx >= ACT_EXIT_X && mx <= ACT_EXIT_X + ACT_EXIT_W
        && my >= ACT_BTN_Y && my <= ACT_BTN_Y + ACT_BTN_H;
}

static void drawActionBtn(sf::RenderWindow& window, const GameResources& res,
                          float x, float y, float w, const std::string& label,
                          bool hover, sf::Color accent) {
    // V2 #31: nen pill bo tron + glow theo accent (xanh Save / do Exit) khi hover
    drawGlowPill(window, x + w / 2.f, y + ACT_BTN_H / 2.f, w, ACT_BTN_H, hover, accent);

    renderTextCentered(window, res.mainFont, label, hover ? 22 : 20,
        x + w / 2.f, y + ACT_BTN_H / 2.f,
        hover ? accent : sf::Color::White);
}

void renderGameplayActions(sf::RenderWindow& window, const GameResources& res,
                           float mx, float my) {
    TextStrings txt = langGetText(langGetCurrent());
    drawActionBtn(window, res, ACT_SAVE_X, ACT_BTN_Y, ACT_SAVE_W, txt.saveGame,
                  gameplaySaveBtnContains(mx, my),
                  sf::Color(100, 220, 140));      // Xanh la cho Save
    drawActionBtn(window, res, ACT_EXIT_X, ACT_BTN_Y, ACT_EXIT_W, txt.exitGame,
                  gameplayExitBtnContains(mx, my),
                  sf::Color(255, 120, 120));      // Do nhe cho Exit
}

void renderBackButton(sf::RenderWindow& window, const GameResources& res,
                      float mx, float my) {
    TextStrings txt = langGetText(langGetCurrent());
    bool hover = backButtonContains(mx, my);

    // V2 #31: nut Back bo tron + glow vang khi hover (thay box vuong cu)
    drawGlowPill(window, BACK_BTN_X + BACK_BTN_W / 2.f, BACK_BTN_Y + BACK_BTN_H / 2.f,
        BACK_BTN_W, BACK_BTN_H, hover, COLOR_MENU_HOVER);

    // Label "< Back" / "< Quay lai"
    std::string label = std::string("< ") + txt.back;
    renderTextCentered(window, res.mainFont, label, hover ? 22 : 20,
        BACK_BTN_X + BACK_BTN_W / 2.f, BACK_BTN_Y + BACK_BTN_H / 2.f,
        hover ? COLOR_MENU_HOVER : sf::Color::White);
}


// VE MAN HINH NHAP TEN


void renderInputNames(sf::RenderWindow& window, const GameResources& res,
    const std::string& name1, const std::string& name2,
    bool isEditingPlayer1, const std::string& errorMsg, bool isPvC) {
    renderBackdrop(window, res, true);
    TextStrings txt = langGetText(langGetCurrent());

    if (isPvC) {
        // PVC: chỉ 1 ô nhập tên, bỏ luôn số "1" cho mượt
        std::string titleStr = (langGetCurrent() == LANG_VIETNAMESE) ? u8"Nhập tên Người chơi:" : "Enter Player Name:";
        renderTextCentered(window, res.titleFont, titleStr, 32,
            WINDOW_WIDTH / 2.f, 220.f, sf::Color::White);

        sf::RectangleShape box1(sf::Vector2f(420.f, 56.f));
        box1.setOrigin(210.f, 28.f);
        box1.setPosition(WINDOW_WIDTH / 2.f, 310.f);
        box1.setFillColor(sf::Color(60, 60, 80));
        box1.setOutlineThickness(3.f);
        box1.setOutlineColor(COLOR_MENU_HOVER);
        window.draw(box1);

        // Placeholder đổi thành "Player" / "Người chơi" thay vì có số 1
        std::string placeholder = (langGetCurrent() == LANG_VIETNAMESE) ? u8"Người chơi" : "Player";
        renderTextCentered(window, res.mainFont,
            name1.empty() ? placeholder : name1, 24,
            WINDOW_WIDTH / 2.f, 310.f,
            name1.empty() ? sf::Color(150, 150, 150) : sf::Color::White);

        // ĐÃ XÓA DÒNG RENDER THÔNG TIN MÁY Ở ĐÂY

        // Hiển thị lỗi (với chữ viền đen như lúc nãy ông ưng)
        if (!errorMsg.empty()) {
            renderTextCentered(window, res.mainFont, errorMsg, 18,
                WINDOW_WIDTH / 2.f, 430.f,
                sf::Color(255, 100, 100),       // Đỏ pastel
                sf::Color(0, 0, 0, 230), 2.f);  // Viền đen 2px
        }

        renderTextCentered(window, res.mainFont, txt.inputNameHintPvC, 16,
            WINDOW_WIDTH / 2.f, 500.f, sf::Color(220, 220, 220));
    }
    else {
        // PVP: 2 o nhap ten
        renderTextCentered(window, res.titleFont, txt.enterName1, 30,
            WINDOW_WIDTH / 2.f, 150.f, sf::Color::White);

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
            name1.empty() ? sf::Color(150, 150, 150) : sf::Color::White);

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
            name2.empty() ? sf::Color(150, 150, 150) : sf::Color::White);

        if (!errorMsg.empty()) {
            renderTextCentered(window, res.mainFont, errorMsg, 18,
                WINDOW_WIDTH / 2.f, 460.f,
                sf::Color(255, 100, 100),       // Màu đỏ sáng hơn
                sf::Color(0, 0, 0, 230), 2.f);
        }

        renderTextCentered(window, res.mainFont, txt.inputNameHintPvP, 16,
            WINDOW_WIDTH / 2.f, 520.f, sf::Color(220, 220, 220));
    }

    // V2: Nut Back o goc trai duoi (mouse-clickable)
    sf::Vector2i pix = sf::Mouse::getPosition(window);
    sf::Vector2f mp = window.mapPixelToCoords(pix);
    renderBackButton(window, res, mp.x, mp.y);
}



// VE TOAN BO MAN HINH GAMEPLAY

void renderGameplay(sf::RenderWindow& window, const GameState& state,
    const GameResources& res, const WinLine* winLine,
    int hintRow, int hintCol, bool showHint,
    GameResult result) {
    // Ve background toan man hinh (KHONG dim - de thay ro background trong gameplay)
    renderBackdrop(window, res, false);

    // Ve ban co + quan co
    renderBoard(window, res);
    renderPieces(window, state, res);

    // Ve cursor o dang chon (chi khi van con choi)
    if (result == RESULT_NONE)
        renderCursor(window, state.cursorRow, state.cursorCol);

    // Ve hint neu dang bat
    if (showHint && hintRow >= 0 && hintCol >= 0)
        renderHint(window, hintRow, hintCol);

    // Ve duong thang neu co nguoi thang
    if (winLine != nullptr && winLine->count >= WIN_COUNT)
        renderWinLine(window, *winLine);

    // Ve panel thong tin nguoi choi (mascot doi theo result + game timer per-player)
    renderPlayerPanel(window, state, res, result);

    // Ve turn timer bar (chi khi che do Speed) - game timer cua tung nguoi da nam trong panel
    if (state.style == STYLE_SPEED) {
        renderTurnTimer(window, res, timerGetTurnPercent(state.timer));
    }

    // V2: Nut Save / Exit (chi hien khi van con choi)
    if (result == RESULT_NONE) {
        sf::Vector2i pix = sf::Mouse::getPosition(window);
        sf::Vector2f mp = window.mapPixelToCoords(pix);
        renderGameplayActions(window, res, mp.x, mp.y);
    }
}



// VE MAN HINH KET THUC
// VE MAN HINH KET THUC (Đã dời xuống để không đè timer)
void renderGameOver(sf::RenderWindow& window, const GameState& state,
    const GameResources& res, GameResult result,
    int menuIndex, bool askingSave) {

    // ANIMATION CLOCK - reset moi khi result doi (van moi)
    static GameResult lastResult = RESULT_NONE;
    static sf::Clock animClock;
    if (result != lastResult) {
        lastResult = result;
        animClock.restart();
    }

    // Overlay lam mo ban co
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(overlay);

    float panelX = BOARD_OFFSET_X + BOARD_SIZE * CELL_SIZE + UI_PANEL_GAP_LEFT;
    float panelW = WINDOW_WIDTH - panelX - UI_PANEL_GAP_RIGHT;

    float centerX = panelX + panelW / 2.f;
    float startY = UI_GAMEOVER_START_Y;

    TextStrings txt = langGetText(langGetCurrent());

    // (VICTORY/DEFEAT da duoc render trong renderPlayerPanel - giua moi player box)
    // Animation clock van duoc giu lai cho cac hieu ung tuong lai
    (void)animClock;  // suppress unused warning

    // Ket qua - mau sang hon de noi bat
    std::string resultStr;
    sf::Color resultColor;
    if (result == RESULT_PLAYER1_WIN) {
        resultStr = state.player1.name + " " + txt.playerWin;
        resultColor = sf::Color(255, 100, 100);   // Do sang
    }
    else if (result == RESULT_PLAYER2_WIN) {
        resultStr = state.player2.name + " " + txt.playerWin;
        resultColor = sf::Color(100, 180, 255);   // Xanh sang
    }
    else {
        resultStr = txt.drawText;
        resultColor = sf::Color(255, 220, 80);    // Vang sang
    }

    // PLATE den mo phia sau khu vuc Game Over → tao do tuong phan
    sf::RectangleShape plate(sf::Vector2f(panelW + 10.f, 180.f));
    plate.setOrigin((panelW + 10.f) / 2.f, 0.f);
    plate.setPosition(centerX, startY - 25.f);
    plate.setFillColor(sf::Color(15, 20, 40, 210));   // dark navy ~82%
    plate.setOutlineThickness(2.f);
    plate.setOutlineColor(sf::Color(80, 100, 140, 220));
    window.draw(plate);

    // Ten nguoi thang - SIZE BIG + OUTLINE den day → POP cuc manh
    renderTextCentered(window, res.titleFont, resultStr, 38,
        centerX, startY, resultColor,
        sf::Color(0, 0, 0, 230), 3.f);   // black outline 3px

    // Thong ke ti so - them outline nho cho do
  // 1. Vẽ tỉ số CỐ ĐỊNH ở ngay chính giữa màn hình
    std::string scoreStr = std::to_string(state.player1.totalWins) + "  -  " + std::to_string(state.player2.totalWins);
    renderTextCentered(window, res.mainFont, scoreStr, 24, // Size to hơn tí cho đẹp
        centerX, startY + UI_GAMEOVER_STATS_DY,
        sf::Color(255, 220, 80), // Màu vàng cam cho tỉ số nổi bật
        sf::Color(0, 0, 0, 200), 1.5f);

    // 2. Vẽ tên Player 1 (Căn lề PHẢI, đẩy lùi về bên trái tỉ số)
    sf::Text t1;
    t1.setFont(res.mainFont);
    t1.setString(sf::String::fromUtf8(state.player1.name.begin(), state.player1.name.end()));
    t1.setCharacterSize(20);
    t1.setFillColor(sf::Color::White);
    t1.setOutlineColor(sf::Color(0, 0, 0, 200));
    t1.setOutlineThickness(1.5f);
    sf::FloatRect b1 = t1.getLocalBounds();
    t1.setOrigin(b1.left + b1.width, b1.top + b1.height / 2.f); // Mấu chốt để căn lề phải
    t1.setPosition(centerX - 45.f, startY + UI_GAMEOVER_STATS_DY); // Cách tâm 45 pixel về bên trái
    window.draw(t1);

    // 3. Vẽ tên Player 2 (Căn lề TRÁI, đẩy qua bên phải tỉ số)
    sf::Text t2;
    t2.setFont(res.mainFont);
    t2.setString(sf::String::fromUtf8(state.player2.name.begin(), state.player2.name.end()));
    t2.setCharacterSize(20);
    t2.setFillColor(sf::Color::White);
    t2.setOutlineColor(sf::Color(0, 0, 0, 200));
    t2.setOutlineThickness(1.5f);
    sf::FloatRect b2 = t2.getLocalBounds();
    t2.setOrigin(b2.left, b2.top + b2.height / 2.f); // Mấu chốt để căn lề trái
    t2.setPosition(centerX + 45.f, startY + UI_GAMEOVER_STATS_DY); // Cách tâm 45 pixel về bên phải
    window.draw(t2);

    // Cau hoi "Play again?" hoac "Lưu ván đấu?"
    std::string items[] = { txt.yes, txt.no };

    // Nếu askingSave = true thì in câu hỏi lưu, ngược lại thì in câu hỏi chơi tiếp
    std::string questionText = askingSave ?
        (langGetCurrent() == LANG_VIETNAMESE ? u8"Bạn có muốn lưu ván đấu không?" : "Do you want to save this game?")
        : txt.continueText;

    renderTextCentered(window, res.mainFont, questionText, 18,
        centerX, startY + UI_GAMEOVER_QUESTION_DY, sf::Color(230, 230, 230),
        sf::Color(0, 0, 0, 180), 1.f);

    // Nut Yes / No - co outline nhe
    for (int i = 0; i < 2; i++) {
        bool selected = (i == menuIndex);
        float btnX = centerX - UI_GAMEOVER_BTN_GAP_X + i * (UI_GAMEOVER_BTN_GAP_X * 2);
        float btnY = startY + UI_GAMEOVER_BTN_DY;
        // V2 #31: nen pill bo tron + glow khi selected/hover
        drawGlowPill(window, btnX, btnY, 110.f, 44.f, selected, COLOR_MENU_HOVER);
        renderTextCentered(window, res.mainFont, items[i],
            selected ? 24 : 20,
            btnX, btnY,
            selected ? COLOR_MENU_HOVER : COLOR_MENU_TEXT,
            sf::Color(0, 0, 0, 200), 1.5f);
    }
}

// VE SAVE / LOAD

// V2 #30: helper ve 1 rounded rect can giua tai (cx, cy)
// (default args khai bao o forward declaration dau file)
static void drawRoundedCentered(sf::RenderWindow& window, float cx, float cy,
    float w, float h, float radius, sf::Color fill,
    sf::Color outline, float outlineThick) {
    RoundedRectangleShape r(sf::Vector2f(w, h), radius, 8);
    r.setOrigin(w / 2.f, h / 2.f);
    r.setPosition(cx, cy);
    r.setFillColor(fill);
    if (outlineThick > 0.f) {
        r.setOutlineThickness(outlineThick);
        r.setOutlineColor(outline);
    }
    window.draw(r);
}

// V2 #30: Ve phan list co the cuon (dung chung cho Save + Load screen).
// Panel bo goc + hang hover/selected co glow + thanh scrollbar bo goc.
// Chi ve cac item trong cua so [scrollTop, scrollTop + UI_LIST_VISIBLE).
static void renderScrollableList(sf::RenderWindow& window, const GameResources& res,
    const std::vector<std::string>& list, int selectedIndex,
    int scrollTop, float startY) {
    int count = (int)list.size();
    int visible = UI_LIST_VISIBLE;
    int last = scrollTop + visible; if (last > count) last = count;

    const float cx = WINDOW_WIDTH / 2.f;
    const float ROW_W = 500.f;   // be rong vung highlight 1 hang
    const float ROW_H = 30.f;
    const float PANEL_W = 548.f; // panel bao quanh list (rong hon de chua scrollbar)
    const float panelTop = startY - UI_LIST_STEP / 2.f - 6.f;
    const float panelH = visible * UI_LIST_STEP + 12.f;

    // Mouse hover -> highlight hang dang ro chuot (doc lap voi selection)
    sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    int hoveredRow = -1;
    for (int row = 0; row < visible && scrollTop + row < count; row++) {
        float y = startY + row * UI_LIST_STEP;
        if (mp.x > cx - ROW_W / 2.f && mp.x < cx + ROW_W / 2.f
            && mp.y > y - UI_LIST_HALF_HEIGHT && mp.y < y + UI_LIST_HALF_HEIGHT) {
            hoveredRow = row;
        }
    }

    // Panel nen bo goc (mo, vien nhe)
    drawRoundedCentered(window, cx, panelTop + panelH / 2.f, PANEL_W, panelH, 14.f,
        sf::Color(18, 26, 40, 170), sf::Color(120, 150, 190, 90), 1.5f);

    // Cac hang
    for (int i = scrollTop; i < last; i++) {
        int row = i - scrollTop;
        bool selected = (i == selectedIndex);
        bool hovered = (row == hoveredRow);
        float y = startY + row * UI_LIST_STEP;

        if (selected) {
            // glow mo phia sau + hang chinh
            drawRoundedCentered(window, cx, y, ROW_W + 12.f, ROW_H + 8.f, 12.f,
                sf::Color(52, 152, 219, 60));
            drawRoundedCentered(window, cx, y, ROW_W, ROW_H, 9.f,
                sf::Color(52, 152, 219, 130), sf::Color(120, 190, 240, 200), 1.5f);
        }
        else if (hovered) {
            drawRoundedCentered(window, cx, y, ROW_W, ROW_H, 9.f,
                sf::Color(255, 255, 255, 28));
        }

        renderTextCentered(window, res.mainFont, list[i], 16, cx, y,
            selected ? sf::Color::White : COLOR_MENU_TEXT);
    }

    // Khong co gi de cuon -> bo qua scrollbar/arrow
    if (count <= visible) return;

    // Thanh scrollbar bo goc ben phai panel
    float barX = cx + PANEL_W / 2.f - 12.f;
    float trackTop = panelTop + 6.f;
    float trackH = panelH - 12.f;
    drawRoundedCentered(window, barX, trackTop + trackH / 2.f, 6.f, trackH, 3.f,
        sf::Color(255, 255, 255, 35));

    float thumbH = trackH * (float)visible / (float)count;
    if (thumbH < 24.f) thumbH = 24.f;
    float maxScroll = (float)(count - visible);
    float thumbY = trackTop + (trackH - thumbH) * (scrollTop / maxScroll);
    drawRoundedCentered(window, barX, thumbY + thumbH / 2.f, 6.f, thumbH, 3.f,
        COLOR_MENU_HOVER);
}

void renderSaveScreen(sf::RenderWindow& window, const GameResources& res,
    const std::vector<std::string>& saveList,
    const std::string& inputName, int selectedIndex, int scrollTop) {
    renderBackdrop(window, res, true);
    TextStrings txt = langGetText(langGetCurrent());

    renderTextCentered(window, res.titleFont, txt.saveGame, 38,
        WINDOW_WIDTH / 2.f, UI_SAVE_TITLE_Y, sf::Color::White);

    // O nhap ten save
    sf::RectangleShape inputBox(sf::Vector2f(400.f, 45.f));
    inputBox.setOrigin(200.f, 22.5f);
    inputBox.setPosition(WINDOW_WIDTH / 2.f, UI_SAVE_INPUT_Y);
    inputBox.setFillColor(sf::Color(60, 60, 80));
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(COLOR_MENU_HOVER);
    window.draw(inputBox);

    renderTextCentered(window, res.mainFont,
        inputName.empty() ? txt.enterSaveName : inputName, 18,
        WINDOW_WIDTH / 2.f, UI_SAVE_INPUT_Y,
        inputName.empty() ? sf::Color(120, 120, 120) : sf::Color::White);

    // Danh sach file save (scrollable - V2 #30)
    renderScrollableList(window, res, saveList, selectedIndex, scrollTop,
        UI_SAVE_LIST_START_Y);

    // Huong dan (2 dong: keyboard + mouse, sang hon)
    renderTextCentered(window, res.mainFont, txt.saveHintBottom, 15,
        WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 55.f,
        sf::Color(235, 235, 235));
    renderTextCentered(window, res.mainFont, txt.saveHintMouse, 14,
        WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 30.f,
        sf::Color(210, 210, 210));

    // V2: Nut Back goc trai duoi (mouse-clickable)
    sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    renderBackButton(window, res, mp.x, mp.y);
}

void renderLoadScreen(sf::RenderWindow& window, const GameResources& res,
    const std::vector<std::string>& saveList,
    int selectedIndex, int scrollTop) {
    renderBackdrop(window, res, true);
    TextStrings txt = langGetText(langGetCurrent());

    renderTextCentered(window, res.titleFont, txt.loadGame, 38,
        WINDOW_WIDTH / 2.f, UI_LOAD_TITLE_Y, sf::Color::White);

    if (saveList.empty()) {
        renderTextCentered(window, res.mainFont, txt.fileNotFound, 20,
            WINDOW_WIDTH / 2.f, 300.f, sf::Color(180, 180, 180));
    }
    else {
        // Danh sach file (scrollable - V2 #30)
        renderScrollableList(window, res, saveList, selectedIndex, scrollTop,
            UI_LOAD_LIST_START_Y);
    }

    // Huong dan (keyboard + mouse, sang hon)
    renderTextCentered(window, res.mainFont, txt.loadHintBottom, 15,
        WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 55.f,
        sf::Color(235, 235, 235));
    renderTextCentered(window, res.mainFont, txt.loadHintMouse, 14,
        WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 30.f,
        sf::Color(210, 210, 210));

    // V2: Nut Back goc trai duoi (mouse-clickable)
    sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    renderBackButton(window, res, mp.x, mp.y);
}

// VE SETTINGS / HELP / ABOUT

void renderSettings(sf::RenderWindow& window, const GameResources& res,
    int menuIndex, Language lang, int volume, bool sfxOn) {
    renderBackdrop(window, res, true);
    TextStrings txt = langGetText(langGetCurrent());

    renderTextCentered(window, res.titleFont, txt.settings, 38,
        WINDOW_WIDTH / 2.f, UI_SETTINGS_TITLE_Y, sf::Color::White);

    // Cac chuoi label
    std::string langStr = txt.language + " " +
        (lang == LANG_VIETNAMESE ? txt.langVietnamese : txt.langEnglish);
    std::string sfxStr = txt.sfxToggle + " " + (sfxOn ? txt.on : txt.off);

    // Render tung row (V2: bo row "Back" - thay bang corner Back button ben duoi)
    for (int i = 0; i < 3; i++) {
        bool selected = (i == menuIndex);
        sf::Color color = selected ? COLOR_MENU_HOVER : COLOR_MENU_TEXT;
        int fontSize = selected ? 26 : 22;
        float itemY = UI_SETTINGS_START_Y + i * UI_SETTINGS_STEP;

        if (i == 1) {
            // VOLUME ROW: label + slider bar
            std::string volLabel = txt.bgmVolume + " " + std::to_string(volume) + "%";
            renderTextCentered(window, res.mainFont, volLabel, fontSize,
                WINDOW_WIDTH / 2.f, itemY - 14.f, color);

            // Slider track + fill + handle
            const float trackW = 360.f;
            const float trackH = 8.f;
            const float trackX = WINDOW_WIDTH / 2.f - trackW / 2.f;
            const float trackY = itemY + 15.f;

            // Track background (xam dam)
            sf::RectangleShape track(sf::Vector2f(trackW, trackH));
            track.setPosition(trackX, trackY);
            track.setFillColor(sf::Color(80, 90, 110));
            track.setOutlineThickness(1.f);
            track.setOutlineColor(sf::Color(140, 150, 170));
            window.draw(track);

            // Filled portion (mau xanh sang)
            sf::RectangleShape fill(sf::Vector2f(trackW * volume / 100.f, trackH));
            fill.setPosition(trackX, trackY);
            fill.setFillColor(selected ? sf::Color(80, 200, 255) : sf::Color(60, 150, 220));
            window.draw(fill);

            // Handle (tron, trang)
            float handleR = selected ? 12.f : 10.f;
            sf::CircleShape handle(handleR);
            handle.setOrigin(handleR, handleR);
            handle.setPosition(trackX + trackW * volume / 100.f, trackY + trackH / 2.f);
            handle.setFillColor(sf::Color::White);
            handle.setOutlineColor(selected ? sf::Color(80, 200, 255) : sf::Color(60, 150, 220));
            handle.setOutlineThickness(3.f);
            window.draw(handle);
        }
        else {
            // i==0: Language, i==2: SFX
            std::string content = (i == 0) ? langStr : sfxStr;
            renderTextCentered(window, res.mainFont, content, fontSize,
                WINDOW_WIDTH / 2.f, itemY, color);
        }
    }

    // Settings hint o cuoi man hinh
    renderTextCentered(window, res.mainFont, txt.settingsHint, 14,
        WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 40.f,
        sf::Color(230, 230, 230));

    // V2: Nut Back goc trai duoi (mouse-clickable) - dong nhat voi Load/Help/About
    sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    renderBackButton(window, res, mp.x, mp.y);
}

void renderHelp(sf::RenderWindow& window, const GameResources& res) {
    renderBackdrop(window, res, true);
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

    // V2: Nut Back goc trai duoi (mouse-clickable)
    sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    renderBackButton(window, res, mp.x, mp.y);
}

void renderAbout(sf::RenderWindow& window, const GameResources& res) {
    renderBackdrop(window, res, true);
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

    // V2: Nut Back goc trai duoi (mouse-clickable)
    sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    renderBackButton(window, res, mp.x, mp.y);
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
