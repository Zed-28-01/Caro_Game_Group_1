#include "bot.h"
#include <climits>
#include <cstdlib>
#include <ctime>

// ============================================================
// BANG DIEM PATTERN (Quyet dinh suc manh AI)
// ============================================================
static const int SCORE_WIN = 1000000;      // 5+ lien
static const int SCORE_FOUR_OPEN = 100000; // 4 mo 2 dau
static const int SCORE_FOUR_HALF = 5000;   // 4 mo 1 dau
static const int SCORE_THREE_OPEN = 5000;  // 3 mo 2 dau
static const int SCORE_THREE_HALF = 500;   // 3 mo 1 dau
static const int SCORE_TWO_OPEN = 500;     // 2 mo 2 dau
static const int SCORE_TWO_HALF = 50;      // 2 mo 1 dau
static const int SCORE_ONE_OPEN = 10;      // 1 mo 2 dau
static const int SCORE_ONE_HALF = 5;       // 1 mo 1 dau

static const int BOT_PLAYER = 1;
static const int HUMAN_PLAYER = -1;

// 4 huong
static const int DIRS[4][2] = {
    {0, 1}, {1, 0}, {1, 1}, {1, -1}}; // trùng với board.cpp

// Tien ich noi bo - giống hàm trong board.cpp
static bool inBounds(int r, int c) {
  return r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE;
}

// Ham dem lien tiep 1 huong - giống countDirection trong board nhưng bỏ khối
// GameState& State để nhẹ đi
int botCountConsecutive(const Cell board[BOARD_SIZE][BOARD_SIZE], int row,
                        int col, int dRow, int dCol, int player) {
  int count = 0;
  int r = row + dRow, c = col + dCol;
  while (inBounds(r, c) && board[r][c].value == player) {
    count++;
    r += dRow;
    c += dCol;
  }
  return count;
}

