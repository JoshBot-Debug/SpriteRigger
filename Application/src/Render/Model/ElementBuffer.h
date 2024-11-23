#pragma once

#include <GL/glew.h>
#include <vector>

#include "Common.h"

class ElementBuffer
{
private:
  unsigned int ebo = 0;

public:
  ElementBuffer() = default;

  /**
   * Destructor for the ElementBuffer.
   *
   * This function deletes the OpenGL Element Buffer Object (EBO) when the ElementBuffer is destroyed.
   * It ensures that the EBO is properly cleaned up, freeing OpenGL resources.
   *
   * If the EBO is valid (non-zero), it calls `glDeleteBuffers` to delete the EBO from GPU memory.
   */
  ~ElementBuffer();

  /**
   * Disable copy constructor
   */
  ElementBuffer(const ElementBuffer &) = delete;

  /**
   * Disable assignment operator
   */
  ElementBuffer &operator=(const ElementBuffer &) = delete;

  /**
   * Generates the Element Buffer Object (EBO) if it hasn't already been created.
   *
   * This function calls `glGenBuffers` to generate an EBO if the `ebo` handle is not initialized.
   */
  void generate();

  /**
   * Sets the data for the Element Buffer Object (EBO).
   *
   * @param data The data to be stored in the EBO. Typically, this is an array of indices used for element drawing.
   * @param draw Specifies the usage pattern of the EBO. By default, it is set to `VertexDraw::STATIC`, meaning the data will not change frequently.
   *
   * This function binds the EBO and uploads the data to GPU memory using `glBufferData`.
   */
  template <typename T>
  void set(const std::vector<T> &data, VertexDraw draw = VertexDraw::STATIC)
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), (unsigned int)draw);
  }

  /**
   * Sets the data for the Element Buffer Object (EBO) using raw memory.
   *
   * @param size The size of the data in bytes.
   * @param data A pointer to the raw data to be uploaded to the GPU.
   * @param draw Specifies the usage pattern of the EBO. By default, it is set to `VertexDraw::STATIC`.
   *
   * This function binds the EBO and uploads the raw data to GPU memory using `glBufferData`.
   */
  void set(size_t size, const void *data, VertexDraw draw = VertexDraw::STATIC);

  /**
   * Binds the Element Buffer Object (EBO) to the OpenGL context.
   *
   * This function makes the EBO the active element buffer, meaning that subsequent OpenGL element drawing
   * calls will use this EBO until another EBO is bound or the binding is unbound.
   */
  void bind() const;

  /**
   * Unbinds the currently bound Element Buffer Object (EBO).
   *
   * This function unbinds the current EBO, effectively setting OpenGL to use the default EBO.
   */
  void unbind() const;

  /**
   * Gets the OpenGL ID of the Element Buffer Object (EBO).
   *
   * @return The OpenGL ID of the currently bound EBO.
   */
  unsigned int get() const;
};