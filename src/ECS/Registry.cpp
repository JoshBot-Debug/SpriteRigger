#include "Registry.h"
#include "Entity.h"

namespace ECS
{
Registry::~Registry()
{
  for (auto entities : m_EntitiesByETID)
    for (auto e : entities)
      delete e;
}
} // namespace ECS
