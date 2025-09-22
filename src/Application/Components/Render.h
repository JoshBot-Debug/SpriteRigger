#pragma once

#include "imgui.h"

#include "ECS/Entity.h"
#include "Transform.h"

struct CRender {
  Entity *entity = nullptr;

  void RenderHierarchyItem() {
    CTransform *transform = entity->Get<CTransform>();

    ImGui::Text("Position: (%f, %f, %f), Size: (%i, %i)", transform->position.x,
                transform->position.y, transform->position.z, transform->size.x,
                transform->size.y);
  }
};
