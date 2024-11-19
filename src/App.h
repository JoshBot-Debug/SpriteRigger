#pragma once

#include "Application.h"
#include "Utility.h"
#include "Controller/Controller.h"
#include "ProjectManager/ProjectManager.h"

#include "Scene/AssetScene.h"
#include "Scene/HierarchyScene.h"
#include "Scene/HeaderPanelScene.h"
#include "Scene/AnimatorViewport.h"

class App : public Application
{
private:
  ProjectManager *projectManager;

  Controller controller;

  ColorSystem colorSystem;
  RenderSystem renderSystem;
  TransformSystem transformSystem;

  AssetScene assetScene;
  HierarchyScene hierarchyScene;
  HeaderPanelScene headerPanelScene;
  AnimatorViewport animatorViewport;

public:
  App(ProjectManager *projectManager): projectManager(projectManager) {};
  ~App() = default;

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;

  void addProjectToRecentFiles(int maxRecentFiles);
};