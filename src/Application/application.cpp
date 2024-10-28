#include "application.h"

Application::Application() : window(), render(), frame_rate(), mouse_event_handler() {}

Application::~Application()
{
  SDL_Quit();
}

void Application::launch(SDL_Color backgroundColor)
{
  this->window.open();
  this->render.setWindow(this->window, backgroundColor);
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

void Application::onDraw(float deltaTime)
{
  this->mouse_event_handler.onEventLoopStart();
}

void Application::onInput(SDL_Event event)
{
  this->mouse_event_handler.onEvent(event);
}