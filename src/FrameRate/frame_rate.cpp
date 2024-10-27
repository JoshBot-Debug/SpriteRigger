#include "frame_rate.h"

void FrameRate::setFPS(int framesPerSecond)
{
  this->tps = 1000 / framesPerSecond;
}

float FrameRate::start()
{
  startTime = SDL_GetTicks();
  float deltaTime = (startTime - this->lastTime) / 1000.0f;
  this->lastTime = startTime;
  return deltaTime;
}

void FrameRate::end()
{
  ++frames;

  int ticks = SDL_GetTicks() - startTime;

  if (ticks < this->tps)
    SDL_Delay(this->tps - ticks);

  this->frameTime = this->tps - ticks;
}

int FrameRate::getFPS()
{
  if (this->frameTime == 0)
    return -1;
  return 1000 / this->frameTime;
}

float FrameRate::getDeltaTime()
{
  return (SDL_GetTicks() - this->lastTime) / 1000.0f;
}