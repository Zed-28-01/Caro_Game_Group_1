#pragma once
#ifndef BOARD_H
#define BOARD_H

#include "game_types.h"

// ============================================================
// KHOI TAO & RESET
// ============================================================

// Khoi tao ban co moi - dat tat ca o ve 0
void boardInit(GameState& state);

// Reset van moi trong cung 1 tran (giu ten, doi luot di truoc)
void boardResetRound(GameState& state);

// Reset hoan toan (xoa ca thong ke)
void boardResetAll(GameState& state);

// ============================================================
// DANH DAU & DI CHUYEN
// ============================================================

// Dat quan tai vi tri (row, col) cho nguoi choi hien tai
// Tra ve: true neu dat thanh cong, false neu o da co quan
bool boardPlacePiece(GameState& state, int row, int col);

// Chuyen luot choi
void boardSwitchTurn(GameState& state);

// Di chuyen cursor (kiem tra bien)
void boardMoveCursor(GameState& state, int dRow, int dCol);

// ============================================================
// KIEM TRA THANG / HOA
// ============================================================

// Kiem tra nguoi choi vua danh tai (row, col) co thang khong
// Neu thang: dien thong tin vao winLine va tra ve true
bool boardCheckWin(const GameState& state, int row, int col, WinLine& winLine);

// Kiem tra hoa (ban co day)
bool boardCheckDraw(const GameState& state);

// Kiem tra ket qua sau moi nuoc di
// Tra ve GameResult tuong ung
GameResult boardEvaluateResult(const GameState& state, int lastRow, int lastCol, WinLine& winLine);

// ============================================================
// UNDO
// ============================================================

// Huy nuoc di cuoi cung
// PvP: huy 1 nuoc, PvC: huy 2 nuoc (nguoi + may)
// Tra ve: so nuoc da huy (0 neu khong co gi de huy)
int boardUndo(GameState& state);

// ============================================================
// TIEN ICH
// ============================================================

// Kiem tra o (row, col) co trong khong
bool boardIsEmpty(const GameState& state, int row, int col);

// Kiem tra toa do hop le
bool boardIsValid(int row, int col);

// Dem so o trong con lai
int boardCountEmpty(const GameState& state);

#endif // BOARD_H
