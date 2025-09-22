#pragma once

#include <any>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <stdint.h>

class Entity;

using EntityID = uint32_t;

/**
 * Registry is a container for managing entities and their associated
 * components. It supports adding, retrieving, and deleting components of
 * various types associated with each entity.
 */
class Registry {
private:
  EntityID m_EID = 0; ///< The next available entity ID.
  std::vector<std::shared_ptr<Entity>>
      m_Entities; ///< List of all entities in the registry.
  std::unordered_map<EntityID,
                     std::unordered_map<std::type_index, std::shared_ptr<void>>>
      m_Storage; ///< Storage of components indexed by entity ID.

public:
  Registry() = default;

  /**
   * Creates a new entity with a given name.
   *
   * @param name The name of the entity.
   * @return A pointer to the newly created entity.
   */
  Entity *CreateEntity(const std::string &name) {
    ++m_EID;
    auto entity = std::make_shared<Entity>(m_EID, name, this);
    m_Entities.push_back(entity);
    return entity.get();
  };

  /**
   * Adds a component of type T to the specified entity.
   *
   * @param entity The entity ID to which the component will be added.
   * @param args Constructor arguments for the component.
   * @return A pointer to the newly created component.
   */
  template <typename T, typename... Args>
  T *Add(EntityID entity, Args &&...args) {
    auto component = std::make_shared<T>(std::forward<Args>(args)...);
    m_Storage[entity][typeid(T)] = component;
    return component.get();
  }

  /**
   * Checks if an entity has a specific component type.
   *
   * @param entity The entity ID to check.
   * @return True if the entity has the component, false otherwise.
   */
  template <typename T> bool Has(EntityID entity) {
    return (m_Storage.find(entity) != m_Storage.end());
  }

  /**
   * Collects components of specified types from a single entity.
   *
   * @param entity The entity ID from which to collect components.
   * @return A tuple containing pointers to the components.
   */
  template <typename... T> std::tuple<T *...> Collect(EntityID entity) {
    return std::make_tuple(Get<T>(entity)...);
  }

  /**
   * Retrieves a component of type T from a specified entity.
   *
   * @param entity The entity ID from which to retrieve the component.
   * @return A pointer to the component, or nullptr if not found.
   */
  template <typename T> T *Get(EntityID entity) {
    try {
      return std::static_pointer_cast<T>(m_Storage.at(entity).at(typeid(T)))
          .get();
    } catch (const std::exception &e) {
    }
    return nullptr;
  }

  /**
   * Collects all components of specified types across all entities.
   *
   * @return A tuple containing vectors of pointers to the components.
   */
  template <typename... T> std::tuple<std::vector<T *>...> Collect() {
    return std::make_tuple(Get<T>()...);
  }

  /**
   * Retrieves all components of a specified type across all entities.
   *
   * @return A vector of pointers to the components.
   */
  template <typename T> std::vector<T *> Get() {
    std::vector<T *> result;

    for (const auto &[eid, components] : m_Storage)
      try {
        result.push_back(
            std::static_pointer_cast<T>(components.at(typeid(T))).get());
      } catch (const std::bad_any_cast &e) {
      }

    return result;
  }

  /**
   * Retrieves all entities in the registry.
   *
   * @return A vector of pointers to all entities.
   */
  std::vector<Entity *> Entities() {
    std::vector<Entity *> entities(m_Entities.size());
    for (auto entity : m_Entities)
      entities.push_back(entity.get());
    return entities;
  }

  /**
   * Frees a specific component type from the specified entity.
   *
   * @param entity The entity ID from which to free the component.
   */
  template <typename T> void Free(EntityID entity) {
    m_Storage.at(entity).erase(typeid(T));
  }

  /**
   * Frees all components of a specified type across all entities.
   */
  template <typename... T> void Free() {
    for (auto [eid, components] : m_Storage)
      (components.erase(typeid(T)), ...);
  }
};