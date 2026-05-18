#pragma once
#ifndef SOUND_H
#define SOUND_H

#include "game_types.h"

// ============================================================
// KHOI TAO
// ============================================================

// Load tat ca file am thanh tu assets/sounds/
// Tra ve true neu load thanh cong
bool soundLoadAll(GameResources& res);

// ============================================================
// NHAC NEN
// ============================================================

// Bat/tat nhac nen
void soundPlayBGM(GameResources& res, bool play);

// Thay doi am luong nhac nen (0 - 100)
void soundSetBGMVolume(GameResources& res, int volume);

// ============================================================
// HIEU UNG AM THANH
// ============================================================

// Phat am thanh di chuyen cursor
void soundPlayMove(GameResources& res);

// Phat am thanh dat quan co
void soundPlayPlace(GameResources& res);

// Phat am thanh chon menu
void soundPlaySelect(GameResources& res);

// Phat am thanh thang
void soundPlayWin(GameResources& res);

// Phat am thanh hoa
void soundPlayDraw(GameResources& res);

// Bat/tat toan bo hieu ung am thanh
void soundSetSFXEnabled(bool enabled);

// Kiem tra hieu ung am thanh co bat khong
bool soundIsSFXEnabled();

// Lay am luong BGM hien tai (0-100)
int soundGetBGMVolume();

// ============================================================
// LUU / TAI SETTINGS (persist giua cac lan chay game)
// ============================================================

// Doc settings.txt va apply vao language/sound modules
// Goi 1 lan trong main() sau khi loadResources
void settingsLoad(GameResources& res);

// Ghi settings hien tai (language, bgm volume, sfx on/off) ra file
// Goi sau moi lan user thay doi trong Settings menu
void settingsSave();

#endif // SOUND_H
