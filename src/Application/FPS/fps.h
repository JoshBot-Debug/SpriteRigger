#pragma once

#include <SDL3/SDL.h>

/**
 * @brief A class to manage frames per second (FPS) and calculate delta time.
 *
 * This class is responsible for tracking the frame rate of a game or application,
 * allowing the user to set a target FPS and calculate the time elapsed between
 * frames for smooth rendering and updates.
 */
class FPS
{
private:
  float delta;    // The time elapsed since the last frame in seconds.
  float interval; // The time interval for the target FPS in seconds.

  Uint64 last; // The timestamp of the last frame in ticks.

public:
  /**
   * @brief Sets the target frames per second (FPS).
   *
   * This method checks whether enough time has passed to maintain the target FPS.
   * If the time elapsed exceeds the interval for the target FPS, it returns true,
   * indicating that the frame can be processed; otherwise, it returns false.
   *
   * @param fps The target frames per second to maintain.
   * @return true if the frame can be processed; false otherwise.
   */
  bool at(int fps);

  /**
   * @brief Retrieves the time elapsed since the last frame.
   *
   * This method returns the delta time, which is the time in seconds between
   * the current frame and the last frame. It is useful for making frame-rate
   * independent movements and updates.
   *
   * @return The delta time in seconds since the last frame.
   */
  float getDeltaTime();
};