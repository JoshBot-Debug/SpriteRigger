#pragma once

#include <glm/glm.hpp>

#include "imgui.h"

#include "Camera/OrthographicCamera.h"

inline const float ANIMATION_SPEED = 8.0f;

struct SystemData {
  float deltaTime = 0.0f;
  ImVec2 mouse = ImVec2(0.0f, 0.0f);
  ImVec2 deltaMouse = ImVec2(0.0f, 0.0f);
  ImVec2 viewport = ImVec2(0.0f, 0.0f);
  bool isDragging = false;
  bool isMouseDown = false;
  bool isCtrlDown = false;
};

inline ImVec2 GetDeltaMouse(OrthographicCamera *camera, ImGuiIO &io,
                            ImVec2 viewport) {
  return ImVec2(io.MouseDelta.x * camera->Zoom * 2.0f / viewport.x,
                -io.MouseDelta.y * camera->Zoom * 2.0f / viewport.y);
}

namespace Colors {
inline const glm::vec4 HIGHLIGHT = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
inline const glm::vec4 DEFAULT = glm::vec4(1.0f);
} // namespace Colors

namespace Intersect {
inline bool Circle(glm::vec2 mouse, glm::vec2 point, float radius) {
  float dx = point.x - mouse.x;
  float dy = point.y - mouse.y;
  return (dx * dx + dy * dy) <= (radius * radius);
}

inline bool Line(glm::vec2 mouse, glm::vec2 start, glm::vec2 end,
                 float thickness) {
  glm::vec2 dir = end - start;

  float lenSquared = glm::dot(dir, dir);

  if (lenSquared == 0.0f)
    return glm::distance(mouse, start) <= thickness / 2.0f;

  float t = glm::dot(mouse - start, dir) / lenSquared;
  t = glm::clamp(t, 0.0f, 1.0f);

  glm::vec2 closest = start + t * dir;

  return glm::distance(mouse, closest) <= thickness / 2.0f;
}

inline bool Bone(glm::vec2 mouse, glm::vec2 point, glm::vec2 start,
                 glm::vec2 end, float thickness) {
  if (Circle(mouse, start, thickness) || Circle(mouse, end, thickness))
    return true;

  glm::vec2 dir = end - start;
  float lenSquared = glm::dot(dir, dir);

  if (lenSquared == 0.0f)
    return glm::distance(point, start) <= thickness / 2.0f;

  float t = glm::dot(point - start, dir) / lenSquared;
  t = glm::clamp(t, 0.0f, 1.0f);

  glm::vec2 closest = start + t * dir;

  return glm::distance(point, closest) <= thickness / 2.0f;
}
} // namespace Intersect