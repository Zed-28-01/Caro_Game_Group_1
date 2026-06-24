#pragma once
#ifndef SOUND_H
#define SOUND_H

#include "game_types.h"

// NHAC NEN

// V2 #27: track nhac nen theo man (menu vs gameplay)
enum BGMTrack { BGM_MENU = 0, BGM_GAME = 1 };

// Load cac SoundBuffer SFX
// Goi 1 lan trong main() sau renderLoadResources.
void soundLoadResources(GameResources& res);

// Dung va go buffer khoi soundPool truoc khi thoat an toan
// Goi o cuoi main().
void soundShutdown();

// V2 #27: chuyen sang track BGM tuong ung (mo file + loop + play).
// Goi lap lai cung track = no-op (khong mo lai) -> an toan goi moi frame.
void soundPlayBGMTrack(GameResources& res, int track);

// Thay doi am luong nhac nen (0 - 100)
void soundSetBGMVolume(GameResources& res, int volume);

// HIEU UNG AM THANH

// Phat am thanh dat quan co
void soundPlayPlace(GameResources& res);

// Phat am thanh chon menu
void soundPlaySelect(GameResources& res);

// Phat am thanh thang
void soundPlayWin(GameResources& res);

// Phat am thanh hoa
void soundPlayDraw(GameResources& res);

// V2 #27: am thanh hoan tac (undo), goi y (hint), chuong canh bao het gio luot
void soundPlayUndo(GameResources& res);
void soundPlayHint(GameResources& res);
void soundPlayAlarm(GameResources& res);

// Bat/tat toan bo hieu ung am thanh
void soundSetSFXEnabled(bool enabled);

// Kiem tra hieu ung am thanh co bat khong
bool soundIsSFXEnabled();

// Lay am luong BGM hien tai (0-100)
int soundGetBGMVolume();

// LUU / TAI SETTINGS

// Doc settings.txt va apply vao language/sound modules
// Goi 1 lan trong main() sau khi loadResources
void settingsLoad(GameResources& res);

// Ghi settings hien tai (language, bgm volume, sfx on/off) ra file
// Goi sau moi lan user thay doi trong Settings menu
void settingsSave();

#endif // SOUND_H
