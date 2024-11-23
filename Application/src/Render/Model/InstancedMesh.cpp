#include "InstancedMesh.h"

void InstancedMesh::unbind()
{
  vbo.unbind();
  ebo.unbind();
  for (auto &pair : ibos)
    pair.second.ibo.unbind();
  vao.unbind();
}

void InstancedMesh::createInstanceBuffer(unsigned int bufferId, unsigned int count, size_t size, VertexDraw draw)
{
  ibos.emplace(bufferId, InstanceBuffer(VertexBuffer(), count, size, draw));
  ibos.at(bufferId).ibo.generate();
  ibos.at(bufferId).ibo.set(count * size, nullptr, draw);
}

void InstancedMesh::setBufferAttrib(unsigned int index, unsigned int size, VertexDataType type, bool normalized, size_t stride, const void *pointer) const
{
  vbo.bind();
  vao.setVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void InstancedMesh::setInstanceBufferAttrib(unsigned int bufferId, unsigned int index, unsigned int size, VertexDataType type, bool normalized, size_t stride, const void *pointer, unsigned int divisor) const
{
  auto &ib = ibos.at(bufferId);
  ib.ibo.bind();
  ib.setInstanceBufferAttrib(index, size, type, normalized, stride, pointer, divisor);
  vao.setVertexAttribPointer(index, size, type, normalized, stride, pointer, divisor);
}

void InstancedMesh::add(unsigned int bufferId, unsigned int id, size_t size, const void *data)
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

  update(bufferId, id, size, data);
}

void InstancedMesh::update(unsigned int bufferId, unsigned int id, size_t size, const void *data) const
{
  ibos.at(bufferId).ibo.update(instances[id] * size, size, data);
}

void InstancedMesh::update(unsigned int bufferId, size_t offset, size_t size, const void *data) const
{
  ibos.at(bufferId).ibo.update(offset, size, data);
}

void InstancedMesh::draw(DrawPrimitive primitive) const
{
  vao.bind();
  ebo.bind();
  glDrawElementsInstanced((unsigned int)primitive, indices, GL_UNSIGNED_INT, 0, instances.size());
}