#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include "ECS/Entity.h"

enum class MouseState
{
  RELEASED,
  PRESS_LEFT,
  PRESS_RIGHT,
  PRESS_MIDDLE,
  MOVING,
};

enum class MouseBounds
{
  NONE,
  IN_BOUNDS,
  OUT_OF_BOUNDS,
};

enum class MouseOrigin
{
  TOP_LEFT,
  CENTER,
};

struct MouseEntityState
{
  glm::vec2 position;
  bool isDragging;
  bool isFocused;
};

struct Bounds
{
  glm::vec2 *position = nullptr;
  glm::vec2 *dimensions = nullptr;
  MouseOrigin origin = MouseOrigin::TOP_LEFT;
  MouseBounds state = MouseBounds::NONE;

  Bounds() = default;
  Bounds(glm::vec2 *position, glm::vec2 *dimensions, MouseOrigin origin) : position(position), dimensions(dimensions), origin(origin) {}
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
  glm::vec2 entityPosition{0, 0};

  /**
   * The position of the mouse when the drag started, used to calculate movement offset.
   */
  glm::vec2 dragStart{0, 0};

  /**
   * To indicate if the user is dragging
   */
  bool isDragging = false;

  /**
   * To indicate if the entity is focused
   */
  bool isFocused = false;

  /**
   * The offset of the viewport, this will be subracted from the mouse's position.
   */
  glm::vec2 *offset;

  /**
   * The bounds of the mouse region
   */
  Bounds bounds;

  /**
   * Updates the mouse position
   */
  void updateMousePosition(float x, float y);

public:
  /**
   * The current position of the mouse.
   */
  glm::vec2 position{0, 0};

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
  void press(EntityID entity, glm::vec2 offset);

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

  /**
   * Set mouse events
   */
  void onEvent(SDL_Event *event);

  /**
   * Sets the bounds of the mouse.
   * Useful if you only want to capture mouse events within a certain region
   * in your screen. You can also set the origin of the mouse.
   */
  void setBounds(glm::vec2 *position, glm::vec2 *dimensions, MouseOrigin origin);

  /**
   * Check if the mouse intersects with a rect
   */
  bool intersects(const glm::vec2 &position, const glm::vec2 &size);

  /**
   * If setBounds was called, this can be used to determin if the mouse was
   * within the specified bounds or not.
   * @return MouseBounds::IN_BOUNDS or MouseBounds::OUT_OF_BOUNDS or MouseBounds::NONE
   */
  MouseBounds getMouseBoundsState();
};