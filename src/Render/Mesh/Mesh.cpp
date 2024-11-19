#include "Mesh.h"

Mesh::~Mesh()
{
  if (this->vao)
    glDeleteVertexArrays(1, &this->vao);
  if (this->vbo)
    glDeleteBuffers(1, &this->vbo);
  if (this->ebo)
    glDeleteBuffers(1, &this->ebo);
}

void Mesh::createVertexArray() const
{
  // Create a vertex array & bind it.
  if (this->vao == 0)
    glGenVertexArrays(1, &this->vao); // Generate one vertex array
  glBindVertexArray(this->vao);       // Bind
}

void Mesh::createArrayBuffer(std::vector<float> vertices) const
{
  // TODO, may want to pass in static, dynamic or stream
  // Create an array buffer, bind it & provide the mesh data.
  if (this->vbo == 0)
    glGenBuffers(1, &this->vbo);                                                                   // Generate one array buffer
  glBindBuffer(GL_ARRAY_BUFFER, this->vbo);                                                        // Bind the array buffer
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW); // Set the size of the buffer and reference to the vertices.
}

void Mesh::createElementArrayBuffer(std::vector<unsigned int> indices) const
{
  // TODO, may want to pass in static, dynamic or stream
  // Create an element element array buffer, bind it & provide the indices.
  if (this->ebo == 0)
    glGenBuffers(1, &this->ebo);                                                                                // Generate one element array buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);                                                             // Bind the element array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW); // Set the size of the buffer and reference to the indices.
  this->indicesSize = indices.size();
}

void Mesh::setVertexAttrib(unsigned int index, unsigned int count, unsigned int offset) const
{
  // Setup our array buffer locations for shaders
  glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, count * sizeof(float), (const void *)(offset * sizeof(float)));
  glEnableVertexAttribArray(index);
}

void Mesh::createInstanceBuffer(std::string name) const
{
  // Bind a new array buffer for the instance transforms
  glGenBuffers(1, &this->instanceIds[name]);                  // Generate one instance array buffer
  glBindBuffer(GL_ARRAY_BUFFER, this->instanceIds[name]);     // Bind the array buffer
  glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW); // Set the buffer as nullptr
}

void Mesh::setInstanceVertexAttrib(unsigned int index, unsigned int count, unsigned int offset, unsigned int divisor) const
{
  // Setup our instance array buffer locations for shaders
  glVertexAttribPointer(index, count, GL_FLOAT, GL_FALSE, count * sizeof(float), (const void *)(offset * sizeof(float)));
  glEnableVertexAttribArray(index);
  glVertexAttribDivisor(index, divisor);
}

void Mesh::setInstanceBuffer(std::string name, std::vector<float> vertices) const
{
  this->instances[name].push_back(vertices);
  glBindBuffer(GL_ARRAY_BUFFER, this->instanceIds[name]);
  glBufferData(GL_ARRAY_BUFFER, this->instances[name].size() * vertices.size() * sizeof(float), this->instances[name].data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::unbind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

Shader *Mesh::createShader()
{
  this->shader = new Shader();
  return this->shader;
}

Shader *Mesh::getShader()
{
  return this->shader;
}

unsigned int Mesh::getVertexArray()
{
  return this->vao;
}

unsigned int Mesh::getArrayBuffer()
{
  return this->vbo;
}

unsigned int Mesh::getElementArrayBuffer()
{
  return this->ebo;
}

unsigned int Mesh::getIndicesSize()
{
  return this->indicesSize;
}

unsigned int Mesh::getInstanceSize()
{
  if (this->instances.empty())
    return 0;
  return this->instances.begin()->second.size();
}
