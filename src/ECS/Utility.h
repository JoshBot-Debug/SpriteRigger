#pragma once

#include "Entity.h"

namespace ECS {
template <typename C, typename T>
inline bool Mutate(ECS::Registry *registry, T &current, const T &next) {
  if (current != next)
    registry->MarkChanged<C>();
  current = next;
  return current == next;
}
} // namespace ECS
