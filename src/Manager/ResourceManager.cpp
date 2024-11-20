#include "ResourceManager.h"

Mesh *ResourceManager::createBone()
{
  Mesh *mesh = new Mesh("Bone");
  mesh->createVertexArray();

  std::vector<float> vertices = {
    -50.0f, -50.0f,
     50.0f, -50.0f,
     50.0f,  50.0f,
    -50.0f,  50.0f,
  };
  std::vector<unsigned int> indices = {
      0, 1, 2,
      0, 2, 3};

  mesh->createArrayBuffer(vertices);
  mesh->createElementArrayBuffer(indices);

  mesh->setVertexAttrib(0, 2, 2, 0);

  mesh->createInstanceBuffer("transform");
  mesh->setInstanceVertexAttrib(1, 3, 3, 0);

  mesh->createInstanceBuffer("color");
  mesh->setInstanceVertexAttrib(2, 3, 3, 0);

  mesh->unbind();

  return mesh;
}

void ResourceManager::createBoneInstance(Mesh *mesh)
{
  std::vector<float> transform = {0.0f, 0.0f, 0.0f};
  mesh->setInstanceBuffer("transform", transform);

  std::vector<float> color = {1.0f, 1.0f, 1.0f};
  mesh->setInstanceBuffer("color", color);
}
