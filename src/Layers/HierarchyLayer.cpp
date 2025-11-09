#include "HierarchyLayer.h"

#include "Application/Components.h"
#include "Application/Rigger.h"
#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

HierarchyLayer::HierarchyLayer(State *state) : m_State(state) {
  ServiceLocator::Get<Rigger>()->SetHierarchy(&m_Hierarchy);
}

void HierarchyLayer::OnAttach() {

  for (auto &[entity, cHierarchy] :
       ServiceLocator::Get<ECS::Registry>()->Get<EBone, CHierarchy>())
    m_Hierarchy.Add({
        .id = cHierarchy->id,
        .parent = cHierarchy->parent,
    });

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
                           ->Get<EBone, CFlags>(ToInt32(data))
                           ->rename = true;
                   },
           },
       }});

  m_Hierarchy.SetRenderItemData(&m_BoneContextMenu);

  m_Hierarchy.OnRenderItem([](Hierarchy::Item *item, void *data) {
    auto boneContextMenu = static_cast<ContextMenu *>(data);

    std::string inputId = ("##ID:" + std::to_string(item->id)).c_str();
    std::string ctxId = ("bcm:" + std::to_string(item->id)).c_str();

    boneContextMenu->Render(ctxId.c_str(), ToVoidPtr(item->id));

    auto registry = ServiceLocator::Get<ECS::Registry>();

    auto *cHierarchy = registry->Get<EBone, CHierarchy>(item->id);
    auto *cFlags = registry->Get<EBone, CFlags>(item->id);

    if (!cHierarchy)
      return;

    if (ImGui::IsItemClicked()) {
      for (auto [_, cFlag] : registry->Get<EBone, CFlags>())
        cFlag->selected = false;
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

void HierarchyLayer::OnUpdate(float deltaTime) {}

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