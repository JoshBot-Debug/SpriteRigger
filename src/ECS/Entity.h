#pragma once

#include "Registry.h"
#include <stdint.h>
#include <string>

namespace ECS {
using EntityId = uint32_t;

/**
 * Entity represents an object within the ECS.
 * It holds a unique identifier, a name, and a reference to the Registry
 * that manages its components.
 */
class Entity {
private:
  EntityId m_Id = 0;  ///< Unique identifier for the entity.
  std::string m_Name; ///< Name of the entity.
  Registry *m_Registry =
      nullptr; ///< Pointer to the Registry managing this entity.

public:
  /**
   * Constructs an Entity with a given name, ID, and registry reference.
   *
   * @param name The name of the entity.
   * @param id The unique identifier for the entity.
   * @param registry A pointer to the Registry managing this entity.
   */
  Entity(EntityId id, const std::string &name, Registry *registry)
      : m_Id(id), m_Name(name), m_Registry(registry){};

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
   * Retrieves the entity's name.
   *
   * @return The entity's name.
   */
  const std::string &GetName() { return m_Name; }

  /**
   * Adds a component of type T to this entity.
   *
   * @param args Constructor arguments for the component.
   * @return A pointer to the newly created component.
   */
  template <typename T, typename... Args> T *Add(Args &&...args) {
    return m_Registry->Add<T>(m_Id, std::forward<Args>(args)...);
  }

  /**
   * Checks if this entity has a component of type T.
   *
   * @return True if the entity has the component, false otherwise.
   */
  template <typename T> bool Has() { return m_Registry->Has<T>(m_Id); }

  /**
   * Collects components of specified types from this entity.
   *
   * @return A tuple containing pointers to the collected components.
   */
  template <typename... T> std::tuple<T *...> Collect() {
    return m_Registry->Collect<T...>(m_Id);
  }

  /**
   * Retrieves a component of type T from this entity.
   *
   * @return A pointer to the component, or nullptr if not found.
   */
  template <typename T> T *Get() { return m_Registry->Get<T>(m_Id); }

  /**
   * Compares the name of this entity with a given name.
   *
   * @param name The name to compare against.
   * @return True if the names match, false otherwise.
   */
  bool Is(const std::string &name) { return m_Name == name; }

  /**
   * Compares the id of this entity with a given id.
   *
   * @param id The id to compare against.
   * @return True if the id match, false otherwise.
   */
  bool Is(EntityId id) { return m_Id == id; }

  /**
   * Removes components of specified types from this entity.
   */
  template <typename... T> void Remove() { (m_Registry->Remove<T>(m_Id), ...); }

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
} // namespace ECS
