#include <stdio.h>
#include "ShadowFrame/shadow_frame.h"
#include "Button/button.h"

ShadowFrame::ShadowFrame()
{
  this->button = new Button("FirstButton", this->mouse_event_handler, this->render);
  this->button->setPosition(100.0f, 100.0f);
  this->button->setDimensions(300.0f, 100.0f);
  this->button->setBackgroundColor(255, 255, 0, SDL_ALPHA_OPAQUE);
}

ShadowFrame::~ShadowFrame()
{
}

void ShadowFrame::onDraw(float deltaTime)
{
  Application::onDraw(deltaTime);

  // printf("FPS: %i\n", this->frame_rate.getFPS());
  // printf("Delta: %f\n", deltaTime);

  this->render.clear({100, 100, 100, SDL_ALPHA_OPAQUE});
  this->button->render();
  this->render.draw();
}

void ShadowFrame::onInput(SDL_Event event)
{
  Application::onInput(event);

  if (event.type == SDL_EVENT_MOUSE_MOTION)
  {
  }
}