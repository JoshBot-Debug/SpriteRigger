#include <stdexcept>

#include "application.h"

const SDL_WindowFlags WINDOW_FLAGS = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_HIDDEN);

Application::Application()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
    throw std::runtime_error(SDL_GetError());

  if (NFD_Init() != NFD_OKAY)
    throw std::runtime_error(NFD_GetError());

  this->window = SDL_CreateWindow("Application", 1920, 1080, WINDOW_FLAGS);
  if (this->window == NULL)
    throw std::runtime_error(SDL_GetError());

  this->renderer = SDL_CreateRenderer(this->window, nullptr);
  if (this->renderer == NULL)
    throw std::runtime_error(SDL_GetError());

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui::StyleColorsLight();

  ImGui_ImplSDL3_InitForSDLRenderer(this->window, this->renderer);
  ImGui_ImplSDLRenderer3_Init(this->renderer);
}

Application::~Application()
{
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  NFD_Quit();
  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);
  SDL_QuitSubSystem(WINDOW_FLAGS);
  SDL_Quit();
}

void Application::quit()
{
  this->isRunning = false;
}

void Application::setVSync(int vsync)
{
  if (!SDL_SetRenderVSync(renderer, vsync))
    throw std::runtime_error(SDL_GetError());
}

void Application::setWindowDimensions(Vec2 size)
{
  if (!SDL_SetWindowSize(this->window, size.x, size.y))
    throw std::runtime_error(SDL_GetError());
}

void Application::setWindowTitle(const char *title)
{
  if (!SDL_SetWindowTitle(this->window, title))
    throw std::runtime_error(SDL_GetError());
}

void Application::setBackgroundColor(Vec4 backgroundColor)
{
  this->backgroundColor = backgroundColor;
}

void Application::setTheme(Theme theme)
{
  this->theme = theme;
  switch (theme)
  {
  case Theme::LIGHT:
    ImGui::StyleColorsLight();
    break;
  case Theme::DARK:
    ImGui::StyleColorsDark();
    break;
  default:
    break;
  }
}

Theme Application::getTheme()
{
  return this->theme;
}

void Application::setDefaultFont(const char *ttfPath, float fontSize)
{
  ImGuiIO &io = ImGui::GetIO();
  io.FontDefault = io.Fonts->AddFontFromFileTTF(ttfPath, fontSize);
}

void Application::addFont(const char *ttfPath, float fontSize)
{
  ImGuiIO &io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF(ttfPath, fontSize);
}

void Application::open()
{
  SDL_ShowWindow(this->window);
  SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  Uint64 lastTime = SDL_GetTicks();
  SDL_Event event;
  FPS applicationFPS;

  this->onInitialize();
  ImGuiIO &io = ImGui::GetIO();

  while (this->isRunning)
  {
    applicationFPS.at(0);

    float deltaTime = applicationFPS.getDeltaTime();

    while (SDL_PollEvent(&event) != 0)
    {
      ImGui_ImplSDL3_ProcessEvent(&event);

      if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window)))
      {
        this->isRunning = false;
        break;
      }

      if (SDL_GetWindowFlags(this->window) & SDL_WINDOW_MINIMIZED)
      {
        SDL_Delay(10);
        continue;
      }

      this->onInput(&event, deltaTime);
    }

    this->onUpdate(deltaTime);

    auto [r, g, b, a] = this->backgroundColor;
    SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    SDL_RenderClear(renderer);

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    this->onDraw(deltaTime);

    ImGui::Render();

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
  }

  this->onCleanUp();
}