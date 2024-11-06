#pragma once

#include <imgui.h>
#include <string>
#include <filesystem>

namespace Utility
{
  bool isDirectoryEmpty(const std::filesystem::path &dir);

  void HelpMarker(const char *desc);

  void EllipsizeTextBegin(std::string text, float maxWidth);

  bool InputText(const char *label, std::string *str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void *user_data = nullptr);
}
