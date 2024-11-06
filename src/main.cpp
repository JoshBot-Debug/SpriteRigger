#include "SDL3/SDL.h"
#include "start.h"
#include "app.h"

static const ImWchar GlyphRange[] = {
    0xf000,
    0xf8ff,
    0xe000,
    0xefff,
    0,
};

int main(int argc, char *argv[])
{
  ProjectManager projectManager("SpriteRigger.project", "SpriteRigger.recent");

  Start *start = new Start(&projectManager);

  start->setWindowTitle("Sprite Rigger");
  start->setVSync(1);
  start->setWindowDimensions({720, 480});
  start->setBackgroundColor({50, 50, 50, 255});
  start->addFont("Roboto-Regular:18", "assets/fonts/Roboto/Roboto-Regular.ttf", 18);
  start->addFont("Roboto-Regular:24", "assets/fonts/Roboto/Roboto-Regular.ttf", 24);
  start->addFont("MaterialIcons:18", "assets/fonts/MaterialIcons/MaterialIcons.ttf", 18, GlyphRange);
  start->setTheme(Theme::DARK);
  start->open();

  delete start;

  if (!projectManager.isReady())
    return 0;

  App *app = new App(&projectManager);
  app->setWindowTitle("Sprite Rigger");
  app->setVSync(1);
  app->setWindowDimensions({1080, 720});
  app->setBackgroundColor({30, 30, 30, 255});
  app->addFont("Roboto-Regular:18", "assets/fonts/Roboto/Roboto-Regular.ttf", 18);
  app->addFont("Roboto-Regular:24", "assets/fonts/Roboto/Roboto-Regular.ttf", 24);
  app->addFont("MaterialIcons:18", "assets/fonts/MaterialIcons/MaterialIcons.ttf", 18, GlyphRange);
  app->setTheme(Theme::DARK);
  app->open();

  return 0;
}