#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
  std::vector<float> vertices = {
      -0.0f,
      -0.0f,
      100.0f,
      -0.0f,
      100.0f,
      100.0f,
      -0.0f,
      100.0f,
  };

  std::vector<unsigned int> indices = {
      0, 1, 2,
      0, 2, 3};

  InstancedMesh *instance = new InstancedMesh(vertices, indices);

  instance->setBufferAttrib(0, 2, VertexDataType::FLOAT, false, 2 * sizeof(float), 0);

  instance->createInstanceBuffer(0, 100, 3 * sizeof(float), VertexDraw::DYNAMIC);
  instance->setInstanceBufferAttrib(0, 1, 3, VertexDataType::FLOAT, false, 3 * sizeof(float), 0);

  instance->createInstanceBuffer(1, 100, 3 * sizeof(float), VertexDraw::DYNAMIC);
  instance->setInstanceBufferAttrib(1, 2, 3, VertexDataType::FLOAT, false, 3 * sizeof(float), 0);

  instance->unbind();

  this->instances["Bone"] = instance;
}

void ResourceManager::addBone(unsigned int id)
{
  std::vector<float> transform = {0.0f, 0.0f, 0.0f};
  this->instances["Bone"]->add(0, id, transform);
}

void ResourceManager::updateBoneById(unsigned int bufferId, unsigned int id, const std::vector<float> &data)
{
  this->instances["Bone"]->update(bufferId, id, data);
}

void ResourceManager::updateBoneByOffset(unsigned int bufferId, size_t offset, const std::vector<float> &data)
{
  this->instances["Bone"]->update(bufferId, offset, data);
}

void ResourceManager::drawBone()
{
  this->instances["Bone"]->draw();
}
