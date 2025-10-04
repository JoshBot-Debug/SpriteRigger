#pragma once
#include <glm/glm.hpp>

class OrthographicCamera {

private:
  // Matrices
  glm::mat4 m_View{1.0f};
  glm::mat4 m_Projection{1.0f};
  glm::mat4 m_ViewProjection{1.0f};

  glm::mat4 m_InverseView{1.0f};
  glm::mat4 m_InverseProjection{1.0f};
  glm::mat4 m_InverseViewProjection{1.0f};

  // Orientation vectors
  glm::vec3 m_Front{0.0f, 0.0f, -1.0f};
  glm::vec3 m_Up{0.0f, 1.0f, 0.0f};
  glm::vec3 m_Right{1.0f, 0.0f, 0.0f};

public:
  glm::vec3 Position{0.0f, 0.0f, 5.0f};
  glm::vec3 Rotation{0.0f};

  float Zoom = 1.0f;
  float NearPlane = -1.0f;
  float FarPlane = 1.0f;

  uint32_t ViewportWidth = 0;
  uint32_t ViewportHeight = 0;

public:
  OrthographicCamera() = default;

  void Update();
  bool OnResize(uint32_t width, uint32_t height);

  void SetPosition(float x, float y, float z);
  void SetRotation(float pitch, float yaw, float roll);
  void Translate(float dx, float dy, float dz);
  void Rotate(float dp, float dy, float dr);
  void SetProjection(float zoom, float nearPlane, float farPlane);

  const glm::mat4 &GetViewProjectionMatrix() const { return m_ViewProjection; }
  const glm::mat4 &GetProjectionMatrix() const { return m_Projection; }
  const glm::mat4 &GetViewMatrix() const { return m_View; }
  const glm::mat4 &GetInverseViewMatrix() const { return m_InverseView; }
  const glm::mat4 &GetInverseProjectionMatrix() const {
    return m_InverseProjection;
  }
  const glm::mat4 &GetInverseViewProjectionMatrix() const {
    return m_InverseViewProjection;
  }
};
