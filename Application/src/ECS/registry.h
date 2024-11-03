#pragma once

#include <any>
#include <vector>
#include <unordered_map>

// This is called all so it's easy to undersand when looking at the
// intellisense for methods, basically it's the initial value which is zero.
// all ids start from 1
const int ALL = 0;

class Entity;

using EntityId = int;

class Registry
{
private:
  EntityId nEID = ALL;
  std::vector<Entity *> entt;
  std::unordered_map<EntityId, std::vector<std::any>> storage;

public:
  ~Registry();

  Entity *createEntity(const char *name);

  template <typename T, typename... Args>
  T *add(EntityId entity, Args &&...args)
  {
    T *component = new T(std::forward<Args>(args)...);
    this->storage[entity].push_back((std::any)component);
    return component;
  }

  template <typename T>
  bool has(EntityId entity)
  {
    return (this->storage.find(entity) != this->storage.end());
  }

  /**
   * If we are retrieving components from an entity
   * Then we can have only one component of each kind in the entity
   * auto [meshe, transform] = get<Mesh, Transform>();
   * meshe & transform will both be a single object.
   */
  template <typename... T>
  std::tuple<T *...> collect(EntityId entity)
  {
    return std::make_tuple(get<T>(entity)...);
  }

  /**
   * If we are retrieving components from an entity
   * Then we can have only one component of each kind in the entity
   * @returns a component or nullptr if nothing was found
   */
  template <typename U>
  U *get(EntityId entity)
  {
    for (auto &component : this->storage[entity])
      try
      {
        return std::any_cast<U *>(component);
      }
      catch (const std::bad_any_cast &e)
      {
      }
    return nullptr;
  }

  /**
   * If we are just all components regardless of entity
   * Then we will receive many of each components. They will be grouped together
   * in a vector.
   * auto [meshes, transforms] = get<Mesh, Transform>();
   * meshes & transforms will both be a vector.
   */
  template <typename... T>
  std::tuple<std::vector<T *>...> collect()
  {
    return std::make_tuple(get<T>()...);
  }

  /**
   * If we are just all components regardless of entity
   * Then we will receive a vector of components.
   */
  template <typename U>
  std::vector<U *> get()
  {
    std::vector<U *> result;

    for (const auto &[eid, components] : this->storage)
      for (auto &component : components)
        try
        {
          result.push_back(std::any_cast<U *>(component));
        }
        catch (const std::bad_any_cast &e)
        {
        }

    return result;
  }

  std::vector<Entity *> entities()
  {
    return this->entt;
  }

  template <typename T>
  void free(EntityId entity)
  {
    if (this->storage.find(entity) == this->storage.end())
      return;

    for (const auto &component : this->storage[entity])
    {
      try
      {
        delete std::any_cast<T *>(component);
      }
      catch (const std::bad_any_cast &e)
      {
        // The cast failed, this is no the object we want, skip.
      }
    }
  }

  template <typename... T>
  void free()
  {
    for (auto [eid, component] : this->storage)
      (this->free<T>(eid), ...);
  }
};