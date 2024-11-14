#pragma once

#include <string>
#include "ECS/Registry.h"
#include "ECS/Entity.h"

class RenderSystem
{
private:
  unsigned int vao;
  
  // std::string parseShader(const char *filepath);

  // unsigned int createShader(std::string shader);

public:
  RenderSystem();
  ~RenderSystem() = default;

  void draw(float deltaTime, Registry *registry);
};