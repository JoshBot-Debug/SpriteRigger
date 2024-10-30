#pragma once

#include <SDL3/SDL.h>

class FPSManager
{
public:
  FPSManager(int framesPerSecond = 60);

  float update();

  void setFPS(int framesPerSecond);

private:
  int tps;

  float deltaTime;
  Uint64 startTime;
  Uint64 lastTime;
};