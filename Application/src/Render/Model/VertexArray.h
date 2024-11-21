#pragma once

#include <GL/glew.h>

enum class VertexDataType
{
  FLOAT,
  INT,
  UNSIGNED_INT,
  BYTE,
  UNSIGNED_BYTE,
  SHORT,
  UNSIGNED_SHORT,
  BOOL,
};

inline GLenum dataTypeToGLenum(VertexDataType type)
{
  switch (type)
  {
  case VertexDataType::FLOAT:
    return GL_FLOAT;
  case VertexDataType::INT:
    return GL_INT;
  case VertexDataType::UNSIGNED_INT:
    return GL_UNSIGNED_INT;
  case VertexDataType::BYTE:
    return GL_BYTE;
  case VertexDataType::UNSIGNED_BYTE:
    return GL_UNSIGNED_BYTE;
  case VertexDataType::SHORT:
    return GL_SHORT;
  case VertexDataType::UNSIGNED_SHORT:
    return GL_UNSIGNED_SHORT;
  case VertexDataType::BOOL:
    return GL_BOOL;
  default:
    return GL_FLOAT;
  }
}

class VertexArray
{
public:
  VertexArray() = default;

  ~VertexArray()
  {
    if (vao)
      glDeleteVertexArrays(1, &vao);
  }

  /**
   * Disable copy constructor
   */
  VertexArray(const VertexArray &) = delete;

  /**
   * Disable assignment operator
   */
  VertexArray &operator=(const VertexArray &) = delete;

  void generate()
  {
    if (!vao)
      glGenVertexArrays(1, &vao);
  }

  void setVertexAttribPointer(unsigned int index, unsigned int size, VertexDataType type, bool normalized, size_t stride, const void *pointer) const
  {
    glVertexAttribPointer(index, size, dataTypeToGLenum(type), normalized ? GL_TRUE : GL_FALSE, stride, pointer);
    glEnableVertexAttribArray(index);
  }

  void setVertexAttribPointer(unsigned int index, unsigned int size, VertexDataType type, bool normalized, size_t stride, const void *pointer, unsigned int divisor) const
  {
    glVertexAttribPointer(index, size, dataTypeToGLenum(type), normalized ? GL_TRUE : GL_FALSE, stride, pointer);
    glEnableVertexAttribArray(index);
    glVertexAttribDivisor(index, divisor);
  }

  void bind() const { glBindVertexArray(vao); }
  void unbind() const { glBindVertexArray(0); }

  unsigned int get() const { return vao; }

private:
  unsigned int vao = 0;
};