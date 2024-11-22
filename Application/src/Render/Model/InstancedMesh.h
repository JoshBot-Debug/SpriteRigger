#pragma once

#include <cstring>
#include <string>
#include <unordered_map>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ElementBuffer.h"

struct InstanceBufferAttrib
{
  size_t stride;        // The byte offset between consecutive elements in the buffer.
  VertexDataType type;  // The type of data in the buffer (e.g., float, int).
  unsigned int size;    // The size of each attribute in the buffer.
  unsigned int index;   // The index in the vertex attribute array.
  bool normalized;      // Whether the attribute is normalized when being interpreted.
  const void *pointer;  // A pointer to the data in the buffer.
  unsigned int divisor; // The divisor for instanced drawing (controls the frequency of attribute updates).
};

struct InstanceBuffer
{
  VertexBuffer ibo;            // The vertex buffer object that holds the buffer data.
  unsigned int count;          // The number of elements in the buffer.
  size_t size;                 // The size of each element in the buffer.
  VertexDraw draw;             // Specifies the drawing style (e.g., DYNAMIC, STATIC).
  InstanceBufferAttrib attrib; // The attributes of the instance buffer.

  InstanceBuffer(VertexBuffer &&ibo, unsigned int count, size_t size, VertexDraw draw) : ibo(std::move(ibo)), count(count), size(size), draw(draw) {}

  void setInstanceBufferAttrib(unsigned int &index, unsigned int &size, VertexDataType &type, bool &normalized, size_t &stride, const void *pointer, unsigned int &divisor)
  {
    attrib.index = index;
    attrib.size = size;
    attrib.type = type;
    attrib.normalized = normalized;
    attrib.stride = stride;
    attrib.pointer = pointer;
    attrib.divisor = divisor;
  }
};

class InstancedMesh
{
private:
  VertexArray vao;   // Vertext array
  VertexBuffer vbo;  // Vertext buffer
  ElementBuffer ebo; // Element buffer

  size_t indices; // The number of indices to draw

  mutable std::unordered_map<unsigned int, size_t> instances;            // A map storing instances by their ID and their corresponding index.
  mutable std::unordered_map<unsigned int, InstanceBuffer> ibos; // A map storing instance buffers, keyed by buffer ID.

public:
  /**
   * Constructor for creating an InstancedMesh object with vertex and index data.
   *
   * This constructor initializes a vertex array, vertex buffer, and element buffer, then sets
   * the data for the vertex and element buffers. It is used to set up a mesh object with vertices
   * and indices for rendering.
   *
   * @param vertices A vector of vertex data. Each vertex contains the attributes like positions, normals, etc.
   * @param indices A vector of unsigned integers representing the index data. Indices refer to the vertices
   *                in the vertex buffer and are used to define the drawing order of primitives.
   */
  template <typename T>
  InstancedMesh(std::vector<T> vertices, std::vector<unsigned int> indices)
  {
    vao.generate();
    vao.bind();

    vbo.generate();
    ebo.generate();

    vbo.set(vertices);
    ebo.set(indices);

    this->indices = indices.size();
  }

  ~InstancedMesh() = default;

  /**
   * Disable copy constructor
   */
  InstancedMesh(const InstancedMesh &) = delete;

  /**
   * Disable assignment operator
   */
  InstancedMesh &operator=(const InstancedMesh &) = delete;

  /**
   * Unbinds all buffers and the vertex array object (VAO).
   *
   * This function ensures that all OpenGL buffers (VBO, EBO, instance buffers) and the VAO are unbound
   * to prevent accidental changes to their state during subsequent OpenGL operations. Unbinding buffers
   * restores the OpenGL state to its default, where no buffers are bound.
   *
   * It is typically called when the mesh is no longer needed for rendering, or to clean up before
   * binding other buffers for different objects.
   */
  void unbind();

  /**
   * Creates an instance buffer with the given parameters.
   *
   * @param bufferId The unique ID for the instance buffer.
   * @param count The number of elements in the buffer.
   * @param size The size of each element in the buffer (in bytes).
   * @param draw Specifies how the buffer should be drawn (e.g., dynamic, static).
   */
  void createInstanceBuffer(unsigned int bufferId, unsigned int count, size_t size, VertexDraw draw = VertexDraw::DYNAMIC);

