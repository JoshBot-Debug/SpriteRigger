#pragma once

#include <memory>
#include <vector>

#include "imgui.h"

#include "Window/Window.h"

#include "ECS2/Entity.h"
#include "ECS2/System.h"

#include "Shader/Shader.h"

#include "Application/Components.h"

#include "Camera/Components/Grid.h"
#include "Camera/OrthographicCamera.h"

#include "Systems/BoneRenderSystem.h"
#include "Systems/ColorInterpolationSystem.h"
#include "Systems/Common.h"
#include "Systems/HoverSystem.h"
#include "Systems/SelectSystem.h"
#include "Systems/DragSystem.h"

#include "SerializableLayer.h"
#include "State.h"

class ViewportLayer : public SerializableLayer {
private:
  State *m_State = nullptr;

  std::shared_ptr<ECS2::Registry> m_Registry = nullptr;
  std::shared_ptr<ECS2::SystemManager> m_System = nullptr;

  std::shared_ptr<BoneRenderSystem> m_BoneRenderSystem = nullptr;
  std::shared_ptr<ColorInterpolationSystem> m_ColorInterpolationSystem = nullptr;
  std::shared_ptr<HoverSystem> m_HoverSystem = nullptr;
  std::shared_ptr<SelectSystem> m_SelectSystem = nullptr;
  std::shared_ptr<DragSystem> m_DragSystem = nullptr;

  SystemData m_SystemData;

  GLuint m_FrameBuffer = 0;
  GLuint m_ColorAttachment = 0;
  GLuint m_DepthBuffer = 0;

  Grid m_Grid;
  Shader m_Shader;
  OrthographicCamera m_Camera;

  struct Viewport {
    ImVec2 size{0, 0};
    ImVec2 min{0, 0};
    ImVec2 max{0, 0};
  } m_Viewport;

public:
  explicit ViewportLayer(State *state);

  virtual void OnAttach() override;

  virtual void OnRender() override;

  virtual void OnDetach() override;

  virtual void Save(Serializer &serializer) override;

  virtual void Restore(Serializer &serializer) override;

  void ResizeFramebuffer(ImVec2 viewport);
};
