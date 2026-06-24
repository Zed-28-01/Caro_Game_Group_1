#include "bot.h"
#include "utils.h"
#include <climits>
#include <cstdlib>
#include <ctime>

static const int SCORE_WIN = 1000000;
static const int SCORE_FOUR_OPEN = 100000;
static const int SCORE_FOUR_HALF = 5000;
static const int SCORE_THREE_OPEN = 5000;
static const int SCORE_THREE_HALF = 500;
static const int SCORE_TWO_OPEN = 500;
static const int SCORE_TWO_HALF = 50;
static const int SCORE_ONE_OPEN = 10;
static const int SCORE_ONE_HALF = 5;

static const int BOT_PLAYER = CELL_P2;
static const int HUMAN_PLAYER = CELL_P1;

int botGetCandidates(const Cell board[BOARD_SIZE][BOARD_SIZE],
                     int candidates[][2], int radius) {

  int count = 0;
  bool hasAnyPiece = false;

  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      if (board[r][c].value != 0) {
        hasAnyPiece = true;
        break;
      }
    }
    if (hasAnyPiece)
      break;
  }

  if (!hasAnyPiece) {
    candidates[0][0] = BOARD_SIZE / 2;
    candidates[0][1] = BOARD_SIZE / 2;
    return 1;
  }

  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      if (board[r][c].value != 0)
        continue;

      bool nearPiece = false;
      for (int dr = -radius; dr <= radius && !nearPiece; dr++) {
        for (int dc = -radius; dc <= radius && !nearPiece; dc++) {
          if (dr == 0 && dc == 0)
            continue;
          int nr = r + dr, nc = c + dc;
          if (inBounds(nr, nc) && board[nr][nc].value != 0)
            nearPiece = true;
        }
      }
      if (nearPiece) {
        candidates[count][0] = r;
        candidates[count][1] = c;
        count++;
      }
    }
  }
  return count;
}

bool botCheckImmediateWin(const Cell board[BOARD_SIZE][BOARD_SIZE], int player,
                          int &outRow, int &outCol) {
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      if (board[r][c].value != 0)
        continue;
      for (int d = 0; d < 4; d++) {
        int total =
            1 +
            countConsecutive(board, r, c, DIRS[d][0], DIRS[d][1], player) +
            countConsecutive(board, r, c, -DIRS[d][0], -DIRS[d][1], player);
        if (total >= WIN_COUNT) {
          outRow = r;
          outCol = c;
          return true;
        }
      }
    }
  }
  return false;
}

bool botCheckImmediateBlock(const Cell board[BOARD_SIZE][BOARD_SIZE],
                            int botPlayer, int &outRow, int &outCol) {
  int opponent = -botPlayer;
  return botCheckImmediateWin(board, opponent, outRow, outCol);
}

int botScoreLine(const Cell board[BOARD_SIZE][BOARD_SIZE], int row, int col,
                 int dRow, int dCol, int player) {
  int forward = countConsecutive(board, row, col, dRow, dCol, player);
  int backward = countConsecutive(board, row, col, -dRow, -dCol, player);
  int total = forward + backward + 1;

  int openEnds = 0;
  int fr = row + (forward + 1) * dRow;
  int fc = col + (forward + 1) * dCol;
  if (inBounds(fr, fc) && board[fr][fc].value == 0)
    openEnds++;

  int br = row - (backward + 1) * dRow;
  int bc = col - (backward + 1) * dCol;
  if (inBounds(br, bc) && board[br][bc].value == 0)
    openEnds++;

  if (total >= 5)
    return SCORE_WIN;

  switch (total) {
  case 4:
    if (openEnds == 2)
      return SCORE_FOUR_OPEN;
    if (openEnds == 1)
      return SCORE_FOUR_HALF;
    return 0;
  case 3:
    if (openEnds == 2)
      return SCORE_THREE_OPEN;
    if (openEnds == 1)
      return SCORE_THREE_HALF;
    return 0;
  case 2:
    if (openEnds == 2)
      return SCORE_TWO_OPEN;
    if (openEnds == 1)
      return SCORE_TWO_HALF;
    return 0;
  case 1:
    if (openEnds == 2)
      return SCORE_ONE_OPEN;
    if (openEnds == 1)
      return SCORE_ONE_HALF;
    return 0;
  }
  return 0;
}

int botEvaluatePosition(const Cell board[BOARD_SIZE][BOARD_SIZE], int row,
                        int col, int botPlayer) {
  int attackScore = 0;
  int defenseScore = 0;
  int opponent = -botPlayer;

  for (int d = 0; d < 4; d++)
    attackScore +=
        botScoreLine(board, row, col, DIRS[d][0], DIRS[d][1], botPlayer);

  for (int d = 0; d < 4; d++)
    defenseScore +=
        botScoreLine(board, row, col, DIRS[d][0], DIRS[d][1], opponent);

  int centerDist = std::abs(row - BOARD_SIZE / 2) +
                   std::abs(col - BOARD_SIZE / 2);
  int centerBonus = (BOARD_SIZE - centerDist) * 2;

  return attackScore + (defenseScore * 9 / 10) + centerBonus;
}

