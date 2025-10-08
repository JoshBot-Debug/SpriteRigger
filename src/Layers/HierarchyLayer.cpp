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

    CHierarchy *cHierarchy = registry->Get<CHierarchy>(item->id);
    CFlags *cFlags = registry->Get<CFlags>(item->id);

    if (!cHierarchy)
      return;

    if (ImGui::IsItemClicked()) {
      for (auto &flag : registry->Get<CFlags>())
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

  if (registry->AnyChanged<CHierarchy>()) {
    registry->ClearChanged<CHierarchy>();

    const auto &[components] = registry->Collect<CHierarchy>();

    m_Hierarchy.Clear();

    for (size_t i = 0; i < components.size(); i++) {
      uint32_t id = components[i]->id;
      uint32_t parent = components[i]->parent;
      m_Hierarchy.Add({
          .id = id,
          .parent = parent,
      });
    }
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