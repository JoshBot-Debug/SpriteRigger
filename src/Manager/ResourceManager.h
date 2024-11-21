#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include "Render/Model/Mesh.h"

class ResourceManager
{
private:
  std::unordered_map<std::string, Mesh *> meshes;

public:
  ResourceManager();
  ~ResourceManager() = default;

  void addBone(unsigned int id);
  void updateBone(unsigned int id, const std::vector<float> &data);
  void drawBone();
};