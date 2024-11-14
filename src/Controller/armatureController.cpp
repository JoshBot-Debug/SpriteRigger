#include "armatureController.h"
#include "Component/components.h"

ArmatureController::ArmatureController(Registry *registry) : registry(registry) {}

ArmatureController::~ArmatureController()
{
  this->registry = nullptr;
}

void ArmatureController::createArmature()
{
  ++armatureCount;
  ++boneCount;

  Entity *armatureEntity = this->registry->createEntity("Armature");
  CArmature *armature = armatureEntity->add<CArmature>(("Armature " + std::to_string(this->armatureCount)));

  Entity *boneEntity = this->registry->createEntity("Bone");
  boneEntity->add<CTransform>();
  boneEntity->add<CBone>(("Root Bone " + std::to_string(this->boneCount)), armatureEntity->getId());

  armature->bones.push_back(boneEntity->getId());
}
