#pragma once

#include "application.h"
#include "Scene/HeaderPanelScene/headerPanelScene.h"
#include "Scene/ToolsScene/toolsScene.h"
#include "Scene/AssetsScene/assetsScene.h"
#include "Scene/AnimatorViewport/animatorViewport.h"
#include "ProjectManager/projectManager.h"

class App : public Application
{
private:
  ProjectManager *projectManager;

  HeaderPanelScene headerPanelScene;
  AnimatorViewport animatorViewport;
  ToolsScene toolsScene;
  AssetsScene assetsScene;

public:
  App(ProjectManager *projectManager) : projectManager(projectManager), headerPanelScene(this), animatorViewport(this), toolsScene(this), assetsScene(this) {};
  ~App() = default;

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};