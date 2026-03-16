#include <SFML/Graphics.hpp>
#include "../include/render.h"
#include "../include/game_types.h"
void renderAbout(sf::RenderWindow& window, const GameResources& res) {
    // Vẽ lớp nền tối
    sf::RectangleShape fade(sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT));
    fade.setFillColor(sf::Color(0, 0, 0, 220));
    window.draw(fade);

    // Vẽ tiêu đề
    sf::Text title("ABOUT US", res.titleFont, 60);
    title.setFillColor(COLOR_HIGHLIGHT);
    title.setPosition(480.0f, 80.0f);
    window.draw(title);

    // Thông tin nhóm
    sf::Text content;
    content.setFont(res.mainFont);
    content.setCharacterSize(25);
    content.setFillColor(COLOR_MENU_TEXT);

    std::string info = "Group: 1\n\n"
        "24120232: Nguyen Van Toan\n"
        "24120030: Do Quoc Dai\n"
        "24120128: Vu Minh Quang\n"
        "24120209: Van Hoang Nhat\n"
        "24120374: Ho Van Luc\n\n"
        "Instructor: Truong Toan Thinh\n"
        "Source code: github.com/Zed-28-01/Caro_Game_Group_1";

    content.setString(info);
    content.setPosition(350.0f, 180.0f);
    window.draw(content);

    // Hướng dẫn
    sf::Text guide("Press ESC or B to go back", res.mainFont, 20);
    guide.setFillColor(sf::Color(150, 150, 150));
    guide.setPosition(500.0f, 650.0f);
    window.draw(guide);
}

GameScreen handleAbout(sf::RenderWindow& window, const GameResources& res) {
    sf::Event event;
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return SCREEN_ABOUT;
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::B) {
                    return SCREEN_MAIN_MENU;
                }
            }
        }

        window.clear(COLOR_MENU_BG);
        renderAbout(window, res);
        window.display();
    }
    return SCREEN_ABOUT;
}