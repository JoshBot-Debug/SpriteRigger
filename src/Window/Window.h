#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Input.h"
#include "Layer.h"

#include "imgui.h"

#include "ShortcutManager.h"

class Window {
public:
  struct Options {
    std::string title;

    int width = 0;
    int height = 0;

    bool enableDepth = false;
    bool enableVSync = true;
    bool enableTitleBar = true;

    int MSAA = 0;

    bool imguiEnableDocking = true;
    bool imguiEnableKeyboardNavigation = false;
    bool imguiEnableGamepad = false;

    bool darkMode = false;
    bool maximized = false;
  };

private:
  static inline GLFWwindow *s_Window = nullptr;

  static inline glm::vec2 s_Scroll = glm::vec2(0.0f);

  Options m_Options;

  bool m_Running = false;

  double m_TimeStep = 0.0;
  double m_FrameTime = 0.0;
  double m_LastFrameTime = 0.0;

  std::function<void()> m_Menubar = nullptr;

  std::vector<std::shared_ptr<Layer>> m_Layers;

private:
  static void ScrollCallback(GLFWwindow *window, double xoffset,
                             double yoffset);

public:
  Window(const Options &options);
  ~Window();

  template <typename T, typename... Args>
  std::shared_ptr<T> PushLayer(Args &&...args) {
    static_assert(std::is_base_of<Layer, T>::value,
                  "Pushed type is not subclass of Layer!");
    auto layer = std::make_shared<T>(std::forward<Args>(args)...);

    m_Layers.emplace_back(layer);

    layer->SetWindow(this);
    layer->OnAttach();

    return layer;
  }

  void PushLayer(const std::shared_ptr<Layer> &layer) {
    layer->SetWindow(this);
    layer->OnAttach();
    m_Layers.emplace_back(layer);
  }

  void Run();

  void Quit();

  void SetMenubar(const std::function<void()> &callback) {
    m_Menubar = callback;
  }

  static ImFont *GetFont(const std::string &name);
  static void AddFont(const std::string &filepath, const std::string &name,
                      float fontSize = 0.0f,
                      const ImWchar *glyphRanges = nullptr);

  static glm::vec2 GetMouseScroll();
  static glm::vec2 GetMousePosition();
  static bool GetKey(const KeyboardKey &key, const KeyAction &action);
  static bool GetMouseButton(const MouseButton &key, const KeyAction &action);
  static GLFWwindow *GetWindow() { return s_Window; };

  static void RegisterShortcut(const ShortcutManager::Shortcut &shortcut);

  static void GenerateFrameBuffer(const ImVec2 &viewport, GLuint &frameBuffer,
                                  GLuint &depthBuffer, GLuint &colorAttachment);

  static void Init() {
    if (!glfwInit())
      throw std::runtime_error("GLFW initialization failed!");
  };

  static void Shutdown() { glfwTerminate(); }
};

inline ImVec2 operator+(const ImVec2 &lhs, const ImVec2 &rhs) {
  return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}

inline ImVec2 operator-(const ImVec2 &lhs, const ImVec2 &rhs) {
  return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

inline ImVec2 operator*(const ImVec2 &lhs, float rhs) {
  return ImVec2(lhs.x * rhs, lhs.y * rhs);
}

inline ImVec2 operator/(const ImVec2 &lhs, float rhs) {
  return ImVec2(lhs.x / rhs, lhs.y / rhs);
}

inline bool operator==(const ImVec2 &lhs, const ImVec2 &rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator!=(const ImVec2 &lhs, const ImVec2 &rhs) {
  return !(lhs == rhs);
}