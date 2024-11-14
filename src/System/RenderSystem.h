#pragma once

#include <string>
#include "ECS/registry.h"
#include "ECS/entity.h"

class RenderSystem
{
private:
  unsigned int vao;
  
  std::string parseShader(const char *filepath);

  unsigned int createShader(std::string shader);

public:
  RenderSystem();
  ~RenderSystem() = default;

  void draw(float deltaTime, Registry *registry);
};