#include "Keyboard.h"

#include <iostream>

Keyboard::Keyboard() : state(SDL_GetKeyboardState(nullptr)) {}

template <typename... Args>
bool Keyboard::isPressed(Args &&...args) const
{
  return (state[(int)std::forward<Args>(args)] && ...);
}

template bool Keyboard::isPressed<Key>(Key &&) const;
template bool Keyboard::isPressed<Key, Key>(Key &&, Key &&) const;
template bool Keyboard::isPressed<Key, Key, Key>(Key &&, Key &&, Key &&) const;
template bool Keyboard::isPressed<Key, Key, Key>(Key &&, Key &&, Key &&, Key &&) const;
