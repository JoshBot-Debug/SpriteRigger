#include "boneController.h"
#include "Entity/components.h"

BoneController::BoneController(Registry *registry) : registry(registry) {}

BoneController::~BoneController()
{
  this->registry = nullptr;
}

void BoneController::addBone()
{
  ++count;

  Entity *entity = this->registry->createEntity("Bone");
  entity->add<PropertiesComponent>(((std::string)("Bone " + this->count)).c_str(), 1);
  entity->add<MeshComponent>(Vec2{100, 100});
  entity->add<TransformComponent>(Vec2{100, 100});
  entity->add<ColorComponent>(Vec4{255, 255, 255, 255});
}
