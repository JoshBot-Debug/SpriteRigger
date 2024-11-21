#pragma once

#include <GL/glew.h>
#include <vector>

class ElementBuffer
{
public:
  ElementBuffer() = default;
  
  ~ElementBuffer()
  {
    if (ebo)
      glDeleteBuffers(1, &ebo);
  }

  /**
   * Disable copy constructor
   */
  ElementBuffer(const ElementBuffer &) = delete;

  /**
   * Disable assignment operator
   */
  ElementBuffer &operator=(const ElementBuffer &) = delete;

  void generate()
  {
    if (!ebo)
      glGenBuffers(1, &ebo);
  }

  template <typename T>
  void set(const std::vector<T> &data, VertexDraw draw = VertexDraw::STATIC)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), vertexDrawToGLenum(draw));
  }

  void set(size_t size, const void *data, VertexDraw draw = VertexDraw::STATIC)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, vertexDrawToGLenum(draw));
  }


  void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); }
  void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

  unsigned int get() const { return ebo; }

private:
  unsigned int ebo = 0;
};