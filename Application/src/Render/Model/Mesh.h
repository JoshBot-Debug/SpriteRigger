#pragma once

#include <cstring>
#include <string>
#include <unordered_map>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

struct Instance
{
  size_t size;
  size_t offset;
};

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
  std::unordered_map<unsigned int, Instance> instances;

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
    size_t iSize = data.size() * sizeof(T);
    size_t iCount = instances.size();

    if (iCount >= instanceCount)
      ibo.resize(iCount * iSize, (instanceCount *= 2) * iSize);

    ibo.bind();

    void *ptr = glMapBufferRange(GL_ARRAY_BUFFER,
                                 iCount * iSize,
                                 iSize,
                                 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);

    memcpy(ptr, data.data(), iSize);

    glUnmapBuffer(GL_ARRAY_BUFFER);

    instances[id] = Instance{iSize, iCount - 1};
  }

  void addInstance(unsigned int id, size_t size, const void *data, VertexDraw draw = VertexDraw::DYNAMIC)
  {
    size_t iCount = instances.size();

    if (iCount >= instanceCount)
      ibo.resize(iCount * size, (instanceCount *= 2) * size);

    ibo.bind();

    void *ptr = glMapBufferRange(GL_ARRAY_BUFFER,
                                 iCount * size,
                                 size,
                                 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);

    memcpy(ptr, data, size);

    glUnmapBuffer(GL_ARRAY_BUFFER);

    instances[id] = Instance{size, iCount - 1};
  }

  template <typename T>
  void updateInstance(unsigned int id, const std::vector<T> &data)
  {
    // ibo.update(instances[id].offset, data);
  }

  void updateInstance(unsigned int id, size_t size, const void *data)
  {
    // ibo.update(instances[id].offset, size, data);
  }

  void drawInstances()
  {
    vao.bind();
    ebo.bind();
    glDrawElementsInstanced(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0, instances.size());
  }
};