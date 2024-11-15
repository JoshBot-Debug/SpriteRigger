#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <string>

struct ShaderFile
{
  const char *filepath;
  GLenum type;

  ShaderFile(const char *filepath, GLenum type): filepath(filepath), type(type) {};
};

class ShaderProgram
{
private:
  unsigned int program;
  std::vector<unsigned int> shaders;
  std::vector<ShaderFile> recompileFiles;
  std::unordered_map<std::string, int> locations;

public:
  ShaderProgram() = default;
  ~ShaderProgram();

  bool compile(const char *filepath, GLenum type, bool enableRecompile = true);

  bool recompile();

  bool createProgram();

  void bind();

  void unbind();

  void addUniformMatrix4fv(glm::mat4 uniform, const std::string &name);
};