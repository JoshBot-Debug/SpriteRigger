#pragma once

#include <cstring>
#include <string>
#include <unordered_map>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

class Mesh
{
private:
  VertexArray vao;
  VertexBuffer vbo;
  VertexBuffer ibo;
  ElementBuffer ebo;

  size_t indicesCount;

  size_t instanceSize;
  unsigned int instanceCount;
  std::unordered_map<unsigned int, size_t> instances;

public:
  template <typename T>

  // We don't need instanced, instead we can replace it
  // with a size_t to specify the instancePreallocatedSize
  Mesh(std::vector<T> vertices, std::vector<unsigned int> indices, unsigned int instanceCount = 0, size_t instanceSize = 0) : instanceCount(instanceCount), instanceSize(instanceSize)
  {
    vao.generate();
    vao.bind();

    vbo.generate();
    ebo.generate();

    vbo.set(vertices);
    ebo.set(indices);

    if (instanceCount)
    {
      ibo.generate();
      ibo.set(instanceCount * instanceSize, nullptr, VertexDraw::DYNAMIC);
    }

    indicesCount = indices.size();
  }

  ~Mesh() {}

  /**
   * Disable copy constructor
   */
  Mesh(const Mesh &) = delete;

  /**
   * Disable assignment operator
   */
  Mesh &operator=(const Mesh &) = delete;

  void bind()
  {
    vao.bind();
    vbo.bind();
    ebo.bind();
  }

  void unbind()
  {
    vbo.unbind();
    ebo.unbind();
    ibo.unbind();
    vao.unbind();
  }

  void setVertexAttribPointer(unsigned int index, unsigned int size, VertexDataType type, bool normalized, size_t stride, const void *pointer) const
  {
    vbo.bind();
    vao.setVertexAttribPointer(index, size, type, normalized, stride, pointer);
  }
  
  void setInstanceVertexAttribPointer(unsigned int index, unsigned int size, VertexDataType type, bool normalized, size_t stride, const void *pointer, unsigned int divisor = 1) const
  {
    ibo.bind();
    vao.setVertexAttribPointer(index, size, type, normalized, stride, pointer, divisor);
  }

  template <typename T>
  void addInstance(unsigned int id, const std::vector<T> &data, VertexDraw draw = VertexDraw::DYNAMIC)
  {
    instances[id] = instances.size();

    if (instances[id] >= instanceCount)
      ibo.resize((instanceCount *= 2) * data.size() * sizeof(T), VertexDraw::DYNAMIC);

    ibo.update(instances[id], data);
  }

  void addInstance(unsigned int id, size_t size, const void *data, VertexDraw draw = VertexDraw::DYNAMIC)
  {
    instances[id] = instances.size();

    if (instances[id] >= instanceCount)
      ibo.resize((instanceCount *= 2) * size, VertexDraw::DYNAMIC);
      
    ibo.update(instances[id], size, data);
  }

  template <typename T>
  void updateInstance(unsigned int id, const std::vector<T> &data)
  {
    ibo.update(instances[id], data);
  }

  void updateInstance(unsigned int id, size_t size, const void *data)
  {
    ibo.update(instances[id], size, data);
  }

  void drawInstances()
  {
    vao.bind();
    ebo.bind();
    glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0, instances.size());
  }
};