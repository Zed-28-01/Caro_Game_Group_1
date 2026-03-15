#pragma once
#ifndef TIMER_H
#define TIMER_H

#include "game_types.h"

// ============================================================
// TIMER SU DUNG sf::Clock (don gian, khong can thread)
// ============================================================

// Khoi tao / reset timer cho van moi
void timerStart(TimerState& timer, float gameTime, float turnTime);

// Cap nhat timer moi frame (goi trong game loop)
// deltaTime: thoi gian da troi qua tu frame truoc (giay)
// Tra ve true neu con thoi gian, false neu het thoi gian van
bool timerUpdate(TimerState& timer, float deltaTime);

// Reset thoi gian luot moi (goi khi nguoi choi vua danh xong)
void timerResetTurn(TimerState& timer);

// Tam dung timer (khi mo menu, ...)
void timerPause(TimerState& timer);

// Tiep tuc timer sau khi tam dung
void timerResume(TimerState& timer);

// ============================================================
// TRUY VAN TRANG THAI
// ============================================================

// Lay phan tram thoi gian luot con lai (0.0 - 1.0)
// Dung de ve progress bar
float timerGetTurnPercent(const TimerState& timer);

// Lay phan tram thoi gian van con lai (0.0 - 1.0)
float timerGetGamePercent(const TimerState& timer);

// Kiem tra luot hien tai het thoi gian chua
bool timerIsTurnExpired(const TimerState& timer);

// Kiem tra van het thoi gian chua
bool timerIsGameExpired(const TimerState& timer);

// Lay so giay con lai cua luot (hien thi UI)
float timerGetTurnSecondsLeft(const TimerState& timer);

// Lay so giay con lai cua van (hien thi UI)
float timerGetGameSecondsLeft(const TimerState& timer);

#endif // TIMER_H
