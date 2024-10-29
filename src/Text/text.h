#pragma once

#include <Font/font.h>
#include "Render/render.h"

enum Position
{
  POSITION_START,
  POSITION_END,
  POSITION_CENTER,
};

class Text
{
private:
  Font *font;
  Render *renderer;

  TTF_TextEngine *engine;
  TTF_Text *text;

  SDL_FRect rect;

public:
  Text(const char *content, Font *font, Render *renderer);
  ~Text();

  void setPosition(int x, int y);
  bool setColor(SDL_Color color);
  bool setFontSize(float font_size);
  bool setContent(const char *content);

  SDL_FRect *getRect();
  void render();
};