#include "ColorSystem.h"

#include "Utility.h"
#include "Component/Component.h"

void ColorSystem::update(float deltaTime, Registry *registry, Mouse *mouse)
{
  for (auto entity : registry->entities())
  {
    if (entity->is("Bone"))
    {
      auto bone = entity->get<CBone>();
      MouseEntityState state = mouse->getMouseEntityState(entity->getId());
      bone->color.b = state.isFocused ? 0.0f : 1.0f;

      // TODO move this to the right place [ColorSystem | TransformSystem]
      auto mesh = registry->get<CMesh>(bone->armature);
      mesh->mesh->updateInstanceBuffer("color", 0, {bone->color.r, bone->color.g, bone->color.b});
    }
  }
}
