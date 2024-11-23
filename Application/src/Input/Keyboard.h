#pragma once

#include <SDL3/SDL.h>

#include <unordered_set>
#include "KeyboardEvent.h"

class Keyboard
{
private:
  const bool *state;
  std::unordered_set<Key> pressed;

public:
  Keyboard();

  /**
   * Checks if a specific key is currently pressed.
   *
   * @param key The key to check (e.g., `Key::A`, `Key::B`, etc. from the `Key` enum).
   * @return true if the specified key is pressed, false otherwise.
   */
  template <typename... Args>
  bool isPressed(Args &&...args) const;
};