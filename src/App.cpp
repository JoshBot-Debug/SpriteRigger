#include "App.h"

#include <string>
#include <vector>
#include <stdio.h>

#include <imgui_internal.h>

#include "Component/Component.h"

/**
 * Create an armature
 * Joint extendable
 *
 * Process to setup a vertex array, buffer and element buffer
 * Decide between instancing or uniforms.
 * Ability to add textures and bind them to bones.
 *
 * Move out Render to Application
 */

void App::onInitialize()
{
  this->addProjectToRecentFiles(5);
  this->projectManager->setApplication(this);

  this->assetScene.onInitialize();
  this->headerPanelScene.onInitialize(this->projectManager);

  this->controller.onInitialize(&this->registry, &this->colorSystem, &this->renderSystem, &this->transformSystem);

  this->hierarchyScene.onInitialize(&this->registry, &this->controller);
  this->animatorViewport.onInitialize(&this->registry, &this->controller, &this->colorSystem, &this->renderSystem, &this->transformSystem);
}

void App::onInput(SDL_Event *event, float deltaTime)
{
  if (this->isWindowClosing(event))
    this->projectManager->quit();

  this->assetScene.onInput(event, deltaTime);
  this->hierarchyScene.onInput(event, deltaTime);
  this->headerPanelScene.onInput(event, deltaTime);
  this->animatorViewport.onInput(event, deltaTime);
}

void App::onUpdate(float deltaTime)
{
  this->assetScene.onUpdate(deltaTime);
  this->hierarchyScene.onUpdate(deltaTime);
  this->headerPanelScene.onUpdate(deltaTime);
  this->animatorViewport.onUpdate(deltaTime);
}

void App::onDraw(float deltaTime)
{
  this->assetScene.onDraw(deltaTime);
  this->hierarchyScene.onDraw(deltaTime);
  this->headerPanelScene.onDraw(deltaTime);

  ImGuiWindowClass winClass;
  winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  ImGui::SetNextWindowClass(&winClass);
  this->animatorViewport.onDraw(deltaTime);
}

void App::onCleanUp()
{
  this->assetScene.onCleanUp();
  this->hierarchyScene.onCleanUp();
  this->headerPanelScene.onCleanUp();
  this->animatorViewport.onCleanUp();

  // TODO freeing the registry will eventually happen elsewhere
  this->registry.free<CTransform, CBone, CArmature>();
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
