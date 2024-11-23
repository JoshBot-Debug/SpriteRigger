#include "VertexBuffer.h"

VertexBuffer::~VertexBuffer()
{
  if (vbo)
    glDeleteBuffers(1, &vbo);
}

void VertexBuffer::generate()
{
  if (!vbo)
    glGenBuffers(1, &vbo);
}

void VertexBuffer::set(size_t size, const void *data, VertexDraw draw)
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size, data, (unsigned int)draw);
}

void VertexBuffer::update(size_t offset, size_t size, const void *data)
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::resize(size_t size, VertexDraw draw)
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
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, (unsigned int)draw);

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

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, vbo); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

unsigned int VertexBuffer::get() const { return vbo; }