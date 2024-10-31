#pragma once

#include <SDL3/SDL.h>

class FPSManager
{
public:
  /**
   * Initialize the FPS Manager with the desired frames per second.
   *
   * NOTE: If SDL_SetRenderVSync was set to true, you will not be able to increase the fps to anything beyond
   * your screen refresh rate.
   */
  FPSManager(int framesPerSecond = 60);

  /**
   * Invoked at the beginning of each draw cycle.
   * This method is responsible for keeping the application at the desired frame rate.
   *
   * @return Delta time. Can be used to calculate FPS using the formula 1 / deltaTime
   */
  float update();

  /**
   * Set the desired frames per second.
   *
   * NOTE: If SDL_SetRenderVSync was set to true, you will not be able to increase the fps to anything beyond
   * your screen refresh rate.
   */
  void setFPS(int framesPerSecond);

private:
  /**
   * The number of ticks per second (TPS) calculated as 1000 divided by framesPerSecond.
   * At 60 frames per second (FPS), this value will be 16 due to integer division.
   * If treated as a float, it would yield approximately 16.67
   */
  int tps;

  /**
   * The time elapsed between the current and the last frame,
   * measured in milliseconds. This value can be used for frame-rate independent
   * movement and animations.
   *
   * Can be used to calculate FPS using the formular 1 / deltaTime
   */
  float deltaTime;

  /**
   * The starting time of the current update cycle / game loop, measured in milliseconds.
   * This value is used to track the overall duration of the application's runtime.
   */
  Uint64 startTime;

  /**
   * The timestamp of the last frame update, measured in milliseconds.
   * This is startTime of the previous frame.
   */
  Uint64 lastTime;
};