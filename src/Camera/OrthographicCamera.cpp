#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>

void OrthographicCamera::Update() {
  float aspect = (float)ViewportWidth / (float)ViewportHeight;

  m_Projection = glm::ortho(-aspect * Zoom, aspect * Zoom, -1.0f * Zoom,
                            1.0f * Zoom, NearPlane, FarPlane);

  m_Front = glm::normalize(glm::vec3(
      sin(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x)),
      sin(glm::radians(Rotation.x)),
      -cos(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x))));
  m_Right = glm::normalize(glm::cross(m_Front, glm::vec3(0, 1, 0)));
  m_Up = glm::normalize(glm::cross(m_Right, m_Front));

  glm::mat4 roll =
      glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), m_Front);
  m_Right = glm::normalize(glm::vec3(roll * glm::vec4(m_Right, 0.0f)));
  m_Up = glm::normalize(glm::vec3(roll * glm::vec4(m_Up, 0.0f)));

  m_View = glm::lookAt(Position, Position + m_Front, m_Up);

  m_ViewProjection = m_Projection * m_View;
  m_InverseView = glm::inverse(m_View);
  m_InverseProjection = glm::inverse(m_Projection);
  m_InverseViewProjection = glm::inverse(m_ViewProjection);
}

bool OrthographicCamera::OnResize(uint32_t width, uint32_t height) {
  bool resized = ViewportWidth != width || ViewportHeight != height;
  ViewportWidth = width;
  ViewportHeight = height;
  return resized;
}

void OrthographicCamera::SetPosition(float x, float y, float z) {
  Position = glm::vec3(x, y, z);
}

void OrthographicCamera::SetRotation(float pitch, float yaw, float roll) {
  Rotation = glm::vec3(pitch, yaw, roll);
}

void OrthographicCamera::Translate(float dx, float dy, float dz) {
  Position += dx * m_Right;
  Position += dy * m_Up;
  Position += dz * m_Front;
}

void OrthographicCamera::Rotate(float dp, float dy, float dr) {
  Rotation.x += dp;
  Rotation.y += dy;
  Rotation.z += dr;
}

void OrthographicCamera::SetProjection(float zoom, float nearPlane,
                                       float farPlane) {
  Zoom = zoom;
  NearPlane = nearPlane;
  FarPlane = farPlane;
}