#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "GL/glew.h"

class Mesh
{
private:
  std::string name;
  mutable unsigned int vao, vbo, ebo;

  mutable unsigned int indicesSize;

  mutable std::unordered_map<std::string, unsigned int> instanceIds;
  mutable std::unordered_map<std::string, std::vector<std::vector<float>>> instances;

public:
  Mesh(std::string name) : name(name), vao(0), vbo(0), ebo(0), indicesSize(0) {}
  ~Mesh();

  /**
   * Disable copy constructor
   */
  Mesh(const Mesh &) = delete;

  /**
   * Disable assignment operator
   */
  Mesh &operator=(const Mesh &) = delete;

  /**
   * Create a vertex array object (VAO) and bind it.
   *
   * This function generates a VAO, which stores the state for vertex attribute configurations
   * (e.g., bindings of vertex buffers, attribute locations). It then binds the VAO to the current
   * OpenGL context so that subsequent vertex attribute and buffer operations will be associated
   * with this VAO. This is necessary to track all vertex input state for rendering.
   *
   * The VAO is essential for managing the input state of the vertex attributes (such as positions,
   * normals, and texture coordinates) and ensuring that they are correctly used by shaders when
   * rendering.
   */
  void createVertexArray() const;

  /**
   * Create a vertex array buffer (VBO) and bind it.
   *
   * This function generates a vertex buffer object (VBO) and binds it to the OpenGL context.
   * The VBO stores the vertex data (such as positions, colors, or normals) that will be used
   * by shaders during rendering. The data is copied into the VBO so that it can be efficiently
   * accessed by the GPU.
   *
   * This function also uploads the `vertices` data (such as positions and other vertex attributes)
   * to the GPU by using `glBufferData`. The vertex buffer is then ready to be used in conjunction
   * with a VAO to set the input state for the vertex shader.
   *
   * @param vertices Pointer to the vertex data array. This data is typically an array of floats
   *                 representing attributes like positions, normals, colors, etc.
   */
  void createArrayBuffer(std::vector<float> vertices) const;

  /**
   * Create an element array buffer (EBO) and bind it.
   *
   * This function generates an element buffer object (EBO) and binds it to the OpenGL context.
   * The EBO stores the indices that define how the vertices in the vertex array should be assembled
   * into primitives (such as triangles, lines, or points).
   *
   * By using an EBO, you avoid duplicating vertex data in the vertex array, instead referencing the
   * same vertices multiple times with indices. This leads to more efficient memory usage and faster
   * rendering.
   *
   * The `indices` array defines the order of vertex positions to create primitives, using the
   * vertex data in the associated VBO. The EBO allows you to reuse vertex data by referring to it
   * multiple times in the drawing process.
   *
   * @param indices Pointer to the index data array. This data is typically an array of unsigned integers
   *                representing indices into the vertex array (VBO) to define the geometry.
   */
  void createElementArrayBuffer(std::vector<unsigned int> indices) const;

  /**
   * Set up a vertex attribute for the shader.
   *
   * This function configures an OpenGL vertex attribute by specifying how the vertex data is
   * organized in the buffer and how the shader should access it. The attribute is bound to a specific
   * index in the shader, and the parameters like size, stride, and offset are used to define how the data
   * is laid out in the buffer and how it should be accessed by the shader during rendering.
   *
   * This is typically used to set up vertex data such as positions, normals, texture coordinates, etc.
   *
   * @param index The index of the vertex attribute in the shader to which the data will be assigned. This
   *              corresponds to the `layout(location = index)` attribute in the shader code.
   * @param size The number of components in the vertex attribute (e.g., 3 for `vec3` attributes like positions or normals).
   * @param stride The distance in bytes between consecutive attributes within the buffer. It defines how much data
   *               should be skipped to move to the next attribute in the buffer.
   * @param offset The offset in the buffer where the specific attribute data starts. This helps in extracting
   *               the data from the correct position in the buffer.
   */
  void setVertexAttrib(unsigned int index, unsigned int size, unsigned int stride, unsigned int offset) const;

  /**
   * Create the instance's array buffer and bind it.
   *
   * This function generates a buffer object specifically for storing per-instance data. It allocates
   * memory for the instance data (such as transformation matrices, colors, or other per-instance
   * attributes) and binds it to the OpenGL context. The instance buffer is used to store data that
   * changes per instance, such as position, scale, rotation, or any other instance-specific properties.
   *
   * The function returns the ID of the instance buffer, which can be used to reference it in subsequent
   * OpenGL operations (e.g., setting attributes in shaders).
   *
   * @param name The name of the instance buffer. This can be used to reference or manage
   *                     multiple instance buffers for different purposes or objects.
   */
  void createInstanceBuffer(std::string name) const;

