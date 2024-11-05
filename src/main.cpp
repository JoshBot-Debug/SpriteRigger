#include "SDL3/SDL.h"
#include "start.h"
#include "spriteRigger.h"

int main(int argc, char *argv[])
{
  Start *start = new Start();

  start->setWindowTitle("Sprite Rigger");
  start->setVSync(1);
  start->setWindowDimensions({720, 480});
  start->setBackgroundColor({50, 50, 50, 255});
  start->setDefaultFont("assets/fonts/roboto/Roboto-Regular.ttf", 18);
  start->addFont("assets/fonts/roboto/Roboto-Regular.ttf", 22);
  start->setTheme(Theme::DARK);
  start->open();

  std::string projectDirectory = start->getProjectDirectory();

  delete start;

  SpriteRigger app(projectDirectory.c_str());
  app.setWindowTitle("Sprite Rigger");
  app.setVSync(1);
  app.setWindowDimensions({1080, 720});
  app.setBackgroundColor({30, 30, 30, 255});
  app.setDefaultFont("assets/fonts/roboto/Roboto-Regular.ttf", 18);
  app.addFont("assets/fonts/roboto/Roboto-Regular.ttf", 20);
  app.setTheme(Theme::DARK);
  app.open();

  return 0;
}