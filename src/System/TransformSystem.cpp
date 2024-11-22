#include "TransformSystem.h"

#include <glm/glm.hpp>

#include "Utility.h"
#include "Component/Component.h"

void TransformSystem::update(float deltaTime, Registry *registry, Mouse *mouse)
{
  for (auto entity : registry->entities())
  {
    if (entity->is("Bone"))
    {
      auto [transform, bone] = entity->collect<CTransform, CBone>();

      if (mouse->state == MouseState::PRESS_LEFT)
        if (Utility::intersects(mouse->position, transform->position, bone->size))
          mouse->press(entity->getId(), transform->position);
        else if (mouse->getMouseBoundsState() == MouseBounds::IN_BOUNDS)
          mouse->unfocus(entity->getId());

      MouseEntityState state = mouse->getMouseEntityState(entity->getId());

      if (state.isDragging)
        transform->position = glm::mix(transform->position, state.position, deltaTime * 20);
    }
  }
}