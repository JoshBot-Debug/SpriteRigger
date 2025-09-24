#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

struct CTransform {
  glm::vec3 position = glm::vec3{0.0f};
  glm::uvec2 size = glm::uvec2{0};
};

struct CHierarchy
{
  uint32_t id = 0;
  uint32_t parent = 0;
  char name[255] = "";
};

struct CFlags
{
  bool rename = false;
};