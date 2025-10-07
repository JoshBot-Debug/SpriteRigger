#include "ViewportLayer.h"

#include "Application/Components.h"
#include "ServiceLocator/ServiceLocator.h"

#include "Utility.h"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>

#include "ECS/Utility.h"

ViewportLayer::ViewportLayer(State *state)
    : m_State(state), m_Grid(&m_Camera) {}

void ViewportLayer::OnAttach() {
  m_Registry = ServiceLocator::Get<ECS::Registry>();
  m_System = ServiceLocator::Get<ECS::SystemManager>();

  m_BoneInteractionSystem = m_System->Register<BoneInteractionSystem>();
  m_BoneRenderSystem = m_System->Register<BoneRenderSystem>();
  m_JointRenderSystem = m_System->Register<JointRenderSystem>();
  m_JointInteractionSystem = m_System->Register<JointInteractionSystem>();

  m_BoneInteractionSystem->Initialize(m_Registry.get(), &m_Grid);
  m_BoneRenderSystem->Initialize(m_Registry.get(), &m_Shader, &m_Camera);
  m_JointRenderSystem->Initialize(m_Registry.get(), &m_Shader, &m_Camera);
  m_JointInteractionSystem->Initialize(m_Registry.get(), &m_Grid);
}

void ViewportLayer::OnRender() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_None);
  ImGui::PopStyleVar();

  ImVec2 viewport = ImGui::GetContentRegionAvail();
  ImVec2 windowPosition = ImGui::GetWindowPos();

  m_Viewport.min = windowPosition + ImGui::GetWindowContentRegionMin();
  m_Viewport.max = windowPosition + ImGui::GetWindowContentRegionMax();

  {
    m_Grid.Update(m_Viewport.size, m_Viewport.min, m_Viewport.max);
    m_Camera.Update((uint32_t)m_Viewport.size.x, (uint32_t)m_Viewport.size.y);
    m_System->Update<BoneInteractionSystem>();
    m_System->Update<JointInteractionSystem>();
  }

  ResizeFramebuffer(viewport);

  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_System->Update<BoneRenderSystem>();
    m_System->Update<JointRenderSystem>();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  m_Grid.Render(m_Viewport.size, m_Viewport.min);
  ImGui::SetCursorScreenPos(m_Viewport.min);
  ImGui::Image((void *)(intptr_t)m_ColorAttachment, m_Viewport.size);
  ImGui::End();

  m_Registry->ClearChanged<CBone>();
}

void ViewportLayer::OnDetach() { m_State = nullptr; }

void ViewportLayer::Save(Serializer &serializer) {
  std::vector<uint8_t> buffer(sizeof(m_Camera.Position) +
                              sizeof(m_Camera.Zoom));

  std::memcpy(buffer.data(), glm::value_ptr(m_Camera.Position),
              sizeof(m_Camera.Position));
  std::memcpy(buffer.data() + sizeof(m_Camera.Position), &m_Camera.Zoom,
              sizeof(m_Camera.Zoom));

  serializer.Stage("camera", buffer.data(),
                   static_cast<uint64_t>(buffer.size()));
}

void ViewportLayer::Restore(Serializer &serializer) {
  std::vector<uint8_t> buffer = serializer.Get("camera");

  if (buffer.size() < sizeof(m_Camera.Position) + sizeof(m_Camera.Zoom))
    return;

  std::memcpy(glm::value_ptr(m_Camera.Position), buffer.data(),
              sizeof(m_Camera.Position));
  std::memcpy(&m_Camera.Zoom, buffer.data() + sizeof(m_Camera.Position),
              sizeof(m_Camera.Zoom));
}

void ViewportLayer::ResizeFramebuffer(ImVec2 viewport) {
  if (m_FrameBuffer == 0 || viewport != m_Viewport.size) {
    Window::GenerateFrameBuffer(viewport, m_FrameBuffer, m_DepthBuffer,
                                m_ColorAttachment);
    glViewport(0, 0, (GLsizei)viewport.x, (GLsizei)viewport.y);
    m_Viewport.size = viewport;
  }
}
