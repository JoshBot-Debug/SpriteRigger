#include <string>

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
    auto callback = [this](std::string path)
    {
      printf("Open file: %s\n", path.c_str());
    };
    this->application->AsyncTask(callback, NativeFileDialog::SelectFile, this->application->getWindow(), nullptr, 0);
  }
}

void HeaderPanelScene::File_OpenRecent()
{
  if (ImGui::BeginMenu("Open Recent"))
  {
    ImGui::MenuItem("fish_hat.c");
    ImGui::MenuItem("fish_hat.inl");
    ImGui::MenuItem("fish_hat.h");
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

    this->application->AsyncTask(callback, NativeFileDialog::SaveFile, this->application->getWindow(), nullptr, 0);
  }
}

void HeaderPanelScene::File_Options()
{
  if (ImGui::BeginMenu("Options"))
  {
    if (ImGui::BeginMenu("Theme"))
    {
      Theme currentTheme = this->application->getTheme();

      if (ImGui::MenuItem("Light", nullptr, currentTheme == Theme::LIGHT) && currentTheme != Theme::LIGHT)
        this->application->setTheme(Theme::LIGHT);

      if (ImGui::MenuItem("Dark", nullptr, currentTheme == Theme::DARK) && currentTheme != Theme::DARK)
        this->application->setTheme(Theme::DARK);

      ImGui::EndMenu();
    }

    ImGui::EndMenu();
  }
}

void HeaderPanelScene::File_Quit()
{
  if (ImGui::MenuItem("Quit", "Alt+F4"))
    this->application->quit();
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
