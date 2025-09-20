#pragma once

#include "imgui.h"

#include "State.h"
#include "Window/Layer.h"
#include "Window/Window.h"

class InitializeLayer : public Layer {
private:
  State *m_State = nullptr;

  GLuint m_Texture = 0;
  int m_Width = 0, m_Height = 0;

public:
  explicit InitializeLayer(State *state);

  virtual void OnAttach() override;

  virtual void OnRender() override;

  virtual void OnDetach() override;
};
