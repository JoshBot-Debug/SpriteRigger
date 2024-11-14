#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

#include "ECS/Registry.h"
#include "ECS/Entity.h"

class RenderSystem
{
private:
  unsigned int shader;
  unsigned int vao;

  unsigned int instanceVBO;
  std::vector<glm::vec3> instanceTransforms;

  // std::string parseShader(const char *filepath);

  // unsigned int createShader(std::string shader);

public:
  RenderSystem();
  ~RenderSystem() = default;

  void draw(float deltaTime, Registry *registry);

  void input();

  void createBoneInstance(glm::vec3 transform);

  unsigned int loadShader(const char *filepath, GLenum shaderType);
  unsigned int createShaderProgram(const char* vertexPath, const char* fragmentPath);
};