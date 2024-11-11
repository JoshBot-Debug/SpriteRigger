#pragma once

#include "ECS/registry.h"
#include "ECS/entity.h"

class ArmatureManager
{
private:
  Registry *registry;
  int armatureCount = 0;
  int boneCount = 0;

public:
  ArmatureManager(Registry *registry);
  ~ArmatureManager();

  void createArmature();
};