#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <stdint.h>
#include <vector>

#include <iostream>

namespace ECS2 {

using EntityId = size_t;

class Entity;

constexpr size_t MAX_COMPONENTS = 1024;

/**
 * Registry is a container for managing entities and their associated
 * components. It supports adding, retrieving, and deleting components of
 * various types associated with each entity.
 */
class Registry {

private:
  std::vector<std::shared_ptr<Entity>> m_Entities;
  // std::vector<std::shared_ptr<void>, MAX_COMPONENTS> m_Components;

  /**
   * A global counter for unique component type.
   */
  static size_t &GetComponentCounter() {
    static size_t counter = 0;
    return counter;
  }

public:
  Registry() { m_Entities.reserve(100); };

  ~Registry(){};

  /**
   * Retrieve a unique, stable ID for a component type.
   */
  template <typename T> static size_t GetComponentTypeID() {
    static const size_t id = ++GetComponentCounter();
    return id;
  }

  /**
   * Creates a new entity with a given name.
   *
   * @param id The id the entity (optional).
   * @return A pointer to the newly created entity.
   */
  template <typename T> Entity *CreateEntity(EntityId id = 0) {
    EntityId eid = static_cast<EntityId>(m_Entities.size() + 1);
    eid = id > eid ? id : eid;
    auto entity = std::make_shared<Entity>(eid, this);
    m_Entities.emplace_back(entity);
    return entity.get();
  };
};
} // namespace ECS2
