#include "App.h"

#include <string>
#include <vector>
#include <stdio.h>

#include <imgui_internal.h>

#include "Component/Component.h"

#include "Manager/AssetManager/AssetManager.h"
#include "Manager/SystemManager/SystemManager.h"
#include "Manager/ShaderManager/ShaderManager.h"

App::App(ProjectManager *projectManager) : projectManager(projectManager)
{
  this->addProjectToRecentFiles(5);
  this->projectManager->setApplication(this);

  this->assetManager = new AssetManager();
  this->shaderManager = new ShaderManager();
  this->systemManager = new SystemManager(&this->registry, this->input.getMouse(), this->shaderManager);
  
  this->assetScene = new AssetScene(this->assetManager);
  this->hierarchyScene = new HierarchyScene(&this->registry);
  this->headerPanelScene = new HeaderPanelScene(this->projectManager);
  this->animatorViewport = new AnimatorViewport(this->systemManager);
}

App::~App()
{
  delete this->assetScene;
  delete this->hierarchyScene;
  delete this->animatorViewport;
  delete this->headerPanelScene;
}

void App::onInitialize()
{
  this->assetScene->onInitialize();
  this->hierarchyScene->onInitialize();
  this->headerPanelScene->onInitialize();
  this->animatorViewport->onInitialize();
}

void App::onInput(SDL_Event *event, float deltaTime)
{
  if (this->isWindowClosing(event))
    this->projectManager->quit();

  this->input.onEvent(event);
  this->assetScene->onInput(event, deltaTime);
  this->hierarchyScene->onInput(event, deltaTime);
  this->headerPanelScene->onInput(event, deltaTime);
  this->animatorViewport->onInput(event, deltaTime);
}

void App::onUpdate(float deltaTime)
{
  this->assetScene->onUpdate(deltaTime);
  this->hierarchyScene->onUpdate(deltaTime);
  this->headerPanelScene->onUpdate(deltaTime);
  this->animatorViewport->onUpdate(deltaTime);
}

void App::onDraw(float deltaTime)
{
  this->assetScene->onDraw(deltaTime);
  this->hierarchyScene->onDraw(deltaTime);
  this->headerPanelScene->onDraw(deltaTime);

  ImGuiWindowClass winClass;
  winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&winClass);
  this->animatorViewport->onDraw(deltaTime);
}

void App::onCleanUp()
{
  this->assetScene->onCleanUp();
  this->hierarchyScene->onCleanUp();
  this->headerPanelScene->onCleanUp();
  this->animatorViewport->onCleanUp();

  // TODO freeing the registry will eventually happen elsewhere
  this->registry.free<CTransform, CBone, CArmature>();
}

Mouse *App::getMouseInput()
{
  return this->input.getMouse();
}

void App::addProjectToRecentFiles(int maxRecentFiles)
{
  std::vector<std::string> *recent = this->projectManager->recent.vector("recent");
  std::string filePath = std::string(*this->projectManager->state.getSaveFile());

  auto it = std::find(recent->begin(), recent->end(), filePath);
  if (it != recent->end())
    recent->erase(it);

  recent->insert(recent->begin(), filePath);

  if (recent->size() > maxRecentFiles)
    recent->erase(recent->begin() + 5, recent->end());

  this->projectManager->recent.write();
}
