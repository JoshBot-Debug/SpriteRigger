#pragma once

#include "ECS/entity.h"
#include "common.h"

enum class MouseState
{
  RELEASED,
  PRESS_LEFT,
  PRESS_RIGHT,
  PRESS_MIDDLE,
  MOVING,
};

struct MouseEntityState
{
  Vec2 position;
  bool isDragging;
  bool isFocused;
};

class Mouse
{
private:
  /**
   * The ID of the entity currently being dragged.
   * A value of -1 indicates no entity is being dragged.
   */
  EntityID entity = -1;

  /**
   * The position of the entity being dragged at the time the drag started.
   */
  Vec2 entityPosition{0, 0};

  /**
   * The position of the mouse when the drag started, used to calculate movement offset.
   */
  Vec2 dragStart{0, 0};

  /**
   * To indicate if the user is dragging
   */
  bool isDragging = false;

  /**
   * To indicate if the entity is focused
   */
  bool isFocused = false;

public:
  /**
   * The current position of the mouse.
   */
  Vec2 position{0, 0};

  /**
   * The current state of the mouse (e.g., pressed, released, etc.).
   */
  MouseState state = MouseState::RELEASED;

  /**
   * Focuses on the specified entity.
   *
   * @param entity The ID of the entity being dragged.
   * @param position The initial entity position when the drag started.
   */
  void press(EntityID entity, Vec2 offset);

  /**
   * Unfocuses from the specified entity. If left blank, will unfocus from the entity in focus.
   */
  void unfocus(EntityID entity = -1);

  /**
   * Sets the current state of the mouse (pressed, released, etc.).
   *
   * @param state The desired mouse state.
   */
  void setState(MouseState state);

  /**
   * Gets the entity's mouse state.
   */
  MouseEntityState getMouseEntityState(EntityID entity);
};
