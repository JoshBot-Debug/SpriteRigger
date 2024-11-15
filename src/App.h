#pragma once

#include "Application.h"
#include "Utility.h"
#include "Input/Input.h"
#include "Manager/ProjectManager/ProjectManager.h"
#include "Manager/ShaderManager/ShaderManager.h"
#include "Manager/AssetManager/AssetManager.h"

#include "Scene/AssetScene.h"
#include "Scene/HierarchyScene.h"
#include "Scene/HeaderPanelScene.h"
#include "Scene/AnimatorViewport.h"

class App : public Application
{
private:
  ProjectManager *projectManager;
  AssetManager *assetManager;
  ShaderManager *shaderManager;
  SystemManager *systemManager;
  Input input;

  HeaderPanelScene *headerPanelScene;
  AnimatorViewport *animatorViewport;
  HierarchyScene *hierarchyScene;
  AssetScene *assetScene;

public:
  App(ProjectManager *projectManager);
  ~App();

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;

  Mouse *getMouseInput();

  void addProjectToRecentFiles(int maxRecentFiles);
};