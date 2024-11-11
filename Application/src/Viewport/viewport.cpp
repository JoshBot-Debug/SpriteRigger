#include "viewport.h"
#include <algorithm>

Viewport::Viewport(Application *application)
{
  this->texture = SDL_CreateTexture(application->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
  this->application = application;
}

Viewport::~Viewport()
{
  this->free();
}

void Viewport::setTitle(const char *title)
{
  this->title = title;
}

void Viewport::setWindowFlags(ImGuiWindowFlags flags)
{
  this->flags = flags;
}

void Viewport::setDimensions(Vec2 size)
{
  this->size = size;
}

void Viewport::resize(Vec2 size)
{
  SDL_DestroyTexture(this->texture);

  this->setDimensions(size);
  this->texture = SDL_CreateTexture(this->application->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
}

void Viewport::setBackgroundColor(Vec4 setBackgroundColor)
{
  this->backgroundColor = setBackgroundColor;
}

void Viewport::free()
{
  SDL_DestroyTexture(this->texture);
}

void Viewport::onDraw(float deltaTime)
{
  SDL_Renderer *renderer = this->application->getRenderer();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::Begin(this->title, nullptr, this->flags);

  ImVec2 size = ImGui::GetContentRegionAvail();
  this->position = ImGui::GetWindowPos();

  if (this->size != size)
    this->resize(size);

  SDL_SetRenderTarget(renderer, this->texture);

  auto [r, g, b, a] = this->backgroundColor;
  SDL_SetRenderDrawColor(renderer, r, g, b, a);
  SDL_RenderClear(renderer);

  this->onDrawViewport(deltaTime);

  SDL_SetRenderTarget(renderer, nullptr);

  ImGui::Image((ImTextureID)this->texture, ImVec2(this->size));
  ImGui::End();
  ImGui::PopStyleVar();
}

Vec2 Viewport::getMousePosition(Vec2 position)
{
  return Vec2{
      std::clamp(position.x - this->position.x, 0.0f, this->size.x),
      std::clamp(position.y - this->position.y, 0.0f, this->size.y),
  };
}
