#include "timer.h"


// Khoi tao va dieu khien

void timerStart(TimerState& timer, float gameTime, float turnTime) {
	timer.gameTimeLeft = gameTime;
	timer.turnTimeLeft = turnTime;
	timer.isRunning = true;
}

bool timerUpdate(TimerState& timer, float deltaTime) {
	if (!timer.isRunning) return true;
	timer.gameTimeLeft -= deltaTime;
	timer.turnTimeLeft -= deltaTime;
	if (timer.gameTimeLeft < 0) timer.gameTimeLeft = 0;
	if (timer.turnTimeLeft < 0) timer.turnTimeLeft = 0;

	return timer.gameTimeLeft > 0;
}

void timerResetTurn(TimerState& timer) {
	timer.turnTimeLeft = MAX_TURN_TIME;
}

void timerPause(TimerState& timer) {
	timer.isRunning = false;
}

void timerResume(TimerState& timer) {
	timer.isRunning = true;
}

// Cac ham truy van trang thai 

float timerGetTurnPercent(const TimerState& timer) {
	return timer.turnTimeLeft / MAX_TURN_TIME;
}

float timerGetGamePercent(const TimerState& timer) {
	return timer.gameTimeLeft / MAX_GAME_TIME;
}

bool timerIsTurnExpired(const TimerState& timer) {
	return timer.turnTimeLeft <= 0;
}

bool timerIsGameExpired(const TimerState& timer) {
	return timer.gameTimeLeft <= 0;
}


float timerGetTurnSecondsLeft(const TimerState& timer) {
	return timer.turnTimeLeft;
}


float timerGetGameSecondsLeft(const TimerState& timer) {
	return timer.gameTimeLeft;
}