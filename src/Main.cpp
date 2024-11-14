#include "SDL3/SDL.h"
#include "Start.h"
#include "App.h"
#include "Project/Project.h"

#include "StateSerializer/StateSerializer.h"

static const ImWchar GlyphRange[] = {
    0xf000,
    0xf8ff,
    0xe000,
    0xefff,
    0,
};

int main(int argc, char *argv[])
{
  Project project;
  project.recent.setSaveFile((std::filesystem::current_path() / "SpriteRigger.recent").string());
  project.recent.read();

  Start *start = new Start(&project);
  start->setWindowTitle("Sprite Rigger");
  start->setVSync(1);
  start->setWindowDimensions({720, 480});
  start->setBackgroundColor({0.2f, 0.2f, 0.2f, 1.0f});
  start->addFont("Roboto-Regular:18", "assets/fonts/Roboto/Roboto-Regular.ttf", 18);
  start->addFont("Roboto-Regular:24", "assets/fonts/Roboto/Roboto-Regular.ttf", 24);
  start->addFont("MaterialIcons:18", "assets/fonts/MaterialIcons/MaterialIcons.ttf", 18, GlyphRange);
  start->setTheme(Theme::DARK);
  start->open();

  delete start;

  while (project.isRunning())
  {
    auto fileName = *project.state.getSaveFileName();
    App *app = new App(&project);
    app->setWindowTitle((fileName + " - Sprite Rigger").c_str());
    app->setVSync(1);
    app->setWindowDimensions({1080, 720});
    app->setBackgroundColor({0.12f, 0.12f, 0.12f, 1.0f});
    app->addFont("Roboto-Regular:18", "assets/fonts/Roboto/Roboto-Regular.ttf", 18);
    app->addFont("Roboto-Regular:24", "assets/fonts/Roboto/Roboto-Regular.ttf", 24);
    app->addFont("MaterialIcons:18", "assets/fonts/MaterialIcons/MaterialIcons.ttf", 18, GlyphRange);
    app->setTheme(Theme::DARK);
    app->open();

    delete app;
  }
  return 0;
}