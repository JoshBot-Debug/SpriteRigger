#include "utility.h"

namespace Utility
{
  bool isDirectoryEmpty(const std::filesystem::path &dir)
  {
    if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir))
    {
      for (const auto &entry : std::filesystem::directory_iterator(dir))
        return false;
      return true;
    }
    return false;
  }

  void HelpMarker(const char *desc)
  {
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted(desc);
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }
  }

  void ellipsize(std::string &text, float maxWidth, Ellipsize type)
  {
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

    if (textSize.x <= maxWidth)
      return;

    const char *ellipsis = "...";
    float availableWidth = maxWidth - ImGui::CalcTextSize(ellipsis).x;

    size_t length = text.length() - 1;

    if (type == Ellipsize::START)
    {
      while (ImGui::CalcTextSize(text.substr(length).c_str()).x < availableWidth)
        --length;

      text = ellipsis + text.substr(length);
      return;
    }

    while (ImGui::CalcTextSize(text.substr(0, length).c_str()).x > availableWidth)
      --length;

    text = text.substr(0, length) + ellipsis;
  }

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
}