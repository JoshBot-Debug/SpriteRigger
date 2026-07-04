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

#include "Systems/BoneRenderSystem.h"
#include "Systems/Common.h"
#include "Systems/DragSystem.h"
#include "Systems/HoverSystem.h"
#include "Systems/SelectSystem.h"

#include "SerializableLayer.h"
#include "State.h"
#include "Types.h"

#include "Components/ContextMenu2.h"

#include "Animate/System.h"
class ViewportLayer : public SerializableLayer
{
private:
  struct VContextMenuState
  {
    std::optional<std::pair<CHierarchy*, CHierarchy*>> parenting;
  };

  using VContextMenu = ContextMenu2<VContextMenuState>;

private:
  State* m_State = nullptr;

  std::shared_ptr<ECS::Registry>      m_Registry = nullptr;
  std::shared_ptr<ECS::SystemManager> m_System   = nullptr;

  std::shared_ptr<BoneRenderSystem> m_BoneRenderSystem = nullptr;
  std::shared_ptr<HoverSystem>      m_HoverSystem      = nullptr;
  std::shared_ptr<SelectSystem>     m_SelectSystem     = nullptr;
  std::shared_ptr<DragSystem>       m_DragSystem       = nullptr;
  std::shared_ptr<Animate::System>  m_AnimationSystem  = nullptr;

  Systems::Data m_SystemData;

  VContextMenuState m_ContextMenuState;
  VContextMenu      m_ContextMenu{"vcx"};

  GLuint m_FrameBuffer     = 0;
  GLuint m_ColorAttachment = 0;
  GLuint m_DepthBuffer     = 0;

  Grid               m_Grid;
  Shader             m_Shader;
  OrthographicCamera m_Camera;
  Viewport           m_Viewport;

  /// TODO: Set this by checking CSelected
  bool m_TwoBoneSelected = true;

public:
  explicit ViewportLayer(State* state);

  virtual void OnAttach() override;

  virtual void OnBegin() override;

  virtual void OnUpdate(float deltaTime) override;

  virtual void OnRender() override;

  virtual void OnEnd() override;

  virtual void OnDetach() override;

  virtual void Save(Serializer& serializer) override;

  virtual void Restore(Serializer& serializer) override;

  void ResizeFramebuffer(ImVec2 viewport);
};
