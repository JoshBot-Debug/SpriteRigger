#include "ShaderManager.h"

ShaderManager::~ShaderManager()
{
  for (const auto &program : this->programs)
    delete program.second;
}

ShaderProgram *ShaderManager::get(const std::string &name)
{
  return this->programs[name];
}

void ShaderManager::load(const std::string &name, const char *vertexFile, const char *fragmentFile)
{
  ShaderProgram *program = new ShaderProgram();

  program->compile(vertexFile, GL_VERTEX_SHADER);

  program->compile(fragmentFile, GL_FRAGMENT_SHADER);

  program->createProgram();

  this->programs[name] = program;
}

void ShaderManager::bind(const std::string &name)
{
  this->programs[name]->bind();
}

void ShaderManager::unbind(const std::string &name)
{
  this->programs[name]->unbind();
}

void ShaderManager::addUniformMatrix4fv(const std::string &shaderName, glm::mat4 uniform, const std::string &name)
{
  this->programs[shaderName]->addUniformMatrix4fv(uniform, name);
}
