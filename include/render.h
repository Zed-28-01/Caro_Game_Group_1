#pragma once
#ifndef RENDER_H
#define RENDER_H

#include "game_types.h"
#include <vector> // ConfettiSystem + saveList params (game_types.h khong con keo <vector>)

// WINDOW RESIZE / LETTERBOX

// Apply letterbox view voi aspect ratio 16:9 (giu nguyen ty le)
// Goi mot lan sau khi tao window, va trong moi event Resize
void applyLetterbox(sf::RenderWindow& window, unsigned int newWidth, unsigned int newHeight);

// Xu ly event chung: map mouse coords tu pixel -> view, xu ly Closed/Resized.
// Tra ve true neu event la Closed/Resized.
bool handleCommonEvent(sf::RenderWindow& window, sf::Event& event);

// KHOI TAO RESOURCES

// Load tat ca font, texture, sound tu thu muc assets/
// Tra ve true neu thanh cong, false neu thieu file
bool renderLoadResources(GameResources& res);

// VE MENU

// Ve Main Menu (New Game, Load, Settings, Help, About, Exit)
// menuIndex: muc dang duoc chon (highlight)
void renderMainMenu(sf::RenderWindow& window, const GameResources& res,
                    int menuIndex);

// Ve menu chon che do - V2 (11/06): 2 TILE vuong bo goc (PvP | PvC)
// voi anh + hover glow + nut Back goc trai duoi
void renderModeSelect(sf::RenderWindow& window, const GameResources& res,
                      int menuIndex);

// Hit-test 2 tile man chon che do. Tra ve 0=PvP, 1=PvC, -1=khong trung.
int modeSelectHitTest(float mx, float my);

// Ve menu chon do kho AI - V2 (11/06): 4 TILE ngang, moi tile 1 villain
// dai dien (Frieza/Cell/Buu/Broly) + hover glow + Back + hint ESC
void renderDifficultySelect(sf::RenderWindow& window, const GameResources& res,
                            int menuIndex);

// Hit-test 4 tile do kho. Tra ve 0..3, -1 = khong trung.
int diffSelectHitTest(float mx, float my);

// Ve man chon nhan vat - luoi 6 hero.
// pickingPlayer: 1|2. takenIndex: hero da chon truoc do (-1 = chua chon).
void renderCharSelect(sf::RenderWindow& window, const GameResources& res,
                      int menuIndex, int pickingPlayer, int takenIndex);

// Hit-test luoi 6 hero. Tra ve 0..5, -1 = khong trung.
int charSelectHitTest(float mx, float my);

// Ve menu chon kieu choi (Basic / Speed)
void renderStyleSelect(sf::RenderWindow& window, const GameResources& res,
                       int menuIndex);

// Ve menu tam dung (ESC in-game)
// V2: Pause menu expanded - 6 items: Resume / Save / Language / BGM Volume / SFX / Main Menu
void renderPauseMenu(sf::RenderWindow& window, const GameResources& res,
                     int menuIndex, int volume, bool sfxOn);

// NUT BACK CHUNG (dung cho cac man hinh phu)

// Ve nut "Back" o goc trai duoi man hinh.
// Highlight neu chuot dang hover (mx, my).
void renderBackButton(sf::RenderWindow& window, const GameResources& res,
                      float mx = -1.f, float my = -1.f);

// Kiem tra (mx, my) co nam trong vung click cua nut Back khong.
bool backButtonContains(float mx, float my);

// NUT SAVE / EXIT TRONG GAMEPLAY
// Ve 2 nut "Luu" + "Thoat" o duoi panel mascot P2 (chi khi van con choi).
void renderGameplayActions(sf::RenderWindow& window, const GameResources& res,
                           float mx, float my);
bool gameplaySaveBtnContains(float mx, float my);
bool gameplayExitBtnContains(float mx, float my);

// VE MAN HINH NHAP TEN

// Ve form nhap ten nguoi choi
// isPvC = true: chi hien 1 o (Player 1), Player 2 mac dinh = "May"/"Computer"
void renderInputNames(sf::RenderWindow& window, const GameResources& res,
                      const std::string& name1, const std::string& name2,
                      bool isEditingPlayer1, const std::string& errorMsg,
                      bool isPvC = false);

// VE BAN CO & GAMEPLAY

// Ve man hinh choi game (ban co, quan co, highlight, panel, timer).
// result dung de xu ly trang thai khi game over.
void renderGameplay(sf::RenderWindow& window, const GameState& state,
                    const GameResources& res, const WinLine* winLine,
                    int hintRow, int hintCol, bool showHint,
                    GameResult result = RESULT_NONE);

// Ve ban co (grid lines)
void renderBoard(sf::RenderWindow& window, const GameResources& res);

// Ve cac quan co da danh tren ban co
void renderPieces(sf::RenderWindow& window, const GameState& state,
                  const GameResources& res);

// Ve highlight cho o dang chon (cursor)
void renderCursor(sf::RenderWindow& window, int row, int col);

// Ve highlight goi y (hint) - mau khac voi cursor
void renderHint(sf::RenderWindow& window, int row, int col);

