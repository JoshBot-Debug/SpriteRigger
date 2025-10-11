#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <stdint.h>
#include <vector>

#include "Entity.h"
#include <iostream>

namespace ECS2 {

/**
 * Registry is a container for managing entities and their associated
 * components. It supports adding, retrieving, and deleting components of
 * various types associated with each entity.
 */
class Registry {

private:
  /// @brief Vector of Entities by Entity type id
  std::vector<std::vector<Entity *>> m_EntitiesByETID;

  /// @brief Vector of Entity ids by Entity type id
  std::vector<std::vector<EntityId>> m_FreeEntitySlotsByETID;

  /**
   * Registers a new entity type and returns it's id
   */
  template <typename T> EntityTypeId RegisterEntityType() {
    const EntityTypeId id = m_EntitiesByETID.size();
    if (id >= m_EntitiesByETID.size()) {
      m_EntitiesByETID.resize(id + 1);
      m_FreeEntitySlotsByETID.resize(id + 1);
    }
    return id;
  }

  /**
   * Retrieve a unique, stable type ID.
   */
  template <typename T> EntityTypeId GetEntityTypeId() {
    static const EntityTypeId id = RegisterEntityType<T>();
    return id;
  }

  /**
   * Retrieve a unique, stable ID.
   *
   * @return A pair containing the entity id and a bool determining if the eid
   * was reused (true) or new (false)
   */
  template <typename T> std::pair<EntityId, bool> GetEntityId() {
    auto tid = GetEntityTypeId<T>();

    if (!m_FreeEntitySlotsByETID[tid].empty()) {
      EntityId id = m_FreeEntitySlotsByETID[tid].back();
      m_FreeEntitySlotsByETID[tid].pop_back();
      return {id, true};
    }

    EntityId id = m_EntitiesByETID[tid].size();
    return {id, false};
  }

public:
  Registry() = default;

  ~Registry();

  /**
   * Creates a new entity with a given name.
   *
   * @return A pointer to the newly created entity.
   */
  template <typename T> Entity *CreateEntity() {
    EntityTypeId tid = GetEntityTypeId<T>();
    auto [id, reused] = GetEntityId<T>();

    if (reused)
      m_EntitiesByETID[tid][id] = new Entity{tid, id + 1, this};
    else
      m_EntitiesByETID[tid].emplace_back(new Entity{tid, id + 1, this});

    return m_EntitiesByETID[tid][id];
  }

  /**
   * Get an entity by id
   *
   * @return A point to the entity or nullptr if it was destroyed
   */
  template <typename T> Entity *GetEntity(EntityId id) {
    return m_EntitiesByETID[GetEntityTypeId<T>()][id - 1];
  }

  /**
   * Get all entities by type
   *
   * @return A vector of entity pointers, may contain nullptrs
   */
  template <typename T> std::vector<Entity *> GetEntities() {
    return m_EntitiesByETID[GetEntityTypeId<T>()];
  }

  /**
   * Destroys an entity
   */
  template <typename T> void DestroyEntity(EntityId id) {
    EntityTypeId tid = GetEntityTypeId<T>();
    delete m_EntitiesByETID[tid][id - 1];
    m_EntitiesByETID[tid][id - 1] = nullptr;
    m_FreeEntitySlotsByETID[tid].push_back(id - 1);
  }

  /**
   * Adds a component typename C to the specified entity.
   *
   * @param id The entity ID to which the component will be added.
   * @param args Constructor arguments for the component.
   * @return A pointer to the newly created component.
   */
  template <typename E, typename C, typename... CArgs>
  C *Add(EntityId id, CArgs &&...args) {
    EntityTypeId tid = GetEntityTypeId<E>();
    Entity *entity = GetEntity<E>(id);
    return entity->Add<C>(std::forward<CArgs>(args)...);
  }

  /**
   * Checks if any entity has a component C.
   *
   * @return True if the entity has the component, false otherwise.
   */
  template <typename C> bool Has() {
    for (auto entities : m_EntitiesByETID)
      for (auto entity : entities)
        if (entity && entity->Has<C>())
          return true;

    return false;
  }

  /**
   * Checks if an entity E has a component C.
   *
   * @param id The entity id.
   * @return True if the entity has the component, false otherwise.
   */
  template <typename E, typename C> bool Has(EntityId id) {
    EntityTypeId tid = GetEntityTypeId<E>();
    Entity *entity = GetEntity<E>(id);
    return entity->Has<C>();
  }

  /**
   * Retrieves components C from all entities.
   *
   * @return A vector of pointers to the component.
   */
  template <typename C> std::vector<C *> Get() {
    std::vector<C *> components;
    for (auto entities : m_EntitiesByETID)
      for (auto entity : entities) {
        if (C *component = entity->Get<C>())
          components.push_back(component);
      }
    return components;
  }

  /**
   * Retrieves a component C from entity E.
   *
   * @param id The entity id
   * @return A pointer to the component, or nullptr if not found.
   */
  template <typename E, typename C> C *Get(EntityId id) {
    EntityTypeId tid = GetEntityTypeId<E>();
    Entity *entity = GetEntity<E>(id);
    return entity->Get<C>();
  }

  /**
   * Removes all components of a specified type for the entity specified.
   *
   * @param id The entity id
   * @tparam C Component
   */
  template <typename E, typename... C> void Remove(EntityId id) {
    EntityTypeId tid = GetEntityTypeId<E>();
    Entity *entity = GetEntity<E>(id);
    entity->Remove<C...>();
  }

  /**
   * Removes all components from all entities
   *
   * @tparam C Component
   */
  template <typename E, typename... C> void Remove() {
    EntityTypeId tid = GetEntityTypeId<E>();
    std::vector<Entity *> entities = GetEntities<E>();
    for (auto entity : entities)
      entity->Remove<C...>();
  }
};
} // namespace ECS2
