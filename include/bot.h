#pragma once
#ifndef BOT_H
#define BOT_H

#include "game_types.h"

// HAM CHINH - GOI TU GAME LOOP

// Bot tim nuoc di tot nhat dua tren do kho hien tai, luu vao outRow, outCol
void botGetMove(const GameState& state, BotDifficulty difficulty,
                int& outRow, int& outCol);

// CAC MUC DO KHO

// EASY: Random + uu tien chan co ban khi doi thu sap thang
void botEasyMove(const GameState& state, int& outRow, int& outCol);

// MEDIUM: Heuristic scoring - tinh diem cong/thu cho moi o trong
void botMediumMove(const GameState& state, int& outRow, int& outCol);

// HARD: Minimax depth 3 voi Alpha-Beta Pruning (duyet ban kinh 2)
void botHardMove(const GameState& state, int& outRow, int& outCol);

// EXPERT: Minimax depth 4 + Move Ordering + Threat Detection (nhan dien fork)
void botExpertMove(const GameState& state, int& outRow, int& outCol);

// HINT SYSTEM (GOI Y CHO NGUOI CHOI - CHI TRONG PvC)

// Goi y nuoc di bang AI Medium (khong qua manh, khong qua yeu)
void botGetHint(const GameState& state, int& outRow, int& outCol);

// HAM DANH GIA (EVALUATION)

// Danh gia tong diem 4 huong cho 1 o trong (cang cao cang tot cho bot)
int botEvaluatePosition(const Cell board[BOARD_SIZE][BOARD_SIZE],
                        int row, int col, int botPlayer);

// Danh gia toan bo ban co cho bot
// Tra ve diem duong = bot co loi, diem am = doi thu co loi
int botEvaluateBoard(const Cell board[BOARD_SIZE][BOARD_SIZE], int botPlayer);

// HAM DANH GIA CHUOI (PATTERN SCORING)
// Luu y: ham countConsecutive da duoc chuyen sang utils.h

// Danh gia 1 chuoi quan co dua tren so quan lien tiep va do mo/dong cua 2 dau
// Tra ve diem so tuong ung (vd: 5+ lien = 1000000, 4 mo 2 dau = 100000, ...)
int botScoreLine(const Cell board[BOARD_SIZE][BOARD_SIZE],
                 int row, int col, int dRow, int dCol, int player);

// MINIMAX + ALPHA-BETA

// Thuat toan Minimax voi cat tia Alpha-Beta (depth/maxDepth/alpha/beta)
// isMaximizing: true=luot bot, false=luot doi thu
int botMinimax(Cell board[BOARD_SIZE][BOARD_SIZE],
               int depth, int maxDepth,
               int alpha, int beta,
               bool isMaximizing, int botPlayer);

// TIEN ICH BOT

// Lay danh sach cac o nen xet (lan can quan da danh ban kinh radius) vao mang candidates
// Tra ve so luong o hop le
int botGetCandidates(const Cell board[BOARD_SIZE][BOARD_SIZE],
                     int candidates[][2], int radius);

// Sap xep candidates theo diem danh gia (move ordering cho Expert)
void botSortCandidates(const Cell board[BOARD_SIZE][BOARD_SIZE],
                       int candidates[][2], int count, int botPlayer);

// Kiem tra co nuoc thang ngay lap tuc khong
// Tra ve true va dien vao outRow, outCol neu co
bool botCheckImmediateWin(const Cell board[BOARD_SIZE][BOARD_SIZE],
                          int player, int& outRow, int& outCol);

// Kiem tra co can chan khong (doi thu sap thang)
bool botCheckImmediateBlock(const Cell board[BOARD_SIZE][BOARD_SIZE],
                            int botPlayer, int& outRow, int& outCol);

#endif // BOT_H
