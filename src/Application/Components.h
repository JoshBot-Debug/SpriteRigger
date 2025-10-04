#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

struct CBone {
  glm::vec2 start = glm::vec2(0.0f);
  glm::vec2 end = glm::vec2(0.0f);
  glm::vec4 color = glm::vec4(1.0f);
  float thickness = 0.0f;
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
  bool selected = false;
};