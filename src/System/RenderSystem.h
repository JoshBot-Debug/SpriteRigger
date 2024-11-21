#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "ECS/Registry.h"
#include "Render/Model/Mesh.h"
#include "Render/Camera/OrthographicCamera.h"
#include "Manager/ResourceManager.h"

struct BoneMesh
{
  float verticies[8];
  int indices[6];
};

class RenderSystem
{
private:
  Registry *registry;
  OrthographicCamera *camera;
  ResourceManager *resourceManager;

public:
  RenderSystem() = default;
  ~RenderSystem() = default;

  void draw(float deltaTime);

  void setRegistry(Registry *registry);
  void setCamera(OrthographicCamera *camera);
  void setResourceManager(ResourceManager *resourceManager);
};