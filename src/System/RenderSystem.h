#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ECS/Registry.h"
#include "Render/Mesh/Mesh.h"
#include "Render/Camera/OrthographicCamera.h"

struct BoneMesh {
  float verticies[8];
  int indices[6];
};

class RenderSystem
{
private:
  Registry *registry;
  OrthographicCamera *camera;

public:
  RenderSystem() = default;
  ~RenderSystem() = default;

  void draw(float deltaTime);

  void setRegistry(Registry *registry);
  void setCamera(OrthographicCamera *camera);
};