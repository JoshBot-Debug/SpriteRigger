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
   * @tparam E Entity
   * @return A pointer to the newly created entity.
   */
  template <typename E> Entity *CreateEntity() {
    EntityTypeId tid = GetEntityTypeId<E>();
    auto [id, reused] = GetEntityId<E>();

    if (reused)
      m_EntitiesByETID[tid][id] = new Entity{tid, id + 1, this};
    else
      m_EntitiesByETID[tid].emplace_back(new Entity{tid, id + 1, this});

    return m_EntitiesByETID[tid][id];
  }

  /**
   * Get an entity by id
   *
   * @tparam E Entity
   * @return A point to the entity or nullptr if it was destroyed
   */
  template <typename E> Entity *GetEntity(EntityId id) {
    return m_EntitiesByETID[GetEntityTypeId<E>()][id - 1];
  }

  /**
   * Get all entities by type
   *
   * @tparam E Entity
   * @return A vector of entity pointers, may contain nullptrs
   */
  template <typename E> std::vector<Entity *> GetEntities() {
    return m_EntitiesByETID[GetEntityTypeId<E>()];
  }

  /**
   * Destroys an entity
   *
   * @tparam E Entity
   */
  template <typename E> void DestroyEntity(EntityId id) {
    EntityTypeId tid = GetEntityTypeId<E>();
    delete m_EntitiesByETID[tid][id - 1];
    m_EntitiesByETID[tid][id - 1] = nullptr;
    m_FreeEntitySlotsByETID[tid].push_back(id - 1);
  }

  /**
   * Adds a component typename C to the specified entity.
   *
   * @tparam E Entity
   * @tparam C Component
   * @param id The entity ID to which the component will be added.
   * @param args Constructor arguments for the component.
   * @return A pointer to the newly created component.
   */
  template <typename E, typename C, typename... CArgs>
  C *Add(EntityId id, CArgs &&...args) {
    Entity *entity = GetEntity<E>(id);
    return entity->Add<C>(std::forward<CArgs>(args)...);
  }

  /**
   * Checks if any entity has a component C.
   *
   * @tparam C Component
   * @return True if the entity has the component, false otherwise.
   */
  template <typename C> bool Has() {
    for (auto &entities : m_EntitiesByETID)
      for (Entity *entity : entities)
        if (entity && entity->Has<C>())
          return true;

    return false;
  }

  /**
   * Checks if an entity E has a component C.
   *
   * @tparam E Entity
   * @tparam C Component
   * @param id The entity id.
   * @return True if the entity has the component, false otherwise.
   */
  template <typename E, typename C> bool Has(EntityId id) {
    Entity *entity = GetEntity<E>(id);
    return entity->Has<C>();
  }

  /**
   * Retrieves a component C from entity E.
   *
   * @tparam C Component
   * @param id The entity id
   * @return A pointer to the component, or nullptr if not found.
   */
  template <typename E, typename C> C *Get(EntityId id) {
    Entity *entity = GetEntity<E>(id);
    return entity->Get<C>();
  }

  /**
   * Retrieves components C from all entities.
   *
   * @tparam C Component
   * @return A vector of pointers to the component & entity.
   */
  template <typename... C>
  std::vector<std::pair<Entity *, std::tuple<C *...>>> Get() {
    std::vector<std::pair<Entity *, std::tuple<C *...>>> components;
    for (auto &entities : m_EntitiesByETID)
      for (Entity *entity : entities)
        components.emplace_back(entity, std::make_tuple(entity->Get<C>()...));
    return components;
  }

  /**
   * Removes all components of a specified type for the entity specified.
   *
   * @param id The entity id
   * @tparam C Component
   */
  template <typename E, typename... C> void Remove(EntityId id) {
    Entity *entity = GetEntity<E>(id);
    entity->Remove<C...>();
  }

  /**
   * Removes all components from all entities
   *
   * @tparam C Component
   */
  template <typename E, typename... C> void Remove() {
    for (Entity *entity : GetEntities<E>())
      entity->Remove<C...>();
  }

  /**
   * Mark components for removal for an entity
   *
   * @note The component will be removed once ClearChanges() is called
   *
   * @tparam E Entity
   * @tparam C Component
   * @tparam Rest Components
   *
   * @param id The entity id
   */
  template <typename E, typename... C> void MarkForRemove(EntityId id) {
    Entity *entity = GetEntity<E>(id);
    entity->MarkForRemove<C...>();
  }

  /**
   * Mark components for removal for an entity type
   *
   * @note The component will be removed once ClearChanges() is called
   *
   * @tparam E Entity
   * @tparam C Component
   * @tparam Rest Components
   */
  template <typename E, typename... C> void MarkForRemove() {
    for (Entity *entity : GetEntities<E>())
      entity->MarkForRemove<C...>();
  }

  /**
   * Mark components for removal for all entities
   *
   * @note The component will be removed once ClearChanges() is called
   *
   * @tparam E Entity
   * @tparam C Component
   * @tparam Rest Components
   */
  template <typename... C> void MarkForRemove() {
    for (auto &entities : m_EntitiesByETID)
      for (Entity *entity : entities)
        entity->MarkForRemove<C...>();
  }

  /**
   * Mark the given components C as changed for an entity E
   *
   * @tparam E Entity
   * @tparam C Component
   */
  template <typename E, typename... C> void MarkChanged(EntityId id) {
    Entity *entity = GetEntity<E>(id);
    entity->MarkChanged<C...>();
  }

  /**
   * Mark the given components C as changed for all entities of type E
   *
   * @tparam E Entity
   * @tparam C Component
   */
  template <typename E, typename... C> void MarkChanged() {
    for (Entity *entity : GetEntities<E>())
      entity->MarkChanged<C...>();
  }

  /**
   * Get a component if it changed for all entities
   *
   * @tparam C Component
   * @return bool True if changed, false otherwise
   */
  template <typename C> std::vector<std::pair<Entity *, C *>> GetChanged() {
    std::vector<std::pair<Entity *, C *>> components;
    for (auto &entities : m_EntitiesByETID)
      for (Entity *entity : entities)
        if (entity->HasChanged<C>())
          components.emplace_back(entity, entity->Get<C>());
    return components;
  }

  /**
   * Get a component if it changed for entities of type E
   *
   * @tparam E Entity
   * @tparam C Component
   * @return bool True if changed, false otherwise
   */
  template <typename E, typename C>
  std::vector<std::pair<Entity *, C *>> GetChanged() {
    std::vector<std::pair<Entity *, C *>> components;
    for (Entity *entity : GetEntities<E>())
      if (entity->HasChanged<C>())
        components.emplace_back(entity, entity->Get<C>());
    return components;
  }

  /**
   * Check if a component has changed for an entity
   *
   * @tparam E Entity
   * @tparam C Component
   *
   * @param id The entity id
   * @return bool True if changed, false otherwise
   */
  template <typename E, typename C> bool HasChanged(EntityId id) {
    Entity *entity = GetEntity<E>(id);
    return entity->HasChanged<C>();
  }
};
} // namespace ECS2
