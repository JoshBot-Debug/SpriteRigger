#include <stdexcept>

#include "application.h"
#include "FPS/fps.h"

const char *getDriverName(RendererDriver driver)
{
  switch (driver)
  {
  case RendererDriver::VULKAN:
    return "vulkan";
  case RendererDriver::OPENGL:
    return "opengl";
  case RendererDriver::SOFTWARE:
    return "software";
  default:
    return NULL;
  }
}

Application::Application(RendererDriver driver)
{
  if (!SDL_Init(SDL_INIT_VIDEO))
    throw std::runtime_error(SDL_GetError());

  this->window = SDL_CreateWindow("Application", 640, 480, SDL_WINDOW_HIDDEN);
  if (this->window == NULL)
    throw std::runtime_error(SDL_GetError());

  this->renderer = SDL_CreateRenderer(this->window, getDriverName(driver));
  if (this->renderer == NULL)
    throw std::runtime_error(SDL_GetError());

  this->textManager = new TextManager(this->renderer);
}

Application::~Application()
{
  delete this->textManager;

  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);
  TTF_Quit();
  SDL_QuitSubSystem(SDL_INIT_VIDEO);
  SDL_Quit();
}

void Application::setVSync(int vsync)
{
  if (!SDL_SetRenderVSync(renderer, vsync))
    throw std::runtime_error(SDL_GetError());
}

void Application::setWindowDimensions(int w, int h)
{
  if (!SDL_SetWindowSize(this->window, w, h))
    throw std::runtime_error(SDL_GetError());
}

void Application::setWindowTitle(const char *title)
{
  if (!SDL_SetWindowTitle(this->window, title))
    throw std::runtime_error(SDL_GetError());
}

void Application::setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  this->backgroundColor = {r, g, b, a};
}

void Application::open()
{
  SDL_ShowWindow(this->window);

  bool quit = false;
  Uint64 lastTime = SDL_GetTicks();
  SDL_Event event;
  FPS applicationFPS;

  this->onInitialize();

  while (!quit)
  {
    applicationFPS.at(0);

    float deltaTime = applicationFPS.getDeltaTime();

    while (SDL_PollEvent(&event) != 0)
    {
      if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window)))
      {
        quit = true;
        break;
      }

      this->onInput(&event, deltaTime);
    }

    this->onUpdate(deltaTime);

    auto [r, g, b, a] = this->backgroundColor;
    SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    SDL_RenderClear(this->renderer);

    this->onDraw(deltaTime);

    SDL_RenderPresent(renderer);
  }

  this->onCleanUp();
}
