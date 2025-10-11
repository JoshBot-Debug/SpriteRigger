#include "Registry.h"

#include "Entity.h"

namespace ECS {
const std::vector<Entity *> Registry::GetEntities(const std::string &name) {
  std::vector<Entity *> results;
  for (auto &[_, entity] : m_Entities)
    if (entity->Is(name))
      results.push_back(entity.get());
  return results;
}
} // namespace ECS