#include "ColorSystem.h"

#include "Utility.h"
#include "Component/Component.h"

void ColorSystem::update(float deltaTime)
{
  for (auto entity : this->registry->entities())
  {
    if (entity->is("Bone"))
    {
      auto bone = entity->get<CBone>();
      MouseEntityState state = this->mouse->getMouseEntityState(entity->getId());
      bone->color.b = state.isFocused ? 0 : 255;
    }
  }
}
