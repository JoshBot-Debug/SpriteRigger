#pragma once

#include <string>
#include <glm/glm.hpp>

#include "imgui.h"

namespace Utility
{
  enum Ellipsize
  {
    START,
    END
  };

  std::string ellipsize(const std::string &text, float maxWidth, Ellipsize type = Ellipsize::END);

  bool InputText(const char *label, std::string *str, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = nullptr, void *user_data = nullptr);

  bool intersects(const glm::vec2 &point, const glm::vec2 &position, const glm::vec2 &size);
}
