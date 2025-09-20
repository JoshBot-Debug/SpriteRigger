#pragma once

#include "imgui.h"

#include "SerializableLayer.h"
#include "State.h"
#include "Window/Window.h"

class ViewportLayer : public SerializableLayer {
private:
  State *m_State = nullptr;

public:
  explicit ViewportLayer(State *state);

  virtual void OnAttach() override;

  virtual void OnRender() override;

  virtual void OnDetach() override;

  virtual std::string GetName() override;

  virtual void Save(Serializer &serializer) override;

  virtual void Restore(Serializer &serializer) override;
};
