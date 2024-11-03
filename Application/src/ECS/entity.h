#pragma once

#include "registry.h"

using EntityId = int;

class Entity
{
private:
  EntityId id;
  const char *name;
  Registry *registry;

public:
  Entity(const char *name, int id, Registry *registry) : id(id), name(name), registry(registry) {};

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

  template <typename... T>
  std::tuple<T *...> collect()
  {
    return this->registry->collect<T...>(this->id);
  }

  template <typename T>
  T *get()
  {
    return this->registry->get<T>(this->id);
  }

  bool is(const char *name)
  {
    return this->name == name;
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

  operator EntityId() const
  {
    return this->id;
  }
};