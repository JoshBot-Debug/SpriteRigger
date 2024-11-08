#include "app.h"

#include <string>
#include <vector>
#include <stdio.h>

#include "Entity/components.h"

#include <imgui_internal.h>

#include "Scene/HeaderPanelScene/headerPanelScene.h"
#include "Scene/ToolsScene/toolsScene.h"
#include "Scene/AssetsScene/assetsScene.h"
#include "Scene/AnimatorViewport/animatorViewport.h"
#include "Project/project.h"

App::App(Project *project) : project(project)
{
  this->addProjectToRecentFiles(5);
  this->project->setApplication(this);

  this->headerPanelScene = new HeaderPanelScene(this);
  this->toolsScene = new ToolsScene(this);
  this->assetsScene = new AssetsScene(this);
  this->animatorViewport = new AnimatorViewport(this);
}

App::~App()
{
  delete this->headerPanelScene;
}

void App::onInitialize()
{
  this->headerPanelScene->onInitialize();
  this->toolsScene->onInitialize();
  this->assetsScene->onInitialize();
  this->animatorViewport->onInitialize();
}

void App::onInput(SDL_Event *event, float deltaTime)
{
  if (this->isWindowClosing(event))
    this->getProject()->quit();

  this->headerPanelScene->onInput(event, deltaTime);
  this->toolsScene->onInput(event, deltaTime);
  this->assetsScene->onInput(event, deltaTime);
  this->animatorViewport->onInput(event, deltaTime);
}

void App::onUpdate(float deltaTime)
{
  this->headerPanelScene->onUpdate(deltaTime);
  this->toolsScene->onUpdate(deltaTime);
  this->assetsScene->onUpdate(deltaTime);
  this->animatorViewport->onUpdate(deltaTime);
}

void App::onDraw(float deltaTime)
{
  this->headerPanelScene->onDraw(deltaTime);
  this->toolsScene->onDraw(deltaTime);
  this->assetsScene->onDraw(deltaTime);

  ImGuiWindowClass winClass;
  winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&winClass);
  this->animatorViewport->onDraw(deltaTime);
}

void App::onCleanUp()
{
  this->headerPanelScene->onCleanUp();
  this->toolsScene->onCleanUp();
  this->assetsScene->onCleanUp();
  this->animatorViewport->onCleanUp();

  // TODO freeing the registry will eventually happen elsewhere
  this->registry.free<MeshComponent, PropertiesComponent, TransformComponent, ColorComponent>();
}

void App::addProjectToRecentFiles(int maxRecentFiles)
{
  std::vector<std::string> *recent = this->project->recent.vector("recent");
  std::string filePath = std::string(*this->project->state.getSaveFile());

  auto it = std::find(recent->begin(), recent->end(), filePath);
  if (it != recent->end())
    recent->erase(it);

  recent->insert(recent->begin(), filePath);

  if (recent->size() > maxRecentFiles)
    recent->erase(recent->begin() + 5, recent->end());

  this->project->recent.write();
}
