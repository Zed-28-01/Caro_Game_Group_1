#include "board.h"
#include "utils.h" // inBounds, DIRS, countConsecutive (dung chung voi bot.cpp)

// ============================================================
// TIEN ICH
// ============================================================
// Luu y: inBounds (cu) da duoc chuyen sang utils.h thanh ham inBounds
// (chia se voi bot.cpp - DRY principle)

// check cell empty?
bool boardIsEmpty(const GameState &state, int row, int col) {
  return state.board[row][col].value == 0;
}

int boardCountEmpty(const GameState &state) {
  int count = 0;
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      if (state.board[r][c].value == 0)
        count++;
    }
  }
  return count;
}

// ============================================================
// KHOI TAO & RESET
// ============================================================

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
  state.firstPlayerOfRound = 1; // V2: van dau tien P1 di truoc, danh X

  state.player1.moves = 0;
  state.player2.moves = 0;
}

// V2: loserPlayerId = 1 (P1 thua) hoac 2 (P2 thua) hoac 0 (hoa - giu nguyen)
// Nguoi thua se di truoc van sau va luon danh ky hieu X
void boardResetRound(GameState &state, int loserPlayerId) {
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      state.board[r][c].value = 0;
    }
  }

  state.moveCount = 0;
  state.cursorRow = BOARD_SIZE / 2;
  state.cursorCol = BOARD_SIZE / 2;

  // V2: Nguoi thua di truoc van sau (luat co vua)
  // - Neu hoa (loserPlayerId = 0): giu nguyen firstPlayerOfRound
  // - Neu co nguoi thua: firstPlayerOfRound = nguoi thua
  if (loserPlayerId == 1 || loserPlayerId == 2) {
    state.firstPlayerOfRound = loserPlayerId;
  }

  // isPlayer1Turn duoc set theo firstPlayerOfRound (P1 di neu firstPlayer=1)
  state.isPlayer1Turn = (state.firstPlayerOfRound == 1);

  state.player1.moves = 0;
  state.player2.moves = 0;
}

void boardResetAll(GameState &state) {
  boardInit(state);
  state.player1.totalWins = 0;
  state.player2.totalWins = 0;
  // Không cần khởi tạo lại moveHistory vì moveCount đã được khởi tạo lại từ 0
  // nên sẽ tự ghi đè khi ta vào ván mới GameMode, GameStyle, Bot Difficulty,
  // player.name được quản lý ở menu.cpp TimerState được quản lý ở timer.cpp
}

// ============================================================
// DANH DAU & DI CHUYEN
// ============================================================

// Đặt quân cờ và cập nhật lịch sử di chuyển
bool boardPlacePiece(GameState &state, int row, int col) {
  if (!inBounds(row, col))
    return false;
  if (!boardIsEmpty(state, row, col))
    return false;

  int value = state.isPlayer1Turn ? -1 : 1;
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

// Chuyen luot choi
void boardSwitchTurn(GameState &state) {
  state.isPlayer1Turn = !state.isPlayer1Turn;
}

// Di chuyen cursor (kiem tra bien)
void boardMoveCursor(GameState &state, int dRow, int dCol) {
  int newRow = state.cursorRow + dRow;
  int newCol = state.cursorCol + dCol;

  if (inBounds(newRow, newCol)) {
    state.cursorRow = newRow;
    state.cursorCol = newCol;
  }
}

// ============================================================
// KIEM TRA THANG / HOA
// ============================================================
// Luu y: countDirection (cu) da duoc chuyen sang utils.h thanh ham
// countConsecutive (chia se voi bot.cpp - DRY)

bool boardCheckWin(const GameState &state, int row, int col, WinLine &winline) {
  int value = state.board[row][col].value;
  if (value == 0)
    return false;

  // Su dung DIRS tu utils.h thay vi khai bao inline
  for (int d = 0; d < 4; d++) {
    int dRow = DIRS[d][0];
    int dCol = DIRS[d][1];

    int count = 1 +
                countConsecutive(state.board, row, col, dRow, dCol, value) +
                countConsecutive(state.board, row, col, -dRow, -dCol, value);
    if (count >= WIN_COUNT) {
      winline.count = 0;

      // Lùi về điểm xuất phát
      int r = row, c = col;
      while (inBounds(r - dRow, c - dCol) &&
             state.board[r - dRow][c - dCol].value == value) {
        r -= dRow;
        c -= dCol;
      }

      // lấy từ điểm đầu đúng 5 điểm
      // thử test một dải thắng 6, 7, 8, 9 ô là sẽ hiểu logic vì sao lấy 5 ô cụ
      // thể nào đó
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
  // Toi uu O(1): moveCount luon dong bo voi so o da dat
  // (boardPlace++ / boardUndo-- / boardResetRound=0)
  // Ban day <=> moveCount == BOARD_SIZE * BOARD_SIZE = 225
  return state.moveCount == BOARD_SIZE * BOARD_SIZE;
}

GameResult boardEvaluateResult(const GameState &state, int lastRow, int lastCol,
                               WinLine &winline) {
  if (boardCheckWin(state, lastRow, lastCol, winline)) {
    if (state.board[lastRow][lastCol].value == -1)
      return RESULT_PLAYER1_WIN;
    else
      return RESULT_PLAYER2_WIN;
  }

  if (boardCheckDraw(state))
    return RESULT_DRAW;
  return RESULT_NONE;
}

// ============================================================
// UNDO
// ============================================================

// PVP : Huy 1 nuoc, PVC: Huy 2 nuoc
// Tra ve: so nuoc da huy
int boardUndo(GameState &state) {
  if (state.moveCount == 0)
    return 0;

  int undoCount = 0;
  int timesToUndo = (state.mode == MODE_PVC) ? 2 : 1;
  for (int i = 0; i < timesToUndo; i++) {
    if (state.moveCount == 0)
      break; // trường hợp máy chưa kịp đánh trả nhưng đã undo -> cần dòng này

    state.moveCount--;
    Move lastMove = state.moveHistory[state.moveCount];

    state.board[lastMove.row][lastMove.col].value = 0;

    if (lastMove.player == -1)
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
