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
  std::vector<Entity *> entities;
  std::unordered_map<EntityId, std::vector<std::any>> storage;

public:
  ~Registry();

  Entity *createEntity();

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

  template <typename... T>
  std::tuple<std::vector<T *>...> collect(EntityId entity = ALL)
  {
    return std::make_tuple(get<T>(entity)...);
  }

  template <typename U>
  std::vector<U *> get(EntityId entity = ALL)
  {
    std::vector<U *> result;

    if (entity > ALL)
    {
      for (auto &component : this->storage[entity])
        try
        {
          result.push_back(std::any_cast<U *>(component));
        }
        catch (const std::bad_any_cast &e)
        {
        }
      return result;
    }

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