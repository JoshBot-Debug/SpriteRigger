#pragma once

#include <GL/glew.h>
#include <vector>
#include <cstring>

enum class VertexDraw
{
  STATIC,
  DYNAMIC,
  STREAM
};

inline GLenum vertexDrawToGLenum(VertexDraw draw)
{
  switch (draw)
  {
  case VertexDraw::STATIC:
    return GL_STATIC_DRAW;
  case VertexDraw::DYNAMIC:
    return GL_DYNAMIC_DRAW;
  case VertexDraw::STREAM:
    return GL_STREAM_DRAW;
  default:
    return GL_STATIC_DRAW;
  }
}

class VertexBuffer
{
public:
  VertexBuffer() = default;

  ~VertexBuffer()
  {
    if (vbo)
      glDeleteBuffers(1, &vbo);
  }

  /**
   * Disable copy constructor
   */
  VertexBuffer(const VertexBuffer &) = delete;

  /**
   * Disable assignment operator
   */
  VertexBuffer &operator=(const VertexBuffer &) = delete;

  void generate()
  {
    if (!vbo)
      glGenBuffers(1, &vbo);
  }

  template <typename T>
  void set(const std::vector<T> &data, VertexDraw draw = VertexDraw::STATIC)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), vertexDrawToGLenum(draw));
  }

  void set(size_t size, const void *data, VertexDraw draw = VertexDraw::STATIC)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, vertexDrawToGLenum(draw));
  }

  template <typename T>
  void update(unsigned int offset, const std::vector<T> &data)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset * data.size() * sizeof(T), data.size() * sizeof(T), data.data());
  }

  void update(size_t offset, size_t size, const void *data)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
  }

  void resize(size_t size, VertexDraw draw = VertexDraw::STATIC)
  {
    /**
     * Read the size of the current vbo into psize
     * Bind it as GL_COPY_READ_BUFFER
     */
    int psize;
    glBindBuffer(GL_COPY_READ_BUFFER, vbo);
    glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &psize);

    /**
     * Create the new vertex buffer with the new size
     */
    unsigned int nvbo;
    glGenBuffers(1, &nvbo);
    glBindBuffer(GL_ARRAY_BUFFER, nvbo);
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, vertexDrawToGLenum(draw));

    /**
     * Bind the new vertex buffer as GL_COPY_WRITE_BUFFER
     */
    glBindBuffer(GL_COPY_WRITE_BUFFER, nvbo);

    /**
     * Copy the data
     */
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, psize);

    /**
     * Delete the old buffer
     */
    glDeleteBuffers(1, &vbo);

    vbo = nvbo;
  }

  void bind() const { glBindBuffer(GL_ARRAY_BUFFER, vbo); }
  void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

  unsigned int get() const { return vbo; }

private:
  unsigned int vbo = 0;
};