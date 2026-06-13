#pragma once

#include "imgui.h"

#include "Window/Window.h"

#include "Components/Hierarchy.h"
#include "SerializableLayer.h"
#include "State.h"

#include "ECS/Registry.h"

class PropertiesLayer : public SerializableLayer
{
private:
  State*                         m_State    = nullptr;
  ECS::Entity*                   m_Entity   = nullptr;
  std::shared_ptr<ECS::Registry> m_Registry = nullptr;

  Hierarchy m_Hierarchy;

public:
  explicit PropertiesLayer(State* state);

  virtual void OnAttach() override;

  virtual void OnUpdate(float deltaTime) override;

  virtual void OnRender() override;

  virtual void OnDetach() override;

  virtual void Save(Serializer& serializer) override;

  virtual void Restore(Serializer& serializer) override;
};