  /**
   * Set up an instance vertex attribute for per-instance data in the shader.
   *
   * This function configures an OpenGL vertex attribute to fetch per-instance data (e.g., transformations or other attributes)
   * from the instance buffer. It binds the instance data to a specific index and sets various parameters such as
   * the size of the attribute, the stride between consecutive attributes, the offset within the buffer, and the divisor
   * for instanced drawing.
   *
   * The attribute data is accessed per instance during instanced rendering, allowing each instance of an object
   * to have unique attributes (e.g., position, rotation, scale) while sharing the same mesh data.
   *
   * @param index The index of the vertex attribute in the shader to which the data will be assigned.
   * @param size The number of components in the vertex attribute (e.g., 3 for vec3).
   * @param stride The distance in bytes between consecutive attributes within the instance buffer.
   * @param offset The offset in the buffer where the per-instance data starts, used to point to the beginning
   *               of the specific attribute data for each instance.
   * @param divisor The divisor for instancing, which specifies how frequently the attribute should change. A value of
   *                1 indicates the attribute is updated once per instance, while a value of 0 means it’s constant for all instances.
   */
  void setInstanceVertexAttrib(unsigned int index, unsigned int size, unsigned int stride, unsigned int offset, unsigned int divisor = 1) const;

  /**
   * Create an instance buffer for storing per-instance data and bind it.
   *
   * This function creates a buffer to store per-instance data (such as transformation matrices,
   * color data, etc.) for instances of an object. The instance buffer is bound to the OpenGL context
   * and contains the data that varies per instance (as opposed to vertex data which is constant across
   * all instances).
   *
   * After creating the instance buffer, it is bound to OpenGL so that instance-specific attributes
   * (such as transformations for each object instance) can be set and rendered efficiently using instanced drawing.
   *
   * @param name The name of the instance buffer, which could be used for distinguishing or managing
   *             different instance data buffers.
   * @param vertices Pointer to the array of vertex data for the instance, typically used to store
   *                 per-instance attributes, like transformation matrices (e.g., translation, rotation, scale).
   *
   * @return The number of verticies in the buffer
   */
  void setInstanceBuffer(std::string name, std::vector<float> vertices) const;

  /**
   * Update a portion of the instance buffer with new per-instance data.
   *
   * This function updates a specific section of an existing instance buffer with new data.
   * It is useful for modifying per-instance attributes (e.g., transformation matrices like
   * translation, rotation, scale) without having to recreate or rebind the entire buffer.
   * The function allows efficient updating of instance-specific data, enabling dynamic changes
   * for each instance in an object (for example, updating the positions or transformations
   * of objects in a scene).
   *
   * @param name The name of the instance buffer to be updated. This should correspond
   *             to the buffer created earlier, used for managing or distinguishing different
   *             sets of instance data.
   * @param offset The number of **floats** to skip from the beginning of the buffer before
   *               updating. This offset is in terms of the number of per-instance attributes
   *               (e.g., a matrix or vector) and must be provided in **elements** (not bytes).
   *               If you're updating a matrix (e.g., 16 floats), make sure the `offset`
   *               correctly corresponds to the number of matrices or sets of data.
   * @param vertices A vector containing the new instance data (e.g., transformation matrices).
   *                 This data will replace the existing content starting from the specified
   *                 offset, and it must match the size and format expected by the buffer.
   *
   * @note The `offset` is specified in bytes, so you must ensure that it aligns correctly
   *       with the data layout in the buffer (e.g., considering the size of each per-instance
   *       attribute like a matrix or vector).
   */
  void updateInstanceBuffer(std::string name, unsigned int offset, std::vector<float> vertices) const;

  /**
   * Bind the VAO, VBO, EBO if any
   */
  void bind() const;

  /**
   * Unbind the VAO, VBO, EBO if any.
   *
   * This function unbinds the currently bound Vertex Array Object (VAO), Vertex Buffer Object (VBO),
   * and Element Buffer Object (EBO) by calling `glBindVertexArray(0)`, `glBindBuffer(GL_ARRAY_BUFFER, 0)`,
   * and `glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)`. This ensures that no state is left active, and
   * prevents any accidental changes to the buffer bindings during subsequent OpenGL calls.
   *
   * Unbinding is a good practice to avoid unintentional state changes or conflicts when switching between
   * different buffers or VAOs. It helps keep the OpenGL context clean and avoids potential bugs related to
   * state persistence.
   */
  void unbind() const;

  unsigned int getVertexArray() const;
  unsigned int getArrayBuffer() const;
  unsigned int getElementArrayBuffer() const;

  unsigned int getIndicesSize() const;
  unsigned int getInstanceSize() const;

  void drawInstances() const;
};