// Ve duong thang khi co nguoi thang (noi 5 quan)
void renderWinLine(sf::RenderWindow& window, const WinLine& winLine);

// VE PANEL THONG TIN NGUOI CHOI

// Ve panel thong tin nguoi choi (ten, nuoc di, van thang)
// Highlight nguoi dang di
void renderPlayerPanel(sf::RenderWindow& window, const GameState& state,
                       const GameResources& res,
                       GameResult result = RESULT_NONE);

// VE TIMER / PROGRESS BAR

// Ve thanh progress bar countdown cho luot hien tai
// percentage: 0.0 (het gio) -> 1.0 (con day du)
void renderTurnTimer(sf::RenderWindow& window, const GameResources& res,
                     float percentage);

// Ve text "Bot dang suy nghi..." khi bot block main loop (PvC + Speed mode)
// Hien thi tai vung giua panel (vi tri cu cua renderGameTimer)
void renderBotThinking(sf::RenderWindow& window, const GameResources& res);

// VE HIEU UNG

// Hieu ung khi dat quan (animation ngan)
void renderPlaceEffect(sf::RenderWindow& window, int row, int col,
                       int player, float progress);

// CONFETTI (particle system)

// 1 manh confetti: hinh chu nhat nho, xoay + lat mat + roi theo gravity.
struct ConfettiPiece {
  sf::Vector2f pos;        // vi tri hien tai (px)
  sf::Vector2f vel;        // van toc (px/s)
  sf::Color color;         // mau (palette le hoi)
  float angle = 0.f;       // goc xoay hien tai (deg)
  float angVel = 0.f;      // toc do xoay (deg/s)
  float spin = 0.f;        // pha tumbling (lat mat 3D gia lap)
  float spinVel = 0.f;     // toc do lat
  float size = 8.f;        // canh dai (px)
  float life = 0.f;        // thoi gian song con lai (s)
  float maxLife = 1.f;     // tong thoi gian song (de tinh fade)
};

// He thong confetti - caller so huu (local trong handleGameOver).
struct ConfettiSystem {
  std::vector<ConfettiPiece> pieces;
};

// Phun 1 burst tu (cx, cy): toa ra moi huong + boost len, gravity keo xuong.
void confettiSpawnBurst(ConfettiSystem& sys, float cx, float cy, int count);

// Cap nhat vat ly (gravity, xoay, fade) theo dt giay. Tu xoa hat het song/roi khoi man.
void confettiUpdate(ConfettiSystem& sys, float dt);

// Ve toan bo confetti bang 1 sf::VertexArray (Quads) -> 1 draw call.
void confettiDraw(sf::RenderWindow& window, const ConfettiSystem& sys);

// VICTORY SHOCKWAVE (fragment shader)

// Ve vong song shockwave radial lan ra tu (cx, cy)
// No-op neu res.shockwaveOk = false.
void renderShockwave(sf::RenderWindow& window, GameResources& res,
                     float cx, float cy, float elapsed);

// VE MAN HINH KET THUC

// Ve man hinh game over (ai thang, so lieu, hoi tiep tuc?)
void renderGameOver(sf::RenderWindow& window, const GameState& state,
                    const GameResources& res, GameResult result,
                    int menuIndex, bool askingSave = false);

// VE SAVE / LOAD

// Ve man hinh save (danh sach file, nhap ten)
// V2 #30: scrollTop = index file dau tien hien thi, list co the cuon
void renderSaveScreen(sf::RenderWindow& window, const GameResources& res,
                      const std::vector<std::string>& saveList,
                      const std::string& inputName, int selectedIndex,
                      int scrollTop);

// Ve man hinh load (danh sach file, chon file)
void renderLoadScreen(sf::RenderWindow& window, const GameResources& res,
                      const std::vector<std::string>& saveList,
                      int selectedIndex, int scrollTop);

// VE SETTINGS / HELP / ABOUT

void renderSettings(sf::RenderWindow& window, const GameResources& res,
                    int menuIndex, Language lang, int volume, bool sfxOn);

void renderHelp(sf::RenderWindow& window, const GameResources& res);

void renderAbout(sf::RenderWindow& window, const GameResources& res);

// TIEN ICH VE

// Chuyen toa do ban co (row, col) sang pixel (x, y) tren man hinh
// boardOffsetX, boardOffsetY: goc tren trai cua ban co
sf::Vector2f renderBoardToPixel(int row, int col);

// Chuyen pixel (x, y) sang toa do ban co (row, col)
// Tra ve true neu toa do hop le, false neu click ngoai ban co
bool renderPixelToBoard(float x, float y, int& outRow, int& outCol);

// Ve text can giua tai (centerX, centerY)
// Optional: them outline (outlineColor, outlineThickness)
void renderTextCentered(sf::RenderWindow& window, const sf::Font& font,
                        const std::string& text, int fontSize,
                        float centerX, float centerY, sf::Color color,
                        sf::Color outlineColor = sf::Color::Transparent,
                        float outlineThickness = 0.f);

#endif // RENDER_H
