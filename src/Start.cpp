#include "Start.h"
#include "Project/Project.h"

Start::Start(Project *project) : Application(APPLICATION_WINDOW_FLAGS | SDL_WINDOW_BORDERLESS), project(project)
{
  this->project->setApplication(this);
  this->recentProjects = this->project->recent.vector("recent");

  SDL_IOStream *stream = SDL_IOFromConstMem(BANNER_IMAGE_BUFFER, BANNER_IMAGE_BUFFER_SIZE);
  SDL_Surface *surface = IMG_Load_IO(stream, true);

  glGenTextures(1, &this->banner);
  glBindTexture(GL_TEXTURE_2D, this->banner);

  // Flip the image if it was loaded upside down (SDL convention)
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

  // Set texture parameters (e.g., filtering and wrapping)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  SDL_DestroySurface(surface);
};

void Start::onDraw(float deltaTime)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(50, 50, 50, 255));

  glm::vec2 windowSize = this->getWindowDimensions();

  ImGui::SetNextWindowSize(ImVec2{windowSize.x, windowSize.y});
  ImGui::SetNextWindowPos(ImVec2{0, 0});
  ImGui::Begin("New Project", nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings);

  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(1);

  ImGui::Image((ImTextureID)this->banner, ImVec2{windowSize.x / 2, windowSize.y});
  ImGui::SameLine();

  float closeButtonSize = 20;
  ImGui::SetCursorPos(ImVec2{windowSize.x - closeButtonSize - 5, 5});
  ImGui::PushFont(this->getFont("MaterialIcons:18"));
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.3, 0.3, 0.3, 1});
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.25, 0.25, 0.25, 1});
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.15, 0.15, 0.15, 1});
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0, 0});
  if (ImGui::Button("\ue5cd", ImVec2{closeButtonSize, closeButtonSize}))
    this->project->quit();
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(3);
  ImGui::PopFont();

  ImVec2 containerSize = ImVec2{((windowSize.x / 2) / 1.5f), (windowSize.y / 1.5f)};
  ImVec2 centerPos = ImVec2(((windowSize.x / 2) + (windowSize.x - containerSize.x)) * 0.5f,
                            (windowSize.y - containerSize.y) * 0.5f);
  ImGui::SetCursorPos(centerPos);
  ImGui::BeginChild("Centered Child", containerSize, ImGuiChildFlags_None);

  ImGui::PushFont(this->getFont("Roboto-Regular:24"));
  ImGui::Text("Sprite Rigger");
  ImGui::PopFont();

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::SeparatorText("Project");

  ImGui::Spacing();
  ImGui::Spacing();

  if (ImGui::TextLink("Create a new project"))
    this->project->openNew();

  ImGui::Spacing();
  if (ImGui::TextLink("Select a project"))
    this->project->open();

  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::SeparatorText("Recent");

  ImGui::Spacing();
  ImGui::Spacing();

  for (std::string recent : *this->recentProjects)
  {
    std::filesystem::path filePath = recent;

    if (ImGui::TextLink(Utility::ellipsize(filePath.stem().string() + " - " + recent, 230).c_str()))
      this->project->restart(SaveFile{recent});

    if (ImGui::IsItemHovered(ImGuiHoveredFlags_ForTooltip | ImGuiHoveredFlags_DelayNormal | ImGuiHoveredFlags_NoSharedDelay))
      ImGui::SetTooltip("%s", recent.c_str());

    ImGui::Spacing();
  }

  ImGui::EndChild();

  ImGui::End();
}
