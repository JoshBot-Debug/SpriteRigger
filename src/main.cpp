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
  start->addFont("Roboto-Regular:18", "assets/fonts/Roboto/Roboto-Regular.ttf", 18);
  start->addFont("Roboto-Regular:24", "assets/fonts/Roboto/Roboto-Regular.ttf", 24);
  start->addFont("FontAwesome", "assets/fonts/FontAwesome/Font-Awesome-6-Free-Regular-400.otf");
  start->setTheme(Theme::DARK);
  start->open();

  std::string projectDirectory = start->getProjectDirectory();

  delete start;

  if (projectDirectory.empty())
    return 0;

  SpriteRigger *app = new SpriteRigger(projectDirectory.c_str());
  app->setWindowTitle("Sprite Rigger");
  app->setVSync(1);
  app->setWindowDimensions({1080, 720});
  app->setBackgroundColor({30, 30, 30, 255});
  app->addFont("Roboto-Regular:18", "assets/fonts/Roboto/Roboto-Regular.ttf", 18);
  app->addFont("Roboto-Regular:24", "assets/fonts/Roboto/Roboto-Regular.ttf", 24);
  app->addFont("FontAwesome", "assets/fonts/FontAwesome/Font-Awesome-6-Free-Regular-400.otf");
  app->setTheme(Theme::DARK);
  app->open();

  return 0;
}