#include "TransformSystem.h"

#include "Utility.h"
#include "Component/Component.h"

void TransformSystem::update(float deltaTime, Viewport *viewport, Registry *registry, Mouse *mouse)
{
  for (auto entity : registry->entities())
  {
    if (entity->is("Bone"))
    {
      auto [transform, bone] = entity->collect<CTransform, CBone>();

      if (mouse->state == MouseState::PRESS_LEFT)
        if (Utility::intersects((mouse->position - viewport->getPosition()), transform->position, bone->size))
          mouse->press(entity->getId(), transform->position);

      MouseEntityState state = mouse->getMouseEntityState(entity->getId());

      if (state.isDragging)
        transform->position = glm::mix(transform->position, state.position, deltaTime * 20);
    }
  }
}