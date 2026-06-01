#pragma once
#ifndef TIMER_H
#define TIMER_H

#include "game_types.h"

// ============================================================
// TIMER CHESS-CLOCK: moi nguoi co thoi gian rieng (gameTimeLeftP1/P2)
// + 1 turn timer dung chung cho luot hien tai
// ============================================================

// Khoi tao / reset timer cho van moi
// gameTime: thoi gian van CUA MOI NGUOI (vd 600s = 10 phut/nguoi)
// turnTime: thoi gian luot (vd 20s)
void timerStart(TimerState& timer, float gameTime, float turnTime);

// Cap nhat timer moi frame (goi trong game loop)
// deltaTime: thoi gian da troi qua tu frame truoc (giay)
// isPlayer1Turn: dang la luot cua P1? Chi tru thoi gian van CUA NGUOI DANG DI
// Kiem tra hat thoi gian dung timerIsGameExpiredP1/P2 (clean separation)
void timerUpdate(TimerState& timer, float deltaTime, bool isPlayer1Turn);

// Reset thoi gian luot moi (goi khi nguoi choi vua danh xong)
void timerResetTurn(TimerState& timer);

// Tam dung timer (khi mo menu, ...)
void timerPause(TimerState& timer);

// Tiep tuc timer sau khi tam dung
void timerResume(TimerState& timer);

// Tru thoi gian thu cong cho 1 player (dung khi bot block main loop)
void timerConsumeP1(TimerState& timer, float seconds);
void timerConsumeP2(TimerState& timer, float seconds);

// ============================================================
// TRUY VAN TRANG THAI
// ============================================================

// Lay phan tram thoi gian luot con lai (0.0 - 1.0)
// Dung de ve progress bar
float timerGetTurnPercent(const TimerState& timer);

// Lay phan tram thoi gian van con lai cua tung nguoi (0.0 - 1.0)
float timerGetGamePercentP1(const TimerState& timer);
float timerGetGamePercentP2(const TimerState& timer);

// Kiem tra luot hien tai het thoi gian chua
bool timerIsTurnExpired(const TimerState& timer);

// Kiem tra van het thoi gian cua tung nguoi
bool timerIsGameExpiredP1(const TimerState& timer);
bool timerIsGameExpiredP2(const TimerState& timer);

// Lay so giay con lai cua luot (hien thi UI)
float timerGetTurnSecondsLeft(const TimerState& timer);

// Lay so giay con lai cua van (hien thi UI) cho tung nguoi
float timerGetGameSecondsLeftP1(const TimerState& timer);
float timerGetGameSecondsLeftP2(const TimerState& timer);

#endif // TIMER_H
