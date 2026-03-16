#ifndef ABOUT_H
#define ABOUT_H

#include <SFML/Graphics.hpp>
#include "game_types.h" // Để sử dụng kiểu GameScreen và GameResources

// Hàm vẽ giao diện 
void renderAbout(sf::RenderWindow& window, const GameResources& res);

// Hàm xử lý logic/sự kiện của màn hình 
GameScreen handleAbout(sf::RenderWindow& window, const GameResources& res);

#endif
