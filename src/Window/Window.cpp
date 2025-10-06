#include "Window.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Input.h"

#include "Fonts/RobotoBold.embed"
#include "Fonts/RobotoLight.embed"
#include "Fonts/RobotoLightItalic.embed"
#include "Fonts/RobotoMedium.embed"
#include "Fonts/RobotoRegular.embed"

static std::unordered_map<std::string, ImFont *> s_Fonts;

void ErrorCallback(int error, const char *description) {
  std::cerr << "GLFW Error " << error << ":" << description << std::endl;
}

void GLAPIENTRY DebugCallback(GLenum source, GLenum type, GLuint id,
                              GLenum severity, GLsizei length,
                              const GLchar *message, const void *userParam) {
  (void)length;
  (void)userParam;
  std::cerr << "OpenGL Debug Message (" << id << "): " << message << std::endl;
  std::cerr << "Source: " << source << ", Type: " << type
            << ", Severity: " << severity << std::endl;
}

static void SetFrameBufferSize(GLFWwindow *window, int w, int h) {
  (void)window;
  glViewport(0, 0, w, h);
}

static void glDebug() {
#ifdef DEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

  glDebugMessageCallback(DebugCallback, nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                        GL_TRUE);
#endif
}

void Window::Run() {
  if (m_Running)
    return;

  m_Running = true;

  glfwShowWindow(s_Window);

  if (m_Options.maximized)
    glfwMaximizeWindow(s_Window);

  auto &shortcutManager = ShortcutManager::Instance();

  while (!glfwWindowShouldClose(s_Window) && m_Running) {

    glfwPollEvents();

    shortcutManager.PollEvents();

    if (glfwGetWindowAttrib(s_Window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    for (auto &layer : m_Layers)
      layer->OnUpdate(m_TimeStep);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
      static ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;

      // We are using the ImGuiWindowFlags_NoDocking flag to make the parent
      // window not dockable into, because it would be confusing to have two
      // docking targets within each others.
      ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking;
      if (m_Menubar)
        windowFlags |= ImGuiWindowFlags_MenuBar;

      const ImGuiViewport *viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->WorkPos);
      ImGui::SetNextWindowSize(viewport->WorkSize);
      ImGui::SetNextWindowViewport(viewport->ID);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      windowFlags |=
          ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

      // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will
      // render our background and handle the pass-thru hole, so we ask Begin()
      // to not render a background.
      if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        windowFlags |= ImGuiWindowFlags_NoBackground;

      // Important: note that we proceed even if Begin() returns false (aka
      // window is collapsed). This is because we want to keep our DockSpace()
      // active. If a DockSpace() is inactive, all active windows docked into it
      // will lose their parent and become undocked. We cannot preserve the
      // docking relationship between an active window and an inactive docking,
      // otherwise any change of dockspace/settings would lead to windows being
      // stuck in limbo and never being visible.
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
      ImGui::Begin("Window", nullptr, windowFlags);

      ImGui::PopStyleVar(3);

      // Submit the DockSpace
      ImGuiIO &io = ImGui::GetIO();
      if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspaceId = ImGui::GetID("WindowDockspace");
        ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
      }

      if (m_Menubar)
        if (ImGui::BeginMenuBar()) {
          m_Menubar();
          ImGui::EndMenuBar();
        }

      for (auto &layer : m_Layers)
        layer->OnRender();

      ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(s_Window);

    double time = glfwGetTime();
    m_FrameTime = time - m_LastFrameTime;
    m_TimeStep = glm::min<double>(m_FrameTime, 0.0333f);
    m_LastFrameTime = time;
    s_Scroll.x = 0.0f;
    s_Scroll.y = 0.0f;
  }
}

void Window::Quit() { m_Running = false; }

ImFont *Window::GetFont(const std::string &name) { return s_Fonts.at(name); }

void Window::AddFont(const std::string &filepath, const std::string &name,
                     float fontSize, const ImWchar *glyphRanges) {
  ImGuiIO &io = ImGui::GetIO();
  s_Fonts.emplace(name, io.Fonts->AddFontFromFileTTF(filepath.c_str(), fontSize,
                                                     NULL, glyphRanges));
}

glm::vec2 Window::GetMouseScroll() { return s_Scroll; }

