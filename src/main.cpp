#include "SDL3/SDL.h"
#include "spriteRigger.h"

int main(int argc, char *argv[])
{
  SpriteRigger SpriteRigger;

  SpriteRigger.setWindowTitle("SpriteRigger");
  SpriteRigger.setVSync(1);
  SpriteRigger.setWindowDimensions({800, 600});
  SpriteRigger.setBackgroundColor({30, 30, 30, 255});
  SpriteRigger.setDefaultFont("assets/fonts/roboto/Roboto-Regular.ttf", 18);
  SpriteRigger.addFont("assets/fonts/roboto/Roboto-Regular.ttf", 20);
  SpriteRigger.setTheme(Theme::DARK);
  SpriteRigger.open();

  return 0;
}