#include "utils.h"

const int DIRS[4][2] = {
    {0, 1},
    {1, 0},
    {1, 1},
    {1, -1}
};

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
