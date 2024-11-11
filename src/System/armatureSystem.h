#pragma once

#include "ECS/registry.h"
#include "ECS/entity.h"

class ArmatureSystem
{
private:
  Registry *registry;
  int armatureId = 0;
  int boneId = 0;

public:
  ArmatureSystem(Registry *registry);
  ~ArmatureSystem();

  void createArmature();
};