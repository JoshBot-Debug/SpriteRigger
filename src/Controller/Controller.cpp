#include "Controller.h"
#include "Component/Component.h"

Controller::Controller(Registry *registry) : registry(registry) {}

Controller::~Controller()
{
  this->registry = nullptr;
}

void Controller::createArmature()
{
  ++armatureCount;
  ++boneCount;

  Entity *armatureEntity = this->registry->createEntity("Armature");
  CArmature *armature = armatureEntity->add<CArmature>(("Armature " + std::to_string(this->armatureCount)));

  Entity *boneEntity = this->registry->createEntity("Bone");
  boneEntity->add<CTransform>();
  boneEntity->add<CBone>(("Root Bone " + std::to_string(this->boneCount)), armatureEntity->getId());

  armature->bones.push_back(boneEntity->getId());


  // unsigned int instanceVBO;
  // glGenBuffers(1, &instanceVBO);
  // glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), &rectangle, GL_STATIC_DRAW);
}
