#include <stdexcept>

#include "application.h"
#include "FPS/fps.h"

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

  ///////////////////////////////////////////

  struct PositionComponent
  {
    int x;
    int y;

    void print()
    {
      printf("PositionComponent  x:%i, y:%i\n", x, y);
    }

    ~PositionComponent()
    {
      printf("PositionComponent was destroyed\n");
    }
  };

  struct RectComponent
  {
    int width;
    int height;

    void print()
    {
      printf("RectComponent  w:%i, h:%i\n", width, height);
    }

    ~RectComponent()
    {
      printf("RectComponent was destroyed\n");
    }
  };

  Registry registry;
  Entity *entity = registry.createEntity();

  entity->add<RectComponent>(100, 200);

  PositionComponent *pos = entity->add<PositionComponent>(10, 20);
  pos->x = -99;
  entity->add<RectComponent>(99, 88);

  for (auto component : entity->get<PositionComponent>())
  {
    component->print();
    component->y = 88;
  }

  for (auto component : registry.get<PositionComponent>())
    component->print();

  printf("HAS: %i\n", entity->has<PositionComponent>());

  registry.free<PositionComponent, RectComponent>();
  // entity->free<RectComponent, PositionComponent>();

  while (!quit)
  {
    applicationFPS.at(0);

    float deltaTime = applicationFPS.getDeltaTime();

    while (SDL_PollEvent(&event) != 0)
    {
      if (event.type == SDL_EVENT_QUIT)
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
