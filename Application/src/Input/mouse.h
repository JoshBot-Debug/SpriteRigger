#pragma once

#include "SDL3/SDL.h"
#include <any>
#include "common.h"

using GrabId = int;

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
  GrabId grabId = -1;   ///< Identifier for the currently grabbed input. -1 denotes not grabbing anything.
  std::any grabPayload; ///< payload passed into grab, can be retrieved anytime before releasing.

public:
  Vec2 position{0, 0};                     ///< Current position of the mouse.
  MouseState state = MouseState::RELEASED; ///< Current state of the mouse.

  /**
   * Grabs control of the mouse input.
   *
   * @param id The identifier for the grab; must be unique.
   *
   * @returns bool. True if GrabId was set, False if something else is being grabbed.
   */
  bool grab(GrabId id);

  /**
   * Grabs control of the mouse input.
   *
   * More often then not, when you grab an item you may want to capture
   * some information, like the mouseOffset, the zIndex of the mesh, etc.
   * You can pass that information to grab and retrieve it later.
   *
   * NOTE: When the grab is released, that object will be released from memory.
   *
   * @param id The identifier for the grab; must be unique.
   *
   * @returns bool. True if GrabId was set, False if something else is being grabbed.
   */
  template <typename T, typename... Args>
  bool grab(GrabId id, Args &&...args)
  {
    if (this->grabId != -1)
      return false;

    this->grabId = id;
    this->grabPayload = new T(std::forward<Args>(args)...);
    return true;
  }

  /**
   * Releases control of the mouse input grab for a specific identifier.
   *
   * This function will release the grab associated with the given id,
   * allowing other inputs to take control.
   *
   * @param id The identifier of the grab to release. Or blank to release whatever is there
   *
   * @return True if the specified id was release,
   *         false otherwise.
   */
  bool release(GrabId id = -1);

  /**
   * Checks if the mouse is currently grabbed by a specific identifier.
   *
   * @param id The identifier to check. Or blank to check if anything is being grabbed
   * @return True if the specified id is currently grabbing the mouse,
   *         false otherwise.
   */
  bool isGrabbing(GrabId id = -1);

  /**
   * Retrieves the current grab identifier.
   *
   * @return The identifier of the current grab, or -1 if no grab is active.
   */
  GrabId getGrabId();

  /**
   * Retrieves a reference to the grab payload
   *
   * @return The payload casted back to the object specified.
   *         However, if the cast fails or there is no payload, it returns nullptr
   */
  template <typename T>
  T *getGrabPayload()
  {
    try
    {
      return std::any_cast<T *>(this->grabPayload);
    }
    catch (const std::exception &e)
    {
      // Failed to cast do nothing.
    }
    return nullptr;
  }
};
