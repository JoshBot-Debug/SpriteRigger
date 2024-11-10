#pragma once

#include "application.h"

class Project;
class HeaderPanelScene;
class AnimatorViewport;
class HierarchyScene;
class AssetsScene;

class App : public Application
{
private:
  Project *project;

  HeaderPanelScene *headerPanelScene;
  AnimatorViewport *animatorViewport;
  HierarchyScene *hierarchyScene;
  AssetsScene *assetsScene;

public:
  App(Project *project);
  ~App();

  void onInitialize() override;
  void onInput(SDL_Event *event, float deltaTime) override;
  void onUpdate(float deltaTime) override;
  void onDraw(float deltaTime) override;
  void onCleanUp() override;

  Project *getProject() {
    return this->project;
  }

  void addProjectToRecentFiles(int maxRecentFiles);
};