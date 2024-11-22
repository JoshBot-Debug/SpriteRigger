#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

void OrthographicCamera::calculateViewProjection()
{
  /**
   * Set the origin to the center of the screen
   */
  float wSize = this->width / 2.0f;
  float hSize = this->height / 2.0f;
  this->projection = glm::ortho(-wSize, +wSize, -hSize, +hSize, -1.0f, 1.0f);

  glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(-this->position.x, -this->position.y, 0.0f));

  glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(this->rotation), glm::vec3(0.0f, 0.0f, 1.0f));

  this->viewProjection = this->projection * (translation * rotation);
}

OrthographicCamera::OrthographicCamera()
{
  this->projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
  this->viewProjection = this->projection * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
}

void OrthographicCamera::setDimensions(float width, float height)
{
  this->width = width;
  this->height = height;
  this->calculateViewProjection();
}

void OrthographicCamera::setPosition(float x, float y)
{
  /**
   * Set the origin to the center of the screen
   * x0 & y0 will be the center of the screen
   */
  this->position.x = x - (this->width / 2.0f);
  this->position.y = y - (this->height / 2.0f);
  this->calculateViewProjection();
}

void OrthographicCamera::move(float deltaX, float deltaY)
{
  this->position.x += deltaX;
  this->position.y += deltaY;
  this->calculateViewProjection();
}

void OrthographicCamera::setRotation(float rotation)
{
  this->rotation = rotation;
  this->calculateViewProjection();
}

glm::mat4 OrthographicCamera::getViewProjection() const
{
  return this->viewProjection;
}

glm::vec2 OrthographicCamera::getPosition() const
{
  return this->position;
}

float OrthographicCamera::getRotation() const
{
  return this->rotation;
}
