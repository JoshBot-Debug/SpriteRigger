#pragma once

#include <imgui.h>
#include <string>
#include <filesystem>

namespace Utility
{
  enum Ellipsize
  {
    START,
    END
  };

  bool isDirectoryEmpty(const std::filesystem::path &dir);

  void HelpMarker(const char *desc);

  void ellipsize(std::string &text, float maxWidth, Ellipsize type = Ellipsize::END);

  bool InputText(const char *label, std::string *str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void *user_data = nullptr);
}
