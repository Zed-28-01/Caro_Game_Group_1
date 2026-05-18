#include "sound.h"

// ============================================================
// BIEN STATIC NOI BO
// ============================================================
// Pool 8 sf::Sound de phat nhieu am thanh chong cheo nhau
// sf::Sound PHAI ton tai trong khi phat → dung mang static
static sf::Sound soundPool[8];
static int soundIndex = 0;
static bool sfxEnabled = true;
static int bgmVolume = 50;

// ============================================================
// KHOI TAO
// ============================================================
bool soundLoadAll(GameResources& res) {
    // SoundBuffer + Music da duoc load trong renderLoadResources()
    // Ham nay setup BGM ban dau (loop, volume mac dinh)
    res.bgMusic.setLoop(true);
    res.bgMusic.setVolume((float)bgmVolume);
    return true;
}

// ============================================================
// NHAC NEN (BGM)
// ============================================================
void soundPlayBGM(GameResources& res, bool play) {
    if (play) {
        if (res.bgMusic.getStatus() != sf::SoundSource::Playing)
            res.bgMusic.play();
    } else {
        res.bgMusic.pause();
    }
}

void soundSetBGMVolume(GameResources& res, int volume) {
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;
    bgmVolume = volume;
    res.bgMusic.setVolume((float)volume);
}

// ============================================================
// HIEU UNG AM THANH (SFX)
// ============================================================

// Ham noi bo: phat 1 sfx tu pool
static void playSfx(const sf::SoundBuffer& buffer) {
    if (!sfxEnabled) return;
    if (buffer.getSampleCount() == 0) return; // file chua load → bo qua

    soundPool[soundIndex].setBuffer(buffer);
    soundPool[soundIndex].play();
    soundIndex = (soundIndex + 1) % 8;
}

void soundPlayMove(GameResources& res)   { playSfx(res.moveSfx); }
void soundPlayPlace(GameResources& res)  { playSfx(res.placeSfx); }
void soundPlaySelect(GameResources& res) { playSfx(res.menuSfx); }
void soundPlayWin(GameResources& res)    { playSfx(res.winSfx); }
void soundPlayDraw(GameResources& res)   { playSfx(res.drawSfx); }

void soundSetSFXEnabled(bool enabled) { sfxEnabled = enabled; }
bool soundIsSFXEnabled() { return sfxEnabled; }
