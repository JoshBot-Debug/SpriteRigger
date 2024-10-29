#include <stdio.h>
#include "ShadowFrame/shadow_frame.h"
#include "Button/button.h"
#include "Text/text.h"
#include "Font/font.h"

ShadowFrame::ShadowFrame() {}

ShadowFrame::~ShadowFrame() {}

void ShadowFrame::onInitialize()
{
  Font *font = new Font("fonts/Roboto-Regular.ttf");

  Text *text = new Text("File", font, &this->render);
  text->setFontSize(14);
  text->setColor({255, 255, 255, SDL_ALPHA_OPAQUE});

  this->button = new Button(&this->render, &this->m_event_handler);
  this->button->setPosition(100, 100);
  this->button->setBackgroundColor(50, 50, 50, SDL_ALPHA_OPAQUE);
  this->button->setText(text);
  this->button->setPadding(4, 3);
  this->button->build();
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