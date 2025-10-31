#pragma once

#include <glm/glm.hpp>

#include "ECS/Registry.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Systems/Common.h"

class ValueAnimationSystem : public ECS::System {
private:
  ECS::Registry *m_Registry = nullptr;

public:
  void Initialize(ECS::Registry *registry) { m_Registry = registry; }

  void Update(void *d) override {
    auto data = reinterpret_cast<SystemData *>(d);

    for (auto [entity, animation] :
         m_Registry->Get<EBone, CValueAnimation<glm::vec4>>()) {
      if (!animation->timeline || !animation->timeline->active ||
          !animation->target) {
        entity->Remove<CValueAnimation<glm::vec4>>();
        continue;
      }

      *animation->target =
          glm::mix(animation->start, animation->end, animation->timeline->t);
      entity->MarkChanged<CBone>();
    }
  }
};
