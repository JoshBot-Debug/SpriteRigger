#pragma once

#include "application.h"
#include "Scene/HeaderPanelScene/headerPanelScene.h"
#include "Scene/ToolsScene/toolsScene.h"
#include "Scene/AssetsScene/assetsScene.h"
#include "Scene/AnimatorViewport/animatorViewport.h"
#include "project.h"

class SpriteRigger : public Application
{
private:
  Project project;
  HeaderPanelScene headerPanelScene;
  AnimatorViewport animatorViewport;
  ToolsScene toolsScene;
  AssetsScene assetsScene;

public:
  SpriteRigger(Project project) : project(project), headerPanelScene(this), animatorViewport(this), toolsScene(this), assetsScene(this) {};
  ~SpriteRigger() = default;

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;
};