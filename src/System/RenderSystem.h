#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ECS/Registry.h"
#include "Render/Mesh/Mesh.h"

struct BoneMesh {
  float verticies[8];
  int indices[6];
};

class RenderSystem
{
private:
  Registry *registry;
  std::vector<Mesh *> meshes;

public:
  RenderSystem() = default;
  ~RenderSystem() = default;

  void draw(float deltaTime);

  void push(Mesh *mesh);
};