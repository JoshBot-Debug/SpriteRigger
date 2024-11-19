#pragma once

#include <string>
#include <vector>
#include <map>

#include "GL/glew.h"
#include "Render/Shader/Shader.h"

struct Instance
{
  unsigned int vbo;
};

class Mesh
{
private:
  std::string name;
  mutable unsigned int vao, vbo, ebo;

  mutable unsigned int indicesSize;

  mutable std::unordered_map<std::string, unsigned int> instanceIds;
  mutable std::unordered_map<std::string, std::vector<std::vector<float>>> instances;

  Shader *shader;

public:
  Mesh(std::string name) : name(name), vao(0), vbo(0), ebo(0), indicesSize(0), shader(nullptr) {}
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
   * Set the array buffer data location for the shader.
   * This function configures how per-vertex attributes (like positions, normals, texture coordinates, etc.)
   * are passed to the vertex shader. It sets up the attribute pointer for the vertex data and enables
   * it, using `glVertexAttribPointer` and `glEnableVertexAttribArray`.
   *
   * @param index The location index in the shader. This is the attribute location in the shader
   *              where the vertex data will be accessed. The index corresponds to the `layout(location = X)`
   *              in the vertex shader. It should start from 0 and match the attribute location specified in the shader.
   *
   * @param count The number of floats to include in this location. This represents the size of the attribute
   *              being passed to the shader. For example:
   *              - 1 for a `float` (scalar value),
   *              - 2 for a `vec2` (two floats),
   *              - 3 for a `vec3` (three floats),
   *              - 4 for a `vec4` (four floats).
   *              It defines how many components each vertex attribute has for this particular location.
   *
   * @param offset The offset (in terms of floats) to the start of this attribute within the vertex buffer.
   *               This defines the starting point of this particular attribute in the vertex data array.
   *               For example, if the first attribute is a `vec3` at location 0, its offset is 0.
   *               If the second attribute is a `vec2` at location 1 and follows immediately after the `vec3`,
   *               the offset would be 3 (since `vec3` takes up 3 floats). The offset is usually used when
   *               multiple attributes (e.g., position, normal, color) are packed together in the vertex data.
   */
  void setVertexAttrib(unsigned int index, unsigned int count, unsigned int offset) const;

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
   * Set the instance array buffer data location for the shader.
   * This function configures how per-instance attributes (like position, color, scale, etc.)
   * are passed to the vertex shader for each instance of the geometry. It sets up the
   * attribute pointer and enables it, using `glVertexAttribPointer` and `glVertexAttribDivisor`.
   *
   * @param index The location index in the shader. This is the attribute location that the shader
   *              uses to access the instance data. This index should start from 0 and correspond
   *              to the layout location specified in the vertex shader.
   *
   * @param count The number of floats to include in this location. This is the size of the attribute
   *              (e.g., 3 for a `vec3` or 4 for a `vec4`). It specifies how many components are
   *              used to define the per-instance data for each instance.
   *
   * @param offset The offset (in terms of float elements) to the start of this attribute within the
   *               instance buffer. For example, if you have multiple per-instance attributes packed
   *               together, you specify the starting point of this attribute. The first attribute might
   *               have an offset of 0, the second could be 3 for a `vec3`, etc.
   *
   * @param divisor The divisor tells OpenGL how often the attribute should advance (i.e., how often
   *                the value should be updated for each instance). A divisor of 1 means that the
   *                attribute will change once per instance (i.e., each instance gets a different value).
   *                A divisor of 0 means that the value will be the same for all instances (i.e., it's a
   *                per-vertex attribute, not per-instance). This is essential for efficient instanced rendering.
   */
  void setInstanceVertexAttrib(unsigned int index, unsigned int count, unsigned int offset, unsigned int divisor = 1) const;

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
   */
  void setInstanceBuffer(std::string name, std::vector<float> vertices) const;

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

  Shader *createShader();

  Shader *getShader();

  unsigned int getVertexArray();
  unsigned int getArrayBuffer();
  unsigned int getElementArrayBuffer();

  unsigned int getIndicesSize();
  unsigned int getInstanceSize();
};