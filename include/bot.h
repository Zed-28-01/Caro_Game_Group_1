#pragma once
#ifndef BOT_H
#define BOT_H

#include "game_types.h"

// ============================================================
// HAM CHINH - GOI TU GAME LOOP
// ============================================================

// Bot tim nuoc di tot nhat dua tren do kho hien tai
// Ket qua tra ve qua outRow, outCol
void botGetMove(const GameState& state, BotDifficulty difficulty,
                int& outRow, int& outCol);

// ============================================================
// CAC MUC DO KHO
// ============================================================

// EASY: Random + chan co ban khi doi thu sap thang
// - Uu tien: chan chuoi 4, chan chuoi 3 mo 2 dau
// - Con lai: chon ngau nhien trong cac o lan can quan da danh
void botEasyMove(const GameState& state, int& outRow, int& outCol);

// MEDIUM: Heuristic scoring - danh gia diem cho moi o trong
// - Tinh diem tan cong + phong thu cho moi o
// - Chon o co diem cao nhat
void botMediumMove(const GameState& state, int& outRow, int& outCol);

// HARD: Minimax depth 3 voi Alpha-Beta Pruning
// - Chi duyet cac o lan can (ban kinh 2 tu quan da danh)
// - Ham evaluate nang cao: phan biet chuoi mo/dong
void botHardMove(const GameState& state, int& outRow, int& outCol);

// EXPERT: Minimax depth 4 + Move Ordering + Threat Detection
// - Uu tien danh gia cac nuoc "nong" truoc (move ordering)
// - Nhan dien fork (2 chuoi 3 mo dong thoi)
void botExpertMove(const GameState& state, int& outRow, int& outCol);

// ============================================================
// HINT SYSTEM (GOI Y CHO NGUOI CHOI - CHI TRONG PvC)
// ============================================================

// Goi y nuoc di tot nhat cho nguoi choi
// Su dung AI muc Medium de goi y (khong qua manh, khong qua yeu)
void botGetHint(const GameState& state, int& outRow, int& outCol);

// ============================================================
// HAM DANH GIA (EVALUATION)
// ============================================================

// Danh gia diem cho 1 o trong tai (row, col)
// Xet ca 4 huong: ngang, doc, cheo chinh, cheo phu
// Tra ve diem so (cang cao cang tot cho botPlayer)
int botEvaluatePosition(const Cell board[BOARD_SIZE][BOARD_SIZE],
                        int row, int col, int botPlayer);

// Danh gia toan bo ban co cho bot
// Tra ve diem duong = bot co loi, diem am = doi thu co loi
int botEvaluateBoard(const Cell board[BOARD_SIZE][BOARD_SIZE], int botPlayer);

// ============================================================
// HAM DANH GIA CHUOI (PATTERN SCORING)
// ============================================================

// Dem so quan lien tiep tu (row, col) theo huong (dRow, dCol)
// Dung lai khi gap o trong hoac quan doi thu
int botCountConsecutive(const Cell board[BOARD_SIZE][BOARD_SIZE],
                        int row, int col, int dRow, int dCol, int player);

// Danh gia 1 chuoi quan co:
// - Xet so quan lien tiep
// - Xet 2 dau cua chuoi (mo hay bi chan)
// - Tra ve diem dua tren bang diem:
//   5+ lien:       1000000 (thang)
//   4 mo 2 dau:    100000
//   4 mo 1 dau:    5000
//   3 mo 2 dau:    5000
//   3 mo 1 dau:    500
//   2 mo 2 dau:    500
//   2 mo 1 dau:    50
int botScoreLine(const Cell board[BOARD_SIZE][BOARD_SIZE],
                 int row, int col, int dRow, int dCol, int player);

// ============================================================
// MINIMAX + ALPHA-BETA
// ============================================================

// Thuật toán Minimax với cắt tỉa Alpha-Beta
// depth: do sau hien tai
// maxDepth: do sau toi da (thay doi theo do kho)
// alpha, beta: gia tri cat tia
// isMaximizing: true = luot cua bot, false = luot cua doi thu
int botMinimax(Cell board[BOARD_SIZE][BOARD_SIZE],
               int depth, int maxDepth,
               int alpha, int beta,
               bool isMaximizing, int botPlayer);

// ============================================================
// TIEN ICH BOT
// ============================================================

// Lay danh sach cac o nen xet (lan can cac quan da danh, ban kinh radius)
// Tra ve so luong o, dien vao mang candidates
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
