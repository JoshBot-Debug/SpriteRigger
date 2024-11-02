#include "imgui.h"
#include "mainMenu.h"

void MainMenu::fileMenu()
{
  if (ImGui::MenuItem("New"))
  {
  }

  if (ImGui::MenuItem("Open", "Ctrl+O"))
  {
  }
  if (ImGui::BeginMenu("Open Recent"))
  {
    ImGui::MenuItem("fish_hat.c");
    ImGui::MenuItem("fish_hat.inl");
    ImGui::MenuItem("fish_hat.h");
    ImGui::EndMenu();
  }

  ImGui::Separator();

  if (ImGui::MenuItem("Save", "Ctrl+S"))
  {
  }
  if (ImGui::MenuItem("Save As.."))
  {
  }

  ImGui::Separator();

  if (ImGui::BeginMenu("Options"))
  {
    ImGui::MenuItem("Theme", nullptr);
    ImGui::EndMenu();
  }

  ImGui::Separator();

  if (ImGui::MenuItem("Quit", "Alt+F4"))
  {
  }
}

void MainMenu::editMenu()
{
  if (ImGui::MenuItem("Undo", "Ctrl+Z"))
  {
  }
  if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false))
  {
  }
  ImGui::Separator();
  if (ImGui::MenuItem("Cut", "Ctrl+X"))
  {
  }
  if (ImGui::MenuItem("Copy", "Ctrl+C"))
  {
  }
  if (ImGui::MenuItem("Paste", "Ctrl+V"))
  {
  }
  ImGui::EndMenu();
}

void MainMenu::draw()
{
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("File"))
    {
      this->fileMenu();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit"))
    {
      this->editMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

/**
 * TODO
 * 
 * Create a parent class, it should 
 */