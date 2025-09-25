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

    float cx = 400.0f;
    float cy = 300.0f;
    bone->start = glm::vec2(cx - 50.0f, cy);
    bone->end = glm::vec2(cx + 50.0f, cy);
    bone->thickness = 4.0f;

    hierarchy->id = entity->GetId();
    hierarchy->parent = parent;
    std::snprintf(hierarchy->name, sizeof(hierarchy->name), "Bone %u",
                  entity->GetId());
  }

  void RemoveBone(EntityId id) { ServiceLocator::Get<Registry>()->Free(id); }
};