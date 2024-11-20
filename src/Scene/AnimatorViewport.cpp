#include <string>

#include "AnimatorViewport.h"

struct GrabPayload
{
  glm::vec2 offset{0, 0};
  int zIndex = 0;

  GrabPayload(glm::vec2 offset, int zIndex) : offset(offset), zIndex(zIndex) {}
};

void AnimatorViewport::onInitialize(Registry *registry, Controller *controller, ColorSystem *colorSystem, RenderSystem *renderSystem, TransformSystem *transformSystem)
{
  this->setTitle("Animator");
  this->setBackgroundColor(glm::vec4{0.7f, 0.7f, 0.7f, 1.0f});

  this->registry = registry;
  this->colorSystem = colorSystem;
  this->renderSystem = renderSystem;
  this->transformSystem = transformSystem;

  this->mouse.setOffset(this->getPosition());

  this->renderSystem->setCamera(&this->camera);
  this->renderSystem->setRegistry(this->registry);
}

void AnimatorViewport::onInput(SDL_Event *event, float deltaTime)
{
  this->mouse.onEvent(event);
  /**
   * Can now set the position/rotation of the camera
   */
  // this->camera.setPosition(this->mouse.position.x, this->mouse.position.y);
  // this->camera.setRotation(this->mouse.position.x);
}

void AnimatorViewport::onUpdate(float deltaTime)
{
  this->transformSystem->update(deltaTime, this->registry, &this->mouse);
  this->colorSystem->update(deltaTime, this->registry, &this->mouse);
}

void AnimatorViewport::onDrawViewport(float deltaTime)
{
  this->renderSystem->draw(deltaTime);
}

void AnimatorViewport::onResize(float width, float height)
{
  this->camera.setDimensions(width, height);
}