#pragma once

#include "Utility.h"

#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Application/Components/Transform.h"

class Rigger {
private:
public:
  Rigger() = default;

  Entity *NewBone() {

    auto registry = ServiceLocator::Get<Registry>();

    Entity *bone = registry->CreateEntity("bone");

    CTransform *transform = bone->Add<CTransform>();

    transform->size.x = 10;
    transform->size.y = 50;

    return bone;
  }
};