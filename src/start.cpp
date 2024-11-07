#include "start.h"

void Start::onDraw(float deltaTime)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(50, 50, 50, 255));

  ImVec2 windowSize = this->getWindowDimensions();

  ImGui::SetNextWindowSize(ImVec2{windowSize.x, windowSize.y}); // Causes a FOUG kind of issue
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

  for (RecentProject &recent : this->projectManager->recentProjects)
  {
    if (ImGui::TextLink(recent.name.c_str()))
    {
      this->projectManager->deserialize(recent.filepath);
      this->quit();
    }
    ImGui::Spacing();
  }

  ImGui::EndChild();

  ImGui::End();
}

void Start::CreateNewProject()
{
  ProjectData *projectData = this->projectManager->getData();

  if (ImGui::TextLink("Create a new project"))
    ImGui::OpenPopup("NewProject");

  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal("NewProject", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar))
  {
    ImGui::Text("%s", "New Project");
    ImGui::Spacing();
    ImGui::SetNextItemWidth(300);
    Utility::InputText("Name", &projectData->name);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::SameLine(ImGui::GetWindowSize().x - ImGui::GetStyle().ItemSpacing.x - 106);
    ImGui::BeginDisabled(projectData->name.empty());
    if (ImGui::Button("Save"))
    {
      auto callback = [this, projectData](std::string path)
      {
        if (path.empty())
          return;
        projectData->filepath = path;
        this->projectManager->serialize(true);
        this->quit();
      };

      NativeFileDialog::Filters filters[1] = {{"Sprite Rigger", this->projectManager->fileExtension.c_str()}};
      this->AsyncTask(callback, NativeFileDialog::SaveFile, this->getWindow(), filters, 1);
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
  if (ImGui::TextLink("Select a project"))
  {
    auto callback = [this](std::string path)
    {
      if (path.empty())
        return;
      this->projectManager->deserialize(path);
      this->quit();
    };

    NativeFileDialog::Filters filters[1] = {{"Sprite Rigger", "sprig"}};
    this->AsyncTask(callback, NativeFileDialog::SelectFile, this->getWindow(), filters, 1);
  }
  ImGui::SameLine();
  std::string helpText = std::string("The selected project directory must \nbe a valid .") + this->projectManager->getFileExtension() + " file.";
  Utility::HelpMarker(helpText.c_str());
}
