#include <GL/glew.h>

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
