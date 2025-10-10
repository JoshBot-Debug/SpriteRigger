#pragma once

#include <algorithm>
#include <any>
#include <iostream>
#include <map>
#include <memory>
#include <stdint.h>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace ECS {
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
  std::unordered_map<EntityId, std::shared_ptr<Entity>>
      m_Entities; ///< List of all entities in the registry.
  std::map<EntityId, std::unordered_map<std::type_index, std::shared_ptr<void>>>
      m_Components; ///< Components indexed by entity ID.

  std::unordered_map<std::type_index, bool> m_Dirty;

  std::unordered_map<EntityId, std::unordered_map<std::type_index, bool>>
      m_DirtyTwo;
  std::unordered_map<EntityId, std::unordered_map<std::type_index, bool>>
      m_MarkForRemoval;

public:
  Registry() = default;

  ~Registry() { Remove(); };

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
    m_Entities.emplace(eid, entity);
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
    m_Components[entity][std::type_index(typeid(T))] = component;
    MarkChanged<T>(entity);
    return component.get();
  }

  /**
   * Checks if an entity has a specific component type.
   *
   * @param entity The entity ID to check.
   * @return True if the entity has the component, false otherwise.
   */
  template <typename T> bool Has(EntityId entity) {
    auto it = m_Components.find(entity);
    if (it == m_Components.end())
      return false;
    return (m_Components[entity].find(std::type_index(typeid(T))) !=
            m_Components[entity].end());
  }

  /**
   * Checks if any entity has a specific component type.
   *
   * @return True if the entity has the component, false otherwise.
   */
  template <typename T> bool Has() {
    for (auto [eid, components] : m_Components)
      if (components.find(std::type_index(typeid(T))) != components.end())
        return true;
    return false;
  }

  /**
   * Retrieves all components of a specified type across all entities.
   *
   * @return A vector of pointers to the components.
   */
  template <typename T> std::vector<std::pair<EntityId, T *>> Get() {
    std::vector<std::pair<EntityId, T *>> result;

    for (const auto &[eid, components] : m_Components)
      try {
        result.push_back({eid, std::static_pointer_cast<T>(
                                   components.at(std::type_index(typeid(T))))
                                   .get()});
      } catch (const std::exception &e) {
      }

    return result;
  }

  /**
   * Retrieves a component of type T from a specified entity.
   *
   * @param entity The entity ID from which to retrieve the component.
   * @return A pointer to the component, or nullptr if not found.
   */
  template <typename T> T *Get(EntityId entity) {
    try {
      return std::static_pointer_cast<T>(
                 m_Components.at(entity).at(std::type_index(typeid(T))))
          .get();
    } catch (const std::exception &e) {
    }
    return nullptr;
  }

  /**
   * Collects all components of specified types across all entities.
   *
   * @return A vector containing a pair of entity id & vectors of pointers to
   * the components.
   */
  template <typename... T>
  std::tuple<std::vector<std::pair<EntityId, T *>>...> Collect() {
    return std::make_tuple(Get<T>()...);
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
   * Retrieves an entity
   *
   * @return A pointer to the entity.
   */
  Entity *GetEntity(EntityId entity) { return m_Entities.at(entity).get(); }

  /**
   * Retrieves all entities
   *
   * @return A vector of pointers to the entity.
   */
  const std::vector<Entity *> GetEntities() {
    std::vector<Entity *> results;
    for (auto &[_, entity] : m_Entities)
      results.push_back(entity.get());
    return results;
  }

  /**
   * Retrieves all entities by name
   *
   * @return A vector of pointers to the entity.
   */
  const std::vector<Entity *> GetEntities(const std::string &name);

  /**
   * Removes all components of a specified type across all entities.
   */
  template <typename... T> void Remove() {
    for (auto &[eid, components] : m_Components)
      (components.erase(std::type_index(typeid(T))), ...);
  }

  /**
   * Removes all components of a specified type for the entity specified.
   */
  template <typename... T> void Remove(EntityId entity) {
    (m_Components.at(entity).erase(std::type_index(typeid(T))), ...);
  }

  /**
   * Removes the entity & all it's components.
   */
  void Remove(EntityId entity) {
    m_Entities.erase(entity);
    m_Components.erase(entity);
  }

  /**
   * Remove all entities & components
   */
  void Remove() {
    m_EID = 0;
    m_Entities.clear();
    m_Components.clear();
  }

  /**
   * Marks a component for removal, for the specified entity.
   * The component will be removed once ClearChanged() is called.
   */
  template <typename... T> void MarkForRemoval(EntityId entity) {
    (m_DirtyTwo[entity].insert_or_assign(std::type_index(typeid(T)), true),
     ...);
    (m_MarkForRemoval[entity].insert_or_assign(std::type_index(typeid(T)),
                                               true),
     ...);
  }

  /**
   * Check if a component is marked for removal for the specified entity
   * @return True if the component is marked for removal
   */
  template <typename T> bool MarkedForRemoval(EntityId entity) const {
    auto it = m_MarkForRemoval.find(entity);
    if (it == m_MarkForRemoval.end())
      return false;

    auto jt = it->second.find(std::type_index(typeid(T)));
    if (jt == it->second.end())
      return false;

    return jt->second;
  }

  /**
   * @brief Mark the given component type as changed.
   *
   * Sets the "changed" flag for the specified component type to true.
   *
   * @tparam T Component type to mark.
   */
  template <typename T> void MarkChanged(EntityId entity) {
    m_DirtyTwo[entity][std::type_index(typeid(T))] = true;
  }

  /**
   * @brief Check whether any specific component type has been marked as
   * changed.
   *
   * @tparam T Component type to query.
   * @return true if the component type T is marked as changed, false otherwise.
   */
  template <typename... T> bool AnyChanged() {
    for (auto &[eid, components] : m_DirtyTwo)
      if (((components.find(std::type_index(typeid(T))) != components.end() &&
            components[std::type_index(typeid(T))] == true) ||
           ...))
        return true;

    return false;
  }

  /**
   * Retrieves a component of type T from a specified entity if it was changed.
   *
   * @param entity The entity ID from which to retrieve the component.
   * @return A pointer to the component, or nullptr if nothing changed.
   */
  template <typename T> T *GetChanged(EntityId entity) {
    // Check if entity has any changed components
    auto dirtyIt = m_DirtyTwo.find(entity);
    if (dirtyIt == m_DirtyTwo.end())
      return nullptr;

    // Check if this component type is marked as changed
    const auto type = std::type_index(typeid(T));
    auto dirtyEntryIt = dirtyIt->second.find(type);
    if (dirtyEntryIt == dirtyIt->second.end() || !dirtyEntryIt->second)
      return nullptr;

    // Retrieve the component from the entity's component map
    auto compIt = m_Components.find(entity);
    if (compIt == m_Components.end())
      return nullptr;

    auto compEntryIt = compIt->second.find(type);
    if (compEntryIt == compIt->second.end())
      return nullptr;

    // Return the underlying pointer
    return std::static_pointer_cast<T>(compEntryIt->second).get();
  }

  /**
   * Retrieves a vecotr components of type T from all entities if it was
   * changed.
   *
   * @return A vector of pairs with the entity id & component
   */
  template <typename T> std::vector<std::pair<EntityId, T *>> GetChanged() {
    std::vector<std::pair<EntityId, T *>> result;

    for (const auto &[eid, components] : m_DirtyTwo)
      for (const auto &[cid, changed] : components) {
        if (cid != std::type_index(typeid(T)) || !changed)
          continue;
        try {
          result.push_back(
              {eid, std::static_pointer_cast<T>(m_Components.at(eid).at(cid))
                        .get()});
        } catch (const std::exception &e) {
        }
      }

    return result;
  }

  /**
   * Collects components of specified types from a single entity if there are
   * changes, nullptr otherwise.
   *
   * @param entity The entity ID from which to collect components.
   * @return A tuple containing pointers to the components or nullptr if nothing
   * changed.
   */
  template <typename... T> std::tuple<T *...> CollectChanged(EntityId entity) {
    return std::make_tuple(GetChanged<T>(entity)...);
  }

  /**
   * @brief Check whether multiple component types have been marked as changed
   * for the specified entity.
   *
   * Expands to a tuple of booleans, where each element corresponds to the
   * result of HasChanged for the given component type.
   *
   * @param entity The entity id
   * @return std::tuple<bool, bool, ...> with one entry per queried type.
   */
  template <typename... T> auto HasChanged(EntityId entity) {
    auto it = m_DirtyTwo.find(entity);

    if (it == m_DirtyTwo.end())
      return {};

    return std::make_tuple(
        (it->second.find(std::type_index(typeid(T))) != it->second.end() &&
         it->second[std::type_index(typeid(T))])...);
  }

  /**
   * @brief Clear the "changed" flag for a given component type.
   *
   * @note This method also removes any entities & components staged for removal
   * @tparam T Component type to reset.
   */
  template <typename... T> void ClearChanged() {
    for (auto &[eid, components] : m_DirtyTwo)
      for (auto &[cid, value] : components)
        if (((cid == std::type_index(typeid(T))) || ...))
          value = false;

    for (auto &[eid, components] : m_MarkForRemoval)
      ((ClearChanged<T>(eid)), ...);
  }

  /**
   * @brief Clear the "changed" flag for a given component type for the
   * specified entity.
   *
   * @note This method also removes any entities & components staged for
   * removal
   * @tparam T Component type to reset.
   */
  template <typename... T> void ClearChanged(EntityId entity) {
    for (auto &[cid, value] : m_DirtyTwo[entity])
      if (((cid == std::type_index(typeid(T))) || ...))
        value = false;

    auto it = m_MarkForRemoval.find(entity);
    if (it == m_MarkForRemoval.end())
      return;

    ((it->second.erase(std::type_index(typeid(T))) &&
      m_Components[it->first].erase(std::type_index(typeid(T)))),
     ...);
  }

  /**
   * @brief Clear the "changed" flag for a all component types.
   */
  void ClearChanged() {
    m_DirtyTwo.clear();

    for (auto &[eid, components] : m_MarkForRemoval)
      for (auto &[cid, value] : components)
        if (value)
          m_Components[eid].erase(cid);

    m_MarkForRemoval.clear();
  }
};
} // namespace ECS
