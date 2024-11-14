#pragma once

#include "ECS/Registry.h"
#include "ECS/Entity.h"

class Controller
{
private:
  Registry *registry;
  int armatureCount = 0;
  int boneCount = 0;

public:
  Controller(Registry *registry);
  ~Controller();

  void createArmature();
};