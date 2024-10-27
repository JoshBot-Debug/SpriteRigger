#include <stdio.h>
#include "ShadowFrame/shadow_frame.h"

void ShadowFrame::onDraw(float deltaTime)
{
  printf("FPS: %i\n", this->frame_rate.getFPS());
  printf("Delta: %f\n", deltaTime);

  this->render.clear(255, 255, 255, SDL_ALPHA_OPAQUE);

  this->render.draw();
}

void ShadowFrame::onInput(SDL_Event event)
{
  if (event.type == SDL_EVENT_MOUSE_MOTION)
  {
  }
}