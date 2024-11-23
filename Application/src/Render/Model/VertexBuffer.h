#pragma once

#include <GL/glew.h>
#include <vector>
#include <cstring>

#include "Common.h"

class VertexBuffer
{
private:
  unsigned int vbo = 0;

public:
  VertexBuffer() = default;

  /**
   * Destructor for the VertexBuffer.
   *
   * This function deletes the OpenGL buffer object (VBO) when the VertexBuffer is destroyed.
   * It ensures that the VBO is properly cleaned up to free OpenGL resources.
   *
   * If the VBO is valid (not zero), it calls `glDeleteBuffers` to delete the buffer from the GPU memory.
   */
  ~VertexBuffer();

  /**
   * Disable copy constructor
   */
  VertexBuffer(const VertexBuffer &) = delete;

  /**
   * Disable assignment operator
   */
  VertexBuffer &operator=(const VertexBuffer &) = delete;

  /**
   * Define a move constructor
   */
  VertexBuffer(VertexBuffer &&other) noexcept {};

  /**
   * Generates a buffer if it doesn't already exist.
   *
   * This function checks if the vertex buffer object (VBO) is already generated,
   * and if not, it calls OpenGL to generate one.
   */
  void generate();

  /**
   * Sets the buffer data with a vector of generic data type.
   *
   * @param data The data to store in the buffer. This is a vector of any type T.
   * @param draw Specifies how the buffer will be used (static, dynamic, etc.). Default is STATIC.
   *
   * @tparam T The type of the elements in the vector (e.g., float, glm::vec3).
   */
  template <typename T>
  void set(const std::vector<T> &data, VertexDraw draw = VertexDraw::STATIC)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), (unsigned int)draw);
  }

  /**
   * Sets the buffer data using raw data (void pointer).
   *
   * @param size The size of the data in bytes.
   * @param data A pointer to the raw data to be uploaded to the buffer.
   * @param draw Specifies how the buffer will be used (static, dynamic, etc.). Default is STATIC.
   */
  void set(size_t size, const void *data, VertexDraw draw = VertexDraw::STATIC);

  /**
   * Updates part of the buffer with a vector of generic data type.
   *
   * @param offset The offset in the buffer to start updating (in terms of number of elements).
   * @param data The new data to upload to the buffer. This is a vector of any type T.
   *
   * @tparam T The type of the elements in the vector (e.g., float, glm::vec3).
   */
  template <typename T>
  void update(unsigned int offset, const std::vector<T> &data)
  {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, offset * data.size() * sizeof(T), data.size() * sizeof(T), data.data());
  }

  /**
   * Updates part of the buffer using raw data.
   *
   * @param offset The offset in the buffer to start updating (in bytes).
   * @param size The size of the data to update (in bytes).
   * @param data A pointer to the raw data to upload to the buffer.
   */
  void update(size_t offset, size_t size, const void *data);

  /**
   * Resizes the buffer and copies the existing data to the new buffer.
   *
   * @param size The new size for the buffer (in bytes).
   * @param draw Specifies how the buffer will be used (static, dynamic, etc.). Default is STATIC.
   */
  void resize(size_t size, VertexDraw draw = VertexDraw::STATIC);

  /**
   * Binds the buffer to the GL_ARRAY_BUFFER target.
   */
  void bind() const;

  /**
   * Unbinds the buffer from the GL_ARRAY_BUFFER target.
   */
  void unbind() const;

  /**
   * Gets the OpenGL ID of the buffer.
   *
   * @return The OpenGL ID of the buffer.
   */
  unsigned int get() const;
};