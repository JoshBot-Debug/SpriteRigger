#pragma once

#include "Scene/Scene.h"
#include "Manager/ProjectManager/ProjectManager.h"

class HeaderPanelScene : public Scene
{
private:
  ProjectManager *projectManager;
  std::vector<std::string> *recentProjects;

public:
  HeaderPanelScene(ProjectManager *projectManager);
  ~HeaderPanelScene() = default;

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