#pragma once

#include <cstring>
#include <string>
#include <unordered_map>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

struct InstanceBufferAttrib
{
  size_t stride;
  VertexDataType type;

  unsigned int size;
  unsigned int index;

  bool normalized;
  const void *pointer;
  unsigned int divisor;
};

struct InstanceBuffer
{
  VertexBuffer ibo;
  unsigned int count;
  size_t size;
  VertexDraw draw;
  InstanceBufferAttrib attrib;

  InstanceBuffer(VertexBuffer &&ibo, unsigned int count, size_t size, VertexDraw draw) : ibo(std::move(ibo)), count(count), size(size), draw(draw) {}

  void setInstanceBufferAttrib(unsigned int &index, unsigned int &size, VertexDataType &type, bool &normalized, size_t &stride, const void *pointer, unsigned int &divisor)
  {
    attrib.index = index;
    attrib.size = size;
    attrib.type = type;
    attrib.normalized = normalized;
    attrib.stride = stride;
    attrib.pointer = pointer;
    attrib.divisor = divisor;
  }
};

class InstancedMesh
{
private:
  VertexArray vao;   // Vertext array
  VertexBuffer vbo;  // Vertext buffer
  ElementBuffer ebo; // Element buffer

  size_t indices; // The number of indices to draw

  std::unordered_map<unsigned int, size_t> instances;
  mutable std::unordered_map<unsigned int, InstanceBuffer> ibos;

public:
  template <typename T>
  InstancedMesh(std::vector<T> vertices, std::vector<unsigned int> indices)
  {
    vao.generate();
    vao.bind();

    vbo.generate();
    ebo.generate();

    vbo.set(vertices);
    ebo.set(indices);

    this->indices = indices.size();
  }

  ~InstancedMesh() {}

  /**
   * Disable copy constructor
   */
  InstancedMesh(const InstancedMesh &) = delete;

  /**
   * Disable assignment operator
   */
  InstancedMesh &operator=(const InstancedMesh &) = delete;

  void unbind()
  {
    vbo.unbind();
    ebo.unbind();
    for (auto &pair : ibos)
      pair.second.ibo.unbind();
    vao.unbind();
  }

  void createInstanceBuffer(unsigned int bufferId, unsigned int count, size_t size, VertexDraw draw = VertexDraw::DYNAMIC)
  {
    ibos.emplace(bufferId, InstanceBuffer(VertexBuffer(), count, size, draw));
    ibos.at(bufferId).ibo.generate();
    ibos.at(bufferId).ibo.set(count * size, nullptr, draw);
  }

  void setBufferAttrib(unsigned int index, unsigned int size, VertexDataType type, bool normalized, size_t stride, const void *pointer) const
  {
    vbo.bind();
    vao.setVertexAttribPointer(index, size, type, normalized, stride, pointer);
  }

  void setInstanceBufferAttrib(unsigned int bufferId, unsigned int index, unsigned int size, VertexDataType type, bool normalized, size_t stride, const void *pointer, unsigned int divisor = 1) const
  {
    auto &ib = ibos.at(bufferId);
    ib.ibo.bind();
    ib.setInstanceBufferAttrib(index, size, type, normalized, stride, pointer, divisor);
    vao.setVertexAttribPointer(index, size, type, normalized, stride, pointer, divisor);
  }

  template <typename T>
  void add(unsigned int bufferId, unsigned int id, const std::vector<T> &data)
  {
    if (!instances.contains(id))
      instances[id] = instances.size();

    for (auto &pair : ibos)
      if (instances[id] >= pair.second.count)
      {
        pair.second.ibo.resize((pair.second.count *= 2) * pair.second.size, pair.second.draw);
        auto &attrib = pair.second.attrib;
        vao.setVertexAttribPointer(attrib.index, attrib.size, attrib.type, attrib.normalized, attrib.stride, attrib.pointer, attrib.divisor);
      }

    update(bufferId, id, data);
  }

  void add(unsigned int bufferId, unsigned int id, size_t size, const void *data)
  {
    if (!instances.contains(id))
      instances[id] = instances.size();

    for (auto &pair : ibos)
      if (instances[id] >= pair.second.count)
      {
        pair.second.ibo.resize((pair.second.count *= 2) * pair.second.size, pair.second.draw);
        auto &attrib = pair.second.attrib;
        vao.setVertexAttribPointer(attrib.index, attrib.size, attrib.type, attrib.normalized, attrib.stride, attrib.pointer, attrib.divisor);
      }

    update(bufferId, id, size, data);
  }

  template <typename T>
  void update(unsigned int bufferId, unsigned int id, const std::vector<T> &data)
  {
    ibos.at(bufferId).ibo.update(instances[id], data);
  }

  void update(unsigned int bufferId, unsigned int id, size_t size, const void *data)
  {
    ibos.at(bufferId).ibo.update(instances[id] * size, size, data);
  }

  template <typename T>
  void update(unsigned int bufferId, size_t offset, const std::vector<T> &data)
  {
    ibos.at(bufferId).ibo.update(offset, data.size() * sizeof(T), data.data());
  }

  void update(unsigned int bufferId, size_t offset, size_t size, const void *data)
  {
    ibos.at(bufferId).ibo.update(offset, size, data);
  }

  void draw()
  {
    vao.bind();
    ebo.bind();
    glDrawElementsInstanced(GL_TRIANGLES, indices, GL_UNSIGNED_INT, 0, instances.size());
  }
};