  /**
   * Sets the vertex attribute pointer for the vertex buffer object.
   *
   * @param index The index in the vertex attribute array for this attribute.
   * @param size The number of components in the attribute (e.g., 3 for a 3D vector).
   * @param type The data type of the attribute (e.g., float, int).
   * @param normalized Whether to normalize the data.
   * @param stride The byte offset between consecutive attributes in the buffer.
   * @param pointer A pointer to the data in the buffer.
   */
  void setBufferAttrib(unsigned int index, unsigned int size, VertexDataType type, bool normalized, size_t stride, const void *pointer) const;

  /**
   * Sets the attributes for the instance buffer.
   *
   * @param index The index in the vertex attribute array for this attribute.
   * @param size The size of the attribute (number of components).
   * @param type The type of the attribute data (e.g., float, int).
   * @param normalized Whether the attribute should be normalized when interpreted (e.g., for fixed-point data types).
   * @param stride The byte offset between consecutive elements in the buffer.
   * @param pointer A pointer to the attribute data in the buffer.
   * @param divisor The divisor used for instanced drawing. It controls how frequently the attribute is updated across instances.
   */
  void setInstanceBufferAttrib(unsigned int bufferId, unsigned int index, unsigned int size, VertexDataType type, bool normalized, size_t stride, const void *pointer, unsigned int divisor = 1) const;

  /**
   * Adds a new instance of data into the specified instance buffer.
   *
   * @param bufferId The ID of the buffer to which the data is added.
   * @param id The unique ID of the instance.
   * @param data The data to be added to the buffer.
   *
   * @tparam T The type of the data to be added (e.g., float, glm::vec3).
   */
  template <typename T>
  void add(unsigned int bufferId, unsigned int id, const std::vector<T> &data)
  {
    if (!instances.contains(id))
      instances[id] = instances.size();

    for (auto &pair : ibos)
      if (instances[id] >= pair.second.count)
      {
        pair.second.ibo.resize((pair.second.count *= 2) * pair.second.size, pair.second.draw);
        auto &attrib = pair.second.attrib;
        vao.setVertexAttribPointer(attrib.index, attrib.size, attrib.type, attrib.normalized, attrib.stride, attrib.pointer, attrib.divisor);
      }

    update(bufferId, id, data);
  }

  /**
   * Adds a new instance of raw data into the specified instance buffer.
   *
   * @param bufferId The ID of the buffer to which the data is added.
   * @param id The unique ID of the instance.
   * @param size The size of the raw data to be added (in bytes).
   * @param data A pointer to the raw data to be added.
   */
  void add(unsigned int bufferId, unsigned int id, size_t size, const void *data);

  /**
   * Updates the instance buffer with new data for a specific instance ID.
   *
   * @param bufferId The ID of the buffer to be updated.
   * @param id The ID of the instance to update.
   * @param data The new data to update the buffer with.
   *
   * @tparam T The type of the data being updated (e.g., float, glm::vec3).
   */
  template <typename T>
  void update(unsigned int bufferId, unsigned int id, const std::vector<T> &data) const
  {
    ibos.at(bufferId).ibo.update(instances[id], data);
  }

  /**
   * Updates the instance buffer with raw data for a specific instance ID.
   *
   * @param bufferId The ID of the buffer to be updated.
   * @param id The ID of the instance to update.
   * @param size The size of the data to update (in bytes).
   * @param data A pointer to the raw data to update the buffer with.
   */
  void update(unsigned int bufferId, unsigned int id, size_t size, const void *data) const;

  /**
   * Updates the instance buffer with data at a specific offset.
   *
   * @param bufferId The ID of the buffer to be updated.
   * @param offset The offset in the buffer where the data should be updated.
   * @param data The new data to update the buffer with.
   *
   * @tparam T The type of the data being updated (e.g., float, glm::vec3).
   */
  template <typename T>
  void update(unsigned int bufferId, size_t offset, const std::vector<T> &data) const
  {
    ibos.at(bufferId).ibo.update(offset, data.size() * sizeof(T), data.data());
  }

  /**
   * Updates the instance buffer with raw data at a specific offset.
   *
   * @param bufferId The ID of the buffer to be updated.
   * @param offset The offset in the buffer where the data should be updated.
   * @param size The size of the data to update (in bytes).
   * @param data A pointer to the raw data to update the buffer with.
   */
  void update(unsigned int bufferId, size_t offset, size_t size, const void *data) const;

  /**
   * Draws the instances of the mesh using instanced rendering.
   * This method binds the necessary buffers and issues the draw call for all instances.
   */
  void draw() const;
};