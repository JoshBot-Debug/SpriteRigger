#include <stdexcept>

#include "application.h"
#include "FPS/fps.h"

const SDL_WindowFlags WINDOW_FLAGS = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_HIDDEN);

Application::Application()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
    throw std::runtime_error(SDL_GetError());

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

  SDL_DestroyRenderer(this->renderer);
  SDL_DestroyWindow(this->window);
  SDL_QuitSubSystem(WINDOW_FLAGS);
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

void Application::setTheme(Theme theme)
{
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

  bool quit = false;
  Uint64 lastTime = SDL_GetTicks();
  SDL_Event event;
  FPS applicationFPS;

  this->onInitialize();
  ImGuiIO &io = ImGui::GetIO();

  while (!quit)
  {
    applicationFPS.at(0);

    float deltaTime = applicationFPS.getDeltaTime();

    while (SDL_PollEvent(&event) != 0)
    {
      ImGui_ImplSDL3_ProcessEvent(&event);

      if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window)))
      {
        quit = true;
        break;
      }

      if (SDL_GetWindowFlags(this->window) & SDL_WINDOW_MINIMIZED)
      {
        SDL_Delay(10);
        continue;
      }

      if (!io.WantCaptureMouse && !io.WantCaptureKeyboard)
        this->onInput(&event, deltaTime);
    }

    this->onUpdate(deltaTime);

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

    this->onDraw(deltaTime);

    ImGui::Render();
    auto [r, g, b, a] = this->backgroundColor;
    SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
    SDL_RenderClear(renderer);

    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
  }

  this->onCleanUp();
}
