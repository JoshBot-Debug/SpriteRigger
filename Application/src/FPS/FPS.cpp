#include "FPS.h"

bool FPS::at(int fps)
{
  Uint64 current = SDL_GetTicks();

  this->interval = fps == 0 ? 0 : 1000 / fps;

  bool frame = fps == 0 ? true : current - this->last >= this->interval;

  if (frame)
  {
    this->delta = (current - this->last) / 1000.0f;
    this->last = current;
  }

  return frame;
}

float FPS::getDeltaTime()
{
  return this->delta;
}