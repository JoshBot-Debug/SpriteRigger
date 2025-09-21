#pragma once

#include "imgui.h"
#include <functional>
#include <vector>

class ContextMenu {
public:
  struct Item {
    const char *name = nullptr;
    const char *shortcut = nullptr;
    bool *selected = nullptr;
    bool enabled = true;
    std::function<void()> onClick = nullptr;
  };

  struct Options {
    const char *id = nullptr;
    std::vector<Item> items;
  };

private:
  Options m_Options;

public:
  ContextMenu() = default;

  void Register(const Options &options) { m_Options = options; }

  void OnRender() {
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right, false))
      ImGui::OpenPopup(m_Options.id);

    if (ImGui::BeginPopup(m_Options.id)) {
      for (auto &item : m_Options.items)
        if (ImGui::MenuItem(item.name, item.shortcut, item.selected,
                            item.enabled))
          item.onClick();

      ImGui::EndPopup();
    }
  }
};