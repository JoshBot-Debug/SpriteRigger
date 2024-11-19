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

class Shader
{
private:
  unsigned int program;
  std::vector<unsigned int> shaders;
  std::vector<ShaderFile> recompileFiles;
  mutable std::unordered_map<std::string, int> uniformLocation;

public:
  Shader() = default;
  ~Shader();

  /**
   * Disable copy constructor
   */
  Shader(const Shader &) = delete;

  /**
   * Disable assignment operator
   */
  Shader &operator=(const Shader &) = delete;


  bool compile(const char *filepath, GLenum type, bool enableRecompile = true);

  bool recompile();

  bool createProgram();

  void bind() const;

  void unbind() const;

  void addUniformMatrix4fv(const std::string &name, glm::mat4 uniform) const;
};