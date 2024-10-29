#pragma once

#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

class Font
{
private:
  TTF_Font *font;

public:
  Font(const char *path, float ptsize = 16);
  ~Font();

  TTF_Font *getFont();
};