// Tìm kiếm các ô ứng viên để giảm số lượng kịch bản mà bot cần nghĩ
// Xét các ô trong mà có quan cách tới ô đã có đánh trong bán kính radius (2)
// Hãy tưởng tượng, bàn cờ Caro có 15x15 = 225 ô.
// Nếu bắt con Bot suy nghĩ trước 4 nước (Độ sâu Minimax = 4),
// nó sẽ phải vạch ra số lượng kịch bản là: 225 * 224 * 223 * 222 ≈ 2.5 Tỷ kịch
// bản! Dù máy tính có khỏe đến mấy cũng sẽ bị "treo" vài phút. Do đó giảm số
// lượng ô - chỉ xét các candidate để giảm kịch bản mà bot cần nghĩ
int botGetCandidates(const Cell board[BOARD_SIZE][BOARD_SIZE],
                     int candidates[][2], int radius) {

  // heuristic - chỉ xét các ô trong bán kính 2 ô xung quanh quân đã có
  // để giảm thời gian tính toán
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

  // Ban co trong -> tra ve o trung tam
  if (!hasAnyPiece) {
    candidates[0][0] = BOARD_SIZE / 2;
    candidates[0][1] = BOARD_SIZE / 2;
    return 1;
  }

  // Nếu đã có cờ đánh -> xét toàn bộ thế cờ (bàn cờ) hiện tại để tìm ứng cử
  // viên chỉ lọt ứng cử viên (ô chưa đánh) nếu xung quanh bán kính radius = 2
  // đã có ô đánh
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      if (board[r][c].value != 0)
        continue; // ô đã có cờ -> bỏ qua

      // Kiem tra co quan nao trong ban kinh radius khong
      bool nearPiece = false;
      for (int dr = -radius; dr <= radius && !nearPiece; dr++) {
        for (int dc = -radius; dc <= radius && !nearPiece; dc++) {
          if (dr == 0 && dc == 0)
            continue; // chính là ô (row, col) hiện tại
          int nr = r + dr, nc = c + dc;
          if (inBounds(nr, nc) &&
              board[nr][nc].value !=
                  0) // không xét loại X hay O, đối thủ hay mình mà có ở gần đó
                     // đều đưa node đó vào ứng cử viên
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
  return count; // return về 0 nếu hiện tại bàn cờ đã hết ô - hòa cờ
}

// hàm này không phải là hàm kiểm tra thắng giống board.cpp nhé -> chú ý kiểm
// tra thuật toán này kiểm tra tất cả các ô còn trống + đặt tại ô đó có >= 5
// quân không nếu có trả true, nếu không trả false
bool botCheckImmediateWin(const Cell board[BOARD_SIZE][BOARD_SIZE], int player,
                          int &outRow, int &outCol) {
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      if (board[r][c].value != 0)
        continue;
      // Thu dat quan, kiem tra 4 huong co >= 5 lien khong
      for (int d = 0; d < 4; d++) {
        int total =
            1 +
            botCountConsecutive(board, r, c, DIRS[d][0], DIRS[d][1], player) +
            botCountConsecutive(board, r, c, -DIRS[d][0], -DIRS[d][1], player);
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
  int opponent = -botPlayer; // giả định đối thủ của bot (người chơi) để xem
                             // người chơi có thể win ngay lập tức không
  return botCheckImmediateWin(board, opponent, outRow, outCol);
}

// Hai hàm block và win trên đều được sử dụng cho 4 bot tuy nhiên có một
// chút khác biệt ở thứ tự đối với bot easy thì ưu tiên chặn trước win -> giảm
// độ intelligence

// hàm quan trọng - đánh giá nước đi theo một hướng duy nhất
int botScoreLine(const Cell board[BOARD_SIZE][BOARD_SIZE], int row, int col,
                 int dRow, int dCol, int player) {
  // Dem lien tiep 2 phia
  int forward = botCountConsecutive(board, row, col, dRow, dCol, player);
  int backward = botCountConsecutive(board, row, col, -dRow, -dCol, player);
  int total = forward + backward + 1; // +1 cho o (row,col)

  // Dem so dau "mo" (o trong) - quan trong de phan biet 3 mo 2 dau vs 3 bi chan
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
  return 0; // chưa có ô nào xung quanh ô đó
}

// Chấm điểm 4 hướng tại ô (row, col)
int botEvaluatePosition(const Cell board[BOARD_SIZE][BOARD_SIZE], int row,
                        int col, int botPlayer) {
  int attackScore = 0;
  int defenseScore = 0;
  int opponent = -botPlayer;

  // Cộng tổng điểm 4 hướng là key point để giúp con Bot nhận biết được nước cờ
  // đôi
  for (int d = 0; d < 4; d++)
    attackScore +=
        botScoreLine(board, row, col, DIRS[d][0], DIRS[d][1], botPlayer);

  // Diem phong thu: tuong tu voi opponent
  for (int d = 0; d < 4; d++)
    defenseScore +=
        botScoreLine(board, row, col, DIRS[d][0], DIRS[d][1], opponent);

  // Bonus o gan trung tam
  // Khoảng cách Manhattan (Manhattan Distance) -> giải thích lí do không dùng
  // ct toán học bthuong
  int centerDist = std::abs(row - BOARD_SIZE / 2) +
                   std::abs(col - BOARD_SIZE / 2); // dist từ ô đó đến (7,7)
  int centerBonus = (BOARD_SIZE - centerDist) * 2;

  return attackScore + (defenseScore * 9 / 10) +
         centerBonus; // giá trị của một nước đi
  // tính lãi kép. Xem nước đó tấn công được bao nhiêu và ngăn chặn ý đồ tấn
  // công của đối thủ là bao nhiêu (ưu tiên tấn công hơn phòng thủ)
}

// Cốt lõi của MiniMax do Expert và Hard sử dụng -> đánh giá thế cờ hiện tại
// đang ai có lợi >0 bot có lợi, <0 người chơi có lợi
int botEvaluateBoard(const Cell board[BOARD_SIZE][BOARD_SIZE], int botPlayer) {
  int score = 0;
  int opponent = -botPlayer;

  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      if (board[r][c].value == botPlayer) {
        // Chi tinh tu o "dau chuoi" de tranh dem trung
        for (int d = 0; d < 4; d++) {
          int pr = r - DIRS[d][0], pc = c - DIRS[d][1];
          if (inBounds(pr, pc) && board[pr][pc].value == botPlayer)
            continue;
          score +=
              botScoreLine(board, r, c, DIRS[d][0], DIRS[d][1],
                           botPlayer); // tổng điểm của bot sau khi cộng toàn bộ
                                       // thế cờ, không lo việc bỏ sót khi có
                                       // một ô đầu hàng nhưng chéo cột,...
          // nói cách khác bất kì ô đường ngang dọc chéo nào cugnx được tính
          // đúng 1 lần duy nhất bởi cái ô đứng đầu của đường thẳng đó
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
  // 1. Doi thu sap thang -> chan ngay
  if (botCheckImmediateBlock(state.board, BOT_PLAYER, outRow, outCol))
    return;
  if (botCheckImmediateWin(state.board, BOT_PLAYER, outRow, outCol))
    return;

  // 2. Chon ngau nhien tu candidates
  int candidates[BOARD_SIZE * BOARD_SIZE][2];
  int count = botGetCandidates(state.board, candidates, 2);

  // Phanh an toàn (Safety Guard): Chống Crash Game
  // Về lý thuyết, count = 0 chỉ xảy ra khi bàn cờ đã đầy kín (Hòa cờ).
  // Khi đó ván game đã kết thúc, người chơi không thể đi tiếp.
  // Tuy nhiên, nếu vì một bug nào đó mà hệ thống vẫn gọi hàm này khi bàn cờ đã
  // đầy, mảng candidates sẽ rỗng. Nếu cố truy cập candidates[0] bên dưới sẽ gây
  // văng game (Crash). Nên ở đây thà trả về ô tâm bàn cờ để báo lỗi UI còn hơn
  // là Crash toàn bộ game.
  if (count == 0) {
    outRow = BOARD_SIZE / 2;
    outCol = BOARD_SIZE / 2;
    return;
  }
  // đánh random tại 1 trong các ô candidate
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

  // Phanh an toàn (Safety Guard)
  if (count == 0) {
    outRow = BOARD_SIZE / 2;
    outCol = BOARD_SIZE / 2;
    return;
  }

  int bestScore = INT_MIN;
  outRow = candidates[0][0];
  outCol = candidates[0][1];

  // lựa candidate có điểm cao nhất trong toàn bộ
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

int botMinimax(Cell board[BOARD_SIZE][BOARD_SIZE], int depth, int maxDepth,
               int alpha, int beta, bool isMaximizing, int botPlayer) {
  if (depth == maxDepth) {
    return botEvaluateBoard(
        board, botPlayer); // return về score của thế cờ hiện tại >0, <0, =0
  }

  int candidates[BOARD_SIZE * BOARD_SIZE][2];
  int count =
      botGetCandidates(board, candidates, 1); // ban kinh 1 trong minimax

  if (count == 0)
    return botEvaluateBoard(board, botPlayer);

  int currentPlayer = isMaximizing ? botPlayer : -botPlayer;

  if (isMaximizing) {
    int maxEval = INT_MIN;
    for (int i = 0; i < count; i++) {
      int r = candidates[i][0], c = candidates[i][1];
      board[r][c].value = currentPlayer;
      int eval =
          botMinimax(board, depth + 1, maxDepth, alpha, beta, false, botPlayer);
      board[r][c].value = 0;

      if (eval > maxEval)
        maxEval = eval;
      if (eval > alpha)
        alpha = eval;
      if (beta <= alpha)
        break; // CAT TIA
    }
    return maxEval;
  } else {
    int minEval = INT_MAX;
    for (int i = 0; i < count; i++) {
      int r = candidates[i][0], c = candidates[i][1];
      board[r][c].value = currentPlayer;
      int eval =
          botMinimax(board, depth + 1, maxDepth, alpha, beta, true, botPlayer);
      board[r][c].value = 0;

      if (eval < minEval)
        minEval = eval;
      if (eval < beta)
        beta = eval;
      if (beta <= alpha)
        break; // CAT TIA
    }
    return minEval;
  }
}

void botHardMove(const GameState &state, int &outRow, int &outCol) {
  if (botCheckImmediateWin(state.board, BOT_PLAYER, outRow, outCol))
    return;
  if (botCheckImmediateBlock(state.board, BOT_PLAYER, outRow, outCol))
    return;

  // Copy board ra mang co the modify
  Cell board[BOARD_SIZE][BOARD_SIZE];
  for (int r = 0; r < BOARD_SIZE; r++)
    for (int c = 0; c < BOARD_SIZE; c++)
      board[r][c] = state.board[r][c];

  int candidates[BOARD_SIZE * BOARD_SIZE][2];
  int count = botGetCandidates(board, candidates, 2);

  // Phanh an toàn (Safety Guard)
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

    // TIEBREAKER: cong center bonus (~max 28) - chi pha tie,
    // khong meo mo quyet dinh vi pattern score >> 28
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

// Sắp xếp lại các ứng viên theo thứ tự ưu tiên lớn đến bé
// để khi loop for (int i = 0; i < count; i++) ... sẽ ưu tiên nước có điểm cao
// nhất trước Lại là một thuật toán giảm số kịch bản Chỉ lựa top 12 ô đầu tiên
// cao nhất để tính toán -> được thực hiện cho expert vì expert có độ sâu 4 nên
// cần giảm số kịch bản so với hard chỉ có độ sâu 3
void botSortCandidates(const Cell board[BOARD_SIZE][BOARD_SIZE],
                       int candidates[][2], int count, int botPlayer) {
  int scores[BOARD_SIZE * BOARD_SIZE];
  for (int i = 0; i < count; i++) {
    scores[i] = botEvaluatePosition(board, candidates[i][0], candidates[i][1],
                                    botPlayer);
  }
  // Selection sort giam dan (don gian, count thuong < 50)
  for (int i = 0; i < count - 1; i++) {
    int maxIdx = i;
    for (int j = i + 1; j < count; j++) {
      if (scores[j] > scores[maxIdx])
        maxIdx = j;
    }
    if (maxIdx != i) {
      std::swap(scores[i], scores[maxIdx]);
      std::swap(candidates[i][0], candidates[maxIdx][0]); // đổi r
      std::swap(candidates[i][1], candidates[maxIdx][1]); // đổi c
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

  // Phanh an toàn (Safety Guard)
  if (count == 0) {
    outRow = BOARD_SIZE / 2;
    outCol = BOARD_SIZE / 2;
    return;
  }

  // MOVE ORDERING - sap xep candidates theo diem
  botSortCandidates(board, candidates, count, BOT_PLAYER);

  // Chi xet top 12 candidates (cat bot de depth 4 chay nhanh)
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

    // TIEBREAKER: center bonus
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
  // Goi y dung Medium - khong qua manh, khong qua yeu
  // NHUNG goi voi player = HUMAN (vi goi y cho NGUOI choi)
  // Cach don gian: tam dao vai tro, dung Medium logic

  // Kiem tra nguoi choi co the thang ngay khong
  if (botCheckImmediateWin(state.board, HUMAN_PLAYER, outRow, outCol))
    return;
  // Hoac can chan bot khong
  if (botCheckImmediateBlock(state.board, HUMAN_PLAYER, outRow, outCol))
    return;

  // Chon o tot nhat (danh gia theo goc nhin nguoi choi)
  int candidates[BOARD_SIZE * BOARD_SIZE][2];
  int count = botGetCandidates(state.board, candidates, 2);

  // Phanh an toàn (Safety Guard): Chống Crash Game
  // Về lý thuyết, count = 0 chỉ xảy ra khi bàn cờ đã đầy kín (Hòa cờ).
  // Khi đó ván game đã kết thúc, người chơi không thể đi tiếp.
  // Tuy nhiên, nếu vì một bug nào đó mà hệ thống vẫn gọi hàm Hint này khi bàn
  // cờ đã đầy, mảng candidates sẽ rỗng. Nếu cố truy cập candidates[0] bên dưới
  // sẽ gây văng game (Crash). Nên ở đây thà trả về ô tâm bàn cờ để báo lỗi UI
  // còn hơn là Crash toàn bộ game.
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