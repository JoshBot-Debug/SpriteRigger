#include "SDL3/SDL.h"
#include "SpriteSkel/spriteSkel.h"

int main(int argc, char *argv[])
{
  SpriteSkel spriteSkel;

  spriteSkel.setWindowTitle("SpriteSkel");
  spriteSkel.setVSync(1);
  spriteSkel.setWindowDimensions(800, 600);
  spriteSkel.setBackgroundColor(255, 255, 255, SDL_ALPHA_OPAQUE);
  spriteSkel.setDefaultFont("assets/fonts/roboto/Roboto-Regular.ttf", 18);
  spriteSkel.addFont("assets/fonts/roboto/Roboto-Regular.ttf", 20);
  spriteSkel.setTheme(Theme::DARK);
  spriteSkel.open();

  return 0;
}