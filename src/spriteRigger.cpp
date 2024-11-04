#include "spriteRigger.h"

#include <string>
#include <vector>
#include <stdio.h>

#include "Entity/components.h"

void SpriteRigger::onInitialize()
{
  this->headerPanelScene.onInitialize();
  this->toolsScene.onInitialize();
  this->animatorViewport.onInitialize();
}

void SpriteRigger::onInput(SDL_Event *event, float deltaTime)
{
  this->headerPanelScene.onInput(event, deltaTime);
  this->toolsScene.onInput(event, deltaTime);
  this->animatorViewport.onInput(event, deltaTime);
}

void SpriteRigger::onUpdate(float deltaTime)
{
  this->headerPanelScene.onUpdate(deltaTime);
  this->toolsScene.onUpdate(deltaTime);
  this->animatorViewport.onUpdate(deltaTime);
}

void SpriteRigger::onDraw(float deltaTime)
{
  this->headerPanelScene.onDraw(deltaTime);
  this->toolsScene.onDraw(deltaTime);
  this->animatorViewport.onDraw(deltaTime);
}

void SpriteRigger::onCleanUp()
{
  this->headerPanelScene.onCleanUp();
  this->toolsScene.onCleanUp();
  this->animatorViewport.onCleanUp();
  this->registry.free<MeshComponent, PropertiesComponent, TransformComponent, ColorComponent>();
}