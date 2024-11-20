#pragma once

#include "ECS/Entity.h"
#include "ECS/Registry.h"

#include "Manager/ResourceManager.h"

#include "Component/Component.h"

class Controller
{
private:
  Registry *registry;
  ResourceManager *resourceManager;

public:
  Controller();
  ~Controller();

  void onInitialize(Registry *registry, ResourceManager *resourceManager);

  void createArmature();
};