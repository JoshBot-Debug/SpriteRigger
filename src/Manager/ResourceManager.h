#pragma once

#include "Render/Mesh/Mesh.h"

class ResourceManager
{
private:
public:
  ResourceManager() = default;
  ~ResourceManager() = default;

  Mesh *createBone();
  unsigned int createBoneInstance(Mesh *mesh);
};