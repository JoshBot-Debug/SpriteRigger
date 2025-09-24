#pragma once

#include "imgui.h"
#include <exception>
#include <functional>
#include <vector>

class ContextMenu {
public:
  struct Item {
    const char *name = nullptr;
    const char *shortcut = nullptr;
    bool *selected = nullptr;
    bool enabled = true;
    std::function<void(Item *item, void *)> onRenderItem = nullptr;
    std::function<void(void *)> onClick = nullptr;
  };

  enum class PopupContext {
    NONE = 0,
    WINDOW = 1,
    ITEM = 2,
  };

  struct Options {
    PopupContext renderOn = PopupContext::NONE;
    std::vector<Item> items;
  };

private:
  Options m_Options;
  void *m_Data = nullptr;

private:
  void RenderItems() {
    for (auto &item : m_Options.items) {
      if (item.onRenderItem)
        item.onRenderItem(&item, m_Data);
      else if (ImGui::MenuItem(item.name, item.shortcut, item.selected,
                               item.enabled))
        item.onClick(m_Data);
    }
  }

public:
  ContextMenu() = default;

  void Register(const Options &options) { m_Options = options; }

  void Open(const char *id, void *data = nullptr) {
    if (data)
      m_Data = data;
    ImGui::OpenPopup(id);
  }

  void Render(const char *id, void *data = nullptr) {
    switch (m_Options.renderOn) {
    case PopupContext::NONE: {
      if (ImGui::BeginPopup(id, ImGuiWindowFlags_NoFocusOnAppearing)) {
        RenderItems();
        ImGui::EndPopup();
      }
      break;
    }
    case PopupContext::WINDOW: {
      if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        if (data)
          m_Data = data;
        ImGui::OpenPopup(id);
      }

      if (ImGui::BeginPopup(id)) {
        RenderItems();
        ImGui::EndPopup();
      }
      break;
    }
    case PopupContext::ITEM: {
      if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
        if (data)
          m_Data = data;
        ImGui::OpenPopup(id);
      }

      if (ImGui::BeginPopup(id)) {
        RenderItems();
        ImGui::EndPopup();
      }
      break;
    }

    default:
      throw std::runtime_error("Invalid renderOn choice");
    }
  }
};