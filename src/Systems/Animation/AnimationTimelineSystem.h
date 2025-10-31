#pragma once

#include <glm/glm.hpp>

#include "ECS/Registry.h"
#include "ECS/System.h"
#include "ECS/Utility.h"

#include "Systems/Common.h"

class AnimationTimelineSystem : public ECS::System {
private:
  ECS::Registry *m_Registry = nullptr;

public:
  void Initialize(ECS::Registry *registry) { m_Registry = registry; }

  void Update(void *d) override {
    auto data = reinterpret_cast<SystemData *>(d);

    for (auto [entity, timeline] :
         m_Registry->Get<EBone, CAnimationTimeline>()) {

      if (!timeline->active) {
        entity->Remove<CAnimationTimeline>();
        continue;
      }

      timeline->elapsed += data->deltaTime;
      timeline->t =
          glm::clamp(timeline->elapsed / timeline->duration, 0.0f, 1.0f);

      if (timeline->t >= 1.0f)
        timeline->active = false;
    }
  }
};
