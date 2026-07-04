#pragma once

#include <exception>
#include <functional>
#include <string>
#include <vector>
#include "imgui.h"

template <typename T> class ContextMenu2
{
public:
  struct Item
  {
    const char* name                           = nullptr;
    const char* shortcut                       = nullptr;
    bool*       selected                       = nullptr;
    bool*       enabled                        = nullptr;
    void (*onRenderItem)(Item* item, T* state) = nullptr;
    void (*onClick)(T* state)                  = nullptr;
  };

  enum class PopupContext
  {
    NONE   = 0,
    WINDOW = 1,
    ITEM   = 2,
  };

  struct Options
  {
    PopupContext      renderOn = PopupContext::NONE;
    std::vector<Item> items;
  };

private:
  std::string m_Id;
  Options     m_Options;
  T*          m_State = nullptr;

  std::function<void(T*&)> m_CaptureState = nullptr;

private:
  void RenderItems()
  {
    for (auto& item : m_Options.items)
    {
      if (item.onRenderItem)
        item.onRenderItem(&item, m_State);
      else if (ImGui::MenuItem(item.name, item.shortcut, item.selected ? *item.selected : false,
                               item.enabled ? *item.enabled : true))
        item.onClick(m_State);
    }
  }

public:
  ContextMenu2(const std::string& id) : m_Id(id){};

  void SetOptions(const Options& options)
  {
    m_Options = options;
  }

  void OnCaptureState(const std::function<void(T*&)>& callback)
  {
    m_CaptureState = callback;
  }

  void Render()
  {
    const char* id = m_Id.c_str();

    switch (m_Options.renderOn)
    {
    case PopupContext::NONE:
    {
      if (ImGui::BeginPopup(id, ImGuiWindowFlags_NoFocusOnAppearing))
      {
        RenderItems();
        ImGui::EndPopup();
      }
      break;
    }
    case PopupContext::WINDOW:
    {
      if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) &&
          ImGui::IsMouseClicked(ImGuiMouseButton_Right))
      {
        if (m_CaptureState)
          m_CaptureState(m_State);
        ImGui::OpenPopup(id);
      }

      if (ImGui::BeginPopup(id))
      {
        RenderItems();
        ImGui::EndPopup();
      }
      break;
    }
    case PopupContext::ITEM:
    {
      if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
      {
        if (m_CaptureState)
          m_CaptureState(m_State);
        ImGui::OpenPopup(id);
      }

      if (ImGui::BeginPopup(id))
      {
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