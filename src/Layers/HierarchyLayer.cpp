#include "HierarchyLayer.h"

#include "Application/Components.h"
#include "Application/Rigger.h"
#include "ECS/Entity.h"
#include "ServiceLocator/ServiceLocator.h"

HierarchyLayer::HierarchyLayer(State *state) : m_State(state) {}

void HierarchyLayer::OnAttach() {

  m_BoneContextMenu.Register({
      .renderOn = ContextMenu::PopupContext::ITEM,
      .items = {{
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
                          ServiceLocator::Get<Rigger>()->RemoveBone(
                              ToInt32(data));
                        },
                },
                {
                    .name = "Rename",
                    .onClick =
                        [](void *data) {
                          ServiceLocator::Get<Registry>()
                              ->Get<CHierarchy>(ToInt32(data))
                              ->rename = true;
                        },
                }},
  });

  m_Hierarchy.OnRenderItem([&](Hierarchy::Item *item) {
    std::string inputId = ("##ID:" + std::to_string(item->id)).c_str();
    std::string ctxId = ("bcm:" + std::to_string(item->id)).c_str();

    m_BoneContextMenu.Render(ctxId.c_str(), ToVoidPtr(item->id));

    auto registry = ServiceLocator::Get<Registry>();

    CHierarchy *cHierarchy = registry->Get<CHierarchy>(item->id);

    if (!cHierarchy)
      return;

    ImGui::SameLine(0.0f, 0.0f);
    ImGui::SetItemAllowOverlap();
    if (!cHierarchy->rename)
      ImGui::TextUnformatted(cHierarchy->name);
    else {
      ImGui::SetKeyboardFocusHere();
      ImGui::InputText(inputId.c_str(), cHierarchy->name,
                       IM_ARRAYSIZE(cHierarchy->name),
                       ImGuiInputTextFlags_EnterReturnsTrue);
      if (ImGui::IsItemDeactivated())
        cHierarchy->rename = false;
    }
  });

  auto onNewBone = [](void *data) {
    ServiceLocator::Get<Rigger>()->NewBone(0);
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

void HierarchyLayer::OnUpdate(float deltaTime) {
  auto registry = ServiceLocator::Get<Registry>();

  if (registry->HasChanged<CHierarchy>()) {
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

  ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_None);

  m_ContextMenu.Render("cm");

  m_Hierarchy.Render("Hierarchy");

  ImGui::End();
}

void HierarchyLayer::OnDetach() { m_State = nullptr; }

void HierarchyLayer::Save(Serializer &serializer) {}

void HierarchyLayer::Restore(Serializer &serializer) {}