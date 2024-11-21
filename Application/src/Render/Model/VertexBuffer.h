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
    glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(T), data.size() * sizeof(T), data.data());
  }

  void update(size_t offset, size_t size, const void *data)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
  }

  void resize(size_t psize, size_t nsize)
  {
    unsigned int nvbo;
    glGenBuffers(1, &nvbo);
    glBindBuffer(GL_ARRAY_BUFFER, nvbo);
    glBufferData(GL_ARRAY_BUFFER, nsize, nullptr, GL_DYNAMIC_DRAW);

    void *nptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, nsize,
                                  GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    void *pptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, psize,
                                  GL_MAP_READ_BIT);

    memcpy(nptr, pptr, psize);

    glUnmapBuffer(GL_ARRAY_BUFFER);
    
    glBindBuffer(GL_ARRAY_BUFFER, nvbo);
    glUnmapBuffer(GL_ARRAY_BUFFER);

    vbo = nvbo;
  }

  void bind() const { glBindBuffer(GL_ARRAY_BUFFER, vbo); }
  void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

  unsigned int get() const { return vbo; }

private:
  unsigned int vbo = 0;
};