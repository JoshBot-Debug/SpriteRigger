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

      MouseEntityState state = mouse->getMouseEntityState(entity->getId());

      if (state.isDragging)
        transform->position = glm::mix(transform->position, state.position, deltaTime * 20);

      // TODO move this to the right place [ColorSystem | TransformSystem]
      auto mesh = registry->get<CMesh>(bone->armature);
      mesh->mesh->updateInstanceBuffer("transform", 0, {transform->position.x, transform->position.y, 0.0f});
    }
  }
}