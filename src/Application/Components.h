#pragma once

#include <glm/glm.hpp>
#include <stdint.h>

struct CBone {
  glm::vec2 start = glm::vec2(0.0f);
  glm::vec2 end = glm::vec2(0.0f);
  glm::vec4 color = glm::vec4(1.0f);
  glm::vec4 sColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
  glm::vec4 eColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
  float thickness = 0.0f;

  bool Intersects(float x, float y) {
    glm::vec2 point(x, y);
    glm::vec2 dir = end - start;
    float lenSquared = glm::dot(dir, dir);

    if (lenSquared == 0.0f)
      return glm::distance(point, start) <= thickness / 2.0f;

    float t = glm::dot(point - start, dir) / lenSquared;
    t = glm::clamp(t, 0.0f, 1.0f);

    glm::vec2 closest = start + t * dir;

    return glm::distance(point, closest) <= thickness / 2.0f;
  }
};

struct CHierarchy {
  uint32_t id = 0;
  uint32_t parent = 0;
  char name[255] = "";
};

struct CFlags {
  bool rename = false;
  bool selected = false;
};