#pragma once

#include <SDL3/SDL.h>

#include "KeyboardEvent.h"

class Keyboard
{
private:
  bool isLGuiDown = false;
  bool isLAltDown = false;
  bool isLCtrlDown = false;
  bool isLShiftDown = false;

  bool isRGuiDown = false;
  bool isRAltDown = false;
  bool isRCtrlDown = false;
  bool isRShiftDown = false;

  Key pressed;

public:
  /**
   * Set keyboard events
   */
  void onEvent(SDL_Event *event);

  /**
   * Checks if a specific key is currently pressed.
   *
   * @param key The key to check (e.g., `Key::A`, `Key::B`, etc. from the `Key` enum).
   * @return true if the specified key is pressed, false otherwise.
   */
  bool isPressed(Key key) const;

  /**
   * Checks if the specified Left or Right GUI (Windows) key is pressed.
   *
   * @param lr Specifies whether to check the Left (`LRKey::LEFT`) or Right (`LRKey::RIGHT`) GUI key.
   * @return true if the specified Left or Right GUI key is pressed, false otherwise.
   */
  bool isGuiPressed(LRKey lr) const;

  /**
   * Checks if the specified Left or Right Alt key is pressed.
   *
   * @param lr Specifies whether to check the Left (`LRKey::LEFT`) or Right (`LRKey::RIGHT`) Alt key.
   * @return true if the specified Left or Right Alt key is pressed, false otherwise.
   */
  bool isAltPressed(LRKey lr) const;

  /**
   * Checks if the specified Left or Right Control key is pressed.
   *
   * @param lr Specifies whether to check the Left (`LRKey::LEFT`) or Right (`LRKey::RIGHT`) Control key.
   * @return true if the specified Left or Right Control key is pressed, false otherwise.
   */
  bool isCtrlPressed(LRKey lr) const;

  /**
   * Checks if the specified Left or Right Shift key is pressed.
   *
   * @param lr Specifies whether to check the Left (`LRKey::LEFT`) or Right (`LRKey::RIGHT`) Shift key.
   * @return true if the specified Left or Right Shift key is pressed, false otherwise.
   */
  bool isShiftPressed(LRKey lr) const;

  /**
   * Retrieves the currently pressed key.
   *
   * @return The key that is currently pressed, represented by an element of the `Key` enum.
   *         If no key is pressed, the return value may be a special value (e.g., `Key::NONE` or
   *         similar, depending on your implementation).
   */
  Key getPressed() const;
};