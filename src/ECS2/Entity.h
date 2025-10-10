#pragma once

#include "Registry.h"

#include <array>
#include <bitset>
#include <stdint.h>

namespace ECS2 {

/**
 * Entity represents an object within the ECS.
 * It holds a unique identifier, a name, and a reference to the Registry
 * that manages its components.
 */
class Entity {
private:
  EntityId m_Id = 0;
  Registry *m_Registry = nullptr;

  std::bitset<MAX_COMPONENTS> m_HasComponent;
  std::bitset<MAX_COMPONENTS> m_DirtyComponent;

public:
  /**
   * Constructs an Entity with a given name, ID, and registry reference.
   *
   * @param name The name of the entity.
   * @param id The unique identifier for the entity.
   * @param registry A pointer to the Registry managing this entity.
   */
  Entity(EntityId id, Registry *registry) : m_Id(id), m_Registry(registry){};

  ~Entity() {
    m_Id = 0;
    m_Registry = nullptr;
  }

  /**
   * Retrieves the entity's unique identifier.
   *
   * @return The entity's ID.
   */
  EntityId GetId() { return m_Id; }

  /**
   * Equality operator to compare two entities by their IDs.
   *
   * @param other The other entity to compare.
   * @return True if both entities have the same ID, false otherwise.
   */
  bool operator==(const Entity &other) const { return m_Id == other.m_Id; };

  /**
   * Implicit conversion operator to convert Entity to EntityId.
   *
   * @return The entity's ID.
   */
  operator EntityId() const { return m_Id; }
};
} // namespace ECS2
