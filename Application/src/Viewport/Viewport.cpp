#include "Viewport.h"

#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <stdio.h>

Viewport::Viewport()
{
  this->createFrameBuffer();
}

Viewport::~Viewport() {}

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
  this->setDimensions(size);

  // TODO Mostly need to resize the texture here & the viewport somewhere.
  // glViewport(0, 0, size.x, size.y);
  // glBindTexture(GL_TEXTURE_2D, this->texture);
  // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
}

void Viewport::createFrameBuffer()
{
  // Create & bind the frame buffer
  glGenFramebuffers(1, &this->framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

  // Create a texture to render to & bind it
  glGenTextures(1, &this->texture);
  glBindTexture(GL_TEXTURE_2D, this->texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->size.x, this->size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Attach the texture to the framebuffer
  // The texture will now serve as the output for any rendering done to this framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture, 0);

  // Unbind texture & framebuffer
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Viewport::setBackgroundColor(glm::vec4 setBackgroundColor)
{
  this->backgroundColor = setBackgroundColor;
}

void Viewport::onDraw(float deltaTime)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
  ImGui::Begin(this->title, nullptr, this->flags);

  ImVec2 size = ImGui::GetContentRegionAvail();
  ImVec2 position = ImGui::GetWindowPos();
  this->position.x = position.x;
  this->position.y = position.y;

  if (this->size.x != size.x || this->size.y != size.y)
    this->resize({size.x, size.y});

  glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

  glClearColor(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  this->onDrawViewport(deltaTime);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  ImGui::Image((ImTextureID)this->texture, ImVec2(this->size.x, this->size.y));

  ImGui::End();
  ImGui::PopStyleVar();
}

glm::vec2 *Viewport::getSize()
{
  return &this->size;
}

glm::vec2 *Viewport::getPosition()
{
  return &this->position;
}