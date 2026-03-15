#pragma once
#ifndef RENDER_H
#define RENDER_H

#include "game_types.h"

// ============================================================
// KHOI TAO RESOURCES
// ============================================================

// Load tat ca font, texture, sound tu thu muc assets/
// Tra ve true neu thanh cong, false neu thieu file
bool renderLoadResources(GameResources& res);

// ============================================================
// VE MAN HINH CHINH (GOI TRONG GAME LOOP)
// ============================================================

// Ve man hinh tuong ung voi trang thai hien tai
// Ham nay la "dispatcher" - goi cac ham ve cu the ben duoi
void renderScreen(sf::RenderWindow& window, const GameState& state,
                  const GameResources& res, GameScreen screen);

// ============================================================
// VE MENU
// ============================================================

// Ve Main Menu (New Game, Load, Settings, Help, About, Exit)
// menuIndex: muc dang duoc chon (highlight)
void renderMainMenu(sf::RenderWindow& window, const GameResources& res,
                    int menuIndex);

// Ve menu chon che do (PvP / PvC)
void renderModeSelect(sf::RenderWindow& window, const GameResources& res,
                      int menuIndex);

// Ve menu chon do kho AI
void renderDifficultySelect(sf::RenderWindow& window, const GameResources& res,
                            int menuIndex);

// Ve menu chon kieu choi (Basic / Speed)
void renderStyleSelect(sf::RenderWindow& window, const GameResources& res,
                       int menuIndex);

// Ve menu tam dung (ESC in-game)
void renderPauseMenu(sf::RenderWindow& window, const GameResources& res,
                     int menuIndex);

// ============================================================
// VE MAN HINH NHAP TEN
// ============================================================

// Ve form nhap ten nguoi choi
// inputText: chuoi dang nhap hien tai
// isPlayer1: dang nhap cho Player 1 hay 2
void renderInputNames(sf::RenderWindow& window, const GameResources& res,
                      const std::string& name1, const std::string& name2,
                      bool isEditingPlayer1, bool showError);

// ============================================================
// VE BAN CO & GAMEPLAY
// ============================================================

// Ve toan bo man hinh choi game:
// - Background
// - Ban co (grid lines)
// - Cac quan X, O da danh
// - Highlight o dang chon (cursor)
// - Panel thong tin 2 nguoi choi
// - Timer (neu STYLE_SPEED)
// - Hint highlight (neu dang hien goi y)
void renderGameplay(sf::RenderWindow& window, const GameState& state,
                    const GameResources& res, const WinLine* winLine,
                    int hintRow, int hintCol, bool showHint);

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

// ============================================================
// VE PANEL THONG TIN NGUOI CHOI
// ============================================================

// Ve panel ben phai man hinh:
// - Ten nguoi choi 1 & 2
// - So nuoc di
// - So van thang
// - Highlight nguoi dang di
void renderPlayerPanel(sf::RenderWindow& window, const GameState& state,
                       const GameResources& res);

// ============================================================
// VE TIMER / PROGRESS BAR
// ============================================================

// Ve thanh progress bar countdown cho luot hien tai
// percentage: 0.0 (het gio) -> 1.0 (con day du)
void renderTurnTimer(sf::RenderWindow& window, const GameResources& res,
                     float percentage);

// Ve thoi gian con lai cua van dau
void renderGameTimer(sf::RenderWindow& window, const GameResources& res,
                     float secondsLeft);

// ============================================================
// VE HIEU UNG
// ============================================================

// Hieu ung khi dat quan (animation ngan)
void renderPlaceEffect(sf::RenderWindow& window, int row, int col,
                       int player, float progress);

// Hieu ung khi thang (nhap nhay, phan hoa, ...)
void renderWinEffect(sf::RenderWindow& window, const WinLine& winLine,
                     const GameResources& res, float progress);

// Hieu ung khi hoa
void renderDrawEffect(sf::RenderWindow& window, const GameResources& res,
                      float progress);

// ============================================================
// VE MAN HINH KET THUC
// ============================================================

// Ve man hinh game over (ai thang, so lieu, hoi tiep tuc?)
void renderGameOver(sf::RenderWindow& window, const GameState& state,
                    const GameResources& res, GameResult result,
                    int menuIndex);

// ============================================================
// VE SAVE / LOAD
// ============================================================

// Ve man hinh save (danh sach file, nhap ten)
void renderSaveScreen(sf::RenderWindow& window, const GameResources& res,
                      const std::string saveList[], int saveCount,
                      const std::string& inputName, int selectedIndex);

// Ve man hinh load (danh sach file, chon file)
void renderLoadScreen(sf::RenderWindow& window, const GameResources& res,
                      const std::string saveList[], int saveCount,
                      int selectedIndex);

// ============================================================
// VE SETTINGS / HELP / ABOUT
// ============================================================

void renderSettings(sf::RenderWindow& window, const GameResources& res,
                    int menuIndex, Language lang, int volume, bool sfxOn);

void renderHelp(sf::RenderWindow& window, const GameResources& res);

void renderAbout(sf::RenderWindow& window, const GameResources& res);

// ============================================================
// TIEN ICH VE
// ============================================================

// Chuyen toa do ban co (row, col) sang pixel (x, y) tren man hinh
// boardOffsetX, boardOffsetY: goc tren trai cua ban co
sf::Vector2f renderBoardToPixel(int row, int col);

// Chuyen pixel (x, y) sang toa do ban co (row, col)
// Tra ve true neu toa do hop le, false neu click ngoai ban co
bool renderPixelToBoard(float x, float y, int& outRow, int& outCol);

// Ve 1 text can giua tai vi tri (centerX, centerY)
void renderTextCentered(sf::RenderWindow& window, const sf::Font& font,
                        const std::string& text, int fontSize,
                        float centerX, float centerY, sf::Color color);

#endif // RENDER_H
