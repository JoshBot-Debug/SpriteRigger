#include "viewport.h"
#include <stdio.h>

Viewport::Viewport(SDL_Renderer *renderer, int w, int h, SDL_Color backgroundColor) : renderer(renderer), w(w), h(h), backgroundColor(backgroundColor)
{
  this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->w, this->h);
}

Viewport::~Viewport()
{
  this->free();
}

void Viewport::setDimensions(float w, float h)
{
  this->w = w;
  this->h = h;
}

void Viewport::resize(float w, float h)
{
  this->w = w;
  this->h = h;
  SDL_DestroyTexture(this->texture);
  this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
}

void Viewport::setRenderer(SDL_Renderer *renderer)
{
  this->renderer = renderer;
  this->texture = SDL_CreateTexture(this->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->w, this->h);
}

void Viewport::setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
  this->backgroundColor.r = r;
  this->backgroundColor.g = g;
  this->backgroundColor.b = b;
  this->backgroundColor.a = a;
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
  if (this->w != size.x || this->h != size.y)
    this->resize(size.x, size.y);

  SDL_SetRenderTarget(this->renderer, this->texture);

  auto [r, g, b, a] = this->backgroundColor;
  SDL_SetRenderDrawColor(this->renderer, r, g, b, a);
  SDL_RenderClear(this->renderer);

  callback();
  SDL_SetRenderTarget(this->renderer, nullptr);

  ImGui::Image(reinterpret_cast<ImTextureID>(this->texture), ImVec2(this->w, this->h));
  ImGui::End();
  ImGui::PopStyleVar();
}
