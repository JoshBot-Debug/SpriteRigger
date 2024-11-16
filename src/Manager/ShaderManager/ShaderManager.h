#pragma once

#include "ShaderProgram.h"

class ShaderManager
{
private:
  std::unordered_map<std::string, ShaderProgram *> programs;

public:
  ShaderManager() = default;
  ~ShaderManager();

  ShaderProgram *get(const std::string &name);

  void load(const std::string &name, const char *vertexFile, const char *fragmentFile);

  void bind(const std::string &name);
  
  void unbind(const std::string &name);

  void addUniformMatrix4fv(const std::string &shaderName, glm::mat4 uniform, const std::string &name);
};