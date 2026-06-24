#pragma once
#ifndef UTILS_H
#define UTILS_H

#include "game_types.h"

// TIEN ICH DUNG CHUNG (Shared Utilities)
// File nay tap trung cac helper duoc dung chung boi nhieu module:
// - bot.cpp: tinh diem, dem chuoi quan
// - board.cpp: kiem tra thang, validate toa do
// Muc dich: DRY (Don't Repeat Yourself) - tranh lap code

// HANG SO HUONG (4 huong cua ban co)
// Mang 4 huong: ngang, doc, cheo chinh, cheo phu
// Khai bao extern de chia se mot bang giua nhieu file
//
// Cach dung: DIRS[i][0] = dRow, DIRS[i][1] = dCol
//   {0, 1}  - ngang (sang phai)
//   {1, 0}  - doc (xuong duoi)
//   {1, 1}  - cheo len-phai (xuong + sang phai)
//   {1, -1} - cheo len-trai (xuong + sang trai)
extern const int DIRS[4][2];

// KIEM TRA TOA DO HOP LE
// Kiem tra (r, c) co nam trong ban co 15x15 khong
// Inline de tang toc (goi rat nhieu trong loop count, win check)
inline bool inBounds(int r, int c) {
  return r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE;
}

// DEM QUAN LIEN TIEP THEO 1 HUONG
// Dem so quan cua player lien tiep theo 1 huong tu (row, col) - KHONG dem chinh o (row, col).
// Dung lai khi ra khoi ban co, gap o trong, hoac quan khac.
int countConsecutive(const Cell board[BOARD_SIZE][BOARD_SIZE], int row, int col,
                     int dRow, int dCol, int player);

#endif // UTILS_H
