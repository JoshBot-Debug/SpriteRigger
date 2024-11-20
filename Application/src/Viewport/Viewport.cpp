#include "Viewport.h"

#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <stdio.h>

Viewport::~Viewport() {}

void Viewport::setTitle(const char *title)
{
  this->title = title;
}

void Viewport::setWindowFlags(ImGuiWindowFlags flags)
{
  this->flags = flags;
}

void Viewport::setDimensions(glm::vec2 dimensions)
{
  this->dimensions = dimensions;
}

void Viewport::resize(glm::vec2 size)
{
  if (this->texture)
  {
    /**
     * The viewport resized so we need to resize the texture we are rendering
     * to! Duh!
     */
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  this->setDimensions(size);
  this->onResize(size.x, size.y);
}

void Viewport::createFrameBuffer()
{
  // Create & bind the frame buffer
  glGenFramebuffers(1, &this->framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

  // Create a texture to render to & bind it
  glGenTextures(1, &this->texture);
  glBindTexture(GL_TEXTURE_2D, this->texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->dimensions.x, this->dimensions.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
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

  if (this->dimensions.x != size.x || this->dimensions.y != size.y)
    this->resize({size.x, size.y});

  /**
   * Why create the during onDraw?
   * This avoids creating the framebuffer with the wrong initial size.
   * It used to be in the constructor but we don't have the correction dimentions to begin with.
   * Hence, it was moved here so that we get the size of the viewport and can then create the
   * texture with the correct size!
   */
  if (this->framebuffer == 0)
    this->createFrameBuffer();

  glViewport(0, 0, size.x, size.y);
  glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

  glClearColor(this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, this->backgroundColor.a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  this->onDrawViewport(deltaTime);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  ImGui::Image((ImTextureID)this->texture, ImVec2(this->dimensions.x, this->dimensions.y));

  ImGui::End();
  ImGui::PopStyleVar();
}

glm::vec2 *Viewport::getDimensions()
{
  return &this->dimensions;
}

glm::vec2 *Viewport::getPosition()
{
  return &this->position;
}