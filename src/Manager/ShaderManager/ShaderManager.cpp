#include "ShaderManager.h"

ShaderProgram &ShaderManager::get(const std::string &name)
{
  return this->programs[name];
}

void ShaderManager::load(const std::string &name, const char *vertexFile, const char *fragmentFile)
{
  ShaderProgram program;

  if (!program.compile(vertexFile, GL_VERTEX_SHADER))
    printf("%s\n", "ShaderProgram Failed to compile vertex shader file.");

  if (!program.compile(fragmentFile, GL_FRAGMENT_SHADER))
    printf("%s\n", "ShaderProgram Failed to compile fragment shader file.");

  if (!program.createProgram())
    printf("%s\n", "ShaderProgram Failed to create program.");

  this->programs[name] = program;
}

void ShaderManager::bind(const std::string &name)
{
  this->programs[name].bind();
}

void ShaderManager::unbind(const std::string &name)
{
  this->programs[name].unbind();
}

void ShaderManager::addUniformMatrix4fv(const std::string &shaderName, glm::mat4 uniform, const std::string &name)
{
  this->programs[shaderName].addUniformMatrix4fv(uniform, name);
}
