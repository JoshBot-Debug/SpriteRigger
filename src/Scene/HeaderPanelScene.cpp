#include "HeaderPanelScene.h"

#include <string>
#include <filesystem>

#include "Utility.h"

HeaderPanelScene::HeaderPanelScene(ProjectManager *projectManager): projectManager(projectManager)
{
  this->recentProjects = this->projectManager->recent.vector("recent");
}

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
    this->projectManager->openNew();
}

void HeaderPanelScene::File_Open()
{
  if (ImGui::MenuItem("Open", "Ctrl+O"))
    this->projectManager->open();
}

void HeaderPanelScene::File_OpenRecent()
{
  if (ImGui::BeginMenu("Open Recent"))
  {
    for (std::string recent : *this->recentProjects)
    {
      std::filesystem::path filePath = recent;
      if (ImGui::MenuItem(Utility::ellipsize((filePath.stem().string() + " - " + recent), 300, Utility::Ellipsize::START).c_str()))
        this->projectManager->restart(SaveFile{recent});

      ImGui::Spacing();
    }

    ImGui::EndMenu();
  }
}

void HeaderPanelScene::File_Save()
{
  if (ImGui::MenuItem("Save", "Ctrl+S"))
    this->projectManager->save();
}

void HeaderPanelScene::File_SaveAs()
{
  if (ImGui::MenuItem("Save As.."))
    this->projectManager->saveAs();
}

void HeaderPanelScene::File_Options()
{
  if (ImGui::BeginMenu("Options"))
  {
    if (ImGui::BeginMenu("Theme"))
    {
      // Theme currentTheme = this->app->getTheme();

      // if (ImGui::MenuItem("Light", nullptr, currentTheme == Theme::LIGHT) && currentTheme != Theme::LIGHT)
      //   this->app->setTheme(Theme::LIGHT);

      // if (ImGui::MenuItem("Dark", nullptr, currentTheme == Theme::DARK) && currentTheme != Theme::DARK)
      //   this->app->setTheme(Theme::DARK);

      ImGui::EndMenu();
    }

    ImGui::EndMenu();
  }
}

void HeaderPanelScene::File_Quit()
{
  if (ImGui::MenuItem("Quit", "Alt+F4"))
    this->projectManager->quit();
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
