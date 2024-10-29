#include "text.h"

Text::Text(const char *content, Font *font, Render *renderer) : font(font), renderer(renderer), rect({0, 0, 0, 0})
{
  this->engine = TTF_CreateRendererTextEngine(this->renderer->getRenderer());
  if (!this->engine)
    printf("TTF_CreateRendererTextEngine Failed to create engine Error: %s\n", SDL_GetError());

  this->text = TTF_CreateText(this->engine, this->font->getFont(), content, strlen(content));
  if (!this->text)
    printf("TTF_CreateText Failed to create text Error: %s\n", SDL_GetError());

  int width = 0;
  int height = 0;
  TTF_GetTextSize(this->text, &width, &height);
  this->rect.w = static_cast<float>(width);
  this->rect.h = static_cast<float>(height);
}

Text::~Text()
{
  TTF_DestroyText(this->text);
  TTF_DestroyRendererTextEngine(this->engine);
}

bool Text::setColor(SDL_Color color)
{
  if (!TTF_SetTextColor(this->text, color.r, color.g, color.b, color.a))
  {
    printf("TTF_SetTextColor Failed to set text color Error: %s\n", SDL_GetError());
    return false;
  }
  return true;
}

bool Text::setFontSize(float font_size)
{
  if (!TTF_SetFontSize(this->font->getFont(), font_size))
  {
    printf("TTF_SetFontSize Failed to set font size Error: %s\n", SDL_GetError());
    return false;
  }
  return true;
}

bool Text::setContent(const char *content)
{
  // TODO not working
  if (!TTF_SetTextString(this->text, content, 0))
  {
    printf("TTF_SetTextString Failed to set text content Error: %s\n", SDL_GetError());
    return false;
  }
  return true;
}

SDL_FRect *Text::getRect()
{
  return &this->rect;
}

void Text::setPosition(int x, int y)
{
  this->rect.x = x;
  this->rect.y = y;
}

// void Text::justify(Position position, SDL_FRect *bounding_rect)
// {
//   if (!bounding_rect)
//     return;

//   switch (position)
//   {
//   case Position::POSITION_START:
//     this->rect.x = bounding_rect->x;
//     break;

//   case Position::POSITION_END:
//     this->rect.x = bounding_rect->x + bounding_rect->w - this->rect.w;
//     break;

//   case Position::POSITION_CENTER:
//     this->rect.x = bounding_rect->x + (bounding_rect->w / 2) - (this->rect.w / 2);
//     break;

//   default:
//     break;
//   }
// }

// void Text::align(Position position, SDL_FRect *bounding_rect)
// {
//   if (!bounding_rect)
//     return;

//   switch (position)
//   {
//   case Position::POSITION_START:
//     this->rect.y = bounding_rect->y;
//     break;

//   case Position::POSITION_END:
//     this->rect.y = bounding_rect->y + bounding_rect->h - this->rect.h;
//     break;

//   case Position::POSITION_CENTER:
//     this->rect.y = bounding_rect->y + (bounding_rect->h / 2) - (this->rect.h / 2);
//     break;

//   default:
//     break;
//   }
// }

void Text::render()
{
  if (!TTF_DrawRendererText(this->text, this->rect.x, this->rect.y))
    printf("TTF_DrawRendererText Failed to render text Error: %s\n", SDL_GetError());
}