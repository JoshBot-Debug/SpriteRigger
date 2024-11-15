#pragma once

#include "App.h"
#include "Scene/Scene.h"

class HeaderPanelScene : public Scene
{
private:
  App *app;
  std::vector<std::string> *recentProjects;

public:
  HeaderPanelScene(App *app);
  ~HeaderPanelScene() {}

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