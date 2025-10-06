#pragma once
#include <glm/glm.hpp>

class OrthographicCamera {

private:
  // Matrices
  glm::mat4 m_View{1.0f};
  glm::mat4 m_Projection{1.0f};
  glm::mat4 m_ViewProjection{1.0f};

public:
  float Zoom = 5.0f;
  glm::vec2 Position{0.0f, 0.0f};

  float Aspect = 0.0f;
  uint32_t ViewportWidth = 0;
  uint32_t ViewportHeight = 0;

public:
  OrthographicCamera() = default;

  void Update();
  void Update(uint32_t width, uint32_t height);
  bool OnResize(uint32_t width, uint32_t height);

  void SetPosition(float x, float y);
  void Translate(float dx, float dy);

  const glm::mat4 &GetViewProjectionMatrix() const { return m_ViewProjection; }
  const glm::mat4 &GetProjectionMatrix() const { return m_Projection; }
  const glm::mat4 &GetViewMatrix() const { return m_View; }
};
