#pragma once

#include "registry.h"

using EntityId = int;

class Entity
{
private:
  EntityId id;
  Registry *registry;

public:
  Entity(int id, Registry *registry) : id(id), registry(registry) {};

  template <typename T, typename... Args>
  T *add(Args &&...args)
  {
    return this->registry->add<T>(this->id, std::forward<Args>(args)...);
  }

  template <typename T>
  bool has()
  {
    return this->registry->has<T>(this->id);
  }

  template <typename T>
  std::vector<T *> get()
  {
    return this->registry->get<T>(this->id);
  }

  template <typename... T>
  void free()
  {
    (this->registry->free<T>(this->id), ...);
    delete this;
  }

  bool operator==(const Entity &other) const
  {
    return id == other.id;
  };
};