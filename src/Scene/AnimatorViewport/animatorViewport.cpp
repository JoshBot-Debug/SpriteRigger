#include "animatorViewport.h"
#include "string.h"

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
   * This logic here should be moved to a PositionSystem class.
   * Eventually I will add a ResizeSystem, RotateSystem, PositionSystem, etc.
   */
  for (auto entity : registry->entities())
  {
    if (entity->is("Bone"))
    {
      auto [mesh, properties, transform] = entity->collect<MeshComponent, PropertiesComponent, TransformComponent>();

      if (mouse->state == MouseState::PRESS_LEFT)
      {
        if (!viewportMouse.intersects(transform->position, mesh->size))
          continue;

        auto payload = mouse->getGrabPayload<GrabPayload>();

        if (payload && payload->zIndex < properties->zIndex)
          mouse->release();

        mouse->grab<GrabPayload>(entity->getId(),
                                 viewportMouse - transform->position,
                                 properties->zIndex);
      }

      if (mouse->state == MouseState::RELEASED)
        mouse->release(entity->getId());

      if (mouse->isGrabbing(entity->getId()))
        transform->position = Vec2::lerp(transform->position, viewportMouse - mouse->getGrabPayload<GrabPayload>()->offset, deltaTime * 30);
    }
  }
}

void AnimatorViewport::onDrawViewport(float deltaTime)
{
  Registry *registry = this->app->getRegistry();

  std::vector<std::pair<int, int>> bones;

  for (auto entity : registry->entities())
    if (entity->is("Bone"))
      bones.emplace_back(entity->getId(), entity->get<PropertiesComponent>()->zIndex);

  std::sort(bones.begin(), bones.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
            { return a.second < b.second; });

  for (auto [entityId, zIndex] : bones)
  {
    auto [mesh, properties, transform, color] = registry->collect<MeshComponent, PropertiesComponent, TransformComponent, ColorComponent>(entityId);
    SDL_FRect rect = {transform->position.x, transform->position.y, mesh->size.x, mesh->size.y};
    SDL_SetRenderDrawColor(this->app->getRenderer(), color->color.x, color->color.y, color->color.z, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(this->app->getRenderer(), &rect);
  }
}

void AnimatorViewport::onCleanUp()
{
}
