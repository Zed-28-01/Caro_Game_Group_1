#include "sound.h"
#include "language.h"
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

static const std::string SETTINGS_FILE = "../saves/settings.txt";

static const int SOUND_POOL_SIZE = 8;
static sf::Sound soundPool[SOUND_POOL_SIZE];
static int soundIndex = 0;
static bool sfxEnabled = true;
static int bgmVolume = 50;

void soundSetBGMVolume(GameResources& res, int volume) {
    if (volume < 0) volume = 0;
    if (volume > 100) volume = 100;
    bgmVolume = volume;
    res.bgMusic.setVolume((float)volume);
}

static int currentBGMTrack = -1;

void soundPlayBGMTrack(GameResources& res, int track) {
    if (track == currentBGMTrack) return;
    const char* file = (track == BGM_GAME) ? "../assets/sounds/bgm_game.ogg"
                                           : "../assets/sounds/bgm_menu.ogg";
    if (!fs::exists(file)) return;
    currentBGMTrack = track;
    res.bgMusic.stop();
    if (res.bgMusic.openFromFile(file)) {
        res.bgMusic.setLoop(true);
        res.bgMusic.setVolume((float)bgmVolume);
        res.bgMusic.play();
    }
}

static void playSfx(const sf::SoundBuffer& buffer) {
    if (!sfxEnabled) return;
    if (buffer.getSampleCount() == 0) return;

    soundPool[soundIndex].setBuffer(buffer);
    soundPool[soundIndex].play();
    soundIndex = (soundIndex + 1) % SOUND_POOL_SIZE;
}

void soundPlayPlace(GameResources& res)  { playSfx(res.placeSfx); }
void soundPlaySelect(GameResources& res) { playSfx(res.menuSfx); }
void soundPlayWin(GameResources& res)    { playSfx(res.winSfx); }
void soundPlayDraw(GameResources& res)   { playSfx(res.drawSfx); }
void soundPlayUndo(GameResources& res)   { playSfx(res.undoSfx); }
void soundPlayHint(GameResources& res)   { playSfx(res.hintSfx); }
void soundPlayAlarm(GameResources& res)  { playSfx(res.alarmSfx); }

void soundSetSFXEnabled(bool enabled) { sfxEnabled = enabled; }
bool soundIsSFXEnabled() { return sfxEnabled; }
int soundGetBGMVolume() { return bgmVolume; }

void soundLoadResources(GameResources& res) {
    res.placeSfx.loadFromFile("../assets/sounds/place.wav");
    res.winSfx.loadFromFile("../assets/sounds/win.wav");
    res.drawSfx.loadFromFile("../assets/sounds/draw.wav");
    res.menuSfx.loadFromFile("../assets/sounds/menu.wav");
    res.undoSfx.loadFromFile("../assets/sounds/undo.ogg");
    res.hintSfx.loadFromFile("../assets/sounds/hint.ogg");
    res.alarmSfx.loadFromFile("../assets/sounds/alarm.wav");
}

void soundShutdown() {
    for (int i = 0; i < SOUND_POOL_SIZE; i++) {
        soundPool[i].stop();
        soundPool[i].resetBuffer();
    }
    currentBGMTrack = -1;
}

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
        catch (...) {}
    }
}

void settingsSave() {
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
