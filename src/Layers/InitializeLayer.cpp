#include "InitializeLayer.h"

#include "Assets/BannerImage.h"
#include "Utility.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

static const ImWchar GlyphRange[] = {
    0xf000, 0xf8ff, 0xe000, 0xefff, 0,
};

InitializeLayer::InitializeLayer(State *state) : m_State(state) {}

void InitializeLayer::OnAttach() {
  Window::AddFont(GetExecutableDirectory() +
                      "/../src/Assets/Fonts/MaterialIcons/MaterialIcons.ttf",
                  "MaterialIcons", 0.0f, GlyphRange);

  int channels;

  unsigned char *imageData =
      stbi_load_from_memory(BANNER_IMAGE_BUFFER, BANNER_IMAGE_BUFFER_SIZE,
                            &m_Width, &m_Height, &channels, 0);

  if (!imageData)
    throw std::runtime_error(stbi_failure_reason());

  glGenTextures(1, &m_Texture);
  glBindTexture(GL_TEXTURE_2D, m_Texture);

  GLenum format;
  if (channels == 4)
    format = GL_RGBA;
  else if (channels == 3)
    format = GL_RGB;
  else
    throw std::runtime_error("Unsupported image format");

  glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format,
               GL_UNSIGNED_BYTE, imageData);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  stbi_image_free(imageData);
}

void InitializeLayer::OnRender() {
  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse |
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking |
      ImGuiWindowFlags_NoSavedSettings;

  ImVec2 windowSize = ImGui::GetWindowSize();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.0f, 0.0f});
  ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(50, 50, 50, 255));
  ImGui::SetNextWindowPos(ImVec2{0, 0});
  ImGui::SetNextWindowSize(windowSize);
  ImGui::Begin("##Initialize", nullptr, windowFlags);

  ImGui::PopStyleVar(3);
  ImGui::PopStyleColor(1);

  ImGui::Image((ImTextureID)m_Texture, ImVec2{(float)m_Width, (float)m_Height});

  ImGui::SameLine();

  // The exit button
  {
    float closeButtonSize = 20;
    ImGui::SetCursorPos(ImVec2{windowSize.x - closeButtonSize - 5, 5});
    ImGui::PushFont(Window::GetFont("MaterialIcons"), 16.0f);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.3, 0.3, 0.3, 1});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.25, 0.25, 0.25, 1});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.15, 0.15, 0.15, 1});
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0, 0});
    if (ImGui::Button("\ue5cd", ImVec2{closeButtonSize, closeButtonSize})) {
      m_Window->Quit();
      m_State->Quit();
    }

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(3);
    ImGui::PopFont();
  }

  // The header
  {
    ImVec2 containerSize =
        ImVec2{((windowSize.x / 2) / 1.5f), (windowSize.y / 1.5f)};
    ImVec2 centerPos =
        ImVec2(((windowSize.x / 2) + (windowSize.x - containerSize.x)) * 0.5f,
               (windowSize.y - containerSize.y) * 0.5f);
    ImGui::SetCursorPos(centerPos);
    ImGui::BeginChild("#CenteredChild", containerSize, ImGuiChildFlags_None);

    ImGui::PushFont(Window::GetFont("RobotoRegular"), 32.0f);
    ImGui::Text("Sprite Rigger");
    ImGui::PopFont();
  }

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::SeparatorText("Project");

  ImGui::Spacing();
  ImGui::Spacing();

  if (ImGui::TextLink("Create a new project")) {
    if (m_State->New())
      m_Window->Quit();
  }

  ImGui::Spacing();
  if (ImGui::TextLink("Open a project")) {
    if (m_State->Open())
      m_Window->Quit();
  }

  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::SeparatorText("Recent");

  ImGui::Spacing();
  ImGui::Spacing();

  for (const auto &project : m_State->GetRecentProjects()) {
    std::filesystem::path filePath = project;

    if (ImGui::TextLink(
            Ellipsize(filePath.stem().string() + " - " + project, 230)
                .c_str())) {
      m_State->Open(project);
      m_Window->Quit();
    }

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip |
                             ImGuiHoveredFlags_DelayNormal |
                             ImGuiHoveredFlags_NoSharedDelay))
      ImGui::SetTooltip("%s", project.c_str());

    ImGui::Spacing();
  }

  ImGui::EndChild();

  ImGui::End();
}

void InitializeLayer::OnDetach() {
  glDeleteTextures(1, &m_Texture);
  m_State = nullptr;
}
