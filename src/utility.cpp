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

  std::string TruncateText(const std::string &p_text, float p_truncated_width)
  {
    std::string truncated_text = p_text;

    const float text_width =
        ImGui::CalcTextSize(p_text.c_str(), nullptr, true).x;

    if (text_width > p_truncated_width)
    {
      constexpr const char *ELLIPSIS = " ...";
      const float ellipsis_size = ImGui::CalcTextSize(ELLIPSIS).x;

      int visible_chars = 0;
      for (size_t i = 0; i < p_text.size(); i++)
      {
        const float current_width = ImGui::CalcTextSize(
                                        p_text.substr(0, i).c_str(), nullptr, true)
                                        .x;
        if (current_width + ellipsis_size > p_truncated_width)
        {
          break;
        }

        visible_chars = i;
      }

      truncated_text = (p_text.substr(0, visible_chars) + ELLIPSIS).c_str();
    }

    return truncated_text;
  }
}