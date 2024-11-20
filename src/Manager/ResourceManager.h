#pragma once

#include "Render/Mesh/Mesh.h"

class ResourceManager
{
private:
public:
  ResourceManager() = default;
  ~ResourceManager() = default;

  Mesh *createBone();
  void createBoneInstance(Mesh *mesh);
};