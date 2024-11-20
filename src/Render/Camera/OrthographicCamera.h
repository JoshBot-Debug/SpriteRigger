#pragma once

#include <glm/glm.hpp>

class OrthographicCamera
{
private:
  float width = 1.0f;
  float height = 1.0f;
  float rotation = 0.0f;
  glm::vec2 position = {0.0f, 0.0f};
  glm::mat4 projection;
  glm::mat4 viewProjection;

  void calculateViewProjection();

public:
  OrthographicCamera();
  ~OrthographicCamera() = default;

  void setDimensions(float width, float height);
  void setPosition(float x, float y);
  void setRotation(float rotation);

  glm::mat4 getViewProjection() const;
};