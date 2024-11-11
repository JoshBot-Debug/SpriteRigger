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
  Vec2 viewportMouse = this->getMousePosition(mouse->position);

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
        if (!viewportMouse.intersects(transform->position, bone->size))
          continue;

        auto payload = mouse->getGrabPayload<GrabPayload>();

        if (payload && payload->zIndex < bone->zIndex)
          mouse->release();

        mouse->grab<GrabPayload>(entity->getId(),
                                 viewportMouse - transform->position,
                                 bone->zIndex);
      }

      if (mouse->state == MouseState::RELEASED)
        mouse->release(entity->getId());

      if (mouse->isGrabbing(entity->getId()))
        transform->position = Vec2::lerp(transform->position, viewportMouse - mouse->getGrabPayload<GrabPayload>()->offset, deltaTime * 20);
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
    SDL_SetRenderDrawColor(this->app->getRenderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(this->app->getRenderer(), &rect);
  }
}

void AnimatorViewport::onCleanUp()
{
}
