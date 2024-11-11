#include "armatureSystem.h"
#include "Entity/components.h"

ArmatureSystem::ArmatureSystem(Registry *registry) : registry(registry) {}

ArmatureSystem::~ArmatureSystem()
{
  this->registry = nullptr;
}

void ArmatureSystem::createArmature()
{
  ++armatureId;
  ++boneId;

  Entity *armatureEntity = this->registry->createEntity("Armature");
  CArmature *armature = armatureEntity->add<CArmature>(this->armatureId, "Armature " + std::to_string(this->armatureId));

  Entity *boneEntity = this->registry->createEntity("Bone");
  boneEntity->add<CTransform>();
  boneEntity->add<CBone>(this->boneId, "Root Bone " + std::to_string(this->boneId));

  armature->bones.push_back(boneEntity);
}