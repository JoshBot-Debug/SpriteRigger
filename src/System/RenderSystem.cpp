#include "RenderSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

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

  this->createBoneInstance({0.0f, 0.0f, 0.0f});

  this->shaders.compile("src/Shader/vertex.glsl", GL_VERTEX_SHADER);
  this->shaders.compile("src/Shader/fragment.glsl", GL_FRAGMENT_SHADER);
  this->shaders.createProgram();
  this->shaders.bind();

  glm::mat4 projection = glm::ortho(0.0f, 772.0f, 0.0f, 467.0f, -1.0f, 1.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 10.0f, 0.0f));
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  glm::mat4 mvp = projection * view * model;

  this->shaders.addUniformMatrix4fv(mvp, "mvp");
}

// std::string RenderSystem::parseShader(const char *filepath)
// {
// }

// unsigned int RenderSystem::createShader(std::string shader)
// {
// }

void RenderSystem::draw(float deltaTime, Registry *registry)
{

  // for (auto entity : registry->entities())
  // {
  //   if (entity->is("Bone"))
  //   {
  //     const auto [bone, transform] = entity->collect<CBone, CTransform>();
  //     glBindVertexArray(this->vao);
  //     glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, this->instances.size());
  //   }
  // }

  glBindVertexArray(this->vao);
  glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, this->instances.size());
}

// TMP Method, will be removed and a button will be added in the GUI
void RenderSystem::input()
{
  this->shaders.recompile();
  this->shaders.createProgram();
  this->shaders.bind();

  // This is the camera, probably, we'll need to to keep that outside as well.
  glm::mat4 projection = glm::ortho(0.0f, 772.0f, 0.0f, 467.0f, -1.0f, 1.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f, 10.0f, 0.0f));
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

  glm::mat4 mvp = projection * view * model;
  this->shaders.addUniformMatrix4fv(mvp, "mvp");
}

void RenderSystem::createBoneInstance(glm::vec3 transform)
{
  this->instances.push_back(transform);
  glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
  glBufferData(GL_ARRAY_BUFFER, this->instances.size() * 3 * sizeof(float), this->instances.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