glm::vec2 Window::GetMousePosition() {
  double x, y;
  glfwGetCursorPos(s_Window, &x, &y);
  return glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

bool Window::GetKey(const KeyboardKey &key, const KeyAction &action) {
  return glfwGetKey(s_Window, static_cast<int>(key)) ==
         static_cast<int>(action);
}

bool Window::GetMouseButton(const MouseButton &key, const KeyAction &action) {
  return glfwGetMouseButton(s_Window, static_cast<int>(key)) ==
         static_cast<int>(action);
}

void Window::RegisterShortcut(const ShortcutManager::Shortcut &shortcut) {
  ShortcutManager::Instance().Register(shortcut);
}

void Window::GenerateFrameBuffer(const ImVec2 &viewport, GLuint &frameBuffer,
                                 GLuint &depthBuffer, GLuint &colorAttachment) {
  if (frameBuffer != 0)
    glDeleteFramebuffers(1, &frameBuffer);

  if (colorAttachment != 0)
    glDeleteTextures(1, &colorAttachment);

  if (depthBuffer != 0)
    glDeleteRenderbuffers(1, &depthBuffer);

  glGenFramebuffers(1, &frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

  // Color attachment
  glGenTextures(1, &colorAttachment);
  glBindTexture(GL_TEXTURE_2D, colorAttachment);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, viewport.x, viewport.y, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         colorAttachment, 0);

  // Depth buffer
  glGenRenderbuffers(1, &depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, viewport.x,
                        viewport.y);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, depthBuffer);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cerr << "Framebuffer not complete!" << std::endl;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Window::Window(const Window::Options &options) : m_Options(options) {
  ShortcutManager::Instance().SetWindow(this);

  const char *glsl_version = "#version 330 core";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

  glfwWindowHint(GLFW_DECORATED,
                 options.enableTitleBar ? GLFW_TRUE : GLFW_FALSE);

  glfwWindowHint(GLFW_SAMPLES, options.MSAA);

  const GLFWvidmode *screen = glfwGetVideoMode(glfwGetPrimaryMonitor());

  s_Window = glfwCreateWindow(options.width ? options.width : screen->width,
                              options.height ? options.height : screen->height,
                              options.title.c_str(), nullptr, nullptr);
  if (!s_Window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create a window!");
  }

  glfwMakeContextCurrent(s_Window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    glfwTerminate();
    throw std::runtime_error("Failed to initialize GLAD");
  }

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  glfwSetFramebufferSizeCallback(s_Window, SetFrameBufferSize);

  glDebug();

  glfwSwapInterval(options.enableVSync ? 1 : 0);

  if (options.enableDepth)
    glEnable(GL_DEPTH_TEST);

  int windowWidth, windowHeight;
  glfwGetWindowSize(s_Window, &windowWidth, &windowHeight);
  glfwSetWindowPos(s_Window, (screen->width - windowWidth) / 2,
                   (screen->height - windowHeight) / 2);

  IMGUI_CHECKVERSION();

  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();

  if (options.imguiEnableKeyboardNavigation)
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  if (options.imguiEnableGamepad)
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
  if (options.imguiEnableDocking)
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  if (options.darkMode)
    ImGui::StyleColorsDark();
  else
    ImGui::StyleColorsLight();

  ImGui_ImplGlfw_InitForOpenGL(s_Window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Load default fonts
  {
    ImFontConfig fontConfig;
    fontConfig.FontDataOwnedByAtlas = false;

    auto regularFont = s_Fonts.emplace(
        "RobotoRegular", io.Fonts->AddFontFromMemoryTTF((void *)g_RobotoRegular,
                                                        sizeof(g_RobotoRegular),
                                                        0.0f, &fontConfig));
    s_Fonts.emplace("RobotoBold", io.Fonts->AddFontFromMemoryTTF(
                                      (void *)g_RobotoBold,
                                      sizeof(g_RobotoBold), 0.0f, &fontConfig));
    s_Fonts.emplace("RobotoMedium",
                    io.Fonts->AddFontFromMemoryTTF((void *)g_RobotoMedium,
                                                   sizeof(g_RobotoMedium), 0.0f,
                                                   &fontConfig));
    s_Fonts.emplace("RobotoLight",
                    io.Fonts->AddFontFromMemoryTTF((void *)g_RobotoLight,
                                                   sizeof(g_RobotoLight), 0.0f,
                                                   &fontConfig));
    s_Fonts.emplace("RobotoLightItalic",
                    io.Fonts->AddFontFromMemoryTTF((void *)g_RobotoLightItalic,
                                                   sizeof(g_RobotoLightItalic),
                                                   0.0f, &fontConfig));

    io.FontDefault = regularFont.first->second;
  }

  glfwSetScrollCallback(s_Window, ScrollCallback);
}

void Window::ScrollCallback(GLFWwindow *window, double xoffset,
                            double yoffset) {
  s_Scroll.x += static_cast<float>(xoffset);
  s_Scroll.y += static_cast<float>(yoffset);
  ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

Window::~Window() {
  for (auto &layer : m_Layers)
    layer->OnDetach();

  m_Layers.clear();
  s_Fonts.clear();
  m_Menubar = nullptr;
  ShortcutManager::Instance().Free();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(s_Window);

  s_Window = nullptr;
  s_Scroll = glm::vec2(0.0f);
}