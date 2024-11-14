#include "Utility.h"

namespace Utility
{
  struct UserData
  {
    void *data;
    std::string *string;
    ImGuiInputTextCallback callback;
  };

  int InputTextCallback(ImGuiInputTextCallbackData *data)
  {
    UserData *userData = (UserData *)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
      std::string *str = userData->string;
      IM_ASSERT(data->Buf == str->c_str());
      str->resize(data->BufTextLen);
      data->Buf = (char *)str->c_str();
    }
    else if (userData->callback)
    {
      data->UserData = userData->data;
      return userData->callback(data);
    }
    return 0;
  }

  bool InputText(const char *label, std::string *str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void *data)
  {
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    UserData userData;
    userData.string = str;
    userData.callback = callback;
    userData.data = data;

    return ImGui::InputText(label, (char *)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &userData);
  }

  std::string ellipsize(const std::string &text, float maxWidth, Ellipsize type)
  {
    std::string truncatedText = text;

    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

    if (textSize.x <= maxWidth)
      return truncatedText;

    constexpr const char *ellipsis = "...";
    float availableWidth = maxWidth - ImGui::CalcTextSize(ellipsis).x;

    size_t length = text.length() - 1;

    if (type == Ellipsize::START)
    {
      while (ImGui::CalcTextSize(text.substr(length).c_str()).x < availableWidth)
        --length;

      truncatedText = ellipsis + text.substr(length);
    }

    if (type == Ellipsize::END)
    {
      while (ImGui::CalcTextSize(text.substr(0, length).c_str()).x > availableWidth)
        --length;

      truncatedText = text.substr(0, length) + ellipsis;
    }

    return truncatedText;
  }

  bool intersects(const glm::vec2 &point, const glm::vec2 &position, const glm::vec2 &size)
  {
    return (point.x >= position.x && point.x <= position.x + size.x) &&
           (point.y >= position.y && point.y <= position.y + size.y);
  }
}