#include "HierarchyLayer.h"

#include "Application/Rigger.h"
#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

HierarchyLayer::HierarchyLayer(State *state) : m_State(state) {}

void HierarchyLayer::OnAttach() {

  m_Hierarchy.OnRenderItem([&](Hierarchy::Item *item) {
    std::string ctxId = ("bcm:" + std::to_string(item->id)).c_str();
    m_BoneContextMenu.Render(ctxId.c_str(), ToVoidPtr(item->id));
  });

  m_BoneContextMenu.Register({
      .renderOn = ContextMenu::PopupContext::ITEM,
      .items = {{
          .name = "New child",
          .onClick =
              [hierarchy = &m_Hierarchy](void *data) {
                uint32_t eid = *ServiceLocator::Get<Rigger>()->NewBone();
                hierarchy->Add({
                    .id = eid,
                    .parent = ToInt32(data),
                    .label = "Bone " + std::to_string(eid),
                });
              },
      }},
  });

  auto onNewBone = [hierarchy = &m_Hierarchy](void *data) {
    uint32_t eid = *ServiceLocator::Get<Rigger>()->NewBone();
    hierarchy->Add({
        .id = eid,
        .parent = 0,
        .label = "Bone " + std::to_string(eid),
    });
  };

  m_ContextMenu.Register({
      .renderOn = ContextMenu::PopupContext::WINDOW,
      .items = {{
          .name = "New bone",
          .shortcut = "Ctrl B",
          .onClick = onNewBone,
      }},
  });

  Window::RegisterShortcut({
      .ctrl = true,
      .key = ImGuiKey_B,
      .callback = onNewBone,
  });
}

void HierarchyLayer::OnUpdate(float deltaTime) {}

void HierarchyLayer::OnRender() {
  ImGui::ShowDemoWindow();

  ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_None);

  m_ContextMenu.Render("cm");

  m_Hierarchy.Render("Hierarchy");

  ImGui::End();
}

void HierarchyLayer::OnDetach() { m_State = nullptr; }

void HierarchyLayer::Save(Serializer &serializer) {}

void HierarchyLayer::Restore(Serializer &serializer) {}