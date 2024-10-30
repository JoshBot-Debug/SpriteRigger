#include "FPSManager.h"
#include <stdio.h>

FPSManager::FPSManager(int framesPerSecond) : tps(1000 / framesPerSecond), startTime(0), deltaTime(0), lastTime(0) {};

void FPSManager::setFPS(int framesPerSecond)
{
  this->tps = 1000 / framesPerSecond;
}

float FPSManager::update()
{
  Uint64 ticks = SDL_GetTicks() - this->startTime;
  if (ticks < this->tps)
    SDL_Delay(this->tps - ticks);

  this->startTime = SDL_GetTicks();
  this->deltaTime = (this->startTime - this->lastTime) / 1000.0f;
  this->lastTime = this->startTime;
  return this->deltaTime;
}