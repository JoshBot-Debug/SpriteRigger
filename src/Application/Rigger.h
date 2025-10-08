#pragma once

#include "Utility.h"

#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Application/Components.h"

class Rigger {
private:
public:
  Rigger() = default;

  void NewBone(ECS::EntityId parent) {
    auto registry = ServiceLocator::Get<ECS::Registry>();

    ECS::Entity *entity = registry->CreateEntity("bone");

    CBone *bone = entity->Add<CBone>();
    CHierarchy *hierarchy = entity->Add<CHierarchy>();
    CFlags *flags = entity->Add<CFlags>();

    bone->thickness = 0.2f;
    bone->joints[CBone::StartJoint] = {.position = glm::vec2(1.0f, 1.0f)};
    bone->joints[CBone::EndJoint] = {.position = glm::vec2(-2.0f, -2.0f)};

    hierarchy->id = entity->GetId();
    hierarchy->parent = parent;
    std::snprintf(hierarchy->name, sizeof(hierarchy->name), "Bone %u",
                  entity->GetId());
  }

  void RemoveBone(ECS::EntityId id) {
    ServiceLocator::Get<ECS::Registry>()->Free(id);
  }
};