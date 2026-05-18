#include "sound.h"
#include "language.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

static const std::string SETTINGS_FILE = "../saves/settings.txt";

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
int soundGetBGMVolume() { return bgmVolume; }

// ============================================================
// LUU / TAI SETTINGS
// ============================================================
void settingsLoad(GameResources& res) {
    std::ifstream f(SETTINGS_FILE);
    if (!f.is_open()) return;

    std::string line;
    while (std::getline(f, line)) {
        size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);

        try {
            if (key == "language") {
                langSetCurrent((Language)std::stoi(val));
            }
            else if (key == "bgm_volume") {
                soundSetBGMVolume(res, std::stoi(val));
            }
            else if (key == "sfx_enabled") {
                soundSetSFXEnabled(std::stoi(val) == 1);
            }
        }
        catch (...) { /* bo qua line loi */ }
    }
}

void settingsSave() {
    // Dam bao thu muc saves/ ton tai
    try {
        fs::create_directories("../saves");
    }
    catch (...) {}

    std::ofstream f(SETTINGS_FILE);
    if (!f.is_open()) return;

    f << "language=" << (int)langGetCurrent() << "\n";
    f << "bgm_volume=" << bgmVolume << "\n";
    f << "sfx_enabled=" << (sfxEnabled ? 1 : 0) << "\n";
}