int botEvaluateBoard(const Cell board[BOARD_SIZE][BOARD_SIZE], int botPlayer) {
  int score = 0;
  int opponent = -botPlayer;

  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      if (board[r][c].value == botPlayer) {
        for (int d = 0; d < 4; d++) {
          int pr = r - DIRS[d][0], pc = c - DIRS[d][1];
          if (inBounds(pr, pc) && board[pr][pc].value == botPlayer)
            continue;
          score +=
              botScoreLine(board, r, c, DIRS[d][0], DIRS[d][1], botPlayer);
        }
      } else if (board[r][c].value == opponent) {
        for (int d = 0; d < 4; d++) {
          int pr = r - DIRS[d][0], pc = c - DIRS[d][1];
          if (inBounds(pr, pc) && board[pr][pc].value == opponent)
            continue;
          score -= botScoreLine(board, r, c, DIRS[d][0], DIRS[d][1], opponent);
        }
      }
    }
  }
  return score;
}

void botEasyMove(const GameState &state, int &outRow, int &outCol) {
  if (botCheckImmediateBlock(state.board, BOT_PLAYER, outRow, outCol))
    return;
  if (botCheckImmediateWin(state.board, BOT_PLAYER, outRow, outCol))
    return;

  int candidates[BOARD_SIZE * BOARD_SIZE][2];
  int count = botGetCandidates(state.board, candidates, 2);

  if (count == 0) {
    outRow = BOARD_SIZE / 2;
    outCol = BOARD_SIZE / 2;
    return;
  }
  int idx = std::rand() % count;
  outRow = candidates[idx][0];
  outCol = candidates[idx][1];
}

void botMediumMove(const GameState &state, int &outRow, int &outCol) {
  if (botCheckImmediateWin(state.board, BOT_PLAYER, outRow, outCol))
    return;
  if (botCheckImmediateBlock(state.board, BOT_PLAYER, outRow, outCol))
    return;

  int candidates[BOARD_SIZE * BOARD_SIZE][2];
  int count = botGetCandidates(state.board, candidates, 2);

  if (count == 0) {
    outRow = BOARD_SIZE / 2;
    outCol = BOARD_SIZE / 2;
    return;
  }

  int bestScore = INT_MIN;
  outRow = candidates[0][0];
  outCol = candidates[0][1];

  for (int i = 0; i < count; i++) {
    int r = candidates[i][0], c = candidates[i][1];
    int score = botEvaluatePosition(state.board, r, c, BOT_PLAYER);
    if (score > bestScore) {
      bestScore = score;
      outRow = r;
      outCol = c;
    }
  }
}

static bool botIsWinningMove(const Cell board[BOARD_SIZE][BOARD_SIZE], int r, int c) {
  int player = board[r][c].value;
  if (player == 0) return false;
  for (int d = 0; d < 4; d++) {
    int total = 1
        + countConsecutive(board, r, c, DIRS[d][0], DIRS[d][1], player)
        + countConsecutive(board, r, c, -DIRS[d][0], -DIRS[d][1], player);
    if (total >= WIN_COUNT) return true;
  }
  return false;
}

int botMinimax(Cell board[BOARD_SIZE][BOARD_SIZE], int depth, int maxDepth,
               int alpha, int beta, bool isMaximizing, int botPlayer) {
  if (depth == maxDepth) {
    return botEvaluateBoard(board, botPlayer);
  }

  int candidates[BOARD_SIZE * BOARD_SIZE][2];
  int count = botGetCandidates(board, candidates, 1);

  if (count == 0)
    return botEvaluateBoard(board, botPlayer);

  int currentPlayer = isMaximizing ? botPlayer : -botPlayer;

  if (isMaximizing) {
    int maxEval = INT_MIN;
    for (int i = 0; i < count; i++) {
      int r = candidates[i][0], c = candidates[i][1];
      board[r][c].value = currentPlayer;
      int eval = botIsWinningMove(board, r, c)
          ? (SCORE_WIN - depth)
          : botMinimax(board, depth + 1, maxDepth, alpha, beta, false, botPlayer);
      board[r][c].value = 0;

      if (eval > maxEval)
        maxEval = eval;
      if (eval > alpha)
        alpha = eval;
      if (beta <= alpha)
        break;
    }
    return maxEval;
  } else {
    int minEval = INT_MAX;
    for (int i = 0; i < count; i++) {
      int r = candidates[i][0], c = candidates[i][1];
      board[r][c].value = currentPlayer;
      int eval = botIsWinningMove(board, r, c)
          ? -(SCORE_WIN - depth)
          : botMinimax(board, depth + 1, maxDepth, alpha, beta, true, botPlayer);
      board[r][c].value = 0;

      if (eval < minEval)
        minEval = eval;
      if (eval < beta)
        beta = eval;
      if (beta <= alpha)
        break;
    }
    return minEval;
  }
}

