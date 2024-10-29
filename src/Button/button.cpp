#include <stdio.h>
#include <algorithm>
#include "button.h"

Button::Button(Render *renderer, MouseEventHandler *event_handler) : Pressable(event_handler), renderer(renderer), text(nullptr), rect({0, 0, 0, 0}), background_color({}), padding({0, 0}) {}

void Button::onPress()
{
  printf("onPress \n");
}

void Button::onPressIn()
{
  printf("onPressIn \n");
}

void Button::onPressOut()
{
  printf("onPressOut \n");
}

void Button::onPressLong()
{
  printf("onPressLong \n");
}

void Button::setText(Text *text)
{
  this->text = text;
}

void Button::setDimensions(float w, float h)
{
  this->rect.w = w;
  this->rect.h = h;
  Pressable::setBoundingRect(&this->rect);
}

void Button::setPosition(float x, float y)
{
  this->rect.x = x;
  this->rect.y = y;
  Pressable::setBoundingRect(&this->rect);
}

void Button::setBoundingRect(SDL_FRect rect)
{
  this->rect = rect;
  Pressable::setBoundingRect(&this->rect);
}

void Button::setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  this->background_color.r = r;
  this->background_color.g = g;
  this->background_color.b = b;
  this->background_color.a = a;
}

void Button::setPadding(int x, int y)
{
  this->padding.x = x;
  this->padding.y = y;
}

void Button::build()
{
  SDL_FRect *text_rect = this->text->getRect();

  this->rect.w = std::max(this->rect.w, text_rect->w + (this->padding.x * 2));
  this->rect.h = std::max(this->rect.h, text_rect->h + (this->padding.y * 2));

  // (text_rect->w - 4) & (text_rect->h - 4) is to compensate for the font center issue
  float x = this->rect.x + (this->rect.w / 2) - ((text_rect->w - 4) / 2);
  float y = this->rect.y + (this->rect.h / 2) - ((text_rect->h - 4) / 2);
  this->text->setPosition(x, y);
}

void Button::render()
{
  this->renderer->rect(this->rect, this->background_color);
  if (this->text)
    this->text->render();
}