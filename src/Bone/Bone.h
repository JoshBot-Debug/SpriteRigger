#pragma once

#include <vector>
#include <glm/glm.hpp>

struct BoneMesh {
  std::vector<float> vertices = {
    -50.0f, -50.0f,
     50.0f, -50.0f,
     50.0f,  50.0f,
    -50.0f,  50.0f,
  };

  std::vector<unsigned int> indices = {
    0, 1, 2,
    0, 2, 3
  };
};

struct BoneInstance {
  float rotation;
  glm::vec2 position;
  glm::vec4 color;
};