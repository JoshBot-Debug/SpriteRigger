#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include "Render/Model/InstancedMesh.h"

struct ModelData {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
};

class ResourceManager
{
private:
  std::unordered_map<std::string, InstancedMesh *> instances;

public:
  ResourceManager();
  ~ResourceManager() = default;

  void addBone(unsigned int id);
  void updateBoneById(unsigned int bufferId, unsigned int id, const std::vector<float> &data);
  void updateBoneByOffset(unsigned int bufferId, size_t offset, const std::vector<float> &data);
  void drawBone();

  void loadFBX(const std::string &filename, std::vector<ModelData> &models);
};