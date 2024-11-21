#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include "Render/Model/InstancedMesh.h"

class ResourceManager
{
private:
  std::unordered_map<std::string, InstancedMesh *> meshes;

public:
  ResourceManager();
  ~ResourceManager() = default;

  void addBone(unsigned int id);
  void updateBone(unsigned int id, const std::vector<float> &data);
  void drawBone();
};