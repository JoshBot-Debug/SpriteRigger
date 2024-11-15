#include "SystemManager.h"
#include "Utility.h"

SystemManager::SystemManager(Registry *registry, Mouse *mouse, ShaderManager *shaderManager) : registry(registry), mouse(mouse), shaderManager(shaderManager)
{
  this->render = new RenderSystem(registry, shaderManager);
  this->transform = new TransformSystem(registry, mouse);
  this->color = new ColorSystem(registry, mouse);
}

void SystemManager::update(float deltaTime)
{
  // if (mouse->state == MouseState::PRESS_LEFT)
  //   if (Utility::intersects(mouse->position, viewport->getPosition(), viewport->getSize()))
  //     mouse->unfocus();

  this->transform->update(deltaTime);
  this->color->update(deltaTime);
}

void SystemManager::draw(float deltaTime)
{
  this->render->draw(deltaTime);
}