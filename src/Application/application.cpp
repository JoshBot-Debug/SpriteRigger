#include <stdexcept>

#include "application.h"

Application::Application()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
    throw std::runtime_error(SDL_GetError());

  this->window = SDL_CreateWindow("Application", 640, 480, SDL_WINDOW_HIDDEN);
  if (this->window == NULL)
    throw std::runtime_error(SDL_GetError());

  this->renderer = SDL_CreateRenderer(this->window, NULL);
  if (this->renderer == NULL)
    throw std::runtime_error(SDL_GetError());

  this->textManager = new TextManager(this->renderer);

  this->fpsManager = new FPSManager();
}

Application::~Application()
{
  delete this->displayFPS;
  delete this->textManager;
  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);
  TTF_Quit();
  SDL_Quit();
}

FPSManager *Application::getFPSManager()
{
  return this->fpsManager;
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
  this->backgroundColor.r = r;
  this->backgroundColor.g = g;
  this->backgroundColor.b = b;
  this->backgroundColor.a = a;
}

void Application::showFPS(const char *fontName, const char *textName, const char *file, float ptsize, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  this->displayFPS = new Text;
  this->displayFPS->textName = textName;
  this->displayFPS->x = 0;
  this->displayFPS->y = 0;

  this->textManager->createFont(fontName, file, ptsize);
  this->textManager->createText(textName, fontName);
  this->textManager->setTextColor(textName, r, g, b, a);
}

void Application::open()
{
  SDL_ShowWindow(this->window);

  bool quit = false;
  SDL_Event e;

  while (!quit)
  {
    float deltaTime = this->fpsManager->update();

    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_EVENT_QUIT)
      {
        quit = true;
        break;
      }

      this->onInput(&e, deltaTime);
    }

    this->onUpdate(deltaTime);

    SDL_SetRenderDrawColor(this->renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
    SDL_RenderClear(this->renderer);

    if (this->displayFPS)
    {
      std::string fps = std::string("FPS: ") + std::to_string((int)(1 / deltaTime));
      this->displayFPS->content = fps.c_str();
      this->displayFPS->length = strlen(this->displayFPS->content);
      this->textManager->render(this->displayFPS);
    }

    this->onDraw(deltaTime);

    SDL_RenderPresent(renderer);
  }
}
