#include <stdio.h>
#include "button.h"

void Button::onPress()
{
  printf("[%s] onPress \n", this->label);
}

void Button::onPressIn()
{
  printf("[%s] onPressIn \n", this->label);
}

void Button::onPressOut()
{
  printf("[%s] onPressOut \n", this->label);
}

void Button::onPressLong()
{
  printf("[%s] onPressLong \n", this->label);
}

void Button::setLabel(const char *label)
{
  this->label = label;
}

void Button::setDimensions(float w, float h)
{
  this->rect.w = w;
  this->rect.h = h;
  this->setBoundingRect(&this->rect);
}

void Button::setPosition(float x, float y)
{
  this->rect.x = x;
  this->rect.y = y;
  this->setBoundingRect(&this->rect);
}

void Button::setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  this->backgroundColor.r = r;
  this->backgroundColor.g = g;
  this->backgroundColor.b = b;
  this->backgroundColor.a = a;
}

void Button::render()
{
  this->renderer.rect(this->rect, this->backgroundColor);
}