void botHardMove(const GameState &state, int &outRow, int &outCol) {
  if (botCheckImmediateWin(state.board, BOT_PLAYER, outRow, outCol))
    return;
  if (botCheckImmediateBlock(state.board, BOT_PLAYER, outRow, outCol))
    return;

  Cell board[BOARD_SIZE][BOARD_SIZE];
  for (int r = 0; r < BOARD_SIZE; r++)
    for (int c = 0; c < BOARD_SIZE; c++)
      board[r][c] = state.board[r][c];

  int candidates[BOARD_SIZE * BOARD_SIZE][2];
  int count = botGetCandidates(board, candidates, 2);

  if (count == 0) {
    outRow = BOARD_SIZE / 2;
    outCol = BOARD_SIZE / 2;
    return;
  }

  int bestScore = INT_MIN;
  outRow = candidates[0][0];
  outCol = candidates[0][1];

  for (int i = 0; i < count; i++) {
    int r = candidates[i][0], c = candidates[i][1];
    board[r][c].value = BOT_PLAYER;
    int score = botMinimax(board, 1, 3, INT_MIN, INT_MAX, false, BOT_PLAYER);
    board[r][c].value = 0;

    int centerDist =
        std::abs(r - BOARD_SIZE / 2) + std::abs(c - BOARD_SIZE / 2);
    score += (BOARD_SIZE - centerDist) * 2;

    if (score > bestScore) {
      bestScore = score;
      outRow = r;
      outCol = c;
    }
  }
}

void botSortCandidates(const Cell board[BOARD_SIZE][BOARD_SIZE],
                       int candidates[][2], int count, int botPlayer) {
  int scores[BOARD_SIZE * BOARD_SIZE];
  for (int i = 0; i < count; i++) {
    scores[i] = botEvaluatePosition(board, candidates[i][0], candidates[i][1],
                                    botPlayer);
  }
  for (int i = 0; i < count - 1; i++) {
    int maxIdx = i;
    for (int j = i + 1; j < count; j++) {
      if (scores[j] > scores[maxIdx])
        maxIdx = j;
    }
    if (maxIdx != i) {
      std::swap(scores[i], scores[maxIdx]);
      std::swap(candidates[i][0], candidates[maxIdx][0]);
      std::swap(candidates[i][1], candidates[maxIdx][1]);
    }
  }
}

void botExpertMove(const GameState &state, int &outRow, int &outCol) {
  if (botCheckImmediateWin(state.board, BOT_PLAYER, outRow, outCol))
    return;
  if (botCheckImmediateBlock(state.board, BOT_PLAYER, outRow, outCol))
    return;

  Cell board[BOARD_SIZE][BOARD_SIZE];
  for (int r = 0; r < BOARD_SIZE; r++)
    for (int c = 0; c < BOARD_SIZE; c++)
      board[r][c] = state.board[r][c];

  int candidates[BOARD_SIZE * BOARD_SIZE][2];
  int count = botGetCandidates(board, candidates, 2);

  if (count == 0) {
    outRow = BOARD_SIZE / 2;
    outCol = BOARD_SIZE / 2;
    return;
  }

  botSortCandidates(board, candidates, count, BOT_PLAYER);

  if (count > 12)
    count = 12;

  int bestScore = INT_MIN;
  outRow = candidates[0][0];
  outCol = candidates[0][1];

  for (int i = 0; i < count; i++) {
    int r = candidates[i][0], c = candidates[i][1];
    board[r][c].value = BOT_PLAYER;
    int score = botMinimax(board, 1, 4, INT_MIN, INT_MAX, false, BOT_PLAYER);
    board[r][c].value = 0;

    int centerDist =
        std::abs(r - BOARD_SIZE / 2) + std::abs(c - BOARD_SIZE / 2);
    score += (BOARD_SIZE - centerDist) * 2;

    if (score > bestScore) {
      bestScore = score;
      outRow = r;
      outCol = c;
    }
  }
}

void botGetMove(const GameState &state, BotDifficulty difficulty, int &outRow,
                int &outCol) {
  switch (difficulty) {
  case BOT_EASY:
    botEasyMove(state, outRow, outCol);
    break;
  case BOT_MEDIUM:
    botMediumMove(state, outRow, outCol);
    break;
  case BOT_HARD:
    botHardMove(state, outRow, outCol);
    break;
  case BOT_EXPERT:
    botExpertMove(state, outRow, outCol);
    break;
  }
}

void botGetHint(const GameState &state, int &outRow, int &outCol) {
  if (botCheckImmediateWin(state.board, HUMAN_PLAYER, outRow, outCol))
    return;
  if (botCheckImmediateBlock(state.board, HUMAN_PLAYER, outRow, outCol))
    return;

  int candidates[BOARD_SIZE * BOARD_SIZE][2];
  int count = botGetCandidates(state.board, candidates, 2);

  if (count == 0) {
    outRow = BOARD_SIZE / 2;
    outCol = BOARD_SIZE / 2;
    return;
  }

  int bestScore = INT_MIN;
  outRow = candidates[0][0];
  outCol = candidates[0][1];

  for (int i = 0; i < count; i++) {
    int r = candidates[i][0], c = candidates[i][1];
    int score = botEvaluatePosition(state.board, r, c, HUMAN_PLAYER);
    if (score > bestScore) {
      bestScore = score;
      outRow = r;
      outCol = c;
    }
  }
}