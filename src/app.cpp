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

App::App(ProjectManager *projectManager) : projectManager(projectManager)
{
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
  this->projectManager->updateRecentProjects();
  
  this->headerPanelScene->onInitialize();
  this->toolsScene->onInitialize();
  this->assetsScene->onInitialize();
  this->animatorViewport->onInitialize();
}

void App::onInput(SDL_Event *event, float deltaTime)
{
  if (this->isWindowClosing(event))
    this->projectManager->quit();

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