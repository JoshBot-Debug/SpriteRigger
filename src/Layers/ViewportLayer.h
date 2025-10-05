#pragma once

#include "imgui.h"

#include "Window/Window.h"

#include "SerializableLayer.h"
#include "Shader/Shader.h"
#include "State.h"

#include "Application/Components.h"
#include "Camera/OrthographicCamera.h"
#include "Camera/Components/Grid.h"
#include <vector>

class ViewportLayer : public SerializableLayer {
private:
  State *m_State = nullptr;

  Shader m_Shader;

  GLuint m_VAO = 0;
  GLuint m_InstanceVBO = 0;

  GLuint m_FrameBuffer = 0;
  GLuint m_ColorAttachment = 0;
  GLuint m_DepthBuffer = 0;

  std::vector<CBone> m_Bones;

  ImVec2 m_ViewportSize{0, 0};

  OrthographicCamera m_Camera;
  
  Grid m_Grid;

public:
  explicit ViewportLayer(State *state);

  virtual void OnAttach() override;

  virtual void OnUpdate(float deltaTime) override;

  virtual void OnRender() override;

  virtual void OnDetach() override;

  virtual void Save(Serializer &serializer) override;

  virtual void Restore(Serializer &serializer) override;
};
