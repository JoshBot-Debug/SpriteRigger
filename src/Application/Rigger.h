#pragma once

#include "Utility.h"

#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Application/Components.h"

class Rigger {
private:
public:
  Rigger() = default;

  void NewBone(EntityId parent) {
    auto registry = ServiceLocator::Get<Registry>();

    Entity *entity = registry->CreateEntity("bone");

    CBone *bone = entity->Add<CBone>();
    CHierarchy *hierarchy = entity->Add<CHierarchy>();
    CFlags *flags = entity->Add<CFlags>();

    bone->start = glm::vec2(0.0f, -1.0f);
    bone->end = glm::vec2(0.0f, 1.0f);
    bone->thickness = 0.2f;

    hierarchy->id = entity->GetId();
    hierarchy->parent = parent;
    std::snprintf(hierarchy->name, sizeof(hierarchy->name), "Bone %u",
                  entity->GetId());
  }

  void RemoveBone(EntityId id) { ServiceLocator::Get<Registry>()->Free(id); }
};