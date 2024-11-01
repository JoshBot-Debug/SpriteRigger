#pragma once

#include <any>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdio.h>

class Entity;

using EntityId = int;

class Registry
{
private:
  EntityId nEID = 0;
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

  template <typename T>
  std::vector<T *> get(EntityId entity)
  {
    std::vector<T *> components;

    if (this->storage.find(entity) == this->storage.end())
      return components;

    for (const auto &component : this->storage[entity])
    {
      try
      {
        components.push_back(std::any_cast<T *>(component));
      }
      catch (const std::bad_any_cast &e)
      {
        // The cast failed, this is no the object we want, skip.
      }
    }

    return components;
  }

  template <typename... T>
  std::tuple<std::vector<T *>...> get()
  {
    return std::make_tuple(collect<T>()...);
  }

  template <typename U>
  std::vector<U *> collect()
  {
    std::vector<U *> results;

    for (const auto &[eid, components] : this->storage)
    {
      for (auto &component : components)
      {
        try
        {
          results.push_back(std::any_cast<U *>(component));
        }
        catch (const std::bad_any_cast &e)
        {
          // The cast failed, this is no the object we want, skip.
        }
      }
    }

    return results;
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