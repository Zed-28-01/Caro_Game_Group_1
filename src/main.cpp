#include "menu.h"
#include "render.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Caro Game - Group 1",
        sf::Style::Close);
    window.setFramerateLimit(60);

    GameResources res;
    if (!renderLoadResources(res)) {
        return -1; // Khong load duoc font → thoat
    }

    gameRun(window, res);

    return 0;
}
