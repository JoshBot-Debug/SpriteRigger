#include "SystemManager.h"
#include "utility.h"

SystemManager::SystemManager()
{
  this->render = new RenderSystem();
  this->transform = new TransformSystem();
  this->color = new ColorSystem();
}

void SystemManager::update(float deltaTime, Viewport *viewport, Registry *registry, Mouse *mouse)
{
  if (mouse->state == MouseState::PRESS_LEFT)
    if (Utility::intersects(mouse->position, viewport->getPosition(), viewport->getSize()))
      mouse->unfocus();

  this->transform->update(deltaTime, viewport, registry, mouse);
  this->color->update(deltaTime, viewport, registry, mouse);
}

void SystemManager::draw(float deltaTime, Registry *registry)
{
  this->render->draw(deltaTime, registry);
}
