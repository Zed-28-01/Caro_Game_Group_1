#include "menu.h"
#include "render.h"
#include "sound.h"
#include <cstdlib>
#include <ctime>

int main() {
  std::srand((unsigned)std::time(nullptr));

  // Lệnh yêu cầu window cấp phát cửa sổ 1280 x 720 cùng với các nút close,
  // resize (phóng to màn hình)
  sf::RenderWindow window(
      sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Caro Game - Group 1",
      sf::Style::Close | sf::Style::Resize); // đây là các SFML Style Flags
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
