#pragma once

#include "ShaderProgram.h"

class ShaderManager
{
private:
  std::unordered_map<std::string, ShaderProgram> programs;

public:
  ShaderManager() = default;
  ~ShaderManager() = default;

  ShaderProgram &get(const std::string &name);

  void load(const std::string &name, const char *vertexFile, const char *fragmentFile);

  bool bind(const std::string &name);
  
  bool unbind(const std::string &name);
};