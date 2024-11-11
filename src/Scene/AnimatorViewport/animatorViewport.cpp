#include "animatorViewport.h"
#include "string.h"

struct GrabPayload
{
  Vec2 offset{0, 0};
  int zIndex = 0;

  GrabPayload(Vec2 offset, int zIndex) : offset(offset), zIndex(zIndex) {}
};

void AnimatorViewport::onInitialize()
{
  this->setTitle("Animator");
  this->setBackgroundColor(Vec4{180, 180, 180, 180});
}

void AnimatorViewport::onInput(SDL_Event *event, float deltaTime) {}

void AnimatorViewport::onUpdate(float deltaTime)
{
  Registry *registry = this->app->getRegistry();
  Mouse *mouse = this->app->getMouseInput();
  Vec2 mousePosition = this->getMousePosition(mouse->position);

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
      {

        if (!mousePosition.intersects(transform->position, bone->size))
          continue;

        // Need to make this a little more generic
        // Need to handle focused entity here as well.
        mouse->press(entity->getId(), transform->position, bone->zIndex);
      }

      auto [entityId, offset] = mouse->drag();

      if (entity->getId() == entityId)
        transform->position = Vec2::lerp(transform->position, offset, deltaTime * 20);
    }
  }
}

void AnimatorViewport::onDrawViewport(float deltaTime)
{
  Registry *registry = this->app->getRegistry();

  std::vector<std::pair<int, int>> bones;

  /**
   * This logic here should be moved to a RenderSystem class.
   * We'll need to account for
   *  - zIndex
   *  - Layers
   *  - etc...
   */
  for (auto entity : registry->entities())
    if (entity->is("Bone"))
      bones.emplace_back(entity->getId(), entity->get<CBone>()->zIndex);

  std::sort(bones.begin(), bones.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
            { return a.second < b.second; });

  for (auto [entityId, zIndex] : bones)
  {
    auto [transform, bone] = registry->collect<CTransform, CBone>(entityId);
    SDL_FRect rect = {transform->position.x, transform->position.y, bone->size.x, bone->size.y};
    SDL_SetRenderDrawColor(this->app->getRenderer(), bone->color.x, bone->color.y, bone->color.z, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(this->app->getRenderer(), &rect);
  }
}

void AnimatorViewport::onCleanUp()
{
}
