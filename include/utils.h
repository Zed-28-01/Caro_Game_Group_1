#pragma once
#ifndef UTILS_H
#define UTILS_H

#include "game_types.h"

// ============================================================
// TIEN ICH DUNG CHUNG (Shared Utilities)
// ============================================================
// File nay tap trung cac helper duoc dung chung boi nhieu module:
// - bot.cpp: tinh diem, dem chuoi quan
// - board.cpp: kiem tra thang, validate toa do
// Muc dich: DRY (Don't Repeat Yourself) - tranh lap code

// ============================================================
// HANG SO HUONG (4 huong cua ban co)
// ============================================================
// Mang 4 huong: ngang, doc, cheo chinh, cheo phu
// Khai bao extern de chia se mot bang giua nhieu file
//
// Cach dung: DIRS[i][0] = dRow, DIRS[i][1] = dCol
//   {0, 1}  - ngang (sang phai)
//   {1, 0}  - doc (xuong duoi)
//   {1, 1}  - cheo len-phai (xuong + sang phai)
//   {1, -1} - cheo len-trai (xuong + sang trai)
extern const int DIRS[4][2];

// ============================================================
// KIEM TRA TOA DO HOP LE
// ============================================================
// Kiem tra (r, c) co nam trong ban co 15x15 khong
// Inline de tang toc (goi rat nhieu trong loop count, win check)
inline bool inBounds(int r, int c) {
  return r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE;
}

// ============================================================
// DEM QUAN LIEN TIEP THEO 1 HUONG
// ============================================================
// Tu vi tri (row, col), dem so quan cua "player" lien tiep theo huong (dRow,
// dCol). KHONG dem o (row, col) - chi dem nhung o sau no theo huong.
//
// Vi du: board[5][5] = X, board[5][6] = X, board[5][7] = X
//        countConsecutive(board, 5, 5, 0, 1, X) = 2  (chi dem [5][6] va [5][7])
//
// Dung lai khi: ra ngoai bien HOAC gap o trong HOAC gap quan khac
//
// Dung cho: boardCheckWin (board.cpp) + botScoreLine (bot.cpp) +
// botCheckImmediateWin (bot.cpp)
int countConsecutive(const Cell board[BOARD_SIZE][BOARD_SIZE], int row, int col,
                     int dRow, int dCol, int player);

#endif // UTILS_H
