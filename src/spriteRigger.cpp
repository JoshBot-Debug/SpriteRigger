#include "spriteRigger.h"

#include <string>
#include <vector>
#include <stdio.h>

#include "Entity/components.h"

void SpriteRigger::onInitialize()
{
  this->headerPanelScene.onInitialize();
  this->explorerScene.onInitialize();
  this->animatorViewport.onInitialize();
}

void SpriteRigger::onInput(SDL_Event *event, float deltaTime)
{
  this->headerPanelScene.onInput(event, deltaTime);
  this->explorerScene.onInput(event, deltaTime);
  this->animatorViewport.onInput(event, deltaTime);
}

void SpriteRigger::onUpdate(float deltaTime)
{
  this->headerPanelScene.onUpdate(deltaTime);
  this->explorerScene.onUpdate(deltaTime);
  this->animatorViewport.onUpdate(deltaTime);
}

void SpriteRigger::onDraw(float deltaTime)
{
  this->headerPanelScene.onDraw(deltaTime);
  this->explorerScene.onDraw(deltaTime);
  this->animatorViewport.onDraw(deltaTime);
}

void SpriteRigger::onCleanUp()
{
  this->headerPanelScene.onCleanUp();
  this->explorerScene.onCleanUp();
  this->animatorViewport.onCleanUp();
  this->registry.free<MeshComponent, PropertiesComponent, TransformComponent, ColorComponent>();
}