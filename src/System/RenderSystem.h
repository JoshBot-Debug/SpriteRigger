#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "ECS/Registry.h"
#include "ECS/Entity.h"
#include "ShaderManager/ShaderProgram.h"

class RenderSystem
{
private:
  unsigned int vao;

  unsigned int instanceVBO;
  std::vector<glm::vec3> instances;

  ShaderProgram shaders;

public:
  RenderSystem();
  ~RenderSystem() = default;

  void draw(float deltaTime, Registry *registry);

  void input();

  void createBoneInstance(glm::vec3 transform);
};