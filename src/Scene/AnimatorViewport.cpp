#include <string>

#include "AnimatorViewport.h"
#include "Manager/SystemManager/SystemManager.h"

struct GrabPayload
{
  glm::vec2 offset{0, 0};
  int zIndex = 0;

  GrabPayload(glm::vec2 offset, int zIndex) : offset(offset), zIndex(zIndex) {}
};

void AnimatorViewport::onInitialize()
{
  this->setTitle("Animator");
  this->setBackgroundColor(glm::vec4{0.7f, 0.7f, 0.7f, 1.0f});
}

void AnimatorViewport::onUpdate(float deltaTime)
{
  this->systemManager->update(deltaTime);
}

void AnimatorViewport::onDrawViewport(float deltaTime)
{
  this->systemManager->draw(deltaTime);
}