#pragma once

#include <SDL3/SDL.h>

#include "MouseEventHandler/mouse_event_handler.h"
#include "Pressable/pressable.h"
#include "Render/render.h"
#include "Text/text.h"

enum LabelPosition {
  LABEL_POS_START,
  LABEL_POS_END,
  LABEL_POS_CENTER,
};

class Button : public Pressable
{

private:
  Render *renderer;
  Text *text;

  SDL_FRect rect;
  SDL_Color background_color;
  SDL_Point padding;

public:
  Button(Render *renderer, MouseEventHandler *event_handler);

  void setText(Text *text);

  void onPress() override;
  void onPressIn() override;
  void onPressOut() override;
  void onPressLong() override;

  void setBoundingRect(SDL_FRect rect);
  void setDimensions(float w, float h);
  void setPosition(float x, float y);
  void setPadding(int x, int y);
  void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

  void build();
  void render();
};