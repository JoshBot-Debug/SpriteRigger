#include "start.h"
#include <filesystem>
#include <utility.h>

void Start::onDraw(float deltaTime)
{
  // ImGui::ShowDemoWindow();
  // return;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(50, 50, 50, 255));

  ImVec2 windowSize = this->getWindowDimensions();

  ImGui::SetNextWindowPos(ImVec2{0, 0});
  ImGui::SetNextWindowSize(windowSize);
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
    this->quit();
  ImGui::PopStyleVar(2);
  ImGui::PopStyleColor(3);
  ImGui::PopFont();

  ImVec2 containerSize = ImVec2{((windowSize.x / 2) / 1.5f), (windowSize.y / 1.5f)};
  ImVec2 centerPos = ImVec2(((windowSize.x / 2) + (windowSize.x - containerSize.x)) * 0.5f,
                            (windowSize.y - containerSize.y) * 0.5f);
  ImGui::SetCursorPos(centerPos);
  ImGui::BeginChild("Centered Child", containerSize, ImGuiChildFlags_None);

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

  this->CreateNewProject();
  ImGui::Spacing();
  this->SelectProjectDirectory();

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

Project Start::getProject()
{
  return this->project;
}

void Start::CreateNewProject()
{
  ProjectData *projectData = this->project.getData();

  if (ImGui::TextLink("Create a new project"))
    ImGui::OpenPopup("Create Project");

  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal("Create Project", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
  {
    ImGui::Text("Project Name:");
    ImGui::InputText("##ProjectName", projectData->name, IM_ARRAYSIZE(projectData->name));

    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Project Directory:");
    if (ImGui::Button("Select folder"))
    {
      auto callback = [projectData](std::string path)
      {
        if (path.empty() || !Utility::isDirectoryEmpty(path))
          return;
        projectData->directory = path;
      };

      this->AsyncTask(callback, NativeFileDialog::SelectFolder, this->getWindow());
    }

    ImGui::SameLine();
    Utility::EllipsizeTextBegin(projectData->directory, 100);
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::BeginDisabled(!this->project.isReady());
    if (ImGui::Button("OK"))
    {
      this->createProjectInDirectory();
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndDisabled();

    ImGui::SameLine();

    if (ImGui::Button("Cancel"))
      ImGui::CloseCurrentPopup();

    ImGui::EndPopup();
  }
}

void Start::SelectProjectDirectory()
{
  if (ImGui::TextLink("Select a project directory"))
  {
    auto callback = [this](std::string path)
    {
      if (path.empty())
        return;
      this->loadProjectFromDirectory(path);
    };

    this->AsyncTask(callback, NativeFileDialog::SelectFolder, this->getWindow());
  }
  ImGui::SameLine();
  Utility::HelpMarker("The selected project directory must \nbe a valid sprite rigger project.");
}

/**
 * TODO
 * Validate path for a project.xml
 * Load the main application (this is where the project.xml file will be initialized)
 */
void Start::loadProjectFromDirectory(std::string path)
{
  this->project.deserialize(path);
  this->quit();
  return;
}

/**
 * TODO
 * Create the project.xml file.
 * Load the main application (this is where the project.xml file will be initialized)
 */
void Start::createProjectInDirectory()
{
  this->project.serialize();
  this->quit();
  return;
}
