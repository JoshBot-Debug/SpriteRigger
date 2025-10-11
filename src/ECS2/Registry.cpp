#include "Registry.h"
#include "Entity.h"

namespace ECS2 {
Registry::~Registry() {
  for (auto entities : m_EntitiesByETID)
    for (auto e : entities)
      delete e;

  m_EntitiesByETID.clear();
  m_FreeEntitySlotsByETID.clear();
}
} // namespace ECS2
