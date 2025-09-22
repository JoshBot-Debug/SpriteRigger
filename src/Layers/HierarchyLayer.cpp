#include "HierarchyLayer.h"

#include "Application/Rigger.h"
#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Application/Components/Render.h"

HierarchyLayer::HierarchyLayer(State *state) : m_State(state) {}

void HierarchyLayer::OnAttach() {

  m_Registry = ServiceLocator::Get<Registry>();

  auto rigger = ServiceLocator::Get<Rigger>();

  m_ContextMenu.Register({
      .id = "contextMenu",
      .items = {{
          .name = "New bone",
          .shortcut = "Ctrl B",
          .onClick = [&rigger]() { rigger->NewBone(); },
      }},
  });

  m_Hierarchy.Add(0, {
    .id = 1,
    .label = "Child 1",
  });
  m_Hierarchy.Add(0, {
    .id = 2,
    .label = "Child 2",
  });
  m_Hierarchy.Add(2, {
    .id = 3,
    .label = "Child of child 2",
  });

  Window::RegisterShortcut({
      .ctrl = true,
      .key = ImGuiKey_B,
      .callback = [&rigger](Window *window) { rigger->NewBone(); },
  });
}

void HierarchyLayer::OnUpdate(float deltaTime) {}

void HierarchyLayer::OnRender() {
  ImGui::ShowDemoWindow();

  ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_None);

  m_ContextMenu.Render();

  m_Hierarchy.Render("HierarchyLayer");

  auto [renders] = m_Registry->Collect<CRender>();

  for (const auto &render : renders)
    render->RenderHierarchyItem();

  ImGui::End();
}

void HierarchyLayer::OnDetach() { m_State = nullptr; }

void HierarchyLayer::Save(Serializer &serializer) {}

void HierarchyLayer::Restore(Serializer &serializer) {}