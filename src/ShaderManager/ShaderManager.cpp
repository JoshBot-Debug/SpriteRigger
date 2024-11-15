#include "ShaderManager.h"

ShaderProgram &ShaderManager::get(const std::string &name)
{
  return this->programs[name];
}

void ShaderManager::load(const std::string &name, const char *vertexFile, const char *fragmentFile)
{
  ShaderProgram program;
  program.compile(vertexFile, GL_VERTEX_SHADER);
  program.compile(fragmentFile, GL_FRAGMENT_SHADER);
  program.createProgram();
  this->programs[name] = program;
}

bool ShaderManager::bind(const std::string &name)
{
  if (!this->programs[name])
    return false;
  this->programs[name].bind();
  return true;
}

bool ShaderManager::unbind(const std::string &name)
{
  if (!this->programs[name])
    return false;
  this->programs[name].unbind();
  return true;
}
