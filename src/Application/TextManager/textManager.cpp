#include <stdexcept>

#include "textManager.h"

TextManager::TextManager(SDL_Renderer *renderer) : textEngine(nullptr)
{
  if (!TTF_Init())
    throw std::runtime_error(SDL_GetError());

  this->textEngine = TTF_CreateRendererTextEngine(renderer);
  if (this->textEngine == NULL)
    throw std::runtime_error(SDL_GetError());
}

TextManager::~TextManager()
{
  this->free();
}

void TextManager::free()
{
  for (const auto &text : this->texts)
    TTF_DestroyText(text.second);

  for (const auto &font : this->fonts)
    TTF_CloseFont(font.second);

  TTF_DestroyRendererTextEngine(this->textEngine);

  this->fonts.clear();
  this->texts.clear();
  this->textEngine = nullptr;
}

TTF_Font *TextManager::createFont(const char *name, const char *file, float ptsize)
{
  this->fonts[name] = TTF_OpenFont(file, ptsize);
  return this->fonts[name];
}

TTF_Text *TextManager::createText(const char *name, const char *fontName)
{
  if (this->fonts.find(fontName) == this->fonts.end())
    throw std::runtime_error(SDL_GetError());

  this->texts[name] = TTF_CreateText(this->textEngine, this->fonts[fontName], NULL, 0);
  return this->texts[name];
}

void TextManager::setTextColor(const char *textName, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  TTF_SetTextColor(this->texts[textName], r, g, b, a);
}

void TextManager::render(Text *text)
{
  TTF_SetTextString(this->texts[text->textName], text->content, text->length);
  TTF_DrawRendererText(this->texts[text->textName], text->x, text->y);
}