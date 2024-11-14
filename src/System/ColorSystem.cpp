#include "ColorSystem.h"
#include "utility.h"
#include "Component/components.h"

void ColorSystem::update(float deltaTime, Viewport *viewport, Registry *registry, Mouse *mouse)
{
  for (auto entity : registry->entities())
  {
    if (entity->is("Bone"))
    {
      auto bone = entity->get<CBone>();
      MouseEntityState state = mouse->getMouseEntityState(entity->getId());
      bone->color.b = state.isFocused ? 0 : 255;
    }
  }
}
