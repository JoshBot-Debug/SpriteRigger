#include "ElementBuffer.h"

ElementBuffer::~ElementBuffer()
{
  if (ebo)
    glDeleteBuffers(1, &ebo);
}

void ElementBuffer::generate()
{
  if (!ebo)
    glGenBuffers(1, &ebo);
}

void ElementBuffer::set(size_t size, const void *data, VertexDraw draw)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, (unsigned int)draw);
}

void ElementBuffer::bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); }

void ElementBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

unsigned int ElementBuffer::get() const { return ebo; }
