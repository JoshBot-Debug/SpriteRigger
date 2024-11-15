#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ECS/Registry.h"
#include "Manager/ShaderManager/ShaderManager.h"

class RenderSystem
{
private:
  unsigned int vao;

  unsigned int instanceVBO;
  std::vector<glm::vec3> instances;

  Registry *registry;
  ShaderManager *shaderManager;

public:
  RenderSystem(Registry *registry, ShaderManager *shaderManager);
  ~RenderSystem() = default;

  void draw(float deltaTime);

  void createBoneInstance(glm::vec3 transform);
};