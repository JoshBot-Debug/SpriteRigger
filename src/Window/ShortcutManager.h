#pragma once

#include "imgui.h"
#include <functional>
#include <vector>

struct Window;

class ShortcutManager {

public:
  struct Shortcut {
    bool ctrl = false;
    bool shift = false;
    bool alt = false;
    bool super = false;
    bool repeat = false;
    ImGuiKey key = ImGuiKey_None;
    std::function<void(Window *m_Window)> callback;
  };

private:
  Window *m_Window;
  std::vector<Shortcut> m_Shortcuts;

public:
  static ShortcutManager &Instance() {
    static ShortcutManager instance;
    return instance;
  }

  void SetWindow(Window *window) { m_Window = window; }

  void Free() {
    m_Shortcuts.clear();
    m_Window = nullptr;
  };

  void Register(const Shortcut &shortcut) { m_Shortcuts.push_back(shortcut); }

  void PollEvents() {
    for (auto &s : m_Shortcuts) {
      if (!s.callback)
        continue;

      bool pressed = ImGui::IsKeyPressed(s.key, s.repeat);
      if (!pressed)
        continue;

      if ((s.ctrl == ImGui::IsKeyDown(ImGuiMod_Ctrl)) &&
          (s.shift == ImGui::IsKeyDown(ImGuiMod_Shift)) &&
          (s.alt == ImGui::IsKeyDown(ImGuiMod_Alt)) &&
          (s.super == ImGui::IsKeyDown(ImGuiMod_Super))) {
        s.callback(m_Window);
      }
    }
  }
};
