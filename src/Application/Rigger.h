#pragma once

#include "Utility.h"

#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Application/Components/Render.h"
#include "Application/Components/Transform.h"

class Rigger {
private:
public:
  Rigger() = default;

  void NewBone() {

    auto registry = ServiceLocator::Get<Registry>();

    Entity *bone = registry->CreateEntity("bone");

    CTransform *transform = bone->Add<CTransform>();
    CRender *render = bone->Add<CRender>();

    transform->size.x = 10;
    transform->size.y = 50;

    render->entity = bone;
  }
};