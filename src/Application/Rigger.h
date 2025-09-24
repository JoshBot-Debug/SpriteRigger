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

    Entity *bone = registry->CreateEntity("bone");

    CTransform *transform = bone->Add<CTransform>();
    CHierarchy *hierarchy = bone->Add<CHierarchy>();

    transform->size.x = 10;
    transform->size.y = 50;

    hierarchy->id = bone->GetId();
    hierarchy->parent = parent;
  }

  void RemoveBone(EntityId id) { ServiceLocator::Get<Registry>()->Free(id); }
};