#pragma once

#include "Scene/Scene.h"
#include "Manager/ProjectManager.h"
#include "ECS/Registry.h"

class HeaderPanelScene : public Scene
{
private:
  Registry *registry;
  ProjectManager *projectManager;
  std::vector<std::string> *recentProjects;

public:
  HeaderPanelScene() = default;
  ~HeaderPanelScene() = default;

  void onInitialize(Registry *registry, ProjectManager *projectManager);
  void onDraw(float deltaTime) override;

  void File_Menu();

  void File_New();
  void File_Open();
  void File_OpenRecent();
  void File_Save();
  void File_SaveAs();

  void File_Options();
  void File_Quit();

  void Edit_Menu();

  void Edit_Undo();
  void Edit_Redo();
  void Edit_Cut();
  void Edit_Copy();
  void Edit_Paste();
};