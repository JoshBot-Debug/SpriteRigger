#include "spriteRigger.h"

#include <string>
#include <vector>
#include <stdio.h>

#include "Entity/components.h"

#include <imgui_internal.h>

void SpriteRigger::onInitialize()
{
  ProjectData *projectData = this->project.getData();
  printf("SpriteRigger: Name: %s, Directory: %s\n", projectData->name.c_str(), projectData->directory.c_str());
  this->headerPanelScene.onInitialize();
  this->toolsScene.onInitialize();
  this->assetsScene.onInitialize();
  this->animatorViewport.onInitialize();
}

void SpriteRigger::onInput(SDL_Event *event, float deltaTime)
{
  this->headerPanelScene.onInput(event, deltaTime);
  this->toolsScene.onInput(event, deltaTime);
  this->assetsScene.onInput(event, deltaTime);
  this->animatorViewport.onInput(event, deltaTime);
}

void SpriteRigger::onUpdate(float deltaTime)
{
  this->headerPanelScene.onUpdate(deltaTime);
  this->toolsScene.onUpdate(deltaTime);
  this->assetsScene.onUpdate(deltaTime);
  this->animatorViewport.onUpdate(deltaTime);
}

void SpriteRigger::onDraw(float deltaTime)
{
  // this->headerPanelScene.onDraw(deltaTime);
  // this->toolsScene.onDraw(deltaTime);
  // this->assetsScene.onDraw(deltaTime);

  // ImGuiWindowClass winClass;
  // winClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
  // ImGui::SetNextWindowClass(&winClass);
  // this->animatorViewport.onDraw(deltaTime);
}

void SpriteRigger::onCleanUp()
{
  this->headerPanelScene.onCleanUp();
  this->toolsScene.onCleanUp();
  this->assetsScene.onCleanUp();
  this->animatorViewport.onCleanUp();

  // TODO freeing the registry will eventually happen elsewhere
  this->registry.free<MeshComponent, PropertiesComponent, TransformComponent, ColorComponent>();
}