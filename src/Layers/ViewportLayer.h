#pragma once

#include <memory>
#include <vector>

#include "imgui.h"

#include "Window/Window.h"

#include "ECS/Entity.h"
#include "ECS/System.h"

#include "Shader/Shader.h"

#include "Application/Components.h"

#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"

#include "Systems/BoneInteractionSystem.h"
#include "Systems/BoneRenderSystem.h"

#include "State.h"
#include "SerializableLayer.h"

struct Viewport {
  ImVec2 size{0, 0};
  ImVec2 min{0, 0};
  ImVec2 max{0, 0};
};

class ViewportLayer : public SerializableLayer {
private:
  State *m_State = nullptr;
  std::shared_ptr<ECS::Registry> m_Registry = nullptr;
  std::shared_ptr<ECS::SystemManager> m_System = nullptr;

  std::shared_ptr<BoneInteractionSystem> m_BoneInteractionSystem = nullptr;
  std::shared_ptr<BoneRenderSystem> m_BoneRenderSystem = nullptr;

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

  virtual void OnRender() override;

  virtual void OnDetach() override;

  virtual void Save(Serializer &serializer) override;

  virtual void Restore(Serializer &serializer) override;

  void ResizeFramebuffer(ImVec2 viewport);
};
