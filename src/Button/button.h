#pragma once

#include <SDL3/SDL.h>
#include "MouseEventHandler/mouse_event_handler.h"
#include "Pressable/pressable.h"
#include "Render/render.h"

class Button : public Pressable
{

private:
  const char *label;
  Render &renderer;
  SDL_FRect rect;
  SDL_Color backgroundColor;

public:
  Button(const char *label, MouseEventHandler &mouse_event_handler, Render &renderer) : Pressable(mouse_event_handler), label(label), renderer(renderer), rect({0, 0, 0, 0}), backgroundColor({}) {};

  void setLabel(const char *label);

  void onPress() override;
  void onPressIn() override;
  void onPressOut() override;
  void onPressLong() override;

  void setDimensions(float w, float h);
  void setPosition(float x, float y);
  void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

  void render();
};