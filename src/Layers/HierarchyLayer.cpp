#include "HierarchyLayer.h"

#include "Application/Components.h"
#include "Application/Rigger.h"
#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

HierarchyLayer::HierarchyLayer(State *state) : m_State(state) {}

void HierarchyLayer::OnAttach() {

  m_BoneContextMenu.Register(
      {.renderOn = ContextMenu::PopupContext::ITEM,
       .items = {
           {
               .name = "Add child",
               .onClick =
                   [](void *data) {
                     ServiceLocator::Get<Rigger>()->NewBone(ToInt32(data));
                   },
           },
           {
               .name = "Remove child",
               .onClick =
                   [](void *data) {
                     ServiceLocator::Get<Rigger>()->RemoveBone(ToInt32(data));
                   },
           },
           {
               .name = "Rename",
               .onRenderItem =
                   [](ContextMenu::Item *item, void *data) {
                     ImGui::Separator();
                     if (ImGui::MenuItem(item->name, item->shortcut,
                                         item->selected, item->enabled))
                       ServiceLocator::Get<ECS::Registry>()
                           ->Get<CFlags>(ToInt32(data))
                           ->rename = true;
                   },
           },
       }});

  m_Hierarchy.OnRenderItem([&](Hierarchy::Item *item) {
    std::string inputId = ("##ID:" + std::to_string(item->id)).c_str();
    std::string ctxId = ("bcm:" + std::to_string(item->id)).c_str();

    m_BoneContextMenu.Render(ctxId.c_str(), ToVoidPtr(item->id));

    auto registry = ServiceLocator::Get<ECS::Registry>();

    auto *cHierarchy = registry->Get<CHierarchy>(item->id);
    auto *cFlags = registry->Get<CFlags>(item->id);

    if (!cHierarchy)
      return;

    if (ImGui::IsItemClicked()) {
      for (auto &[_, flag] : registry->Get<CFlags>())
        flag->selected = false;
      cFlags->selected = true;
    }

    ImGui::SameLine(0.0f, 0.0f);
    ImGui::SetItemAllowOverlap();
    if (!cFlags->rename)
      ImGui::TextUnformatted(cHierarchy->name);
    else {
      ImGui::SetKeyboardFocusHere();
      ImGui::InputText(inputId.c_str(), cHierarchy->name,
                       IM_ARRAYSIZE(cHierarchy->name),
                       ImGuiInputTextFlags_EnterReturnsTrue);
      if (ImGui::IsItemDeactivated())
        cFlags->rename = false;
    }

    ImGui::BeginChild("Debug");
    auto *cHovered = registry->Get<CHovered>(item->id);
    auto *cSelected = registry->Get<CSelected>(item->id);
    
    ImGui::Text("cHovered %i %s %s", !cHovered ? -1 : (int)cHovered->target, registry->MarkedForRemoval<CHovered>(item->id) ? "Marked for removal" : "", registry->GetChanged<CHovered>(item->id) ? "Dirty" : "Clean");
    ImGui::Text("cSelected %i %s %s", !cSelected ? -1 : (int)cSelected->target, registry->MarkedForRemoval<CSelected>(item->id) ? "Marked for removal" : "", registry->GetChanged<CSelected>(item->id) ? "Dirty" : "Clean");
    ImGui::EndChild();

    if (cFlags->selected)
      item->flags = ImGuiTreeNodeFlags_Selected;
    else
      item->flags = ImGuiTreeNodeFlags_None;
  });

  m_ContextMenu.Register({
      .renderOn = ContextMenu::PopupContext::WINDOW,
      .items = {{
          .name = "New bone",
          .shortcut = "Ctrl B",
          .onClick =
              [](void *data) { ServiceLocator::Get<Rigger>()->NewBone(0); },
      }},
  });
}

void HierarchyLayer::OnUpdate(float deltaTime) {
  auto registry = ServiceLocator::Get<ECS::Registry>();

  const auto components = registry->GetChanged<CHierarchy>();

  if (components.size())
    m_Hierarchy.Clear();

  for (auto &[eid, cHierarchy] : components) {
    registry->ClearChanged<CHierarchy>(eid);
    m_Hierarchy.Add({
        .id = cHierarchy->id,
        .parent = cHierarchy->parent,
    });
  }
}

void HierarchyLayer::OnRender() {
  ImGui::ShowDemoWindow();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2({0, 0}));
  ImGui::Begin("Hierarchy");
  ImGui::PopStyleVar();

  m_ContextMenu.Render("cm");

  m_Hierarchy.Render("Hierarchy");

  ImGui::End();
}

void HierarchyLayer::OnDetach() { m_State = nullptr; }

void HierarchyLayer::Save(Serializer &serializer) {}

void HierarchyLayer::Restore(Serializer &serializer) {}