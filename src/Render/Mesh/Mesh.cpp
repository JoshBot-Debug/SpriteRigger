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

void Mesh::setVertexAttrib(unsigned int index, unsigned int size, unsigned int stride, unsigned int offset) const
{
  // Setup our array buffer locations for shaders
  glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (const void *)(offset * sizeof(float)));
  glEnableVertexAttribArray(index);
}

void Mesh::createInstanceBuffer(std::string name) const
{
  // Bind a new array buffer for the instance transforms
  glGenBuffers(1, &this->instanceIds[name]);                  // Generate one instance array buffer
  glBindBuffer(GL_ARRAY_BUFFER, this->instanceIds[name]);     // Bind the array buffer
  glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW); // Set the buffer as nullptr
}

void Mesh::setInstanceVertexAttrib(unsigned int index, unsigned int size, unsigned int stride, unsigned int offset, unsigned int divisor) const
{
  // Setup our instance array buffer locations for shaders
  glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (const void *)(offset * sizeof(float)));
  glEnableVertexAttribArray(index);
  glVertexAttribDivisor(index, divisor);
}

unsigned int Mesh::setInstanceBuffer(std::string name, std::vector<float> vertices) const
{
  this->instances[name].push_back(vertices);
  glBindBuffer(GL_ARRAY_BUFFER, this->instanceIds[name]);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return vertices.size();
}

void Mesh::bind() const
{
  if (this->vao)
    glBindVertexArray(this->vao);
  if (this->vbo)
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
  if (this->ebo)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
}

void Mesh::unbind() const
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

unsigned int Mesh::getVertexArray() const
{
  return this->vao;
}

unsigned int Mesh::getArrayBuffer() const
{
  return this->vbo;
}

unsigned int Mesh::getElementArrayBuffer() const
{
  return this->ebo;
}

unsigned int Mesh::getIndicesSize() const
{
  return this->indicesSize;
}

unsigned int Mesh::getInstanceSize() const
{
  if (this->instances.empty())
    return 0;
  return this->instances.begin()->second.size();
}

void Mesh::drawInstances() const
{
  glBindVertexArray(this->getVertexArray());
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
  glDrawElementsInstanced(GL_TRIANGLES, this->indicesSize, GL_UNSIGNED_INT, 0, this->getInstanceSize());
}
