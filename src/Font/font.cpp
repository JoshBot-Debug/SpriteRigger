#include "font.h"
#include <stdio.h>

Font::Font(const char *path, float ptsize) : font(nullptr)
{
  if (!TTF_Init())
    printf("TTF_Init failed with error: %s\n", SDL_GetError());

  this->font = TTF_OpenFont(path, ptsize);
  if (!font)
    printf("TTF_OpenFont Failed to load font at %s, ptsize: %f: %s\n", path, ptsize, SDL_GetError());
}

Font::~Font()
{
  TTF_CloseFont(this->font);
  TTF_Quit();
}

TTF_Font *Font::getFont()
{
  return this->font;
}