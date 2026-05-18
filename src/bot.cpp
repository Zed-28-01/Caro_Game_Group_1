#include "bot.h"
#include<cstdlib>
#include<ctime>
#include<climits>


// ============================================================
// BANG DIEM PATTERN (Quyet dinh suc manh AI)
// ============================================================
static const int SCORE_WIN = 1000000;  // 5+ lien
static const int SCORE_FOUR_OPEN = 100000;   // 4 mo 2 dau
static const int SCORE_FOUR_HALF = 5000;     // 4 mo 1 dau
static const int SCORE_THREE_OPEN = 5000;     // 3 mo 2 dau
static const int SCORE_THREE_HALF = 500;      // 3 mo 1 dau
static const int SCORE_TWO_OPEN = 500;      // 2 mo 2 dau
static const int SCORE_TWO_HALF = 50;       // 2 mo 1 dau
static const int SCORE_ONE_OPEN = 10;       // 1 mo 2 dau
static const int SCORE_ONE_HALF = 5;        // 1 mo 1 dau

static const int BOT_PLAYER = 1;   
static const int HUMAN_PLAYER = -1;

// 4 huong
static const int DIRS[4][2] = { {0,1}, {1,0}, {1,1}, {1,-1} };

// Tien ich noi bo
static bool inBounds(int r, int c) {
    return r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE;
}

// Ham dem lien tiep 1 huong
int botCountConsecutive(const Cell board[BOARD_SIZE][BOARD_SIZE],
    int row, int col, int dRow, int dCol, int player) {
    int count = 0;
    int r = row + dRow, c = col + dCol;
    while (inBounds(r, c) && board[r][c].value == player) {
        count++;
        r += dRow;
        c += dCol;
    }
    return count;
}

