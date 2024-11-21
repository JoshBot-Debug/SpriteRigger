#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <vector>
#include <string>

enum class ShaderType {
  VERTEX_SHADER,
  FRAGMENT_SHADER
};

struct ShaderFile
{
  const char *filepath;
  ShaderType type;

  ShaderFile(const char *filepath, ShaderType type): filepath(filepath), type(type) {};
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


  bool compile(const char *filepath, ShaderType type, bool enableRecompile = true);

  bool recompile();

  bool createProgram();

  void bind() const;

  void unbind() const;

  void addUniformMatrix4fv(const std::string &name, glm::mat4 uniform) const;
};