#include "utils.h"

// ============================================================
// DEFINITION CHO HANG SO DIRS
// ============================================================
// Day la noi DUY NHAT dinh nghia bang 4 huong
// (header chi khai bao extern de tranh multiple definition)
const int DIRS[4][2] = {
    {0, 1},  // ngang
    {1, 0},  // doc
    {1, 1},  // cheo len-phai
    {1, -1}  // cheo len-trai
};

// ============================================================
// DEM QUAN LIEN TIEP
// ============================================================
// Logic giong het ham cu (botCountConsecutive trong bot.cpp +
// countDirection trong board.cpp) - chi khac la duoc dung chung
int countConsecutive(const Cell board[BOARD_SIZE][BOARD_SIZE], int row, int col,
                     int dRow, int dCol, int player) {
  int count = 0;
  int r = row + dRow;
  int c = col + dCol;
  while (inBounds(r, c) && board[r][c].value == player) {
    count++;
    r += dRow;
    c += dCol;
  }
  return count;
}
