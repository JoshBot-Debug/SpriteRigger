#include "utility.h"

namespace Utility
{
  struct InputTextCallback_UserData
  {
    std::string *Str;
    ImGuiInputTextCallback ChainCallback;
    void *ChainCallbackUserData;
  };

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

  void EllipsizeTextBegin(std::string text, float maxWidth)
  {
    ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

    if (textSize.x <= maxWidth)
      return ImGui::Text("%s", text.c_str());

    const char *ellipsis = "...";
    float availableWidth = maxWidth - ImGui::CalcTextSize(ellipsis).x;

    size_t maxLength = text.length() - 1;
    while (ImGui::CalcTextSize(text.substr(maxLength).c_str()).x < availableWidth)
      --maxLength;

    std::string truncatedText = ellipsis + text.substr(maxLength);
    ImGui::Text("%s", truncatedText.c_str());
  }

  int InputTextCallback(ImGuiInputTextCallbackData *data)
  {
    InputTextCallback_UserData *user_data = (InputTextCallback_UserData *)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
      // Resize string callback
      // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
      std::string *str = user_data->Str;
      IM_ASSERT(data->Buf == str->c_str());
      str->resize(data->BufTextLen);
      data->Buf = (char *)str->c_str();
    }
    else if (user_data->ChainCallback)
    {
      // Forward to user callback, if any
      data->UserData = user_data->ChainCallbackUserData;
      return user_data->ChainCallback(data);
    }
    return 0;
  }

  bool InputText(const char *label, std::string *str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void *user_data)
  {
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;

    return ImGui::InputText(label, (char *)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
  }
}