#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include "RenderSystem.h"

#include "Component/Component.h"
#include "Bone/Bone.h"

RenderSystem::RenderSystem()
{
  unsigned int vbo, ebo;

  // Create a vertex array & bind it.
  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);

  // Create an array buffer, bind it & provide the mesh data.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(float), &BONE_VERTICES, GL_STATIC_DRAW);

  // Create an element array buffer, bind it & provide the indices.
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(unsigned int), &BONE_INDICES, GL_STATIC_DRAW);

  // Setup our vertex locations for shaders
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)0);
  glEnableVertexAttribArray(0);

  // Setup our color locations for shaders
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Bind a new array buffer for the instance transforms
  glGenBuffers(1, &this->instanceVBO);
  glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
  // Keet it empty for now by passing nullptr
  glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

  // Setup our transform and rotation locations for shaders
  // using a vec3 x,y,z(rotation degrees)
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
  glEnableVertexAttribArray(2);
  glVertexAttribDivisor(2, 1);

  // Unbind vertex array
  glBindVertexArray(0);

  this->createBoneInstance({0.0f, 0.0f, 45.0f});

  // Load the shader program
  this->shader = this->createShaderProgram("src/Shader/vertex.glsl", "src/Shader/fragment.glsl");
}

// std::string RenderSystem::parseShader(const char *filepath)
// {
// }

// unsigned int RenderSystem::createShader(std::string shader)
// {
// }

void RenderSystem::draw(float deltaTime, Registry *registry)
{
  glUseProgram(this->shader);

  // for (auto entity : registry->entities())
  // {
  //   if (entity->is("Bone"))
  //   {
  //     const auto [bone, transform] = entity->collect<CBone, CTransform>();
  //     glBindVertexArray(this->vao);
  //     glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, this->instanceTransforms.size());
  //     glBindVertexArray(0);
  //   }
  // }
  glBindVertexArray(this->vao);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, this->instanceTransforms.size());
  glBindVertexArray(0);
}

void RenderSystem::input()
{
  this->shader = this->createShaderProgram("src/Shader/vertex.glsl", "src/Shader/fragment.glsl");
}

void RenderSystem::createBoneInstance(glm::vec3 transform)
{
  this->instanceTransforms.push_back(transform);
  glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, this->instanceTransforms.size() * 3 * sizeof(float), this->instanceTransforms.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned int RenderSystem::loadShader(const char *filepath, GLenum shaderType)
{
  std::ifstream shaderFile(filepath);
  if (!shaderFile.is_open())
  {
    std::cerr << "ERROR: Failed to open shader file: " << filepath << std::endl;
    return 0;
  }

  // Read the file content into a stringstream
  std::stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();
  shaderFile.close();
  std::string shaderCode = shaderStream.str();
  const char *shaderCodeCStr = shaderCode.c_str();

  // Create the shader object
  unsigned int shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderCodeCStr, nullptr);
  glCompileShader(shader);

  // Check for compilation errors
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    int logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    char *infoLog = new char[logLength];
    glGetShaderInfoLog(shader, logLength, &logLength, infoLog);
    std::cerr << "ERROR: Shader compilation failed (" << filepath << "):\n"
              << infoLog << std::endl;
    delete[] infoLog;
    glDeleteShader(shader); // Clean up
    return 0;
  }

  return shader;
}

unsigned int RenderSystem::createShaderProgram(const char *vertexPath, const char *fragmentPath)
{
  // Load vertex and fragment shaders
  unsigned int vertexShader = this->loadShader(vertexPath, GL_VERTEX_SHADER);
  unsigned int fragmentShader = this->loadShader(fragmentPath, GL_FRAGMENT_SHADER);

  // Create shader program and attach the shaders
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  // Link the shader program
  glLinkProgram(shaderProgram);

  // Check for linking errors
  int success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    int logLength;
    glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
    char *infoLog = new char[logLength];
    glGetProgramInfoLog(shaderProgram, logLength, &logLength, infoLog);
    std::cerr << "ERROR: Shader program linking failed:\n"
              << infoLog << std::endl;
    delete[] infoLog;
    glDeleteProgram(shaderProgram); // Clean up
    return 0;
  }

  // Clean up: shaders are no longer needed after being linked into a program
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}
