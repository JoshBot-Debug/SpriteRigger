#include <string>

#include "animatorViewport.h"

struct GrabPayload
{
  glm::vec2 offset{0, 0};
  int zIndex = 0;

  GrabPayload(glm::vec2 offset, int zIndex) : offset(offset), zIndex(zIndex) {}
};

void AnimatorViewport::onInitialize()
{
  this->setTitle("Animator");
  this->setBackgroundColor(glm::vec4{180, 180, 180, 180});
}

void AnimatorViewport::onInput(SDL_Event *event, float deltaTime) {}

void AnimatorViewport::onUpdate(float deltaTime)
{
  Registry *registry = this->app->getRegistry();
  Mouse *mouse = this->app->getMouseInput();

  if (mouse->state == MouseState::PRESS_LEFT)
    // if (mouse->position.intersects(this->getPosition(), this->getSize()))
    if (Utility::intersects(mouse->position, this->getPosition(), this->getSize()))
      mouse->unfocus();

  /**
   * This logic here should be moved to a TransformSystem class.
   * Eventually I will add a ResizeSystem, TransformSystem, etc.
   */
  for (auto entity : registry->entities())
  {
    if (entity->is("Bone"))
    {
      auto [transform, bone] = entity->collect<CTransform, CBone>();

      if (mouse->state == MouseState::PRESS_LEFT)
        if (Utility::intersects((mouse->position - this->getPosition()), transform->position, bone->size))
          mouse->press(entity->getId(), transform->position);

      MouseEntityState state = mouse->getMouseEntityState(entity->getId());

      bone->color.b = state.isFocused ? 0 : 255;

      if (state.isDragging)
        transform->position = glm::mix(transform->position, state.position, deltaTime * 20);
    }
  }
}

void AnimatorViewport::onDrawViewport(float deltaTime)
{
  Registry *registry = this->app->getRegistry();

  for (auto entity : registry->entities())
  {
    if (entity->is("Bone"))
    {
      auto [transform, bone] = entity->collect<CTransform, CBone>();
      SDL_FRect rect = {transform->position.x, transform->position.y, bone->size.x, bone->size.y};
      SDL_SetRenderDrawColor(this->app->getRenderer(), bone->color.r, bone->color.g, bone->color.b, SDL_ALPHA_OPAQUE);
      SDL_RenderFillRect(this->app->getRenderer(), &rect);
    }
  }
}

void AnimatorViewport::onCleanUp()
{
}
