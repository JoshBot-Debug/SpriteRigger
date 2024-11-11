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

class Mouse
{
private:
  /**
   * The ID of the entity currently being dragged.
   * A value of -1 indicates no entity is being dragged.
   */
  EntityID dragEntity = -1;

  /**
   * The position of the entity being dragged at the time the drag started.
   */
  Vec2 dragEntityPosition{-1, -1};

  /**
   * The z-index of the entity being dragged.
   * A value of -1 indicates no z-index has been set.
   */
  int dragEntityZIndex = -1;

  /**
   * The position of the mouse when the drag started, used to calculate movement offset.
   */
  Vec2 dragStart{-1, -1};

  /**
   * Resets the drag state, clearing any dragged entity and resetting
   * the drag-related attributes to their default values.
   */
  void resetDrag();

public:
  /**
   * The current position of the mouse.
   * Initialized to {-1, -1} to indicate no valid position.
   */
  Vec2 position{-1, -1};

  /**
   * The current state of the mouse (e.g., pressed, released, etc.).
   */
  MouseState state = MouseState::RELEASED;

  /**
   * Starts a drag operation for a specified entity.
   *
   * @param entity The ID of the entity being dragged.
   * @param position The initial entity position when the drag started.
   * @param zIndex Optional z-index to control the stacking order of the dragged entity.
   */
  void press(EntityID entity, Vec2 offset, int zIndex = -1);

  /**
   * @return A tuple containing the dragged entity ID and the current drag offset.
   */
  std::tuple<EntityID, Vec2> drag();

  /**
   * Sets the current state of the mouse (pressed, released, etc.).
   *
   * @param state The desired mouse state.
   */
  void setState(MouseState state);
};
