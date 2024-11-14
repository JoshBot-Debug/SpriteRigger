#include <GL/glew.h>
#include "RenderSystem.h"

#include "Component/components.h"

const float rectangle[] = {
    -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 1.0f, 1.0f, 1.0f};

const unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3};

RenderSystem::RenderSystem()
{
  unsigned int vbo, ebo;

  // Create a vertex array & bind it.
  glGenVertexArrays(1, &this->vao);
  glBindVertexArray(this->vao);

  // Create an array buffer, bind it & provide the mesh data.
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(float), &rectangle, GL_STATIC_DRAW);

  // Create an element array buffer, bind it & provide the indices.
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * 2 * sizeof(unsigned int), &indices, GL_STATIC_DRAW);

  // Setup our vertex shader locations
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)0);
  glEnableVertexAttribArray(0);

  // Setup our fragment shader locations
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Unbind the vertex array
  glBindVertexArray(0);
}

std::string RenderSystem::parseShader(const char *filepath)
{
}

unsigned int RenderSystem::createShader(std::string shader)
{
}

void RenderSystem::draw(float deltaTime, Registry *registry)
{
  for (auto entity : registry->entities())
  {
    if (entity->is("Bone"))
    {
      const auto [bone, transform] = entity->collect<CBone, CTransform>();

      glBindVertexArray(this->vao);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void *)0);
      glBindVertexArray(0);
    }
  }
}
