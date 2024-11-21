#include "ResourceManager.h"

ResourceManager::ResourceManager()
{
  std::vector<float> vertices = {
      -50.0f,
      -50.0f,
      50.0f,
      -50.0f,
      50.0f,
      50.0f,
      -50.0f,
      50.0f,
  };

  std::vector<unsigned int> indices = {
      0, 1, 2,
      0, 2, 3};

  InstancedMesh *mesh = new InstancedMesh(vertices, indices, 3, 5 * sizeof(float));

  mesh->setVertexAttribPointer(0, 2, VertexDataType::FLOAT, false, 2 * sizeof(float), 0);
  mesh->setInstanceVertexAttribPointer(1, 2, VertexDataType::FLOAT, false, 5 * sizeof(float), (const void *)0);
  mesh->setInstanceVertexAttribPointer(2, 3, VertexDataType::FLOAT, false, 5 * sizeof(float), (const void *)(2 * sizeof(float)));

  mesh->unbind();

  this->meshes["Bone"] = mesh;
}

void ResourceManager::addBone(unsigned int id)
{
  std::vector<float> instance = {(id - 1) * 20.0f, 0.0f, 1.0f, 0.0f, 0.0f};
  this->meshes["Bone"]->addInstance(id, instance);
  this->meshes["Bone"]->setInstanceVertexAttribPointer(1, 2, VertexDataType::FLOAT, false, 5 * sizeof(float), (const void *)0);
  this->meshes["Bone"]->setInstanceVertexAttribPointer(2, 3, VertexDataType::FLOAT, false, 5 * sizeof(float), (const void *)(2 * sizeof(float)));
}

void ResourceManager::updateBone(unsigned int id, const std::vector<float> &data)
{
  this->meshes["Bone"]->updateInstance(id, data);
}

void ResourceManager::drawBone()
{
  this->meshes["Bone"]->draw();
}
