#include <string>

#include "imgui.h"
#include "mainMenu.h"
#include "SpriteRigger/NativeFileSystemDialog/nativeFileSystemDialog.h"

void MainMenu::setApplication(Application *application)
{
  this->application = application;
}

void MainMenu::onDraw()
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

void MainMenu::File_Menu()
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

void MainMenu::File_New()
{
  if (ImGui::MenuItem("New"))
  {
  }
}

void MainMenu::File_Open()
{
  if (ImGui::MenuItem("Open", "Ctrl+O"))
  {
    auto callback = [this](nfdchar_t *path)
    {
      printf("Open file: %s\n", path);
      delete path;
    };

    Application::AsyncTask(callback, NativeFileSystemDialog, NativeFileSystem::FILE_PICKER, "txt;png,jpg");
  }
}

void MainMenu::File_OpenRecent()
{
  if (ImGui::BeginMenu("Open Recent"))
  {
    ImGui::MenuItem("fish_hat.c");
    ImGui::MenuItem("fish_hat.inl");
    ImGui::MenuItem("fish_hat.h");
    ImGui::EndMenu();
  }
}

void MainMenu::File_Save()
{
  if (ImGui::MenuItem("Save", "Ctrl+S"))
  {
  }
}

void MainMenu::File_SaveAs()
{

  if (ImGui::MenuItem("Save As.."))
  {
    auto callback = [this](nfdchar_t *path)
    {
      printf("Save to: %s\n", path);
      delete path;
    };

    Application::AsyncTask(callback, NativeFileSystemDialog, NativeFileSystem::FOLDER_PICKER, nullptr);
  }
}

void MainMenu::File_Options()
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

void MainMenu::File_Quit()
{
  if (ImGui::MenuItem("Quit", "Alt+F4"))
    this->application->quit();
}

void MainMenu::Edit_Menu()
{
  this->Edit_Undo();
  this->Edit_Redo();

  ImGui::Separator();

  this->Edit_Cut();
  this->Edit_Copy();
  this->Edit_Paste();

  ImGui::EndMenu();
}

void MainMenu::Edit_Undo()
{
  if (ImGui::MenuItem("Undo", "Ctrl+Z"))
  {
  }
}

void MainMenu::Edit_Redo()
{
  if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false))
  {
  }
}

void MainMenu::Edit_Cut()
{
  if (ImGui::MenuItem("Cut", "Ctrl+X"))
  {
  }
}

void MainMenu::Edit_Copy()
{
  if (ImGui::MenuItem("Copy", "Ctrl+C"))
  {
  }
}

void MainMenu::Edit_Paste()
{
  if (ImGui::MenuItem("Paste", "Ctrl+V"))
  {
  }
}
