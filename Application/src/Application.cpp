#include <stdexcept>
#include "Application.h"

Application::Application(SDL_WindowFlags flags)
{
  if (!SDL_Init(SDL_INIT_VIDEO))
    throw std::runtime_error(SDL_GetError());

  if (NFD_Init() != NFD_OKAY)
    throw std::runtime_error(NFD_GetError());

  const char *glslVersion = "#version 130";
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  this->window = SDL_CreateWindow("Application", 1080, 720, flags);
  if (this->window == NULL)
    throw std::runtime_error(SDL_GetError());

  this->glContext = SDL_GL_CreateContext(window);
  SDL_GL_MakeCurrent(window, this->glContext);

  if (glewInit() != GLEW_OK)
    throw std::runtime_error("Failed to initialize GLEW!");

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  ImGui::StyleColorsLight();

  ImGui_ImplSDL3_InitForOpenGL(window, this->glContext);
  ImGui_ImplOpenGL3_Init(glslVersion);
}

Application::~Application()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DestroyContext(this->glContext);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
  NFD_Quit();
}

void Application::quit()
{
  this->isRunning = false;
}

bool Application::isWindowClosing(SDL_Event *event)
{
  return event->type == SDL_EVENT_QUIT || (event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event->window.windowID == SDL_GetWindowID(this->window));
}

void Application::setVSync(int vsync)
{
  if (!SDL_GL_SetSwapInterval(vsync))
    throw std::runtime_error(SDL_GetError());
}

void Application::setWindowDimensions(glm::vec2 size)
{
  if (!SDL_SetWindowSize(this->window, size.x, size.y))
    throw std::runtime_error(SDL_GetError());
}

glm::vec2 Application::getWindowDimensions()
{
  int gWindowWidth, gWindowHeight;
  SDL_GetWindowSize(this->window, &gWindowWidth, &gWindowHeight);
  return {(float)gWindowWidth, (float)gWindowHeight};
}

void Application::setWindowTitle(const char *title)
{
  if (!SDL_SetWindowTitle(this->window, title))
    throw std::runtime_error(SDL_GetError());
}

void Application::setBackgroundColor(glm::vec4 backgroundColor)
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

void Application::addFont(std::string name, const char *ttfPath, float fontSize, const ImWchar *glyph_ranges)
{
  ImGuiIO &io = ImGui::GetIO();
  ImFont *font = io.Fonts->AddFontFromFileTTF(ttfPath, fontSize, NULL, glyph_ranges);

  if (this->fonts.empty())
    io.FontDefault = font;

  this->fonts[name] = font;
}

ImFont *Application::getFont(const char *name)
{
  return this->fonts.at(name);
}

void Application::open()
{
  bool hidden = true;

  Uint64 lastTime = SDL_GetTicks();
  SDL_Event event;
  FPS fps;

  this->onInitialize();
  ImGuiIO &io = ImGui::GetIO();

  while (this->isRunning)
  {
    fps.at(0);

    float deltaTime = fps.getDeltaTime();

    while (SDL_PollEvent(&event) != 0)
    {
      if (SDL_GetWindowFlags(this->window) & SDL_WINDOW_MINIMIZED)
      {
        SDL_Delay(10);
        continue;
      }

      ImGui_ImplSDL3_ProcessEvent(&event);

      this->onInput(&event, deltaTime);

      if (this->isWindowClosing(&event))
      {
        this->isRunning = false;
        break;
      }
    }

    this->onUpdate(deltaTime);

    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    this->onDraw(deltaTime);

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);

    if (hidden)
    {
      hidden = false;
      SDL_ShowWindow(this->window);
      SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    }
  }

  this->onCleanUp();
}