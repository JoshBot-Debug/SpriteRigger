#pragma once

#include <SDL3/SDL.h>

class FrameRate
{
public:
  FrameRate() : tps(16), frames(0), startTime(0), lastTime(SDL_GetTicks()) {};

  float start();
  void end();

  int getFPS();
  float getDeltaTime();

  void setFPS(int framesPerSecond);

private:
  int frames;
  int tps;

  Uint32 startTime;
  Uint32 frameTime;
  Uint32 lastTime;
};