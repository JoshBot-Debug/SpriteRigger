#include <string>
#include <filesystem>

#include "Scene/scene.h"
#include "headerPanelScene.h"

void HeaderPanelScene::onDraw(float deltaTime)
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      this->File_Menu();
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit"))
      this->Edit_Menu();

    ImGui::EndMainMenuBar();
  }
}

void HeaderPanelScene::File_Menu()
{
  this->File_New();
  this->File_Open();
  this->File_OpenRecent();

  ImGui::Separator();

  this->File_Save();
  this->File_SaveAs();

  ImGui::Separator();

  this->File_Options();

  ImGui::Separator();

  this->File_Quit();
}

void HeaderPanelScene::File_New()
{
  if (ImGui::MenuItem("New"))
  {
  }
}

void HeaderPanelScene::File_Open()
{
  if (ImGui::MenuItem("Open", "Ctrl+O"))
  {
    ProjectManager *projectManager = this->app->getProjectManager();
    auto callback = [this, projectManager](std::string filepath)
    {
      projectManager->deserialize(filepath);
      this->app->quit();
    };
    NativeFileDialog::Filters filters[1] = {{"Sprite Rigger", projectManager->fileExtension.c_str()}};
    this->app->AsyncTask(callback, NativeFileDialog::SelectFile, this->app->getWindow(), filters, 1);
  }
}

void HeaderPanelScene::File_OpenRecent()
{
  if (ImGui::BeginMenu("Open Recent"))
  {
    ProjectManager *projectManager = this->app->getProjectManager();
    for (RecentProject &recent : projectManager->recentProjects)
    {
      if (ImGui::MenuItem(recent.name.c_str()))
      {
        projectManager->deserialize(recent.filepath);
        this->app->quit();
      }
    }
    ImGui::EndMenu();
  }
}

void HeaderPanelScene::File_Save()
{
  if (ImGui::MenuItem("Save", "Ctrl+S"))
  {
  }
}

void HeaderPanelScene::File_SaveAs()
{

  if (ImGui::MenuItem("Save As.."))
  {
    auto callback = [](std::string path)
    {
      printf("Save to: %s\n", path.c_str());
    };

    this->app->AsyncTask(callback, NativeFileDialog::SaveFile, this->app->getWindow(), nullptr, 0);
  }
}

void HeaderPanelScene::File_Options()
{
  if (ImGui::BeginMenu("Options"))
  {
    if (ImGui::BeginMenu("Theme"))
    {
      Theme currentTheme = this->app->getTheme();

      if (ImGui::MenuItem("Light", nullptr, currentTheme == Theme::LIGHT) && currentTheme != Theme::LIGHT)
        this->app->setTheme(Theme::LIGHT);

      if (ImGui::MenuItem("Dark", nullptr, currentTheme == Theme::DARK) && currentTheme != Theme::DARK)
        this->app->setTheme(Theme::DARK);

      ImGui::EndMenu();
    }

    ImGui::EndMenu();
  }
}

void HeaderPanelScene::File_Quit()
{
  if (ImGui::MenuItem("Quit", "Alt+F4"))
  {
    this->app->quit();
  }
}

void HeaderPanelScene::Edit_Menu()
{
  this->Edit_Undo();
  this->Edit_Redo();

  ImGui::Separator();

  this->Edit_Cut();
  this->Edit_Copy();
  this->Edit_Paste();

  ImGui::EndMenu();
}

void HeaderPanelScene::Edit_Undo()
{
  if (ImGui::MenuItem("Undo", "Ctrl+Z"))
  {
  }
}

void HeaderPanelScene::Edit_Redo()
{
  if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false))
  {
  }
}

void HeaderPanelScene::Edit_Cut()
{
  if (ImGui::MenuItem("Cut", "Ctrl+X"))
  {
  }
}

void HeaderPanelScene::Edit_Copy()
{
  if (ImGui::MenuItem("Copy", "Ctrl+C"))
  {
  }
}

void HeaderPanelScene::Edit_Paste()
{
  if (ImGui::MenuItem("Paste", "Ctrl+V"))
  {
  }
}
