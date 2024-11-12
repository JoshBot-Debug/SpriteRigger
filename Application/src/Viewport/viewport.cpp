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

void Viewport::setDimensions(glm::vec2 size)
{
  this->size = size;
}

void Viewport::resize(glm::vec2 size)
{
  SDL_DestroyTexture(this->texture);

  this->setDimensions(size);
  this->texture = SDL_CreateTexture(this->application->getRenderer(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
}

void Viewport::setBackgroundColor(glm::vec4 setBackgroundColor)
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
  ImVec2 position = ImGui::GetWindowPos();
  this->position.x = position.x;
  this->position.y = position.y;

  if (this->size.x != size.x || this->size.y != size.y)
    this->resize({size.x, size.y});

  SDL_SetRenderTarget(renderer, this->texture);

  SDL_SetRenderDrawColor(renderer, this->backgroundColor.x, this->backgroundColor.y, this->backgroundColor.z, this->backgroundColor.w);
  SDL_RenderClear(renderer);

  this->onDrawViewport(deltaTime);

  SDL_SetRenderTarget(renderer, nullptr);

  ImGui::Image((ImTextureID)this->texture, ImVec2{this->size.x, this->size.y});
  ImGui::End();
  ImGui::PopStyleVar();
}

glm::vec2 Viewport::getSize()
{
  return this->size;
}

glm::vec2 Viewport::getPosition()
{
  return this->position;
}