#include "RenderSystem.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Component/Component.h"
#include "Render/Shader/Shader.h"

unsigned int vao, vbo, ebo, instanceVBO, count(0);

void RenderSystem::draw(float deltaTime)
{
  glm::mat4 projection = glm::ortho(0.0f, 725.0f, 0.0f, 504.0f, -1.0f, 1.0f);
  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
  glm::mat4 mvp = projection * view * model;

  // glBindVertexArray(vao);
  // glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, count);

  for (auto &mesh : this->meshes)
  {
    mesh->getShader()->bind();
    mesh->getShader()->addUniformMatrix4fv("mvp", mvp);
    glBindVertexArray(mesh->getVertexArray());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getElementArrayBuffer());
    glDrawElementsInstanced(GL_TRIANGLES, mesh->getIndicesSize(), GL_UNSIGNED_INT, 0, mesh->getInstanceSize());
  }
}

void RenderSystem::push(Mesh *mesh)
{
  this->meshes.push_back(mesh);

  // std::vector<float> vertices = {
  //   -50.0f, -50.0f,
  //    50.0f, -50.0f,
  //    50.0f,  50.0f,
  //   -50.0f,  50.0f,
  // };

  // std::vector<unsigned int> indices = {
  //   0, 1, 2,
  //   0, 2, 3
  // };

  // // this->meshes.push_back(mesh);
  //  unsigned int vbo, ebo;

  // // Create a vertex array & bind it.
  // glGenVertexArrays(1, &vao);
  // glBindVertexArray(vao);

  // // Create an array buffer, bind it & provide the mesh data.
  // glGenBuffers(1, &vbo);
  // glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

  // // Create an element array buffer, bind it & provide the indices.
  // glGenBuffers(1, &ebo);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  // // Setup our vertex locations for shaders
  // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void *)0);
  // glEnableVertexAttribArray(0);

  // // Bind a new array buffer for the instance transforms
  // glGenBuffers(1, &instanceVBO);
  // glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  // glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

  // // Setup our transform and rotation locations for shaders
  // // using a vec3 x,y,z(rotation degrees)
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void *)0);
  // glEnableVertexAttribArray(1);
  // glVertexAttribDivisor(1, 1);

  // // Unbind vertex array
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

  // Shader shader;
  // shader.compile("src/Shader/vertex.glsl", GL_VERTEX_SHADER);
  // shader.compile("src/Shader/fragment.glsl", GL_FRAGMENT_SHADER);
  // shader.createProgram();
  // shader.bind();

  // std::vector<float> transform = {50.0f, 50.0f, 0.0f};

  // glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  // glBufferData(GL_ARRAY_BUFFER, transform.size() * 3 * sizeof(float), transform.data(), GL_DYNAMIC_DRAW);
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // ++count;
}