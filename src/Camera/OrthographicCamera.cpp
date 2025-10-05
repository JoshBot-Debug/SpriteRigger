#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

void OrthographicCamera::Update() {
  Aspect = (float)ViewportWidth / (float)ViewportHeight;

  m_Projection =
      glm::ortho(-Aspect * Zoom, Aspect * Zoom, -Zoom, Zoom, -1.0f, 1.0f);

  m_View =
      glm::translate(glm::mat4(1.0f), glm::vec3(-Position.x, Position.y, 0.0f));

  m_ViewProjection = m_Projection * m_View;
}

bool OrthographicCamera::OnResize(uint32_t width, uint32_t height) {
  bool resized = ViewportWidth != width || ViewportHeight != height;
  ViewportWidth = width;
  ViewportHeight = height;
  return resized;
}

void OrthographicCamera::SetPosition(float x, float y) {
  Position.x = x;
  Position.y = y;
}

void OrthographicCamera::Translate(float dx, float dy) {
  Position.x += dx;
  Position.y += dy;
}