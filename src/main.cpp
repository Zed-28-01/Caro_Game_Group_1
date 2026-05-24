#include "menu.h"
#include "render.h"
#include "sound.h"
#include <cstdlib>
#include <ctime>

int main() {
    // Seed random cho bot Easy (neu khong se luon cho cung 1 ket qua)
    std::srand((unsigned)std::time(nullptr));

    // Window hien thi co nut Maximize + co the resize keo gop
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Caro Game - Group 1",
        sf::Style::Close | sf::Style::Resize);
    window.setFramerateLimit(60);

    // Khoi tao letterbox view (giu ty le 16:9 khi resize)
    applyLetterbox(window, WINDOW_WIDTH, WINDOW_HEIGHT);

    GameResources res;
    if (!renderLoadResources(res)) {
        return -1; // Khong load duoc font → thoat
    }

    // Load settings (language, volume, sfx) tu file - phai goi SAU loadResources
    settingsLoad(res);

    gameRun(window, res);

    return 0;
}
