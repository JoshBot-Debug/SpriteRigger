#pragma once

#include <glm/glm.hpp>
#include <stdint.h>

struct EBone {};

struct CBone {
  enum Part : uint8_t { StartJoint = 0, EndJoint, Shaft, None };

  struct Joint {
    glm::vec2 position = glm::vec2(0.0f);
    glm::vec4 color = glm::vec4(1.0f);
  };

  float thickness = 0.0f;
  glm::vec4 color = glm::vec4(1.0f);
  Joint joints[2] = {};
};

struct CHovered {
  CBone::Part target = CBone::Part::None;
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