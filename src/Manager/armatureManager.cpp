#include "armatureManager.h"
#include "Entity/components.h"

ArmatureManager::ArmatureManager(Registry *registry) : registry(registry) {}

ArmatureManager::~ArmatureManager()
{
  this->registry = nullptr;
}

void ArmatureManager::createArmature()
{
  ++armatureCount;
  ++boneCount;

  Entity *armatureEntity = this->registry->createEntity("Armature");
  CArmature *armature = armatureEntity->add<CArmature>(("Armature " + std::to_string(this->armatureCount)).c_str());

  Entity *boneEntity = this->registry->createEntity("Bone");
  boneEntity->add<CTransform>();
  CBone *bone = boneEntity->add<CBone>(("Root Bone " + std::to_string(this->boneCount)).c_str(), armatureEntity->getId());
  if (boneCount == 1)
    bone->color.y = 0;
  if (boneCount == 2)
    bone->color.x = 0;

  armature->bones.push_back(boneEntity->getId());
}