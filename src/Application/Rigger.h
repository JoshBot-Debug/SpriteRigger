#pragma once

#include "Utility.h"

#include "ECS/Registry.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Application/Components.h"
#include "Components/Hierarchy.h"

class Rigger
{
private:
  Hierarchy* m_Hierarchy = nullptr;

public:
  Rigger() = default;

  void SetHierarchy(Hierarchy* hierarchy)
  {
    m_Hierarchy = hierarchy;
  }

  void NewBone(ECS::EntityId parent)
  {
    auto registry = ServiceLocator::Get<ECS::Registry>();

    ECS::Entity* entity = registry->CreateEntity<EBone>();

    CBone*      bone      = entity->Add<CBone>();
    CHierarchy* hierarchy = entity->Add<CHierarchy>();
    CFlags*     flags     = entity->Add<CFlags>();

    bone->thickness                 = 0.2f;
    bone->joints[static_cast<int>(CBone::Part::StartJoint)] = {.position = glm::vec2(1.0f, 1.0f)};
    bone->joints[static_cast<int>(CBone::Part::EndJoint)]   = {.position = glm::vec2(-2.0f, -2.0f)};

    hierarchy->id     = entity->GetId();
    hierarchy->parent = parent;

    std::snprintf(hierarchy->name, sizeof(hierarchy->name), "Bone %lu", entity->GetId());

    m_Hierarchy->Add({
        .id     = hierarchy->id,
        .parent = hierarchy->parent,
    });
  }

  void RemoveBone(ECS::EntityId id)
  {
    auto registry = ServiceLocator::Get<ECS::Registry>();

    std::vector<ECS::EntityId> chain{id};

    auto search = [&](auto&& self, ECS::EntityId target) -> void
    {
      for (auto& [bone, hierarchy] : registry->Get<EBone, CHierarchy>())
        if (hierarchy->parent == target)
        {
          chain.push_back(hierarchy->id);
          self(self, hierarchy->id);
        }
    };

    search(search, id);

    for (auto it = chain.rbegin(); it != chain.rend(); ++it)
    {
      m_Hierarchy->Remove(*it);
      registry->DestroyEntity<EBone>(*it);
    }
  }
};