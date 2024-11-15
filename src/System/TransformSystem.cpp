#include "TransformSystem.h"

#include <glm/glm.hpp>

#include "Utility.h"
#include "Component/Component.h"

void TransformSystem::update(float deltaTime)
{
  for (auto entity : this->registry->entities())
  {
    if (entity->is("Bone"))
    {
      auto [transform, bone] = entity->collect<CTransform, CBone>();

      // if (mouse->state == MouseState::PRESS_LEFT)
      //   if (Utility::intersects((mouse->position - viewport->getPosition()), transform->position, bone->size))
      //     mouse->press(entity->getId(), transform->position);

      MouseEntityState state = this->mouse->getMouseEntityState(entity->getId());

      if (state.isDragging)
        transform->position = glm::mix(transform->position, state.position, deltaTime * 20);
    }
  }
}