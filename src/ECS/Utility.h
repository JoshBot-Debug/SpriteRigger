#pragma once

#include "Entity.h"

namespace ECS {
template <typename C, typename T>
inline T Mutate(ECS::Registry *registry, T &current,
                const T &next) {
  if (current != next)
    registry->MarkChanged<C>();
  current = next;
  return current;
}
} // namespace ECS
