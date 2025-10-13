#pragma once

#include "imgui.h"

#include "Window/Window.h"

#include "Components/ContextMenu.h"
#include "Components/Hierarchy.h"
#include "SerializableLayer.h"
#include "State.h"

#include "ECS2/Registry.h"

class HierarchyLayer : public SerializableLayer {
private:
  State *m_State = nullptr;
  
  ContextMenu m_ContextMenu;
  ContextMenu m_BoneContextMenu;
  Hierarchy m_Hierarchy;

public:
  explicit HierarchyLayer(State *state);

  virtual void OnAttach() override;

  virtual void OnUpdate(float deltaTime) override;

  virtual void OnRender() override;

  virtual void OnDetach() override;

  virtual void Save(Serializer &serializer) override;

  virtual void Restore(Serializer &serializer) override;
};
