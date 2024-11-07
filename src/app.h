#pragma once

#include "application.h"
// #include "Scene/HeaderPanelScene/headerPanelScene.h"
// #include "Scene/ToolsScene/toolsScene.h"
// #include "Scene/AssetsScene/assetsScene.h"
// #include "Scene/AnimatorViewport/animatorViewport.h"
#include "ProjectManager/projectManager.h"

class HeaderPanelScene;
class AnimatorViewport;
class ToolsScene;
class AssetsScene;

class App : public Application
{
private:
  ProjectManager *projectManager;

  HeaderPanelScene *headerPanelScene;
  AnimatorViewport *animatorViewport;
  ToolsScene *toolsScene;
  AssetsScene *assetsScene;

public:
  App(ProjectManager *projectManager);
  ~App();

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;

  ProjectManager *getProjectManager() {
    return this->projectManager;
  }
};