#pragma once

#include "ECS/registry.h"
#include "ECS/entity.h"

class BoneController
{
private:
  Registry *registry;
  int count = 0;

public:
  BoneController(Registry *registry);
  ~BoneController();

  void addBone();
};