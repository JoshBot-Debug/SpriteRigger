#pragma once

#include "Utility.h"

#include "ECS/Registry.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Application/Components.h"
#include "Components/Hierarchy.h"

class Rigger {
public:
  Rigger() = default;

  void NewBone(ECS::EntityId parent) {
    auto registry = ServiceLocator::Get<ECS::Registry>();

    ECS::Entity *entity = registry->CreateEntity<EBone>();

    CBone *bone = entity->Add<CBone>();
    CHierarchy *hierarchy = entity->Add<CHierarchy>();
    CFlags *flags = entity->Add<CFlags>();

    bone->thickness = 0.2f;
    bone->joints[CBone::StartJoint] = {.position = glm::vec2(1.0f, 1.0f)};
    bone->joints[CBone::EndJoint] = {.position = glm::vec2(-2.0f, -2.0f)};

    hierarchy->id = entity->GetId();
    hierarchy->parent = parent;
    if (auto pHierarchy = registry->Get<EBone, CHierarchy>(parent))
      pHierarchy->child = hierarchy->id;
    std::snprintf(hierarchy->name, sizeof(hierarchy->name), "Bone %lu",
                  entity->GetId());
  }

  void RemoveBone(ECS::EntityId id) {
    auto registry = ServiceLocator::Get<ECS::Registry>();
    ECS::EntityId eid = id;

    // Traverse forward and mark all linked bones
    std::vector<ECS::EntityId> chain;
    while (eid) {
        chain.push_back(eid);

        auto h = registry->Get<EBone, CHierarchy>(eid);
        if (!h || h->child == 0)
            break;
        eid = h->child;
    }

    // Destroy from last to first
    for (auto it = chain.rbegin(); it != chain.rend(); ++it) {
        registry->DestroyEntity<EBone>(*it);
    }

    registry->MarkChanged<EBone, CHierarchy>();
  }
};