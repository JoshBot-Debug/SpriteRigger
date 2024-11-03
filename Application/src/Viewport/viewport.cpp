#include "viewport.h"
#include <stdio.h>

Viewport::Viewport(SDL_Renderer *renderer, Vec2 size, Vec4 backgroundColor) : renderer(renderer), size(size), backgroundColor(backgroundColor)
{
  this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
}

Viewport::~Viewport()
{
  this->free();
}

void Viewport::setDimensions(Vec2 size)
{
  this->size = size;
}

void Viewport::resize(Vec2 size)
{
  SDL_DestroyTexture(this->texture);

  this->setDimensions(size);
  this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
}

void Viewport::setRenderer(SDL_Renderer *renderer)
{
  this->renderer = renderer;
  this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->size.x, this->size.y);
}

void Viewport::setBackgroundColor(Vec4 setBackgroundColor)
{
  this->backgroundColor = setBackgroundColor;
}

void Viewport::free()
{
  SDL_DestroyTexture(this->texture);
}

void Viewport::draw(const char *title, std::function<void()> callback)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::Begin(title);

  ImVec2 size = ImGui::GetContentRegionAvail();
  this->position = (Vec2)ImGui::GetWindowPos();

  if (this->size != size)
    this->resize(size);

  SDL_SetRenderTarget(this->renderer, this->texture);

  auto [r, g, b, a] = this->backgroundColor;
  SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
  SDL_RenderClear(this->renderer);

  callback();
  SDL_SetRenderTarget(this->renderer, nullptr);

  ImGui::Image(reinterpret_cast<ImTextureID>(this->texture), ImVec2(this->size));
  ImGui::End();
  ImGui::PopStyleVar();
}

Vec2 Viewport::getMousePosition(Vec2 position)
{
  Vec2 relative{
      position.x - this->position.x,
      position.y - this->position.y - ImGui::GetFrameHeight(),
  };

  if (relative.x < 0 || relative.x > this->size.x ||
      relative.y < 0 || relative.y > this->size.y)
    return Vec2{-1, -1};

  return relative;
}
