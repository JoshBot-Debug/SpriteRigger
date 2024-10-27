#include "application.h"

Application::~Application()
{
  SDL_Quit();
}

void Application::launch()
{
  this->window.open();
  this->render.setWindow(this->window);
  this->loop();
}

void Application::setWindowTitle(const std::string &title)
{
  this->window.setTitle(title);
}

void Application::setWindowDimensions(int w, int h)
{
  this->window.setDimensions(w, h);
}

void Application::loop()
{
  bool quit = false;
  SDL_Event e;

  while (!quit)
  {
    float deltaTime = frame_rate.start();

    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_EVENT_QUIT)
      {
        quit = true;
        break;
      }

      this->onInput(e);
    }

    this->onDraw(deltaTime);
    this->frame_rate.end();
  }
}