#include "TransformSystem.h"

#include <glm/glm.hpp>

#include "Utility.h"
#include "Component/Component.h"

void TransformSystem::update(float deltaTime, Registry *registry, Mouse *mouse, OrthographicCamera *camera)
{
  for (auto entity : registry->entities())
  {
    if (entity->is("Bone"))
    {
      auto [transform, bone] = entity->collect<CTransform, CBone>();

      if (mouse->state == MouseState::PRESS_LEFT)
        /**
         * TODO for now, because I'm dealing with a plain
         * since the origin of the mesh is 0,0 and the scale is from -1,1
         * I need to subtract half the size of the bone from the transform position
         * I need a way to be able to know the size of the object without setting it.
         * This needs to be done automatically. 
         * bone.size needs to be removed. I am working with transform.scale
         * 
         * Found the solution. We need to use the Point-in-Polygon algorithm.
         * To optimize for this test, I may want to 
         */
        if (Utility::intersects(mouse->position, transform->position - camera->getPosition() - (bone->size / 2.0f), bone->size))
          mouse->press(entity->getId(), transform->position);
        else if (mouse->getMouseBoundsState() == MouseBounds::IN_BOUNDS)
          mouse->unfocus(entity->getId());

      MouseEntityState state = mouse->getMouseEntityState(entity->getId());

      if (state.isDragging)
        transform->position = glm::mix(transform->position, state.position, deltaTime * 20);
    }
  }
}