// Tim o dang xet - xet o trong ma co quan cach o do trong ban kinh radius (2)
int botGetCandidates(const Cell board[BOARD_SIZE][BOARD_SIZE],
    int candidates[][2], int radius) {
    int count = 0;
    bool hasAnyPiece = false;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (board[r][c].value != 0) { hasAnyPiece = true; break; }
        }
        if (hasAnyPiece) break;
    }

    // Ban co trong -> tra ve o trung tam
    if (!hasAnyPiece) {
        candidates[0][0] = BOARD_SIZE / 2;
        candidates[0][1] = BOARD_SIZE / 2;
        return 1;
    }

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (board[r][c].value != 0) continue;

            // Kiem tra co quan nao trong ban kinh radius khong
            bool nearPiece = false;
            for (int dr = -radius; dr <= radius && !nearPiece; dr++) {
                for (int dc = -radius; dc <= radius && !nearPiece; dc++) {
                    if (dr == 0 && dc == 0) continue;
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



bool botCheckImmediateWin(const Cell board[BOARD_SIZE][BOARD_SIZE],
    int player, int& outRow, int& outCol) {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (board[r][c].value != 0) continue;
            // Thu dat quan, kiem tra 4 huong co >= 5 lien khong
            for (int d = 0; d < 4; d++) {
                int total = 1
                    + botCountConsecutive(board, r, c, DIRS[d][0], DIRS[d][1], player)
                    + botCountConsecutive(board, r, c, -DIRS[d][0], -DIRS[d][1], player);
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
    int botPlayer, int& outRow, int& outCol) {
    int opponent = -botPlayer;
    return botCheckImmediateWin(board, opponent, outRow, outCol);
}

int botScoreLine(const Cell board[BOARD_SIZE][BOARD_SIZE],
    int row, int col, int dRow, int dCol, int player) {
    // Dem lien tiep 2 phia
    int forward = botCountConsecutive(board, row, col, dRow, dCol, player);
    int backward = botCountConsecutive(board, row, col, -dRow, -dCol, player);
    int total = forward + backward + 1; // +1 cho o (row,col)

    // Dem so dau "mo" (o trong) - quan trong de phan biet 3 mo 2 dau vs 3 bi chan
    int openEnds = 0;
    int fr = row + (forward + 1) * dRow;
    int fc = col + (forward + 1) * dCol;
    if (inBounds(fr, fc) && board[fr][fc].value == 0) openEnds++;

    int br = row - (backward + 1) * dRow;
    int bc = col - (backward + 1) * dCol;
    if (inBounds(br, bc) && board[br][bc].value == 0) openEnds++;

    if (total >= 5) return SCORE_WIN;

    switch (total) {
    case 4:
        if (openEnds == 2) return SCORE_FOUR_OPEN;
        if (openEnds == 1) return SCORE_FOUR_HALF;
        return 0;
    case 3:
        if (openEnds == 2) return SCORE_THREE_OPEN;
        if (openEnds == 1) return SCORE_THREE_HALF;
        return 0;
    case 2:
        if (openEnds == 2) return SCORE_TWO_OPEN;
        if (openEnds == 1) return SCORE_TWO_HALF;
        return 0;
    case 1:
        if (openEnds == 2) return SCORE_ONE_OPEN;
        if (openEnds == 1) return SCORE_ONE_HALF;
        return 0;
    }
    return 0;
}


int botEvaluatePosition(const Cell board[BOARD_SIZE][BOARD_SIZE],
    int row, int col, int botPlayer) {
    Cell* mutableBoard = const_cast<Cell*>(&board[0][0]);
    auto get = [&](int r, int c) -> Cell& {
        return *(mutableBoard + r * BOARD_SIZE + c);
        };

    int attackScore = 0;
    int defenseScore = 0;
    int opponent = -botPlayer;

    // Diem tan cong: gia lap dat quan bot
    get(row, col).value = botPlayer;
    for (int d = 0; d < 4; d++)
        attackScore += botScoreLine(board, row, col, DIRS[d][0], DIRS[d][1], botPlayer);

    // Diem phong thu: gia lap dat quan doi thu
    get(row, col).value = opponent;
    for (int d = 0; d < 4; d++)
        defenseScore += botScoreLine(board, row, col, DIRS[d][0], DIRS[d][1], opponent);

    get(row, col).value = 0; // khoi phuc

    // Bonus o gan trung tam (luon dieu chinh nhe)
    int centerDist = std::abs(row - BOARD_SIZE / 2) + std::abs(col - BOARD_SIZE / 2);
    int centerBonus = (BOARD_SIZE - centerDist) * 2;

    // Tan cong x 1.0 + Phong thu x 0.9 (uu tien tan cong)
    return attackScore + (defenseScore * 9 / 10) + centerBonus;
}


int botEvaluateBoard(const Cell board[BOARD_SIZE][BOARD_SIZE], int botPlayer) {
    int score = 0;
    int opponent = -botPlayer;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (board[r][c].value == botPlayer) {
                // Chi tinh tu o "dau chuoi" de tranh dem trung
                for (int d = 0; d < 4; d++) {
                    int pr = r - DIRS[d][0], pc = c - DIRS[d][1];
                    if (inBounds(pr, pc) && board[pr][pc].value == botPlayer) continue;
                    score += botScoreLine(board, r, c, DIRS[d][0], DIRS[d][1], botPlayer);
                }
            }
            else if (board[r][c].value == opponent) {
                for (int d = 0; d < 4; d++) {
                    int pr = r - DIRS[d][0], pc = c - DIRS[d][1];
                    if (inBounds(pr, pc) && board[pr][pc].value == opponent) continue;
                    score -= botScoreLine(board, r, c, DIRS[d][0], DIRS[d][1], opponent);
                }
            }
        }
    }
    return score;
}


void botEasyMove(const GameState& state, int& outRow, int& outCol) {
    // 1. Doi thu sap thang -> chan ngay
    if (botCheckImmediateBlock(state.board, BOT_PLAYER, outRow, outCol)) return;
    if (botCheckImmediateWin(state.board, BOT_PLAYER, outRow, outCol)) return;

    // 2. Chon ngau nhien tu candidates
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


void botMediumMove(const GameState& state, int& outRow, int& outCol) {
    if (botCheckImmediateWin(state.board, BOT_PLAYER, outRow, outCol)) return;
    if (botCheckImmediateBlock(state.board, BOT_PLAYER, outRow, outCol)) return;

    int candidates[BOARD_SIZE * BOARD_SIZE][2];
    int count = botGetCandidates(state.board, candidates, 2);

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

int botMinimax(Cell board[BOARD_SIZE][BOARD_SIZE],
    int depth, int maxDepth,
    int alpha, int beta,
    bool isMaximizing, int botPlayer) {
    if (depth == maxDepth) {
        return botEvaluateBoard(board, botPlayer);
    }

    int candidates[BOARD_SIZE * BOARD_SIZE][2];
    int count = botGetCandidates(board, candidates, 1); // ban kinh 1 trong minimax

    if (count == 0) return botEvaluateBoard(board, botPlayer);

    int currentPlayer = isMaximizing ? botPlayer : -botPlayer;

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (int i = 0; i < count; i++) {
            int r = candidates[i][0], c = candidates[i][1];
            board[r][c].value = currentPlayer;
            int eval = botMinimax(board, depth + 1, maxDepth, alpha, beta, false, botPlayer);
            board[r][c].value = 0;

            if (eval > maxEval) maxEval = eval;
            if (eval > alpha) alpha = eval;
            if (beta <= alpha) break; // CAT TIA
        }
        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        for (int i = 0; i < count; i++) {
            int r = candidates[i][0], c = candidates[i][1];
            board[r][c].value = currentPlayer;
            int eval = botMinimax(board, depth + 1, maxDepth, alpha, beta, true, botPlayer);
            board[r][c].value = 0;

            if (eval < minEval) minEval = eval;
            if (eval < beta) beta = eval;
            if (beta <= alpha) break; // CAT TIA
        }
        return minEval;
    }
}



void botHardMove(const GameState& state, int& outRow, int& outCol) {
    if (botCheckImmediateWin(state.board, BOT_PLAYER, outRow, outCol)) return;
    if (botCheckImmediateBlock(state.board, BOT_PLAYER, outRow, outCol)) return;

    // Copy board ra mang co the modify
    Cell board[BOARD_SIZE][BOARD_SIZE];
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            board[r][c] = state.board[r][c];

    int candidates[BOARD_SIZE * BOARD_SIZE][2];
    int count = botGetCandidates(board, candidates, 2);

    int bestScore = INT_MIN;
    outRow = candidates[0][0];
    outCol = candidates[0][1];

    for (int i = 0; i < count; i++) {
        int r = candidates[i][0], c = candidates[i][1];
        board[r][c].value = BOT_PLAYER;
        int score = botMinimax(board, 1, 3, INT_MIN, INT_MAX, false, BOT_PLAYER);
        board[r][c].value = 0;

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
        scores[i] = botEvaluatePosition(board, candidates[i][0], candidates[i][1], botPlayer);
    }
    // Selection sort giam dan (don gian, count thuong < 50)
    for (int i = 0; i < count - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < count; j++) {
            if (scores[j] > scores[maxIdx]) maxIdx = j;
        }
        if (maxIdx != i) {
            std::swap(scores[i], scores[maxIdx]);
            std::swap(candidates[i][0], candidates[maxIdx][0]);
            std::swap(candidates[i][1], candidates[maxIdx][1]);
        }
    }
}

void botExpertMove(const GameState& state, int& outRow, int& outCol) {
    if (botCheckImmediateWin(state.board, BOT_PLAYER, outRow, outCol)) return;
    if (botCheckImmediateBlock(state.board, BOT_PLAYER, outRow, outCol)) return;

    Cell board[BOARD_SIZE][BOARD_SIZE];
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            board[r][c] = state.board[r][c];

    int candidates[BOARD_SIZE * BOARD_SIZE][2];
    int count = botGetCandidates(board, candidates, 2);

    // MOVE ORDERING - sap xep candidates theo diem
    botSortCandidates(board, candidates, count, BOT_PLAYER);

    // Chi xet top 12 candidates (cat bot de depth 4 chay nhanh)
    if (count > 12) count = 12;

    int bestScore = INT_MIN;
    outRow = candidates[0][0];
    outCol = candidates[0][1];

    for (int i = 0; i < count; i++) {
        int r = candidates[i][0], c = candidates[i][1];
        board[r][c].value = BOT_PLAYER;
        int score = botMinimax(board, 1, 4, INT_MIN, INT_MAX, false, BOT_PLAYER);
        board[r][c].value = 0;

        if (score > bestScore) {
            bestScore = score;
            outRow = r;
            outCol = c;
        }
    }
}


void botGetMove(const GameState& state, BotDifficulty difficulty,
    int& outRow, int& outCol) {
    switch (difficulty) {
    case BOT_EASY:   botEasyMove(state, outRow, outCol);   break;
    case BOT_MEDIUM: botMediumMove(state, outRow, outCol); break;
    case BOT_HARD:   botHardMove(state, outRow, outCol);   break;
    case BOT_EXPERT: botExpertMove(state, outRow, outCol); break;
    }
}

void botGetHint(const GameState& state, int& outRow, int& outCol) {
    // Goi y dung Medium - khong qua manh, khong qua yeu
    // NHUNG goi voi player = HUMAN (vi goi y cho NGUOI choi)
    // Cach don gian: tam dao vai tro, dung Medium logic

    // Kiem tra nguoi choi co the thang ngay khong
    if (botCheckImmediateWin(state.board, HUMAN_PLAYER, outRow, outCol)) return;
    // Hoac can chan bot khong
    if (botCheckImmediateBlock(state.board, HUMAN_PLAYER, outRow, outCol)) return;

    // Chon o tot nhat (danh gia theo goc nhin nguoi choi)
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