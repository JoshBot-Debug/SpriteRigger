#include "ShaderProgram.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

ShaderProgram::~ShaderProgram()
{
  if (this->program != 0)
    glDeleteProgram(this->program);
}

bool ShaderProgram::compile(const char *filepath, GLenum type, bool enableRecompile)
{
  std::ifstream stream(filepath);

  if (enableRecompile)
    this->recompileFiles.emplace_back<ShaderFile>({filepath, type});

  if (!stream.is_open())
  {
    std::cerr << "ERROR: Failed to open shader file: " << filepath << std::endl;
    return false;
  }

  std::stringstream sStream;
  sStream << stream.rdbuf();
  stream.close();

  std::string shaderStr = sStream.str();
  const char *shaderCStr = shaderStr.c_str();

  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &shaderCStr, nullptr);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    char *log = new char[length];
    glGetShaderInfoLog(shader, length, &length, log);
    std::cerr << "ERROR: Shader compilation failed (" << filepath << "):\n"
              << log << std::endl;
    delete[] log;
    glDeleteShader(shader);
    return false;
  }

  this->shaders.push_back(shader);
  return true;
}

bool ShaderProgram::recompile()
{
  for (auto file : this->recompileFiles)
    if (!this->compile(file.filepath, file.type, false))
      return false;
  return true;
}

bool ShaderProgram::createProgram()
{
  unsigned int program = glCreateProgram();

  for (auto shader : this->shaders)
    glAttachShader(program, shader);

  glLinkProgram(program);

  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char *log = new char[length];
    glGetProgramInfoLog(program, length, &length, log);
    std::cerr << "ERROR: Shader program linking failed:\n"
              << log << std::endl;
    delete[] log;
    glDeleteProgram(program);
    return false;
  }

  for (auto shader : this->shaders)
    glDeleteShader(shader);

  this->shaders.clear();

  this->program = program;

  return true;
}

void ShaderProgram::bind()
{
  glUseProgram(this->program);
}

void ShaderProgram::unbind()
{
  glUseProgram(0);
}

void ShaderProgram::addUniformMatrix4fv(glm::mat4 uniform, std::string name)
{
  if (!this->locations[name])
    this->locations[name] = glGetUniformLocation(this->program, name.c_str());
  glUniformMatrix4fv(this->locations[name], 1, GL_FALSE, &uniform[0][0]);
}
