#pragma once

#include <any>
#include <map>
#include <memory>
#include <stdint.h>
#include <typeindex>
#include <unordered_map>
#include <vector>

class Entity;

using EntityId = uint32_t;

/**
 * Registry is a container for managing entities and their associated
 * components. It supports adding, retrieving, and deleting components of
 * various types associated with each entity.
 */
class Registry {
private:
  EntityId m_EID = 0; ///< The next available entity ID.
  std::vector<std::shared_ptr<Entity>>
      m_Entities; ///< List of all entities in the registry.
  std::map<EntityId, std::unordered_map<std::type_index, std::shared_ptr<void>>>
      m_Components; ///< Components indexed by entity ID.

  std::unordered_map<std::type_index, bool> m_Dirty;

public:
  Registry() = default;

  /**
   * Creates a new entity with a given name.
   *
   * @param name The name of the entity.
   * @param id The id the entity (optional).
   * @return A pointer to the newly created entity.
   */
  Entity *CreateEntity(const std::string &name, EntityId id = 0) {
    if (id > m_EID)
      m_EID = id;
    uint32_t eid = id ? id : ++m_EID;
    auto entity = std::make_shared<Entity>(eid, name, this);
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
  T *Add(EntityId entity, Args &&...args) {
    auto component = std::make_shared<T>(std::forward<Args>(args)...);
    m_Components[entity][typeid(T)] = component;
    MarkChanged<T>();
    return component.get();
  }

  /**
   * Checks if an entity has a specific component type.
   *
   * @param entity The entity ID to check.
   * @return True if the entity has the component, false otherwise.
   */
  template <typename T> bool Has(EntityId entity) {
    return (m_Components.find(entity) != m_Components.end());
  }

  /**
   * Collects components of specified types from a single entity.
   *
   * @param entity The entity ID from which to collect components.
   * @return A tuple containing pointers to the components.
   */
  template <typename... T> std::tuple<T *...> Collect(EntityId entity) {
    return std::make_tuple(Get<T>(entity)...);
  }

  /**
   * Retrieves a component of type T from a specified entity.
   *
   * @param entity The entity ID from which to retrieve the component.
   * @return A pointer to the component, or nullptr if not found.
   */
  template <typename T> T *Get(EntityId entity) {
    try {
      return std::static_pointer_cast<T>(m_Components.at(entity).at(typeid(T)))
          .get();
    } catch (const std::exception &e) {
    }
    return nullptr;
  }

  /**
   * Retrieves all entities
   *
   * @return A vector of pointers to the entity.
   */
  const std::vector<Entity *> GetEntities() {
    std::vector<Entity *> results;
    for (auto &entity : m_Entities)
      results.push_back(entity.get());
    return results;
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

    for (const auto &[eid, components] : m_Components)
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
  template <typename T> void Free(EntityId entity) {
    m_Components.at(entity).erase(typeid(T));
    m_Dirty.erase(typeid(T));
  }

  /**
   * Frees all components of a specified type across all entities.
   */
  template <typename... T> void Free() {
    for (auto [eid, components] : m_Components)
      (components.erase(typeid(T)), ...);
    (m_Dirty.erase(typeid(T)), ...);
  }

  /**
   * @brief Mark the given component type as changed.
   *
   * Sets the "changed" flag for the specified component type to true.
   *
   * @tparam T Component type to mark.
   */
  template <typename T> void MarkChanged() { m_Dirty[typeid(T)] = true; }

  /**
   * @brief Check whether a specific component type has been marked as changed.
   *
   * @tparam T Component type to query.
   * @return true if the component type T is marked as changed, false otherwise.
   */
  template <typename T> bool HasChanged() {
    return m_Dirty.contains(typeid(T)) && m_Dirty.at(typeid(T));
  }

  /**
   * @brief Check whether multiple component types have been marked as changed.
   *
   * Expands to a tuple of booleans, where each element corresponds to the
   * result of HasChanged for the given component type.
   *
   * @return std::tuple<bool, bool, ...> with one entry per queried type.
   */
  template <typename T, typename U, typename... Rest>
  std::tuple<bool> HasChanged() {
    return std::tuple_cat(std::make_tuple(HasChanged<T>()),
                          HasChanged<U, Rest...>());
  }

  /**
   * @brief Clear the "changed" flag for a given component type.
   *
   * @tparam T Component type to reset.
   */
  template <typename T> void ClearChanged() { m_Dirty[typeid(T)] = false; }
};