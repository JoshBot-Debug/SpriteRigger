#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

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
    return glm::distance2(mouse, start) <= (thickness * 0.5f) * (thickness * 0.5f);

  float t = glm::dot(mouse - start, dir) / lenSquared;
  t = glm::clamp(t, 0.0f, 1.0f);

  glm::vec2 closest = start + t * dir;

  return glm::distance2(mouse, closest) <= (thickness * 0.5f) * (thickness * 0.5f);
}
} // namespace Intersect

namespace Animation {
template <typename T>
inline T Lerp(const T &current, const T &target, float t, float epsilon = 0.001f) {
  T next = glm::mix(current, target, t);

  if (glm::length2(target - next) < epsilon * epsilon)
    return target;

  return next;
}

} // namespace Animation