#include "board.h"
#include "utils.h"

bool boardIsEmpty(const GameState &state, int row, int col) {
  return state.board[row][col].value == 0;
}

void boardInit(GameState &state) {
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      state.board[r][c].value = 0;
    }
  }

  state.moveCount = 0;
  state.cursorRow = BOARD_SIZE / 2;
  state.cursorCol = BOARD_SIZE / 2;
  state.isPlayer1Turn = true;
  state.firstPlayerOfRound = 1;

  state.player1.moves = 0;
  state.player2.moves = 0;
}

void boardResetRound(GameState &state, int loserPlayerId) {
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      state.board[r][c].value = 0;
    }
  }

  state.moveCount = 0;
  state.cursorRow = BOARD_SIZE / 2;
  state.cursorCol = BOARD_SIZE / 2;

  if (loserPlayerId == 1 || loserPlayerId == 2) {
    state.firstPlayerOfRound = loserPlayerId;
  }

  state.isPlayer1Turn = (state.firstPlayerOfRound == 1);

  state.player1.moves = 0;
  state.player2.moves = 0;
}

void boardResetAll(GameState &state) {
  boardInit(state);
  state.player1.totalWins = 0;
  state.player2.totalWins = 0;
}

bool boardPlacePiece(GameState &state, int row, int col) {
  if (!inBounds(row, col))
    return false;
  if (!boardIsEmpty(state, row, col))
    return false;

  int value = state.isPlayer1Turn ? CELL_P1 : CELL_P2;
  state.board[row][col].value = value;

  state.moveHistory[state.moveCount] = {row, col, value};
  state.moveCount++;

  if (state.isPlayer1Turn) {
    state.player1.moves++;
  } else {
    state.player2.moves++;
  }
  return true;
}

void boardSwitchTurn(GameState &state) {
  state.isPlayer1Turn = !state.isPlayer1Turn;
}

void boardMoveCursor(GameState &state, int dRow, int dCol) {
  int newRow = state.cursorRow + dRow;
  int newCol = state.cursorCol + dCol;

  if (inBounds(newRow, newCol)) {
    state.cursorRow = newRow;
    state.cursorCol = newCol;
  }
}

bool boardCheckWin(const GameState &state, int row, int col, WinLine &winline) {
  int value = state.board[row][col].value;
  if (value == 0)
    return false;

  for (int d = 0; d < 4; d++) {
    int dRow = DIRS[d][0];
    int dCol = DIRS[d][1];

    int count = 1 +
                countConsecutive(state.board, row, col, dRow, dCol, value) +
                countConsecutive(state.board, row, col, -dRow, -dCol, value);
    if (count >= WIN_COUNT) {
      winline.count = 0;

      int r = row, c = col;
      while (inBounds(r - dRow, c - dCol) &&
             state.board[r - dRow][c - dCol].value == value) {
        r -= dRow;
        c -= dCol;
      }

      for (int i = 0; i < WIN_COUNT; i++) {
        winline.positions[i][0] = r;
        winline.positions[i][1] = c;
        winline.count++;
        r += dRow;
        c += dCol;
      }
      return true;
    }
  }
  return false;
}

bool boardCheckDraw(const GameState &state) {
  return state.moveCount == BOARD_SIZE * BOARD_SIZE;
}

GameResult boardEvaluateResult(const GameState &state, int lastRow, int lastCol,
                               WinLine &winline) {
  if (boardCheckWin(state, lastRow, lastCol, winline)) {
    if (state.board[lastRow][lastCol].value == CELL_P1)
      return RESULT_PLAYER1_WIN;
    else
      return RESULT_PLAYER2_WIN;
  }

  if (boardCheckDraw(state))
    return RESULT_DRAW;
  return RESULT_NONE;
}

int boardUndo(GameState &state) {
  if (state.moveCount == 0)
    return 0;

  int undoCount = 0;
  int timesToUndo = (state.mode == MODE_PVC) ? 2 : 1;
  for (int i = 0; i < timesToUndo; i++) {
    if (state.moveCount == 0)
      break;

    state.moveCount--;
    Move lastMove = state.moveHistory[state.moveCount];

    state.board[lastMove.row][lastMove.col].value = 0;

    if (lastMove.player == CELL_P1)
      state.player1.moves--;
    else
      state.player2.moves--;

    state.cursorRow = lastMove.row;
    state.cursorCol = lastMove.col;
    undoCount++;
  }

  if (undoCount % 2 != 0) {
    state.isPlayer1Turn = !state.isPlayer1Turn;
  }

  return undoCount;
}
