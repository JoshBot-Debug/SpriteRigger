#include "ResourceManager.h"

#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

ResourceManager::ResourceManager()
{
  // std::vector<float> vertices = {
  //     -0.0f,
  //     -0.0f,
  //     100.0f,
  //     -0.0f,
  //     100.0f,
  //     100.0f,
  //     -0.0f,
  //     100.0f,
  // };

  // std::vector<unsigned int> indices = {
  //     0, 1, 2,
  //     0, 2, 3};

  std::vector<ModelData> models;

  loadFBX("assets/model/bone.fbx", models);

  ModelData model = models[0];

  InstancedMesh *instance = new InstancedMesh(model.vertices, model.indices);

  for (size_t i = 0; i < model.vertices.size(); i++)
  {
    std::cout << model.vertices[i] << std::endl;
  }
  

  instance->setBufferAttrib(0, 3, VertexDataType::FLOAT, false, 8 * sizeof(float), 0); // Position
  // instance->setBufferAttrib(1, 3, VertexDataType::FLOAT, false, 8 * sizeof(float), (void *)(3 * sizeof(float))); // Normal
  // instance->setBufferAttrib(2, 2, VertexDataType::FLOAT, false, 8 * sizeof(float), (void *)(6 * sizeof(float))); // TexCoord

  instance->createInstanceBuffer(0, 100, 3 * sizeof(float), VertexDraw::DYNAMIC);
  instance->setInstanceBufferAttrib(0, 1, 3, VertexDataType::FLOAT, false, 3 * sizeof(float), 0);

  instance->createInstanceBuffer(1, 100, 2 * sizeof(float), VertexDraw::DYNAMIC);
  instance->setInstanceBufferAttrib(1, 2, 2, VertexDataType::FLOAT, false, 2 * sizeof(float), 0);

  instance->createInstanceBuffer(2, 100, 3 * sizeof(float), VertexDraw::DYNAMIC);
  instance->setInstanceBufferAttrib(2, 3, 3, VertexDataType::FLOAT, false, 3 * sizeof(float), 0);

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
  this->instances["Bone"]->draw(DrawPrimitive::TRIANGLES);
}

void ResourceManager::loadFBX(const std::string &filename, std::vector<ModelData> &models)
{
  // Initialize Assimp importer
  Assimp::Importer importer;

  // Load the model (FBX file)
  const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipWindingOrder | aiProcess_GenSmoothNormals);

  if (!scene)
  {
    std::cerr << "Failed to load FBX model: " << importer.GetErrorString() << std::endl;
    return;
  }

  // Reserve space in models
  models.resize(scene->mNumMeshes);

  // Iterate over each mesh in the scene
  for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
  {
    aiMesh *mesh = scene->mMeshes[i];
    ModelData &data = models[i];

    // Process vertices
    for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
    {
      const aiVector3D &vertex = mesh->mVertices[j];
      data.vertices.push_back(vertex.x);
      data.vertices.push_back(vertex.y);
      data.vertices.push_back(vertex.z);

      const aiVector3D &normal = mesh->mNormals[j];
      data.vertices.push_back(normal.x);
      data.vertices.push_back(normal.y);
      data.vertices.push_back(normal.z);

      // Optional: Process texture coordinates if present
      if (mesh->mTextureCoords[0])
      {
        const aiVector3D &texCoord = mesh->mTextureCoords[0][j];
        data.vertices.push_back(texCoord.x);
        data.vertices.push_back(texCoord.y);
      }
      else
      {
        data.vertices.push_back(0.0f);
        data.vertices.push_back(0.0f);
      }
    }

    // Process indices
    for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
    {
      const aiFace &face = mesh->mFaces[j];
      for (unsigned int k = 0; k < face.mNumIndices; ++k)
        data.indices.push_back(face.mIndices[k]);
    }
  }
}
