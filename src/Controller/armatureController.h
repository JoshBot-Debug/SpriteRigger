#pragma once

#include "ECS/registry.h"
#include "ECS/entity.h"

class ArmatureController
{
private:
  Registry *registry;
  int armatureCount = 0;
  int boneCount = 0;

public:
  ArmatureController(Registry *registry);
  ~ArmatureController();

  void createArmature();
};