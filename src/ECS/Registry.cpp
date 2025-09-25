#include "Registry.h"

#include "Entity.h"

const std::vector<Entity *> Registry::GetEntities(const std::string &name) {
  std::vector<Entity *> results;
  for (auto &entity : m_Entities)
    if (entity->Is(name))
      results.push_back(entity.get());
  return results;
}
