#include "start.h"
#include <imgui_internal.h>
#include <filesystem>

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

void Start::onDraw(float deltaTime)
{
  // ImGui::ShowDemoWindow();
  ImGuiWindowClass winClass;
  winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&winClass);

  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{0, 0});
  ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2{0, 0});
  ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(50, 50, 50, 255));

  ImGui::Begin("New Project", nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
  ImVec2 windowSize = ImGui::GetWindowSize();
  ImVec2 windowPosition = ImGui::GetWindowPos();

  ImGui::PopStyleVar(4);
  ImGui::PopStyleColor(1);

  ImGui::Image((ImTextureID)this->banner, ImVec2{windowSize.x / 2, windowSize.y});

  ImVec2 containerSize = ImVec2{((windowSize.x / 2) / 1.5f), (windowSize.y / 1.5f)};
  ImVec2 centerPos = ImVec2(((windowSize.x / 2) + (windowSize.x - containerSize.x)) * 0.5f,
                            (windowSize.y - containerSize.y) * 0.5f);

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
    this->quit();
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(3);
  ImGui::PopFont();

  ImGui::SetCursorPos(centerPos);
  ImGui::BeginChild("Centered Child", containerSize, ImGuiChildFlags_None);

  ImGuiIO &io = ImGui::GetIO();

  ImGui::PushFont(this->getFont("Roboto-Regular:24"));
  ImGui::Text("Projects");
  ImGui::PopFont();

  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::SeparatorText("Project folder");

  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::TextLink("Create a new project");
  ImGui::Spacing();
  ImGui::TextLink("Select a project directory");

  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::SeparatorText("Recent");

  ImGui::Spacing();
  ImGui::Spacing();

  ImGui::TextLink("/home/joshua/Desktop/ProjectA");
  ImGui::Spacing();
  ImGui::TextLink("/home/joshua/Desktop/ProjectB");
  ImGui::Spacing();
  ImGui::TextLink("/home/joshua/Desktop/ProjectC");
  ImGui::Spacing();
  ImGui::TextLink("/home/joshua/Projects/Spy");

  ImGui::EndChild();

  ImGui::End();
}

std::string Start::getProjectDirectory()
{
  return this->projectDirectory;
}

void Start::selectProjectDirectory()
{
  auto callback = [this](std::string path)
  {
    if (path.empty() || !isDirectoryEmpty(path))
      return;
    this->projectDirectory = path;
    this->quit();
  };

  this->AsyncTask(callback, NativeFileDialog::SelectFolder, this->getWindow());
}