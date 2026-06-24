#include "menu.h"
#include "render.h"
#include "sound.h"
#include <cstdlib>
#include <ctime>

int main() {
  std::srand((unsigned)std::time(nullptr));

  sf::RenderWindow window(
      sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Caro Game - Group 1",
      sf::Style::Close | sf::Style::Resize);
  window.setFramerateLimit(60);

  applyLetterbox(window, WINDOW_WIDTH, WINDOW_HEIGHT);

  GameResources res;
  if (!renderLoadResources(res)) {
    return -1;
  }

  soundLoadResources(res);

  settingsLoad(res);

  gameRun(window, res);

  soundShutdown();

  return 0;
}
