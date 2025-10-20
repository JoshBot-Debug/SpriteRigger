#pragma once

#include "Utility.h"

#include "ECS2/Registry.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Application/Components.h"
#include "Components/Hierarchy.h"

class Rigger {
public:
  Rigger() = default;

  void NewBone(ECS2::EntityId parent) {
    auto registry = ServiceLocator::Get<ECS2::Registry>();

    ECS2::Entity *entity = registry->CreateEntity<EBone>();

    CBone *bone = entity->Add<CBone>();
    CHierarchy *hierarchy = entity->Add<CHierarchy>();
    CFlags *flags = entity->Add<CFlags>();

    bone->thickness = 0.2f;
    bone->joints[CBone::StartJoint] = {.position = glm::vec2(1.0f, 1.0f)};
    bone->joints[CBone::EndJoint] = {.position = glm::vec2(-2.0f, -2.0f)};

    hierarchy->id = entity->GetId();
    hierarchy->parent = parent;
    std::snprintf(hierarchy->name, sizeof(hierarchy->name), "Bone %lu",
                  entity->GetId());
  }

  void RemoveBone(ECS2::EntityId id) {
    ServiceLocator::Get<ECS2::Registry>()->DestroyEntity<EBone>(id);
  }
};