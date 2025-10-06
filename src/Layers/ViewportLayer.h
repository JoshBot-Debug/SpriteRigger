#pragma once

#include "imgui.h"

#include "Window/Window.h"

#include "ECS/Entity.h"
#include "SerializableLayer.h"
#include "Shader/Shader.h"
#include "State.h"

#include "Application/Components.h"
#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"
#include <vector>

#include <memory>

struct Viewport {
  ImVec2 size{0, 0};
  ImVec2 min{0, 0};
  ImVec2 max{0, 0};
};

class ViewportLayer : public SerializableLayer {
private:
  State *m_State = nullptr;
  std::shared_ptr<Registry> m_Registry = nullptr;

  Shader m_Shader;

  GLuint m_VAO = 0;
  GLuint m_InstanceVBO = 0;

  GLuint m_FrameBuffer = 0;
  GLuint m_ColorAttachment = 0;
  GLuint m_DepthBuffer = 0;

  std::vector<CBone> m_Bones;

  Viewport m_Viewport;

  OrthographicCamera m_Camera;

  Grid m_Grid;

public:
  explicit ViewportLayer(State *state);

  virtual void OnAttach() override;

  void OnUpdate();

  virtual void OnRender() override;

  virtual void OnDetach() override;

  virtual void Save(Serializer &serializer) override;

  virtual void Restore(Serializer &serializer) override;
};
