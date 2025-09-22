#pragma once

#include <glm/glm.hpp>

struct CTransform {
  glm::vec3 position = glm::vec3{0.0f};
  glm::uvec2 size = glm::uvec2{0};
};
