#include <string>

#include "AnimatorViewport.h"

struct GrabPayload
{
  glm::vec2 offset{0, 0};
  int zIndex = 0;

  GrabPayload(glm::vec2 offset, int zIndex) : offset(offset), zIndex(zIndex) {}
};

void AnimatorViewport::onInitialize(Registry *registry, Controller *controller, ResourceManager *resourceManager, ColorSystem *colorSystem, RenderSystem *renderSystem, TransformSystem *transformSystem)
{
  this->setTitle("Animator");
  this->setBackgroundColor(glm::vec4{0.7f, 0.7f, 0.7f, 1.0f});

  this->registry = registry;
  this->colorSystem = colorSystem;
  this->renderSystem = renderSystem;
  this->transformSystem = transformSystem;

  this->renderSystem->setCamera(&this->camera);
  this->renderSystem->setRegistry(this->registry);
  this->renderSystem->setResourceManager(resourceManager);

  this->mouse.setBounds(this->getPosition(), this->getDimensions(), MouseOrigin::CENTER);
}

void AnimatorViewport::onInput(SDL_Event *event, float deltaTime)
{
  this->mouse.onEvent(event);
  this->keyboard.onEvent(event);

  float speed = 200.0f;
  glm::vec2 moveDirection(0.0f, 0.0f);

  if (this->keyboard.isPressed(Key::LEFT))
    moveDirection.x = -1.0f;

  if (this->keyboard.isPressed(Key::RIGHT))
    moveDirection.x = 1.0f;

  if (this->keyboard.isPressed(Key::UP))
    moveDirection.y = -1.0f;

  if (this->keyboard.isPressed(Key::DOWN))
    moveDirection.y = 1.0f;

  this->camera.move(speed * moveDirection.x * deltaTime, speed * moveDirection.y * deltaTime);
}

void AnimatorViewport::onUpdate(float deltaTime)
{
  this->transformSystem->update(deltaTime, this->registry, &this->mouse, &this->camera);
  this->colorSystem->update(deltaTime, this->registry, &this->mouse);
}

void AnimatorViewport::onDrawViewport(float deltaTime)
{
  this->renderSystem->draw(deltaTime);
}

void AnimatorViewport::onResize(glm::vec2 size)
{
  this->camera.setDimensions(size.x, size.y);
}