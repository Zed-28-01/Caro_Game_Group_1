#pragma once
#ifndef MENU_H
#define MENU_H

#include "game_types.h"

// ============================================================
// GAME LOOP CHINH
// ============================================================

// Vong lap chinh cua game - xu ly moi thu
// Goi tu main(), chay cho den khi nguoi choi thoat
void gameRun(sf::RenderWindow& window, GameResources& res);

// ============================================================
// XU LY TUNG MAN HINH
// ============================================================

// Xu ly Main Menu: New Game, Load, Settings, Help, About, Exit
// Tra ve GameScreen tiep theo can chuyen sang
GameScreen handleMainMenu(sf::RenderWindow& window, GameResources& res,
                          GameState& state);

// Xu ly chon che do choi: PvP / PvC
GameScreen handleModeSelect(sf::RenderWindow& window, GameResources& res,
                            GameState& state);

// Xu ly chon do kho AI (chi khi PvC)
GameScreen handleDifficultySelect(sf::RenderWindow& window, GameResources& res,
                                  GameState& state);

// Xu ly chon kieu choi: Basic / Speed
GameScreen handleStyleSelect(sf::RenderWindow& window, GameResources& res,
                             GameState& state);

// Xu ly nhap ten nguoi choi
GameScreen handleInputNames(sf::RenderWindow& window, GameResources& res,
                            GameState& state);

// ============================================================
// XU LY GAMEPLAY
// ============================================================

// Vong lap gameplay chinh (choi game)
// Xu ly: input (WASD/Enter/Z/ESC/H), bot turn, timer, win/draw check
// Tra ve GameScreen tiep theo khi game ket thuc hoac ESC
GameScreen handleGameplay(sf::RenderWindow& window, GameResources& res,
                          GameState& state);

// Xu ly menu tam dung (ESC trong game)
// Tra ve: GameScreen (quay lai game, ve menu chinh, save, ...)
GameScreen handlePauseMenu(sf::RenderWindow& window, GameResources& res,
                           GameState& state);

// Xu ly man hinh ket thuc van
// Tra ve: GameScreen (choi tiep, ve menu, ...)
GameScreen handleGameOver(sf::RenderWindow& window, GameResources& res,
                          GameState& state, GameResult result);

// ============================================================
// XU LY SAVE / LOAD / SETTINGS
// ============================================================

GameScreen handleSaveScreen(sf::RenderWindow& window, GameResources& res,
                            GameState& state);

GameScreen handleLoadScreen(sf::RenderWindow& window, GameResources& res,
                            GameState& state);

GameScreen handleSettings(sf::RenderWindow& window, GameResources& res);

GameScreen handleHelp(sf::RenderWindow& window, GameResources& res);

GameScreen handleAbout(sf::RenderWindow& window, GameResources& res);

#endif // MENU_H
