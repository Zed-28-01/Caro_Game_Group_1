#include "timer.h"

void timerStart(TimerState &timer, float gameTime, float turnTime) {
  timer.gameTimeLeftP1 = gameTime;
  timer.gameTimeLeftP2 = gameTime;
  timer.turnTimeLeft = turnTime;
  timer.isRunning = true;
  timer.turnAlarmFired = false;
}

void timerUpdate(TimerState &timer, float deltaTime, bool isPlayer1Turn) {
  if (!timer.isRunning)
    return;

  timer.turnTimeLeft -= deltaTime;
  if (timer.turnTimeLeft < 0)
    timer.turnTimeLeft = 0;

  if (isPlayer1Turn) {
    timer.gameTimeLeftP1 -= deltaTime;
    if (timer.gameTimeLeftP1 < 0)
      timer.gameTimeLeftP1 = 0;
  } else {
    timer.gameTimeLeftP2 -= deltaTime;
    if (timer.gameTimeLeftP2 < 0)
      timer.gameTimeLeftP2 = 0;
  }
}

void timerResetTurn(TimerState &timer) {
  timer.turnTimeLeft = MAX_TURN_TIME;
  timer.turnAlarmFired = false;
}

void timerPause(TimerState &timer) { timer.isRunning = false; }

void timerResume(TimerState &timer) { timer.isRunning = true; }

void timerConsumeP1(TimerState &timer, float seconds) {
  timer.gameTimeLeftP1 -= seconds;
  if (timer.gameTimeLeftP1 < 0)
    timer.gameTimeLeftP1 = 0;
}

void timerConsumeP2(TimerState &timer, float seconds) {
  timer.gameTimeLeftP2 -= seconds;
  if (timer.gameTimeLeftP2 < 0)
    timer.gameTimeLeftP2 = 0;
}

float timerGetTurnPercent(const TimerState &timer) {
  return timer.turnTimeLeft / MAX_TURN_TIME;
}

bool timerIsTurnExpired(const TimerState &timer) {
  return timer.turnTimeLeft <= 0;
}

bool timerIsGameExpiredP1(const TimerState &timer) {
  return timer.gameTimeLeftP1 <= 0;
}

bool timerIsGameExpiredP2(const TimerState &timer) {
  return timer.gameTimeLeftP2 <= 0;
}

float timerGetTurnSecondsLeft(const TimerState &timer) {
  return timer.turnTimeLeft;
}

float timerGetGameSecondsLeftP1(const TimerState &timer) {
  return timer.gameTimeLeftP1;
}

float timerGetGameSecondsLeftP2(const TimerState &timer) {
  return timer.gameTimeLeftP2;
}
